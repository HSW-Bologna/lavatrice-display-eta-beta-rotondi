#include <time.h>
#include <esp_log.h>
#include "controller.h"
#include "model/model.h"
#include "adapters/view/view.h"
#include "gui.h"
#include "com/machine.h"
#include "services/serializer.h"
#include "services/timestamp.h"
#include "bsp/buzzer.h"
#include "configuration/configuration.h"


static const char *TAG = "Controller";


void controller_init(mut_model_t *p_model) {
    (void)p_model;

    machine_init();
    machine_invia_presentazioni();

    view_change_page(&page_main);
}


void controller_process_message(pman_handle_t handle, void *msg) {
    (void)handle;
    (void)msg;
}

void controller_manage(mut_model_t *pmodel) {
    static int           initial_level_check         = 0;
    static unsigned long stato_ts                    = 0;
    static uint8_t       program_payed               = -1;
    static uint8_t       digital_coin_reader_enabled = -1;
    static uint8_t       pending_state_change        = 0;
    machine_response_t   machine_response;

#if 0
    msc_response_t       msc_response;

    size_t current_program_num = model_get_program_num(pmodel);
    if (program_payed != model_lavaggio_pagato(pmodel, current_program_num)) {
        program_payed = model_lavaggio_pagato(pmodel, current_program_num);
        machine_payment_state(program_payed);
    }
    if (digital_coin_reader_enabled != model_gettoniera_digitale_abilitata(pmodel)) {
        digital_coin_reader_enabled = model_gettoniera_digitale_abilitata(pmodel);
        machine_enable_digital_coin_reader(digital_coin_reader_enabled);
    }

    if (msc_get_response(&msc_response)) {
        switch (msc_response.code) {
            case MSC_RESPONSE_CODE_ARCHIVE_EXTRACTION_COMPLETE:
                configuration_load_all_data(pmodel);
                model_set_drive_mounted(pmodel, msc_is_device_mounted());
                view_event((view_event_t){.code = VIEW_EVENT_CODE_CONFIGURATION_LOADED, .error = msc_response.error});
                break;

            case MSC_RESPONSE_CODE_ARCHIVE_SAVING_COMPLETE:
                ESP_LOGI(TAG, "Configuration saved!");
                model_set_drive_mounted(pmodel, msc_is_device_mounted());
                view_event((view_event_t){.code = VIEW_EVENT_CODE_CONFIGURATION_SAVED, .error = msc_response.error});
                break;
        }
    }
#endif

    if (timestamp_is_expired(stato_ts, 500)) {
        //model_set_drive_mounted(pmodel, msc_is_device_mounted());
        //msc_read_archives(pmodel);
        machine_richiedi_stato();
        //view_event((view_event_t){.code = VIEW_EVENT_CODE_DEVICE_UPDATE});
        stato_ts = timestamp_get();
    }


    if (machine_ricevi_risposta(&machine_response)) {
        switch (machine_response.code) {
            case MACHINE_RESPONSE_CODE_DRAIN_REQUIRED:
                ESP_LOGI(TAG, "E' necessario uno scarico");
                if (model_lavaggio_finito(pmodel)) {     // Se sono alla fine devo segnalarlo all'utente
                    pending_state_change = 1;
                    machine_pause();
                    pmodel->run.f_richiedi_scarico = 1;
                } else {     // Altrimenti se l'utente ha richiesto lo stop devo passare automaticamente
                             // allo scarico
                    machine_forza_scarico();
                }
                break;

            case MACHINE_RESPONSE_CODE_REFUSED:
                pending_state_change = 0;
                buzzer_beep(2, 500, 500, 4);
                break;

            case MACHINE_RESPONSE_CODE_ERRORE_COMUNICAZIONE:
                pmodel->system.errore_comunicazione = 1;
                // view_event((view_event_t){.code = VIEW_EVENT_CODE_MODEL_UPDATE});
                break;

            case MACHINE_RESPONSE_CODE_PRESENTAZIONI: {
                //  Invia i parametri macchina
                machine_invia_parmac(&pmodel->prog.parmac);

                // allarme poweroff; se l'autoavvio e' configurato devo ripartire
                if (machine_response.presentazioni.n_all == 0x02 && pmodel->prog.parmac.autoavvio) {
                    machine_azzera_allarmi();
                }

                memcpy(pmodel->system.machine_fw_version, machine_response.presentazioni.machine_fw_version,
                       sizeof(pmodel->system.machine_fw_version));
                memcpy(pmodel->system.machine_fw_date, machine_response.presentazioni.machine_fw_date,
                       sizeof(pmodel->system.machine_fw_date));

                uint8_t allarme = machine_response.presentazioni.n_all;
                uint8_t stato   = machine_response.presentazioni.stato;

                ESP_LOGI(TAG, "Machine initial state %i (alarm %i)", machine_response.presentazioni.stato, allarme);
                if (allarme == 0 || allarme == 2) {
                    // allarme poweroff; se l'autoavvio e' configurato devo ripartire
                    if (allarme == 0x02 && pmodel->prog.parmac.autoavvio) {
                        machine_azzera_allarmi();
                    }

                    if (stato != STATO_MACCHINA_STOP) {
                        uint8_t lavaggio = machine_response.presentazioni.nro_programma;
                        int     step     = machine_response.presentazioni.nro_step;
                        ESP_LOGI(TAG, "Machine executing program %i, step %i", lavaggio, step);

                        if (lavaggio < model_get_num_programs(pmodel)) {
                            configuration_load_program(pmodel, lavaggio);
                            ESP_LOGI(TAG, "Macchina spenta in azione: lavaggio %i e step %i", lavaggio + 1, step + 1);
                            if (model_select_program_step(pmodel, lavaggio, step) >= 0) {
                                ESP_LOGI(TAG, "Caricato lavaggio %i e step %i", model_get_program_num(pmodel) + 1,
                                         pmodel->run.num_step_corrente + 1);
                                if (pmodel->prog.parmac.autoavvio) {
                                    pending_state_change = 1;
                                    machine_start(lavaggio);
                                }
                            }
                        } else {
                            ESP_LOGI(TAG, "Inconsistent state");
                            pending_state_change = 1;
                            machine_stop();
                        }
                    }
                }
                break;
            }

            case MACHINE_RESPONSE_CODE_TEST:
                pmodel->test = machine_response.test;
                // view_event((view_event_t){.code = VIEW_EVENT_CODE_MODEL_UPDATE});
                break;

            case MACHINE_RESPONSE_CODE_STATO: {
                ESP_LOGI(TAG, "LEttora stato");
                if (machine_read_state(pmodel)) {
                    pending_state_change = 0;
                }

                if (!model_macchina_in_stop(pmodel) &&
                    (model_get_program_num(pmodel) != pmodel->run.macchina.numero_programma ||
                     pmodel->run.maybe_programma == 0)) {
                    configuration_load_program(pmodel, pmodel->run.macchina.numero_programma);
                }

                // utils_dump_state(&pmodel->run.macchina);

                if (model_get_livello_centimetri(pmodel) > 0 && !initial_level_check) {
                    pmodel->run.f_richiedi_scarico = 1;
                }
                initial_level_check = 1;

                // view_event((view_event_t){.code = VIEW_EVENT_CODE_MODEL_UPDATE});

                if (pending_state_change) {
                    ESP_LOGI(TAG, "Pending state change, ignoring");
                    break;
                }

                // Passaggio allo step successivo se sono sincronizzato con il quadro, in marcia e senza step
                if (model_macchina_in_marcia(pmodel) && model_step_finito(pmodel)) {
                    if (model_lavaggio_finito(pmodel)) {
                        pending_state_change = 1;
                        ESP_LOGI(TAG, "Program done");
                        machine_stop();
                    } else {
                        model_avanza_step(pmodel);
                        ESP_LOGI(TAG, "Moving to step %i", model_get_current_step_number(pmodel));
                        machine_esegui_step(model_get_current_step(pmodel), model_get_current_step_number(pmodel));
                    }
                }

                if (model_macchina_in_scarico_forzato(pmodel)) {
                    pmodel->run.f_richiedi_scarico = 0;
                }

                if (model_requested_time(pmodel)) {
                    machine_send_time();
                    pmodel->run.macchina.richiesto_aggiornamento_tempo = 0;
                }
                break;
            }

            case MACHINE_RESPONSE_CODE_STATS:
                pmodel->stats = *machine_response.stats;
                free(machine_response.stats);
                // view_event((view_event_t){.code = VIEW_EVENT_CODE_MODEL_UPDATE});
                break;
        }
    }

    controller_gui_manage();
}
