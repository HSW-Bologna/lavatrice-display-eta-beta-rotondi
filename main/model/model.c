#include <math.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "model.h"
#include "services/serializer.h"
#include "services/timestamp.h"
#include "esp_log.h"
#include "parmac.h"


static unsigned int get_credito_macchina(model_t *model);


static const char *TAG = "Model";


void model_init(model_t *pmodel) {
    (void)TAG;
    memset(pmodel, 0, sizeof(model_t));
    pmodel->system.comunicazione_abilitata = 1;
    pmodel->system.removable_drive_state   = REMOVABLE_DRIVE_STATE_MISSING;
    pmodel->system.num_archivi             = 0;
    pmodel->run.maybe_programma            = 0;
    pmodel->run.f_richiedi_scarico         = 0;
    pmodel->run.test_outputs_map           = 0;
    pmodel->prog.contrast                  = 0x1A;

    pmodel->run.digital_coin_reader_test_override = TEST_OVERRIDE_NONE;
    pmodel->run.firmware_update_state             = FIRMWARE_UPDATE_STATE_NONE;

    strcpy(pmodel->prog.password, "12345");
    strcpy(pmodel->prog.parmac.nome, "Pluto");
}


void model_cancella_lavaggio_programmato(model_t *model) {
    model->prog.programmed_wash.configured = 0;
}


int model_lavaggio_programmato_impostato(model_t *model, int *lavaggio) {
    time_t now;
    time(&now);
    if (model->prog.programmed_wash.configured) {
        if (lavaggio) {
            *lavaggio = model->prog.programmed_wash.program_index;
        }
        return 1;
    } else {
        return 0;
    }
}


int model_lavaggio_programmato_minuti_rimanenti(model_t *model) {
    time_t now;
    time(&now);
    if (model_lavaggio_programmato_impostato(model, NULL)) {
        return (model->prog.programmed_wash.start - now) / 60;
    } else {
        return 0;
    }
}



void model_set_drive_mounted(model_t *pmodel, removable_drive_state_t drive_mounted) {
    assert(pmodel != NULL);
    pmodel->system.removable_drive_state = drive_mounted;
}


programma_lavatrice_t *model_get_program(model_t *pmodel) {
    assert(pmodel != NULL);
    return &pmodel->run.programma_caricato;
}


parametri_step_t *model_get_program_step(model_t *pmodel, size_t num) {
    assert(pmodel != NULL);
    if (num < model_get_program(pmodel)->num_steps) {
        return &model_get_program(pmodel)->steps[num];
    } else {
        return NULL;
    }
}


uint16_t model_program_remaining(model_t *pmodel) {
    assert(pmodel != NULL);

    programma_lavatrice_t *p      = model_get_program(pmodel);
    unsigned int           totale = 0;

    for (size_t i = model_get_current_step_number(pmodel) + 1; i < p->num_steps; i++) {
        const parametri_step_t *s = &p->steps[i];
        totale += s->durata;

        if (s->tipo == STEP_CENTRIFUGA) {
            unsigned int rampe[3]  = {s->tempo_rampa_1, s->tempo_rampa_2, s->tempo_rampa_3};
            unsigned int attese[3] = {s->tempo_attesa_centrifuga_1, s->tempo_attesa_centrifuga_2, 0};

            totale += s->tempo_frenata;
            for (size_t j = 0; j < s->numero_rampe; j++)
                totale += rampe[j] + attese[j];

            totale += s->tempo_preparazione;
            for (size_t j = 0; j < pmodel->prog.parmac.abilitazione_preparazione_rotazione; j++) {
                totale += pmodel->prog.parmac.tempo_marcia_preparazione_rotazione * 2 +
                          pmodel->prog.parmac.tempo_sosta_preparazione_rotazione * 2;
            }
        }
    }

    return totale + pmodel->run.macchina.rimanente;
}


void model_sync_program_preview(model_t *pmodel) {
    assert(pmodel != NULL);

    programma_lavatrice_t *program = model_get_program(pmodel);
    programma_preview_t   *preview = &pmodel->prog.preview_programmi[model_get_program_num(pmodel)];
    strcpy(preview->filename, program->filename);
    strcpy(preview->name, program->nomi[model_get_language(pmodel)]);
    preview->prezzo = program->prezzo;
    preview->tipo   = program->tipo;
}


uint16_t model_get_program_num(model_t *pmodel) {
    assert(pmodel != NULL);
    return pmodel->run.num_programma_caricato;
}


uint16_t model_get_language(model_t *pmodel) {
    assert(pmodel != NULL);
    return pmodel->prog.parmac.lingua;
}


int model_oblo_chiuso(model_t *pmodel) {
    assert(pmodel != NULL);
    return pmodel->run.macchina.oblo_aperto_chiuso == 1;
}


int model_oblo_libero(model_t *pmodel) {
    assert(pmodel != NULL);
    return pmodel->run.macchina.chiavistello_chiuso == 0 && pmodel->run.macchina.chiavistello_aperto == 1;
}


int model_oblo_serrato(model_t *pmodel) {
    assert(pmodel != NULL);
    return model_oblo_chiuso(pmodel) && pmodel->run.macchina.chiavistello_chiuso == 1 &&
           pmodel->run.macchina.chiavistello_aperto == 0;
}


void model_reset_temporary_language(model_t *pmodel) {
    assert(pmodel != NULL);
    pmodel->run.lingua = pmodel->prog.parmac.lingua;
}


void model_reset_configuration_to_default(model_t *model) {
    strcpy(model->prog.parmac.nome, "MACCHINA NUOVA");
    parmac_init(model, 1);
    model->prog.num_programmi = 0;
}


uint16_t model_get_temporary_language(model_t *pmodel) {
    assert(pmodel != NULL);
    return pmodel->run.lingua;
}


size_t model_get_num_user_programs(model_t *pmodel) {
    assert(pmodel != NULL);
    if (pmodel->prog.num_programmi > pmodel->prog.parmac.numero_massimo_programmi_utente) {
        return pmodel->prog.parmac.numero_massimo_programmi_utente;
    } else {
        return pmodel->prog.num_programmi;
    }
}


size_t model_get_num_programs(model_t *pmodel) {
    assert(pmodel != NULL);
    return pmodel->prog.num_programmi;
}


const programma_preview_t *model_get_preview(model_t *pmodel, size_t i) {
    assert(pmodel != NULL);
    if (i >= model_get_num_programs(pmodel)) {
        return NULL;
    }
    return &pmodel->prog.preview_programmi[i];
}


void model_avanza_step(model_t *model) {
    const programma_lavatrice_t *p = model_get_program(model);

    if (p && model->run.num_step_successivo >= (int)p->num_steps) {
        model->run.num_step_corrente = 0;
    } else {
        model->run.num_step_corrente = model->run.num_step_successivo;
    }

    if (p && model->run.num_step_successivo + 1 >= (int)p->num_steps) {
        model->run.model_lavaggio_finito = 1;
    } else {
        model->run.model_lavaggio_finito = 0;

        const parametri_step_t *s = model_get_current_step(model);
        if (s) {
            model->run.temperature_setpoint = s->temperatura;
            model->run.level_setpoint       = s->livello;
        } else {
            model->run.temperature_setpoint = 0;
            model->run.level_setpoint       = 0;
        }
    }
    model->run.num_step_successivo = (model->run.num_step_successivo + 1) % p->num_steps;
}


void model_arretra_step(model_t *model) {
    const programma_lavatrice_t *prog = model_get_program(model);
    int                          cur  = model->run.num_step_corrente;

    if (prog && model->prog.num_programmi > 0 && prog->num_steps > 0) {
        if (cur > 0) {
            cur--;
        } else {
            cur = prog->num_steps - 1;
        }
        model->run.num_step_corrente   = cur;
        model->run.num_step_successivo = cur + 1;

        if (model->run.num_step_successivo + 1 > (int)prog->num_steps) {
            model->run.model_lavaggio_finito = 1;
        } else {
            model->run.model_lavaggio_finito = 0;
        }
    }
}


void model_azzera_lavaggio(model_t *pmodel) {
    assert(pmodel != NULL);
    pmodel->run.num_step_corrente     = 0;
    pmodel->run.num_step_successivo   = 0;
    pmodel->run.model_lavaggio_finito = 0;
}


int model_macchina_in_pausa(model_t *model) {
    return model->run.macchina.stato == STATO_MACCHINA_PAUSA;
}


int model_macchina_in_frenata(model_t *model) {
    return model->run.macchina.stato == STATO_MACCHINA_FRENATA || model->run.macchina.vis_popup_frenata;
}


int model_macchina_in_scarico_forzato(model_t *model) {
    return model->run.macchina.stato == STATO_MACCHINA_SCARICO_FORZATO;
}


