#include "model/model.h"
#include "adapters/view/view.h"
#include "controller.h"
#include "esp_log.h"
#include "services/timestamp.h"
#include "controller/com/machine.h"
#include "controller/configuration/configuration.h"
#include "bsp/msc.h"
#include "bsp/buzzer.h"
#include "lvgl.h"


static const char *TAG = "Gui";


static void retry_communication(pman_handle_t handle);
static void set_output(pman_handle_t handle, uint16_t output, uint8_t level);
static void clear_outputs(pman_handle_t handle);
static void set_test_mode(pman_handle_t handle, uint8_t test_mode);
static void import_configuration(pman_handle_t handle, const char *name);
static void pressure_calibration(pman_handle_t handle);
static int  load_program(pman_handle_t handle, size_t number);
static void save_parmac(pman_handle_t handle);
static void start_program(pman_handle_t handle);
static void stop_program(pman_handle_t handle);
static void pause_program(pman_handle_t handle);
static void test_dac(pman_handle_t handle, uint8_t value);
static void lock_porthole(pman_handle_t handle);
static void unlock_porthole(pman_handle_t handle, uint8_t force);
static void toggle_lock(pman_handle_t handle);
static void new_program(pman_handle_t handle, uint16_t program_index);
static void beep(void);


view_protocol_t controller_gui_protocol = {
    .retry_communication  = retry_communication,
    .set_output           = set_output,
    .clear_outputs        = clear_outputs,
    .set_test_mode        = set_test_mode,
    .pressure_calibration = pressure_calibration,
    .import_configuration = import_configuration,
    .load_program         = load_program,
    .save_parmac          = save_parmac,
    .start_program        = start_program,
    .stop_program         = stop_program,
    .pause_program        = pause_program,
    .test_dac             = test_dac,
    .unlock_porthole      = unlock_porthole,
    .lock_porthole        = lock_porthole,
    .toggle_lock          = toggle_lock,
    .new_program          = new_program,
    .beep                 = beep,
};


void controller_gui_manage(model_t *model) {
#ifndef BUILD_CONFIG_SIMULATED_APP
    static timestamp_t last_invoked = 0;

    if (last_invoked != timestamp_get()) {
        if (last_invoked > 0) {
            lv_tick_inc(timestamp_interval(last_invoked, timestamp_get()));
        }
        last_invoked = timestamp_get();
    }
#endif

    lv_timer_handler();

    view_manage(model);
}


static void retry_communication(pman_handle_t handle) {
    ESP_LOGI(TAG, "Requesting com retry");
    machine_riavvia_comunicazione();

    mut_model_t *model = view_get_model(handle);

    machine_invia_parmac(&model->prog.parmac);
    model->system.errore_comunicazione = 0;
}


static void set_output(pman_handle_t handle, uint16_t output, uint8_t level) {
    mut_model_t *model = view_get_model(handle);

    model_test_output_activate(model, output);
    machine_imposta_uscita_singola(output, level);
}


static void clear_outputs(pman_handle_t handle) {
    mut_model_t *model = view_get_model(handle);

    model_test_outputs_clear(model);
    machine_imposta_uscita_singola(0, 0);
}


static void set_test_mode(pman_handle_t handle, uint8_t test_mode) {
    mut_model_t *model = view_get_model(handle);

    model->run.test_mode = test_mode;
    machine_test(model->run.test_mode);
}


static void pressure_calibration(pman_handle_t handle) {
    (void)handle;
    machine_offset_pressione();
}


static void import_configuration(pman_handle_t handle, const char *name) {
    mut_model_t *model = view_get_model(handle);

    model->system.storage_status = STORAGE_STATUS_LOADING;
    msc_extract_archive(name);
}


static int load_program(pman_handle_t handle, size_t number) {
    mut_model_t *model = view_get_model(handle);

    if (configuration_load_program(model, number)) {
        return -1;
    } else {
        return 0;
    }
}


static void save_parmac(pman_handle_t handle) {
    mut_model_t *model = view_get_model(handle);

    configuration_save_parmac(&model->prog.parmac);
    //  Invia i parametri macchina
    machine_invia_parmac(&model->prog.parmac);
}


static void start_program(pman_handle_t handle) {
    mut_model_t *model = view_get_model(handle);
    controller_start_program(model);
}


static void stop_program(pman_handle_t handle) {
    (void)handle;
    controller_stop_program();
}


static void pause_program(pman_handle_t handle) {
    (void)handle;
    controller_pause_program();
}


static void test_dac(pman_handle_t handle, uint8_t value) {
    (void)handle;
    machine_imposta_dac(value);
}


static void unlock_porthole(pman_handle_t handle, uint8_t force) {
    (void)handle;
    machine_apri_oblo(force);
}


static void lock_porthole(pman_handle_t handle) {
    (void)handle;
    machine_chiudi_oblo();
}


static void toggle_lock(pman_handle_t handle) {
    mut_model_t *model = view_get_model(handle);

    if (model_oblo_serrato(model)) {
        ESP_LOGI(TAG, "Apertura oblo'");
        machine_apri_oblo(0);
    } else if (model_oblo_libero(model) && model_oblo_chiuso(model)) {
        ESP_LOGI(TAG, "Chiusura oblo'");
        machine_chiudi_oblo();
    } else {
        ESP_LOGI(TAG, "Oblo' aperto, non posso fare nulla");
    }
}


static void new_program(pman_handle_t handle, uint16_t program_index) {
    mut_model_t *model = view_get_model(handle);

    configuration_create_empty_program(model);
    model->prog.num_programmi = configuration_load_programs_preview(model, model->prog.preview_programmi, MAX_PROGRAMMI,
                                                                    model_get_language(model));
    configuration_clear_orphan_programs(model->prog.preview_programmi, model->prog.num_programmi);
}


static void beep(void) {
    buzzer_beep(1, 50, 50, 3);
}