int model_macchina_in_stop(model_t *model) {
    return model->run.macchina.stato == STATO_MACCHINA_STOP;
}


int model_macchina_in_marcia(model_t *model) {
    return model->run.macchina.stato == STATO_MACCHINA_MARCIA;
}


int model_step_finito(model_t *model) {
    return model->run.macchina.sottostato_step == 0;
}


int model_lavaggio_finito(model_t *model) {
    // Se il loop e' abilitato non ho mai finito
    if (model->prog.parmac.abilitazione_loop_prog) {
        return 0;
    }

    return model_step_finito(model) && model->run.macchina.stato != STATO_MACCHINA_STOP &&
           model->run.macchina.stato != STATO_MACCHINA_PAUSA && model->run.model_lavaggio_finito;
}


uint16_t model_get_livello_centimetri(model_t *pmodel) {
    assert(pmodel != NULL);
    return pmodel->run.macchina.livello;
}


int model_get_velocita_corretta(model_t *model) {
    return (model->run.macchina.velocita_rilevata * model->prog.parmac.correzione_contagiri) / 100;
}


uint16_t model_get_livello(model_t *pmodel) {
    assert(pmodel != NULL);
    switch (pmodel->prog.parmac.tipo_livello) {
        case 0:
            return pmodel->run.macchina.livello;
        default:
            return pmodel->run.macchina.livello_litri;
    }
}


uint16_t model_get_current_step_number(model_t *pmodel) {
    assert(pmodel != NULL);
    return pmodel->run.num_step_corrente;
}


parametri_step_t *model_get_current_step(model_t *pmodel) {
    assert(pmodel != NULL);
    assert(pmodel->run.maybe_programma);
    return &model_get_program(pmodel)->steps[model_get_current_step_number(pmodel)];
}


int model_select_program_step(model_t *pmodel, size_t i, size_t step) {
    assert(pmodel != NULL);

    if (model_get_program_num(pmodel) == i) {
        const programma_lavatrice_t *p = model_get_program(pmodel);

        if (p && step < p->num_steps) {
            pmodel->run.num_step_corrente = step;
        } else {
            pmodel->run.num_step_corrente = 0;
        }

        pmodel->run.num_step_successivo = pmodel->run.num_step_corrente;

        const parametri_step_t *s = model_get_current_step(pmodel);
        if (s) {
            pmodel->run.temperature_setpoint = s->temperatura;
            pmodel->run.level_setpoint       = s->livello;
        } else {
            pmodel->run.temperature_setpoint = 0;
            pmodel->run.level_setpoint       = 0;
        }

        return 0;
    } else {
        return -1;
    }
}


uint16_t model_alarm_code(model_t *pmodel) {
    assert(pmodel != NULL);
    return pmodel->run.macchina.codice_allarme;
}


int model_can_work(model_t *pmodel) {
    if (model_get_num_programs(pmodel) > 0 && pmodel->run.maybe_programma && pmodel->system.comunicazione_abilitata &&
        !pmodel->system.errore_comunicazione) {
        return pmodel->run.macchina.numero_programma == model_get_program_num(pmodel) &&
               pmodel->run.macchina.numero_step < model_get_program(pmodel)->num_steps;
    } else {
        return 0;
    }
}


uint16_t model_get_preparation_time(model_t *pmodel) {
    assert(pmodel != NULL);
    if (pmodel->prog.parmac.abilitazione_preparazione_rotazione) {
        return pmodel->prog.parmac.tempo_marcia_preparazione_rotazione * 2 +
               pmodel->prog.parmac.tempo_sosta_preparazione_rotazione * 2;
    } else {
        return 0;
    }
}


void model_formatta_prezzo(char *string, model_t *model, unsigned int prezzo) {
    unsigned int cifre    = model->prog.parmac.cifre_prezzo;
    unsigned int decimali = model->prog.parmac.cifre_decimali_prezzo;
    char         format[32];
    double       fprezzo;
    sprintf(format, "%%0%i.%if", cifre + (decimali > 0 ? 1 : 0), decimali);
    fprezzo = (double)prezzo / pow(10, decimali);
    sprintf(string, format, fprezzo);
}


unsigned int model_get_credito_gettoniera_digitale(model_t *model) {
    assert(model != NULL);
    size_t             d      = model->prog.parmac.cifre_decimali_prezzo;
    unsigned int       m      = (unsigned int)pow(10, d);
    const unsigned int mult[] = {1 * m, (5 * m) / 10, (2 * m) / 10, (1 * m) / 10, 2 * m};
    unsigned int       tot    = 0;
    for (size_t i = LINEA_1_GETTONIERA_DIGITALE; i < LINEA_1_GETTONIERA_DIGITALE + LINEE_GETTONIERA_DIGITALE; i++) {
        size_t ti = i - LINEA_1_GETTONIERA_DIGITALE;
        tot += mult[ti] * model->run.macchina.credito[i];
    }
    return tot;
}


unsigned int model_get_credito_gettoniera_impulsi(model_t *model, size_t linee) {
    assert(model != NULL);
    unsigned int tot = 0;

    if (linee > 2)
        return tot;

    for (size_t i = LINEA_1_GETTONIERA_IMPULSI; i < LINEA_1_GETTONIERA_IMPULSI + linee; i++)
        tot += model->run.macchina.credito[i] * model->prog.parmac.valore_impulso;

    return tot;
}


unsigned int model_get_credito(model_t *pmodel) {
    assert(pmodel != NULL);
    return get_credito_macchina(pmodel) + pmodel->run.credito;
}


int model_gettoniera_digitale_abilitata(model_t *pmodel) {
    assert(pmodel != NULL);
    if (pmodel->run.digital_coin_reader_test_override == TEST_OVERRIDE_NONE) {
        return (pmodel->prog.parmac.tipo_gettoniera == PAGAMENTO_DIGITALE ||
                pmodel->prog.parmac.tipo_gettoniera == PAGAMENTO_DIGITALE_LINEA_SINGOLA) &&
               !model_lavaggio_pagato(pmodel, model_get_program_num(pmodel));
    } else {
        return pmodel->run.digital_coin_reader_test_override == TEST_OVERRIDE_ON;
    }
}


int model_lavaggio_pagato(model_t *pmodel, size_t num_prog) {
    assert(pmodel != NULL);
    if (pmodel->prog.parmac.tipo_gettoniera == PAGAMENTO_NESSUNO) {
        return 1;
    } else if (pmodel->run.macchina.pagato) {
        return 1;
    } else if (pmodel->prog.parmac.prezzo_unico) {
        return model_get_credito(pmodel) >= pmodel->prog.parmac.valore_prezzo_unico;
    } else if (num_prog < pmodel->prog.num_programmi) {
        const programma_preview_t *p = &pmodel->prog.preview_programmi[num_prog];
        return model_get_credito(pmodel) >= p->prezzo;
    } else {
        return 0;
    }
}


int model_pagamento_abilitato(model_t *pmodel) {
    assert(pmodel != NULL);
    return pmodel->prog.parmac.tipo_gettoniera != PAGAMENTO_NESSUNO;
}


void model_scala_credito(model_t *pmodel) {
    assert(pmodel != NULL);
    if (pmodel->prog.parmac.tipo_gettoniera == PAGAMENTO_NESSUNO) {
        return;
    }
    if (pmodel->run.macchina.pagato) {
        return;
    }

#ifdef INTEGRITA_MORALE
    unsigned int consumato = 0;
    if (model->prog.parmac.prezzo_unico) {
        consumato = model->prog.parmac.valore_prezzo_unico;
    } else {
        const programma_lavatrice_t *p = model_get_programma_corrente(model);
        if (p)
            consumato = p->prezzo;
    }

    model->run.credito += get_credito_macchina(model);
    if (model->run.credito > consumato)
        model->run.credito -= consumato;
    else
        model->run.credito = 0;
#else
    pmodel->run.credito = 0;
#endif
}


uint8_t model_get_bit_accesso(uint8_t al) {
    if (al > 1) {
        return 0;
    } else {
        const uint8_t lvls[] = {LVL_UTENTE, LVL_COSTRUTTORE};
        return lvls[al];
    }
}


int model_is_communication_ok(model_t *pmodel) {
    assert(pmodel != NULL);
    return !pmodel->system.errore_comunicazione || !pmodel->system.comunicazione_abilitata;
}


void model_update_preview(model_t *pmodel) {
    assert(pmodel != NULL);
    pmodel->prog.preview_programmi[pmodel->run.num_programma_caricato].tipo = pmodel->run.programma_caricato.tipo;
}


int model_requested_time(model_t *pmodel) {
    assert(pmodel != NULL);
    return pmodel->run.macchina.richiesto_aggiornamento_tempo;
}


int model_is_level_in_cm(parmac_t *parmac) {
    assert(parmac != NULL);
    switch (parmac->tipo_livello) {
        case LIVELLO_CENTIMETRI:
            return 1;
        default:
            return 0;
    }
}


size_t model_serialize_parmac(uint8_t *buffer, parmac_t *p) {
    size_t i = 0;

    memcpy(&buffer[i], p->nome, sizeof(name_t));
    i += sizeof(name_t);

    i += serialize_uint16_be(&buffer[i], (uint16_t)p->lingua);
    i += serialize_uint16_be(&buffer[i], 0);
    i += serialize_uint16_be(&buffer[i], (uint16_t)p->logo);
    i += serialize_uint16_be(&buffer[i], 0);
    i += serialize_uint16_be(&buffer[i], 0);
    i += serialize_uint16_be(&buffer[i], (uint16_t)p->codice_nodo_macchina);
    i += serialize_uint16_be(&buffer[i], (uint16_t)p->diametro_cesto);
    i += serialize_uint16_be(&buffer[i], (uint16_t)p->profondita_cesto);
    i += serialize_uint16_be(&buffer[i], (uint16_t)p->altezza_trappola);
    i += serialize_uint16_be(&buffer[i], (uint16_t)p->correzione_contagiri);
    i += serialize_uint16_be(&buffer[i], (uint16_t)p->numero_massimo_programmi);
    i += serialize_uint16_be(&buffer[i], (uint16_t)p->numero_massimo_programmi_utente);
    i += serialize_uint16_be(&buffer[i], (uint16_t)p->visualizzazione_opl);
    i += serialize_uint16_be(&buffer[i], (uint16_t)p->livello_accesso);
    i += serialize_uint16_be(&buffer[i], (uint16_t)p->interfaccia_semplificata);
    i += serialize_uint16_be(&buffer[i], (uint16_t)p->controllo_pausa_stop);
    i += serialize_uint16_be(&buffer[i], (uint16_t)p->visualizzazione_menu);
    i += serialize_uint16_be(&buffer[i], (uint16_t)p->visualizzazione_menu_saponi);
    i += serialize_uint16_be(&buffer[i], 0);
    i += serialize_uint16_be(&buffer[i], (uint16_t)p->abilitazione_lavaggio_programmato);
    i += serialize_uint16_be(&buffer[i], 0);
    i += serialize_uint32_be(&buffer[i], 0);
    i += serialize_uint16_be(&buffer[i], (uint16_t)p->controllo_lucchetto);
    i += serialize_uint16_be(&buffer[i], (uint16_t)p->controllo_step);
    i += serialize_uint16_be(&buffer[i], 0);
    i += serialize_uint16_be(&buffer[i], 0);
    i += serialize_uint16_be(&buffer[i], 0);
    i += serialize_uint16_be(&buffer[i], (uint16_t)p->secondi_pausa);
    i += serialize_uint16_be(&buffer[i], (uint16_t)p->secondi_stop);
    i += serialize_uint16_be(&buffer[i], (uint16_t)p->tempo_out_pagine);
    i += serialize_uint16_be(&buffer[i], (uint16_t)p->tempo_allarme_livello);
    i += serialize_uint16_be(&buffer[i], (uint16_t)p->tempo_allarme_temperatura);
    i += serialize_uint16_be(&buffer[i], (uint16_t)p->tempo_allarme_scarico);
    i += serialize_uint16_be(&buffer[i], (uint16_t)p->tempo_ritardo_micro_oblo);
    i += serialize_uint16_be(&buffer[i], (uint16_t)p->tipo_out_aux_1);
    i += serialize_uint16_be(&buffer[i], (uint16_t)p->tipo_out_aux_2);
    i += serialize_uint16_be(&buffer[i], (uint16_t)p->tipo_out_aux_3);
    i += serialize_uint16_be(&buffer[i], (uint16_t)p->tipo_out_aux_4);
    i += serialize_uint16_be(&buffer[i], (uint16_t)p->percentuale_livello_carico_ridotto);
    i += serialize_uint16_be(&buffer[i], (uint16_t)p->percentuale_sapone_carico_ridotto);
    i += serialize_uint16_be(&buffer[i], (uint16_t)p->f_scarico_recupero);
    i += serialize_uint16_be(&buffer[i], 1);     // Removed, now fixed
    i += serialize_uint16_be(&buffer[i], (uint16_t)p->f_macchina_libera);
    i += serialize_uint16_be(&buffer[i], (uint16_t)p->abilitazione_espansione_io);
    i += serialize_uint16_be(&buffer[i], (uint16_t)p->tipo_gettoniera);
    i += serialize_uint16_be(&buffer[i], (uint16_t)p->prezzo_unico);
    i += serialize_uint16_be(&buffer[i], (uint16_t)p->valore_impulso);
    i += serialize_uint16_be(&buffer[i], (uint16_t)p->richiesta_pagamento);
    i += serialize_uint16_be(&buffer[i], (uint16_t)p->cifre_prezzo);
    i += serialize_uint16_be(&buffer[i], (uint16_t)p->cifre_decimali_prezzo);
    i += serialize_uint16_be(&buffer[i], (uint16_t)p->modo_vis_prezzo);
    i += serialize_uint16_be(&buffer[i], (uint16_t)p->temperatura_massima);
    i += serialize_uint16_be(&buffer[i], (uint16_t)p->isteresi_temperatura);
    i += serialize_uint16_be(&buffer[i], (uint16_t)p->temperatura_sicurezza);
    i += serialize_uint16_be(&buffer[i], (uint16_t)p->temperatura_termodegradazione);
    i += serialize_uint16_be(&buffer[i], (uint16_t)p->tipo_livello);
    i += serialize_uint16_be(&buffer[i], (uint16_t)p->impulsi_litro);
    i += serialize_uint16_be(&buffer[i], (uint16_t)p->tempo_isteresi_livello);
    i += serialize_uint16_be(&buffer[i], (uint16_t)p->centimetri_max_livello);
    i += serialize_uint16_be(&buffer[i], (uint16_t)p->centimetri_minimo_scarico);
    i += serialize_uint16_be(&buffer[i], (uint16_t)p->centimetri_minimo_riscaldo);
    i += serialize_uint16_be(&buffer[i], (uint16_t)p->litri_massimi);
    i += serialize_uint16_be(&buffer[i], (uint16_t)p->livello_sfioro);
    i += serialize_uint16_be(&buffer[i], (uint16_t)p->litri_minimi_riscaldo);
    i += serialize_uint16_be(&buffer[i], (uint16_t)p->tempo_minimo_scarico);
    i += serialize_uint16_be(&buffer[i], (uint16_t)p->tempo_scarico_servizio);
    i += serialize_uint16_be(&buffer[i], (uint16_t)p->tempo_colpo_aperto_scarico);
    i += serialize_uint16_be(&buffer[i], (uint16_t)p->tipo_inverter);
    i += serialize_uint16_be(&buffer[i], (uint16_t)p->velocita_minima_lavaggio);
    i += serialize_uint16_be(&buffer[i], (uint16_t)p->velocita_massima_lavaggio);
    i += serialize_uint16_be(&buffer[i], 0);
    i += serialize_uint16_be(&buffer[i], (uint16_t)p->abilitazione_preparazione_rotazione);
    i += serialize_uint16_be(&buffer[i], (uint16_t)p->tempo_marcia_preparazione_rotazione);
    i += serialize_uint16_be(&buffer[i], (uint16_t)p->tempo_sosta_preparazione_rotazione);
    i += serialize_uint16_be(&buffer[i], (uint16_t)p->velocita_minima_preparazione);
    i += serialize_uint16_be(&buffer[i], (uint16_t)p->velocita_massima_preparazione);
    i += serialize_uint16_be(&buffer[i], (uint16_t)p->velocita_minima_centrifuga_1);
    i += serialize_uint16_be(&buffer[i], (uint16_t)p->velocita_massima_centrifuga_1);
    i += serialize_uint16_be(&buffer[i], (uint16_t)p->velocita_minima_centrifuga_2);
    i += serialize_uint16_be(&buffer[i], (uint16_t)p->velocita_massima_centrifuga_2);
    i += serialize_uint16_be(&buffer[i], (uint16_t)p->velocita_minima_centrifuga_3);
    i += serialize_uint16_be(&buffer[i], (uint16_t)p->velocita_massima_centrifuga_3);
    i += serialize_uint16_be(&buffer[i], (uint16_t)p->tempo_minimo_rampa);
    i += serialize_uint16_be(&buffer[i], (uint16_t)p->tempo_massimo_rampa);
    i += serialize_uint16_be(&buffer[i], (uint16_t)p->nro_max_sbilanciamenti);
    i += serialize_uint16_be(&buffer[i], (uint16_t)p->f_proximity);
    i += serialize_uint16_be(&buffer[i], (uint16_t)p->tempo_minimo_frenata);
    i += serialize_uint16_be(&buffer[i], (uint16_t)p->numero_raggi);
    i += serialize_uint16_be(&buffer[i], (uint16_t)p->abilitazione_accelerometro);
    i += serialize_uint16_be(&buffer[i], (uint16_t)p->scala_accelerometro);
    i += serialize_uint16_be(&buffer[i], (uint16_t)p->soglia_x_accelerometro);
    i += serialize_uint16_be(&buffer[i], (uint16_t)p->soglia_y_accelerometro);
    i += serialize_uint16_be(&buffer[i], (uint16_t)p->soglia_z_accelerometro);
    i += serialize_uint16_be(&buffer[i], (uint16_t)p->soglia_x_accelerometro_h);
    i += serialize_uint16_be(&buffer[i], (uint16_t)p->soglia_y_accelerometro_h);
    i += serialize_uint16_be(&buffer[i], (uint16_t)p->soglia_z_accelerometro_h);
    i += serialize_uint16_be(&buffer[i], (uint16_t)p->giri_accelerometro);
    i += serialize_uint16_be(&buffer[i], (uint16_t)p->giri_accelerometro_2);
    i += serialize_uint16_be(&buffer[i], (uint16_t)p->delta_val_accelerometro);
    i += serialize_uint16_be(&buffer[i], (uint16_t)p->tempo_attesa_accelerometro);
    i += serialize_uint16_be(&buffer[i], (uint16_t)p->tempo_scarico_accelerometro);
    i += serialize_uint16_be(&buffer[i], (uint16_t)p->numero_saponi_utilizzabili);
    i += serialize_uint16_be(&buffer[i], (uint16_t)p->tempo_h2o_pulizia_saponi);
    i += serialize_uint16_be(&buffer[i], (uint16_t)p->tempo_precarica);
    i += serialize_uint16_be(&buffer[i], (uint16_t)p->tempo_tasto_carico_saponi);
    i += serialize_uint16_be(&buffer[i], (uint16_t)p->abilitazione_min_sec);
    i += serialize_uint16_be(&buffer[i], (uint16_t)p->abilitazione_sblocco_get);
    i += serialize_uint16_be(&buffer[i], (uint16_t)p->inibizione_allarmi);
    i += serialize_uint16_be(&buffer[i], (uint16_t)p->autoavvio);
    i += serialize_uint16_be(&buffer[i], (uint16_t)p->abilitazione_loop_prog);

    i += serialize_uint16_be(&buffer[i], (uint16_t)p->visualizzazione_data_ora);
    i += serialize_uint16_be(&buffer[i], (uint16_t)p->visualizzazione_esclusione_sapone);
    i += serialize_uint16_be(&buffer[i], (uint16_t)p->visualizzazione_pedante);

    memcpy(&buffer[i], p->funzioni_rgb, NUM_CONDIZIONI_MACCHINA);
    i += NUM_CONDIZIONI_MACCHINA;

    i += serialize_uint16_be(&buffer[i], (uint16_t)p->esclusione_sapone);
    i += serialize_uint32_be(&buffer[i], (uint16_t)p->valore_prezzo_unico);
    i += serialize_uint16_be(&buffer[i], (uint16_t)p->tipo_frontale);
    i += serialize_uint16_be(&buffer[i], (uint16_t)p->tipo_serratura);
    i += serialize_uint16_be(&buffer[i], (uint16_t)p->durata_impulso_serratura);

    assert(i == PARMAC_SIZE);
    return i;
}

size_t model_deserialize_parmac(parmac_t *p, uint8_t *buffer) {
    size_t       i    = 0;
    unsigned int stub = 0;

    memcpy(p->nome, &buffer[i], sizeof(name_t));
    i += sizeof(name_t);

    i += UNPACK_UINT16_BE(p->lingua, &buffer[i]);
    i += UNPACK_UINT16_BE(stub, &buffer[i]);
    i += UNPACK_UINT16_BE(p->logo, &buffer[i]);
    i += UNPACK_UINT16_BE(stub, &buffer[i]);
    i += UNPACK_UINT16_BE(stub, &buffer[i]);
    i += UNPACK_UINT16_BE(p->codice_nodo_macchina, &buffer[i]);
    i += UNPACK_UINT16_BE(p->diametro_cesto, &buffer[i]);
    i += UNPACK_UINT16_BE(p->profondita_cesto, &buffer[i]);
    i += UNPACK_UINT16_BE(p->altezza_trappola, &buffer[i]);
    i += UNPACK_UINT16_BE(p->correzione_contagiri, &buffer[i]);
    i += UNPACK_UINT16_BE(p->numero_massimo_programmi, &buffer[i]);
    i += UNPACK_UINT16_BE(p->numero_massimo_programmi_utente, &buffer[i]);
    i += UNPACK_UINT16_BE(p->visualizzazione_opl, &buffer[i]);
    i += UNPACK_UINT16_BE(p->livello_accesso, &buffer[i]);
    i += UNPACK_UINT16_BE(p->interfaccia_semplificata, &buffer[i]);
    i += UNPACK_UINT16_BE(p->controllo_pausa_stop, &buffer[i]);
    i += UNPACK_UINT16_BE(p->visualizzazione_menu, &buffer[i]);
    i += UNPACK_UINT16_BE(p->visualizzazione_menu_saponi, &buffer[i]);
    i += UNPACK_UINT16_BE(stub, &buffer[i]);
    i += UNPACK_UINT16_BE(p->abilitazione_lavaggio_programmato, &buffer[i]);
    i += UNPACK_UINT16_BE(stub, &buffer[i]);
    i += UNPACK_UINT32_BE(stub, &buffer[i]);
    i += UNPACK_UINT16_BE(p->controllo_lucchetto, &buffer[i]);
    i += UNPACK_UINT16_BE(p->controllo_step, &buffer[i]);
    i += UNPACK_UINT16_BE(stub, &buffer[i]);
    i += UNPACK_UINT16_BE(stub, &buffer[i]);
    i += UNPACK_UINT16_BE(stub, &buffer[i]);
    i += UNPACK_UINT16_BE(p->secondi_pausa, &buffer[i]);
    i += UNPACK_UINT16_BE(p->secondi_stop, &buffer[i]);
    i += UNPACK_UINT16_BE(p->tempo_out_pagine, &buffer[i]);
    i += UNPACK_UINT16_BE(p->tempo_allarme_livello, &buffer[i]);
    i += UNPACK_UINT16_BE(p->tempo_allarme_temperatura, &buffer[i]);
    i += UNPACK_UINT16_BE(p->tempo_allarme_scarico, &buffer[i]);
    i += UNPACK_UINT16_BE(p->tempo_ritardo_micro_oblo, &buffer[i]);
    i += UNPACK_UINT16_BE(p->tipo_out_aux_1, &buffer[i]);
    i += UNPACK_UINT16_BE(p->tipo_out_aux_2, &buffer[i]);
    i += UNPACK_UINT16_BE(p->tipo_out_aux_3, &buffer[i]);
    i += UNPACK_UINT16_BE(p->tipo_out_aux_4, &buffer[i]);
    i += UNPACK_UINT16_BE(p->percentuale_livello_carico_ridotto, &buffer[i]);
    i += UNPACK_UINT16_BE(p->percentuale_sapone_carico_ridotto, &buffer[i]);
    i += UNPACK_UINT16_BE(p->f_scarico_recupero, &buffer[i]);
    {
        uint16_t placeholder = 1;
        i += UNPACK_UINT16_BE(placeholder, &buffer[i]);
    }
    i += UNPACK_UINT16_BE(p->f_macchina_libera, &buffer[i]);
    i += UNPACK_UINT16_BE(p->abilitazione_espansione_io, &buffer[i]);
    i += UNPACK_UINT16_BE(p->tipo_gettoniera, &buffer[i]);
    i += UNPACK_UINT16_BE(p->prezzo_unico, &buffer[i]);
    i += UNPACK_UINT16_BE(p->valore_impulso, &buffer[i]);
    i += UNPACK_UINT16_BE(p->richiesta_pagamento, &buffer[i]);
    i += UNPACK_UINT16_BE(p->cifre_prezzo, &buffer[i]);
    i += UNPACK_UINT16_BE(p->cifre_decimali_prezzo, &buffer[i]);
    i += UNPACK_UINT16_BE(p->modo_vis_prezzo, &buffer[i]);
    i += UNPACK_UINT16_BE(p->temperatura_massima, &buffer[i]);
    i += UNPACK_UINT16_BE(p->isteresi_temperatura, &buffer[i]);
    i += UNPACK_UINT16_BE(p->temperatura_sicurezza, &buffer[i]);
    i += UNPACK_UINT16_BE(p->temperatura_termodegradazione, &buffer[i]);
    i += UNPACK_UINT16_BE(p->tipo_livello, &buffer[i]);
    i += UNPACK_UINT16_BE(p->impulsi_litro, &buffer[i]);
    i += UNPACK_UINT16_BE(p->tempo_isteresi_livello, &buffer[i]);
    i += UNPACK_UINT16_BE(p->centimetri_max_livello, &buffer[i]);
    i += UNPACK_UINT16_BE(p->centimetri_minimo_scarico, &buffer[i]);
    i += UNPACK_UINT16_BE(p->centimetri_minimo_riscaldo, &buffer[i]);
    i += UNPACK_UINT16_BE(p->litri_massimi, &buffer[i]);
    i += UNPACK_UINT16_BE(p->livello_sfioro, &buffer[i]);
    i += UNPACK_UINT16_BE(p->litri_minimi_riscaldo, &buffer[i]);
    i += UNPACK_UINT16_BE(p->tempo_minimo_scarico, &buffer[i]);
    i += UNPACK_UINT16_BE(p->tempo_scarico_servizio, &buffer[i]);
    i += UNPACK_UINT16_BE(p->tempo_colpo_aperto_scarico, &buffer[i]);
    i += UNPACK_UINT16_BE(p->tipo_inverter, &buffer[i]);
    i += UNPACK_UINT16_BE(p->velocita_minima_lavaggio, &buffer[i]);
    i += UNPACK_UINT16_BE(p->velocita_massima_lavaggio, &buffer[i]);
    i += UNPACK_UINT16_BE(stub, &buffer[i]);
    i += UNPACK_UINT16_BE(p->abilitazione_preparazione_rotazione, &buffer[i]);
    i += UNPACK_UINT16_BE(p->tempo_marcia_preparazione_rotazione, &buffer[i]);
    i += UNPACK_UINT16_BE(p->tempo_sosta_preparazione_rotazione, &buffer[i]);
    i += UNPACK_UINT16_BE(p->velocita_minima_preparazione, &buffer[i]);
    i += UNPACK_UINT16_BE(p->velocita_massima_preparazione, &buffer[i]);
    i += UNPACK_UINT16_BE(p->velocita_minima_centrifuga_1, &buffer[i]);
    i += UNPACK_UINT16_BE(p->velocita_massima_centrifuga_1, &buffer[i]);
    i += UNPACK_UINT16_BE(p->velocita_minima_centrifuga_2, &buffer[i]);
    i += UNPACK_UINT16_BE(p->velocita_massima_centrifuga_2, &buffer[i]);
    i += UNPACK_UINT16_BE(p->velocita_minima_centrifuga_3, &buffer[i]);
    i += UNPACK_UINT16_BE(p->velocita_massima_centrifuga_3, &buffer[i]);
    i += UNPACK_UINT16_BE(p->tempo_minimo_rampa, &buffer[i]);
    i += UNPACK_UINT16_BE(p->tempo_massimo_rampa, &buffer[i]);
    i += UNPACK_UINT16_BE(p->nro_max_sbilanciamenti, &buffer[i]);
    i += UNPACK_UINT16_BE(p->f_proximity, &buffer[i]);
    i += UNPACK_UINT16_BE(p->tempo_minimo_frenata, &buffer[i]);
    i += UNPACK_UINT16_BE(p->numero_raggi, &buffer[i]);
    i += UNPACK_UINT16_BE(p->abilitazione_accelerometro, &buffer[i]);
    i += UNPACK_UINT16_BE(p->scala_accelerometro, &buffer[i]);
    i += UNPACK_UINT16_BE(p->soglia_x_accelerometro, &buffer[i]);
    i += UNPACK_UINT16_BE(p->soglia_y_accelerometro, &buffer[i]);
    i += UNPACK_UINT16_BE(p->soglia_z_accelerometro, &buffer[i]);
    i += UNPACK_UINT16_BE(p->soglia_x_accelerometro_h, &buffer[i]);
    i += UNPACK_UINT16_BE(p->soglia_y_accelerometro_h, &buffer[i]);
    i += UNPACK_UINT16_BE(p->soglia_z_accelerometro_h, &buffer[i]);
    i += UNPACK_UINT16_BE(p->giri_accelerometro, &buffer[i]);
    i += UNPACK_UINT16_BE(p->giri_accelerometro_2, &buffer[i]);
    i += UNPACK_UINT16_BE(p->delta_val_accelerometro, &buffer[i]);
    i += UNPACK_UINT16_BE(p->tempo_attesa_accelerometro, &buffer[i]);
    i += UNPACK_UINT16_BE(p->tempo_scarico_accelerometro, &buffer[i]);
    i += UNPACK_UINT16_BE(p->numero_saponi_utilizzabili, &buffer[i]);
    i += UNPACK_UINT16_BE(p->tempo_h2o_pulizia_saponi, &buffer[i]);
    i += UNPACK_UINT16_BE(p->tempo_precarica, &buffer[i]);
    i += UNPACK_UINT16_BE(p->tempo_tasto_carico_saponi, &buffer[i]);
    i += UNPACK_UINT16_BE(p->abilitazione_min_sec, &buffer[i]);
    i += UNPACK_UINT16_BE(p->abilitazione_sblocco_get, &buffer[i]);
    i += UNPACK_UINT16_BE(p->inibizione_allarmi, &buffer[i]);
    i += UNPACK_UINT16_BE(p->autoavvio, &buffer[i]);
    i += UNPACK_UINT16_BE(p->abilitazione_loop_prog, &buffer[i]);

    i += UNPACK_UINT16_BE(p->visualizzazione_data_ora, &buffer[i]);
    i += UNPACK_UINT16_BE(p->visualizzazione_esclusione_sapone, &buffer[i]);
    i += UNPACK_UINT16_BE(p->visualizzazione_pedante, &buffer[i]);

    memcpy(p->funzioni_rgb, &buffer[i], NUM_CONDIZIONI_MACCHINA);
    i += NUM_CONDIZIONI_MACCHINA;

    i += UNPACK_UINT16_BE(p->esclusione_sapone, &buffer[i]);
    i += UNPACK_UINT32_BE(p->valore_prezzo_unico, &buffer[i]);
    i += UNPACK_UINT16_BE(p->tipo_frontale, &buffer[i]);

    i += UNPACK_UINT16_BE(p->tipo_serratura, &buffer[i]);
    i += UNPACK_UINT16_BE(p->durata_impulso_serratura, &buffer[i]);

    assert(i == PARMAC_SIZE);
    return i;
}


void model_deserialize_statistics(statistics_t *stats, uint8_t *buffer) {
    size_t i = 0;

    i += deserialize_uint32_be(&stats->cicli_eseguiti, &buffer[i]);
    i += deserialize_uint32_be(&stats->cicli_interrotti, &buffer[i]);
    i += deserialize_uint32_be(&stats->cicli_loop, &buffer[i]);
    i += deserialize_uint32_be(&stats->tempo_accensione, &buffer[i]);
    i += deserialize_uint32_be(&stats->tempo_lavoro, &buffer[i]);
    i += deserialize_uint32_be(&stats->tempo_moto, &buffer[i]);
    i += deserialize_uint32_be(&stats->tempo_riscaldamento, &buffer[i]);
    i += deserialize_uint32_be(&stats->tempo_h2o_fredda, &buffer[i]);
    i += deserialize_uint32_be(&stats->tempo_h2o_calda, &buffer[i]);
    i += deserialize_uint32_be(&stats->tempo_h2o_rec_dep, &buffer[i]);
    i += deserialize_uint32_be(&stats->tempo_h2o_flusso, &buffer[i]);

    for (size_t j = 0; j < NUM_MAX_SAPONI; j++) {
        i += deserialize_uint32_be(&stats->tempo_saponi[j], &buffer[i]);
    }

    i += deserialize_uint32_be(&stats->chiusure_oblo, &buffer[i]);
    i += deserialize_uint32_be(&stats->aperture_oblo, &buffer[i]);
}

void model_unpack_stato_macchina(stato_macchina_t *stato, uint8_t *buffer) {
    int i = 0;

    stato->numero_programma    = buffer[i++];
    stato->numero_step         = buffer[i++];
    stato->codice_step         = buffer[i++];
    stato->sottostato_step     = buffer[i++];
    stato->codice_allarme      = buffer[i++];
    stato->oblo_aperto_chiuso  = buffer[i++];
    stato->chiavistello_chiuso = buffer[i++];
    stato->chiavistello_aperto = buffer[i++];

    stato->allarme_oblo_aperto    = buffer[i++];
    stato->allarme_oblo_sbloccato = buffer[i++];
    stato->allarme_chiavistello   = buffer[i++];     // chiavistello
    stato->allarme_sblocco_oblo   = buffer[i++];
    stato->allarme_oblo_h2o       = buffer[i++];

    for (int j = 0; j < LINEE_PAGAMENTO_GETTONIERA; j++) {
        i += deserialize_uint8(&stato->credito[j], &buffer[i]);
    }
    i += deserialize_uint8(&stato->pagato, &buffer[i]);

    stato->stato                         = buffer[i++];
    uint8_t sottostato                   = buffer[i++];
    stato->sottostato                    = sottostato & 0x7F;
    stato->richiesto_aggiornamento_tempo = (sottostato & 0x80) > 0;

    stato->rimanente = buffer[i] << 8 | buffer[i + 1];
    i += 2;

    stato->alt_tempo_durata = buffer[i++];

    stato->condizionamento_livello     = buffer[i++];
    stato->condizionamento_temperatura = buffer[i++];

    stato->allarme_errore_ram       = buffer[i++];
    stato->allarme_power_off        = buffer[i++];
    stato->allarme_emergenza        = buffer[i++];
    stato->allarme_inverter_ko      = buffer[i++];
    stato->allarme_sbilanciamento   = buffer[i++];
    stato->allarme_emergenza_marcia = buffer[i++];

    stato->livello = buffer[i] << 8 | buffer[i + 1];
    i += 2;
    stato->livello_litri = buffer[i] << 8 | buffer[i + 1];
    i += 2;
    stato->livello_impostato = buffer[i] << 8 | buffer[i + 1];
    i += 2;

    stato->livello_riscaldamento_ok = buffer[i++];
    stato->livello_ok               = buffer[i++];
    stato->livello_scarico_ok       = buffer[i++];

    stato->termodegradazione = buffer[i++];

    stato->acqua_fredda = buffer[i++];
    stato->acqua_calda  = buffer[i++];

    stato->allarme_no_riempimento = buffer[i++];
    stato->tempo_allarme_livello  = buffer[i] << 8 | buffer[i + 1];
    i += 2;

    stato->out_scarico = buffer[i++];

    stato->precarica       = buffer[i++];
    stato->tempo_precarica = buffer[i] << 8 | buffer[i + 1];
    i += 2;

    stato->allarme_no_scarico    = buffer[i++];
    stato->tempo_allarme_scarico = buffer[i] << 8 | buffer[i + 1];
    i += 2;
    stato->temperatura = buffer[i] << 8 | buffer[i + 1];
    i += 2;
    stato->temperatura_impostata = buffer[i] << 8 | buffer[i + 1];
    i += 2;

    stato->abilitazione_riscaldamento = buffer[i++];
    stato->temperatura_ok             = buffer[i++];
    stato->out_riscaldamento          = buffer[i++];

    stato->allarme_sonda_temperatura = buffer[i++];
    stato->allarme_sovratemperatura  = buffer[i++];
    stato->allarme_no_riscaldamento  = buffer[i++];
    stato->tempo_allarme_temperatura = buffer[i] << 8 | buffer[i + 1];
    i += 2;

    for (int j = 0; j < 6; j++)
        stato->out_saponi[j] = buffer[i++];
    for (int j = 0; j < 6; j++) {
        stato->ritardo_saponi[j] = buffer[i] << 8 | buffer[i + 1];
        i += 2;
    }
    for (int j = 0; j < 6; j++) {
        stato->tempo_saponi[j] = buffer[i] << 8 | buffer[i + 1];
        i += 2;
    }

    stato->alt_tempo_durata_saponi            = buffer[i++];
    stato->condizionamento_livello_saponi     = buffer[i++];
    stato->condizionamento_temperatura_saponi = buffer[i++];
    stato->out_flusso                         = buffer[i++];
    stato->tempo_flusso                       = buffer[i] << 8 | buffer[i + 1];
    i += 2;

    for (int j = 6; j < 10; j++)
        stato->out_saponi[j] = buffer[i++];
    for (int j = 6; j < 10; j++) {
        stato->ritardo_saponi[j] = buffer[i] << 8 | buffer[i + 1];
        i += 2;
    }
    for (int j = 6; j < 10; j++) {
        stato->tempo_saponi[j] = buffer[i] << 8 | buffer[i + 1];
        i += 2;
    }

    stato->abilitazione_moto_fermo_riempimento = buffer[i++];
    stato->abilitazione_inversione_riempimento = buffer[i++];
    stato->abilitazione_inversione_lavaggio    = buffer[i++];

    stato->tempo_giro_riempimento = buffer[i] << 8 | buffer[i + 1];
    i += 2;
    stato->tempo_giro_lavaggio = buffer[i] << 8 | buffer[i + 1];
    i += 2;
    stato->tempo_pausa_riempimento = buffer[i] << 8 | buffer[i + 1];
    i += 2;
    stato->tempo_pausa_lavaggio = buffer[i] << 8 | buffer[i + 1];
    i += 2;

    stato->cicli_sbilanciamento_massimi = buffer[i++];
    stato->tipo_sbilanciamento          = buffer[i++];
    stato->cicli_sbilanciamento         = buffer[i++];

    stato->abilitazione_preparazione = buffer[i++];
    stato->cicli_preparazione        = buffer[i++];

    stato->preparazione_centrifuga = buffer[i++];

    stato->out_motore_indietro = buffer[i++];
    stato->out_motore_avanti   = buffer[i++];

    stato->tempo_moto_cesto = buffer[i] << 8 | buffer[i + 1];
    i += 2;
    stato->velocita_rpm = buffer[i] << 8 | buffer[i + 1];
    i += 2;

    stato->sbilanciamento = buffer[i++];

    stato->frenata = buffer[i] << 8 | buffer[i + 1];
    i += 2;

    stato->vis_popup_frenata = buffer[i++];

    stato->velocita_volt = buffer[i] << 8 | buffer[i + 1];
    i += 2;

    stato->velocita_rilevata = buffer[i] << 8 | buffer[i + 1];
    i += 2;

    i += UNPACK_UINT8(stato->descrizione_pedante, &buffer[i]);
}


size_t model_pack_parametri_macchina(uint8_t *buffer, parmac_t *p) {
    size_t i = 0;

    i += serialize_uint16_be(&buffer[i], p->diametro_cesto);
    i += serialize_uint16_be(&buffer[i], p->profondita_cesto);
    i += serialize_uint16_be(&buffer[i], p->altezza_trappola);

    i += serialize_uint8(&buffer[i], p->correzione_contagiri);
    i += serialize_uint8(&buffer[i], p->tempo_allarme_livello);
    i += serialize_uint8(&buffer[i], p->tempo_allarme_temperatura);
    i += serialize_uint8(&buffer[i], p->tempo_allarme_scarico);
    i += serialize_uint8(&buffer[i], p->tempo_ritardo_micro_oblo);

    i += serialize_uint8(&buffer[i], p->tipo_out_aux_1);
    i += serialize_uint8(&buffer[i], p->tipo_out_aux_2);
    i += serialize_uint8(&buffer[i], p->tipo_out_aux_3);
    i += serialize_uint8(&buffer[i], p->tipo_out_aux_4);
    i += serialize_uint8(&buffer[i], 0);     // Removed, it's now fixed

    i += serialize_uint8(&buffer[i], 0);     // Removed, it's now fixed
    i += serialize_uint8(&buffer[i], p->f_scarico_recupero);
    i += serialize_uint8(&buffer[i], 1);     // Removed, it's now fixed
    i += serialize_uint8(&buffer[i], p->f_macchina_libera);
    i += serialize_uint8(&buffer[i], p->abilitazione_espansione_io);
    i += serialize_uint8(&buffer[i], p->tipo_gettoniera);
    i += serialize_uint8(&buffer[i], p->prezzo_unico);
    i += serialize_uint8(&buffer[i], p->valore_impulso);
    i += serialize_uint8(&buffer[i], p->temperatura_massima);
    i += serialize_uint8(&buffer[i], p->isteresi_temperatura);
    i += serialize_uint8(&buffer[i], p->temperatura_sicurezza);
    i += serialize_uint8(&buffer[i], p->temperatura_termodegradazione);
    i += serialize_uint8(&buffer[i], p->tipo_livello);
    i += serialize_uint16_be(&buffer[i], p->impulsi_litro);
    i += serialize_uint8(&buffer[i], p->tempo_isteresi_livello);
    i += serialize_uint16_be(&buffer[i], p->centimetri_max_livello);
    i += serialize_uint16_be(&buffer[i], p->livello_sfioro);
    i += serialize_uint8(&buffer[i], p->centimetri_minimo_scarico);
    i += serialize_uint16_be(&buffer[i], p->centimetri_minimo_riscaldo);
    i += serialize_uint16_be(&buffer[i], p->litri_massimi);
    i += serialize_uint16_be(&buffer[i], p->litri_minimi_riscaldo);
    i += serialize_uint8(&buffer[i], p->tempo_minimo_scarico);
    i += serialize_uint8(&buffer[i], p->tempo_scarico_servizio);
    i += serialize_uint8(&buffer[i], p->tempo_colpo_aperto_scarico);
    i += serialize_uint8(&buffer[i], p->tipo_inverter);
    i += serialize_uint8(&buffer[i], p->velocita_minima_lavaggio);
    i += serialize_uint8(&buffer[i], p->velocita_massima_lavaggio);
    i += serialize_uint8(&buffer[i], 0);
    i += serialize_uint8(&buffer[i], p->abilitazione_preparazione_rotazione);
    i += serialize_uint8(&buffer[i], p->tempo_marcia_preparazione_rotazione);
    i += serialize_uint8(&buffer[i], p->tempo_sosta_preparazione_rotazione);
    i += serialize_uint8(&buffer[i], p->velocita_minima_preparazione);
    i += serialize_uint8(&buffer[i], p->velocita_massima_preparazione);

    i += serialize_uint16_be(&buffer[i], p->velocita_minima_centrifuga_1);
    i += serialize_uint16_be(&buffer[i], p->velocita_massima_centrifuga_1);
    i += serialize_uint16_be(&buffer[i], p->velocita_minima_centrifuga_2);
    i += serialize_uint16_be(&buffer[i], p->velocita_massima_centrifuga_2);
    i += serialize_uint16_be(&buffer[i], p->velocita_minima_centrifuga_3);
    i += serialize_uint16_be(&buffer[i], p->velocita_massima_centrifuga_3);
    i += serialize_uint16_be(&buffer[i], p->tempo_minimo_rampa);
    i += serialize_uint16_be(&buffer[i], p->tempo_massimo_rampa);

    i += serialize_uint8(&buffer[i], p->nro_max_sbilanciamenti);
    i += serialize_uint8(&buffer[i], p->f_proximity);
    i += serialize_uint8(&buffer[i], p->tempo_minimo_frenata);
    i += serialize_uint8(&buffer[i], p->numero_raggi);
    i += serialize_uint8(&buffer[i], p->abilitazione_accelerometro);
    i += serialize_uint8(&buffer[i], p->scala_accelerometro);

    i += serialize_uint16_be(&buffer[i], p->soglia_x_accelerometro);
    i += serialize_uint16_be(&buffer[i], p->soglia_y_accelerometro);
    i += serialize_uint16_be(&buffer[i], p->soglia_z_accelerometro);
    i += serialize_uint16_be(&buffer[i], p->soglia_x_accelerometro_h);
    i += serialize_uint16_be(&buffer[i], p->soglia_y_accelerometro_h);
    i += serialize_uint16_be(&buffer[i], p->soglia_z_accelerometro_h);
    i += serialize_uint16_be(&buffer[i], p->giri_accelerometro);
    i += serialize_uint16_be(&buffer[i], p->giri_accelerometro_2);
    i += serialize_uint8(&buffer[i], p->delta_val_accelerometro);
    i += serialize_uint16_be(&buffer[i], p->tempo_attesa_accelerometro);
    i += serialize_uint16_be(&buffer[i], p->tempo_scarico_accelerometro);

    i += serialize_uint8(&buffer[i], p->numero_saponi_utilizzabili);
    i += serialize_uint8(&buffer[i], p->tempo_h2o_pulizia_saponi);
    i += serialize_uint8(&buffer[i], p->tempo_precarica);
    i += serialize_uint8(&buffer[i], p->tempo_tasto_carico_saponi);
    i += serialize_uint8(&buffer[i], p->abilitazione_min_sec);
    i += serialize_uint8(&buffer[i], p->abilitazione_sblocco_get);
    i += serialize_uint8(&buffer[i], p->inibizione_allarmi);
    i += serialize_uint8(&buffer[i], p->autoavvio);
    i += serialize_uint8(&buffer[i], p->abilitazione_loop_prog);

    memcpy(&buffer[i], p->funzioni_rgb, NUM_CONDIZIONI_MACCHINA);
    i += NUM_CONDIZIONI_MACCHINA;

    i += serialize_uint8(&buffer[i], p->esclusione_sapone);

    i += serialize_uint8(&buffer[i], p->tipo_serratura);
    i += serialize_uint8(&buffer[i], p->durata_impulso_serratura);

    return i;
}


void model_clear_test_data(mut_model_t *model) {
    assert(model);
    model->test.min_proximity_interval = 0;
    model->test.max_proximity_interval = 0;
}


void model_update_test_data(model_t *model, test_data_t test) {
    assert(model);

    model->test.inputs       = test.inputs;
    model->test.inputs_exp   = test.inputs_exp;
    model->test.adc_temp     = test.adc_temp;
    model->test.adc_press    = test.adc_press;
    model->test.offset_press = test.offset_press;

    if (model->test.min_proximity_interval == 0 || model->test.min_proximity_interval > test.pmin) {
        model->test.min_proximity_interval = test.pmin;
    }
    if (model->test.max_proximity_interval < test.pmax) {
        model->test.max_proximity_interval = test.pmax;
    }
    model->test.gettoniera_impulsi_abilitata = test.gettoniera_impulsi_abilitata;
    memcpy(model->test.minp, test.minp, sizeof(model->test.minp));
    memcpy(model->test.maxp, test.maxp, sizeof(model->test.maxp));
    model->test.accelerometro_ok = !test.errore_accelerometro;

    model->test.log_accelerometro[model->test.log_index][0] = test.accelerometer_axis[0];
    model->test.log_accelerometro[model->test.log_index][1] = test.accelerometer_axis[1];
    model->test.log_accelerometro[model->test.log_index][2] = test.accelerometer_axis[2];

    model->test.log_index = (model->test.log_index + 1) % MAX_LOG_ACCELEROMETRO;
}


void model_unpack_test(test_data_t *test, uint8_t *buffer) {
    uint32_t min, max;
    uint16_t x, y, z;
    size_t   i = 0;

    // ADC
    i += deserialize_uint16_be(&test->adc_press, &buffer[i]);
    i += deserialize_uint16_be(&test->adc_temp, &buffer[i]);
    i += deserialize_uint16_be(&test->offset_press, &buffer[i]);

    // Ingressi
    i += deserialize_uint16_be(&test->inputs, &buffer[i]);
    i += deserialize_uint8(&test->inputs_exp, &buffer[i]);

    // Accelerometro
    i += deserialize_uint16_be(&x, &buffer[i]);
    i += deserialize_uint16_be(&y, &buffer[i]);
    i += deserialize_uint16_be(&z, &buffer[i]);
    i += deserialize_uint8(&test->errore_accelerometro, &buffer[i]);

    test->accelerometer_axis[0] = x;
    test->accelerometer_axis[1] = y;
    test->accelerometer_axis[2] = z;

    // Proximity
    i += deserialize_uint32_be(&min, &buffer[i]);
    i += deserialize_uint32_be(&max, &buffer[i]);

    if (test->pmin == 0 || min < test->pmin) {
        test->pmin = min;
    }

    if (max > test->pmax) {
        test->pmax = max;
    }

    i += deserialize_uint8(&test->gettoniera_impulsi_abilitata, &buffer[i]);

    for (int j = 0; j < 3; j++) {
        i += deserialize_uint32_be(&test->minp[j], &buffer[i]);
    }
    for (int j = 0; j < 3; j++) {
        i += deserialize_uint32_be(&test->maxp[j], &buffer[i]);
    }
}


char *model_new_unique_filename(model_t *model, char *filename, unsigned long seed) {
    unsigned long now = seed;
    int           found;

    do {
        found = 0;
        snprintf(filename, STRING_NAME_SIZE, "%lu.bin", now);

        for (size_t i = 0; i < model->prog.num_programmi; i++) {
            if (strcmp(filename, model->prog.preview_programmi[i].filename) == 0) {
                now++;
                found = 1;
                break;
            }
        }
    } while (found);

    return filename;
}


int model_get_minimo_livello_riscaldo(model_t *model) {
    return model->prog.parmac.tipo_livello == 0 ? model->prog.parmac.centimetri_minimo_riscaldo
                                                : model->prog.parmac.litri_minimi_riscaldo;
}


static unsigned int get_credito_macchina(model_t *model) {
    switch (model->prog.parmac.tipo_gettoniera) {
        case PAGAMENTO_NESSUNO:
            return 0;

        case PAGAMENTO_1_LINEA_NA:
        case PAGAMENTO_1_LINEA_NC:
            return model_get_credito_gettoniera_impulsi(model, 1);

        case PAGAMENTO_2_LINEA_NA:
        case PAGAMENTO_2_LINEA_NC:
            return model_get_credito_gettoniera_impulsi(model, 2);

        case PAGAMENTO_DIGITALE:
            return model_get_credito_gettoniera_digitale(model);

        case PAGAMENTO_DIGITALE_LINEA_SINGOLA:
            return model->run.macchina.credito[LINEA_GETTONIERA_SINGOLA] * model->prog.parmac.valore_impulso;

        case PAGAMENTO_CASSA_NA:
        case PAGAMENTO_CASSA_NC:
            return model->run.macchina.credito[LINEA_GETTONIERA_CASSA] * model->prog.parmac.valore_impulso;
    }

    return 0;
}

int deserialize_step(parametri_step_t *s, uint8_t *buffer);

void program_deserialize_preview(model_t *pmodel, programma_preview_t *p, uint8_t *buffer, uint16_t lingua) {
    size_t i = lingua * STRING_NAME_SIZE;
    memcpy(p->name, &buffer[i], STRING_NAME_SIZE);

    i = MAX_LINGUE * STRING_NAME_SIZE;
    uint32_t prezzo;
    i += deserialize_uint32_be(&prezzo, &buffer[i]);
    p->prezzo = prezzo;
    i += UNPACK_UINT16_BE(p->tipo, &buffer[i]);

    uint16_t num_steps;
    i += deserialize_uint16_be(&num_steps, &buffer[i]);

    uint16_t num_washes           = 0;
    uint16_t max_temp             = 0;
    uint16_t max_level            = 0;
    uint16_t max_speed_centrifuge = 0;
    uint16_t max_speed_wash       = 0;
    uint16_t duration             = 0;

    parametri_step_t step = {0};
    for (size_t j = 0; j < num_steps; j++) {
        i += deserialize_step(&step, &buffer[i]);

        duration += step.durata;

        if (step.tipo == STEP_LAVAGGIO || step.tipo == STEP_AMMOLLO || step.tipo == STEP_PRELAVAGGIO ||
            step.tipo == STEP_RISCIACQUO) {
            num_washes++;
            if (step.livello > max_level) {
                max_level = step.livello;
            }
            if (step.velocita_riempimento > max_speed_wash) {
                max_speed_wash = step.velocita_riempimento;
            }
        }
        if (step.tipo == STEP_LAVAGGIO || step.tipo == STEP_AMMOLLO || step.tipo == STEP_PRELAVAGGIO ||
            step.tipo == STEP_RISCIACQUO || step.tipo == STEP_SCARICO || step.tipo == STEP_SROTOLAMENTO) {
            if (step.velocita_lavaggio > max_speed_wash) {
                max_speed_wash = step.velocita_lavaggio;
            }
        }
        if (step.tipo == STEP_LAVAGGIO || step.tipo == STEP_AMMOLLO || step.tipo == STEP_PRELAVAGGIO ||
            step.tipo == STEP_ATTESA) {
            if (step.temperatura > max_temp) {
                max_temp = step.temperatura;
            }
        }

        if (step.tipo == STEP_CENTRIFUGA) {
            unsigned int rampe[3]  = {step.tempo_rampa_1, step.tempo_rampa_2, step.tempo_rampa_3};
            unsigned int attese[3] = {step.tempo_attesa_centrifuga_1, step.tempo_attesa_centrifuga_2, 0};

            duration += step.tempo_frenata;
            for (size_t j = 0; j < step.numero_rampe; j++) {
                duration += rampe[j] + attese[j];
            }

            duration += step.tempo_preparazione;
            for (size_t j = 0; j < pmodel->prog.parmac.abilitazione_preparazione_rotazione; j++) {
                duration += pmodel->prog.parmac.tempo_marcia_preparazione_rotazione * 2 +
                            pmodel->prog.parmac.tempo_sosta_preparazione_rotazione * 2;
            }
        }
    }

    p->num_steps   = num_steps;
    p->lavaggi     = num_washes;
    p->temperatura = max_temp;
    p->livello     = max_level;
    p->velocita    = max_speed_centrifuge > 0 ? max_speed_centrifuge : max_speed_wash;
    p->durata      = duration;
}


void model_test_output_set(mut_model_t *model, uint16_t output, uint8_t value) {
    assert(model != NULL);
    if (value) {
        model->run.test_outputs_map |= 1 << output;
    } else {
        model->run.test_outputs_map &= ~(1 << output);
    }
}


void model_test_outputs_clear(mut_model_t *model) {
    assert(model != NULL);
    model->run.test_outputs_map = 0;
}


uint8_t model_is_test_output_active(model_t *model, uint16_t output) {
    assert(model != NULL);
    return (model->run.test_outputs_map & (1 << output)) > 0;
}


void model_test_outputs_clear_resistors(mut_model_t *model) {
    assert(model != NULL);
    model->run.test_outputs_map &= ~(1 << RESISTORS_OUTPUT_INDEX);
}


void model_reset_storage_operation(mut_model_t *model) {
    assert(model != NULL);
    model->system.storage_status = STORAGE_STATUS_READY;
}


uint8_t model_swap_programs(model_t *model, size_t first, size_t second) {
    if (first >= model->prog.num_programmi) {
        return 0;
    }
    if (second >= model->prog.num_programmi) {
        return 0;
    }
    if (first == second) {
        return 0;
    }

    programma_preview_t p                 = model->prog.preview_programmi[first];
    model->prog.preview_programmi[first]  = model->prog.preview_programmi[second];
    model->prog.preview_programmi[second] = p;

    return 1;
}


unsigned int model_get_total_remaining(model_t *model) {
    unsigned int           totale = 0;
    const parmac_t        *parmac = &model->prog.parmac;
    programma_lavatrice_t *p      = model_get_program(model);
    size_t                 step   = model->run.num_step_corrente;

    if (!p) {
        return 0;
    }

    for (size_t i = step; i < p->num_steps; i++) {
        const parametri_step_t *s = &p->steps[i];
        totale += s->durata;

        if (s->tipo == STEP_CENTRIFUGA) {
            unsigned int rampe[3]  = {s->tempo_rampa_1, s->tempo_rampa_2, s->tempo_rampa_3};
            unsigned int attese[3] = {s->tempo_attesa_centrifuga_1, s->tempo_attesa_centrifuga_2, 0};

            totale += s->tempo_frenata;
            for (size_t j = 0; j < s->numero_rampe; j++)
                totale += rampe[j] + attese[j];

            totale += s->tempo_preparazione;
            for (size_t j = 0; j < parmac->abilitazione_preparazione_rotazione; j++) {
                totale +=
                    parmac->tempo_marcia_preparazione_rotazione * 2 + parmac->tempo_sosta_preparazione_rotazione * 2;
            }
        }
    }

    return totale;
}


uint8_t model_is_emergency_ok(model_t *model) {
    return (model->test.inputs & 0x01) > 0;
}


uint8_t model_test_cesto_in_sicurezza(model_t *model) {
    return model_oblo_serrato(model) && model_get_livello_centimetri(model) == 0 && model_is_emergency_ok(model) &&
           (model->test.inputs & 0x02) && (model->test.inputs & (1 << 10));
}


void model_modify_temperature_setpoint(mut_model_t *model, int16_t modification) {
    assert(model != NULL);

    model->run.temperature_setpoint += modification;

    if (model->run.temperature_setpoint < 0) {
        model->run.temperature_setpoint = 0;
    } else if (model->run.temperature_setpoint > model->prog.parmac.temperatura_massima) {
        model->run.temperature_setpoint = model->prog.parmac.temperatura_massima;
    }
}


void model_modify_level_setpoint(mut_model_t *model, int16_t modification) {
    assert(model != NULL);
    uint16_t livmin, livmax;

    if (model_is_level_in_cm(&model->prog.parmac)) {
        livmin = model->prog.parmac.centimetri_minimo_riscaldo;
        livmax = model->prog.parmac.centimetri_max_livello;
    } else {
        livmin = model->prog.parmac.litri_minimi_riscaldo;
        livmax = model->prog.parmac.litri_massimi;
    }

    model->run.level_setpoint += modification;

    if (model->run.level_setpoint < livmin) {
        model->run.level_setpoint = livmin;
    } else if (model->run.level_setpoint > livmax) {
        model->run.level_setpoint = livmax;
    }
}


void model_modify_speed_setpoint(mut_model_t *model, int16_t modification) {
    assert(model != NULL);
    model->run.speed_setpoint += modification;

    if (model->run.speed_setpoint < model->prog.parmac.velocita_minima_lavaggio) {
        model->run.speed_setpoint = model->prog.parmac.velocita_minima_lavaggio;
    } else if (model->run.level_setpoint > model->prog.parmac.velocita_massima_lavaggio) {
        model->run.level_setpoint = model->prog.parmac.velocita_massima_lavaggio;
    }
}


size_t model_get_event_chunks(model_t *pmodel) {
    assert(pmodel != NULL);
    size_t res = pmodel->run.total_event_log_number / EVENT_LOG_CHUNK;
    if (res == 0) {
        return 1;
    } else if ((pmodel->run.total_event_log_number % EVENT_LOG_CHUNK) == 0) {
        return res;
    } else {
        return res + 1;
    }
}
