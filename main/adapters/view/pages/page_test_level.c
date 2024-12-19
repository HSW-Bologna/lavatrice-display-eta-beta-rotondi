#include "lvgl.h"
#include "model/model.h"
#include "../view.h"
#include "../theme/style.h"
#include "../common.h"
#include "config/app_config.h"
#include "src/widgets/led/lv_led.h"
#include "../intl/intl.h"


#define OUTPUT_WARM_WATER 3
#define OUTPUT_COLD_WATER 4
#define OUTPUT_DRAIN      5


struct page_data {
    lv_obj_t *label_data;

    lv_obj_t *button_calibration;
    lv_obj_t *button_warm_water;
    lv_obj_t *button_cold_water;
    lv_obj_t *button_drain;

    lv_obj_t *led_heating;
    lv_obj_t *led_porthole;
    lv_obj_t *led_temperature;
    lv_obj_t *led_warm_water;
    lv_obj_t *led_emergency;

    uint8_t test_temperature;
};


enum {
    BTN_BACK_ID,
    BTN_NEXT_ID,
    BTN_LOCK_ID,
    BTN_HEATING_ID,
    BTN_WARM_WATER_ID,
    BTN_COLD_WATER_ID,
    BTN_DRAIN_ID,
    BTN_CALIBRATION_ID,
};


static int  test_acqua_in_sicurezza(model_t *model, struct page_data *data);
static int  test_resistenze_in_sicurezza(model_t *model, struct page_data *data);
static void update_page(model_t *model, struct page_data *pdata);


static void *create_page(pman_handle_t handle, void *extra) {
    (void)handle;
    (void)extra;

    struct page_data *pdata = lv_malloc(sizeof(struct page_data));
    assert(pdata != NULL);

    pdata->test_temperature = (uint8_t)(uintptr_t)extra;

    return pdata;
}


static void open_page(pman_handle_t handle, void *state) {
    struct page_data *pdata = state;

    model_t *model = view_get_model(handle);

    view_common_create_title(
        lv_scr_act(), view_intl_get_string(model, pdata->test_temperature ? STRINGS_TEMPERATURA : STRINGS_LIVELLO),
        BTN_BACK_ID, BTN_NEXT_ID);

    lv_obj_t *cont = lv_obj_create(lv_scr_act());
    lv_obj_set_size(cont, LV_HOR_RES, LV_VER_RES - 56);
    lv_obj_align(cont, LV_ALIGN_BOTTOM_MID, 0, 0);

    lv_obj_t *lbl = lv_label_create(cont);
    lv_obj_set_style_text_font(lbl, STYLE_FONT_SMALL, LV_STATE_DEFAULT);
    lv_obj_align(lbl, LV_ALIGN_TOP_LEFT, 0, 0);
    pdata->label_data = lbl;

    {
        lv_obj_t *btn = lv_button_create(cont);
        lv_obj_set_size(btn, 128, 48);
        lv_obj_t *lbl = lv_label_create(btn);
        lv_obj_set_style_text_font(lbl, STYLE_FONT_SMALL, LV_STATE_DEFAULT);
        lv_label_set_text(lbl, view_intl_get_string(model, STRINGS_CALIBRAZIONE));
        lv_obj_center(lbl);
        view_register_object_default_callback(btn, BTN_CALIBRATION_ID);
        lv_obj_align(btn, LV_ALIGN_TOP_LEFT, 0, 56);
        pdata->button_calibration = btn;
    }

    {
        lv_obj_t *btn = lv_button_create(cont);
        lv_obj_remove_flag(btn, LV_OBJ_FLAG_SCROLLABLE);
        lv_obj_set_style_radius(btn, 4, LV_STATE_DEFAULT);
        view_register_object_default_callback(btn, BTN_HEATING_ID);
        lv_obj_set_size(btn, 128, 48);

        lv_obj_t *led = lv_led_create(btn);
        lv_led_set_color(led, VIEW_STYLE_COLOR_RED);
        lv_obj_set_size(led, 28, 28);
        lv_obj_align(led, LV_ALIGN_RIGHT_MID, 0, 0);
        lv_obj_add_flag(led, LV_OBJ_FLAG_EVENT_BUBBLE);

        lv_obj_t *lbl = lv_label_create(btn);
        lv_obj_set_style_text_font(lbl, STYLE_FONT_SMALL, LV_STATE_DEFAULT);
        lv_label_set_text(lbl, "Risc.");
        lv_obj_align(lbl, LV_ALIGN_LEFT_MID, 0, 0);
        pdata->led_heating = led;

        lv_obj_align(btn, LV_ALIGN_TOP_RIGHT, 0, 56);
    }

    {
        lv_obj_t *bottom = lv_obj_create(cont);
        lv_obj_remove_flag(bottom, LV_OBJ_FLAG_SCROLLABLE);
        lv_obj_add_style(bottom, &style_transparent_cont, LV_STATE_DEFAULT);
        lv_obj_set_style_pad_column(bottom, 16, LV_STATE_DEFAULT);
        lv_obj_set_size(bottom, LV_HOR_RES, 52);
        lv_obj_set_layout(bottom, LV_LAYOUT_FLEX);
        lv_obj_set_flex_flow(bottom, LV_FLEX_FLOW_ROW);
        lv_obj_set_flex_align(bottom, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
        lv_obj_set_style_pad_bottom(bottom, 2, LV_STATE_DEFAULT);
        lv_obj_align(bottom, LV_ALIGN_BOTTOM_MID, 0, -56);

        {
            lv_obj_t *btn = lv_button_create(bottom);
            lv_obj_remove_flag(btn, LV_OBJ_FLAG_SCROLLABLE);
            lv_obj_set_style_radius(btn, 4, LV_STATE_DEFAULT);
            view_register_object_default_callback(btn, BTN_WARM_WATER_ID);
            lv_obj_set_size(btn, 84, 48);
            lv_obj_add_flag(btn, LV_OBJ_FLAG_CHECKABLE);
            pdata->button_warm_water = btn;

            lv_obj_t *lbl = lv_label_create(btn);
            lv_obj_set_style_text_font(lbl, STYLE_FONT_SMALL, LV_STATE_DEFAULT);
            lv_label_set_text(lbl, "CALDA");
            lv_obj_center(lbl);
        }

        {
            lv_obj_t *btn = lv_button_create(bottom);
            lv_obj_remove_flag(btn, LV_OBJ_FLAG_SCROLLABLE);
            lv_obj_set_style_radius(btn, 4, LV_STATE_DEFAULT);
            view_register_object_default_callback(btn, BTN_COLD_WATER_ID);
            lv_obj_set_size(btn, 84, 48);
            lv_obj_add_flag(btn, LV_OBJ_FLAG_CHECKABLE);
            pdata->button_cold_water = btn;

            lv_obj_t *lbl = lv_label_create(btn);
            lv_obj_set_style_text_font(lbl, STYLE_FONT_SMALL, LV_STATE_DEFAULT);
            lv_label_set_text(lbl, "FREDDA");
            lv_obj_center(lbl);
        }

        {
            lv_obj_t *btn = lv_button_create(bottom);
            lv_obj_remove_flag(btn, LV_OBJ_FLAG_SCROLLABLE);
            lv_obj_set_style_radius(btn, 4, LV_STATE_DEFAULT);
            view_register_object_default_callback(btn, BTN_DRAIN_ID);
            lv_obj_set_size(btn, 84, 48);
            lv_obj_add_flag(btn, LV_OBJ_FLAG_CHECKABLE);
            pdata->button_drain = btn;

            lv_obj_t *lbl = lv_label_create(btn);
            lv_obj_set_style_text_font(lbl, STYLE_FONT_SMALL, LV_STATE_DEFAULT);
            lv_label_set_text(lbl, "SCARICO");
            lv_obj_center(lbl);
        }
    }

    {
        lv_obj_t *bottom = lv_obj_create(cont);
        lv_obj_remove_flag(bottom, LV_OBJ_FLAG_SCROLLABLE);
        lv_obj_add_style(bottom, &style_transparent_cont, LV_STATE_DEFAULT);
        lv_obj_set_style_pad_column(bottom, 16, LV_STATE_DEFAULT);
        lv_obj_set_size(bottom, LV_HOR_RES, 52);
        lv_obj_set_layout(bottom, LV_LAYOUT_FLEX);
        lv_obj_set_flex_flow(bottom, LV_FLEX_FLOW_ROW);
        lv_obj_set_flex_align(bottom, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
        lv_obj_set_style_pad_bottom(bottom, 2, LV_STATE_DEFAULT);
        lv_obj_align(bottom, LV_ALIGN_BOTTOM_MID, 0, 0);

        {
            lv_obj_t *btn = lv_button_create(bottom);
            lv_obj_remove_flag(btn, LV_OBJ_FLAG_SCROLLABLE);
            lv_obj_set_style_radius(btn, 4, LV_STATE_DEFAULT);
            view_register_object_default_callback(btn, BTN_LOCK_ID);
            lv_obj_set_size(btn, 64, 48);

            lv_obj_t *led = lv_led_create(btn);
            lv_led_set_color(led, VIEW_STYLE_COLOR_RED);
            lv_obj_set_size(led, 28, 28);
            lv_obj_center(led);
            lv_obj_add_flag(led, LV_OBJ_FLAG_EVENT_BUBBLE);

            lv_obj_t *lbl = lv_label_create(btn);
            lv_obj_set_style_text_font(lbl, STYLE_FONT_SMALL, LV_STATE_DEFAULT);
            lv_label_set_text(lbl, "OBLO'");
            lv_obj_center(lbl);
            pdata->led_porthole = led;
        }
        view_common_alarm_led_create(bottom, &pdata->led_temperature, "TEMP.");
        view_common_alarm_led_create(bottom, &pdata->led_emergency, "EMER.");
        view_common_alarm_led_create(bottom, &pdata->led_warm_water, "ACQ.");
    }

    VIEW_ADD_WATCHED_VARIABLE(&model->test.adc_press, 0);
    VIEW_ADD_WATCHED_VARIABLE(&model->test.offset_press, 0);
    VIEW_ADD_WATCHED_VARIABLE(&model->run.test_outputs_map, 0);
    VIEW_ADD_WATCHED_VARIABLE(&model->test.adc_temp, 0);
    VIEW_ADD_WATCHED_VARIABLE(&model->run.macchina.temperatura, 0);

    update_page(model, pdata);
}


static pman_msg_t page_event(pman_handle_t handle, void *state, pman_event_t event) {
    struct page_data *pdata = state;

    model_t *model = view_get_model(handle);

    pman_msg_t msg = PMAN_MSG_NULL;

    switch (event.tag) {
        case PMAN_EVENT_TAG_OPEN:
            view_get_protocol(handle)->set_test_mode(handle, 1);
            break;

        case PMAN_EVENT_TAG_USER: {
            view_event_t *view_event = event.as.user;
            switch (view_event->tag) {
                case VIEW_EVENT_TAG_PAGE_WATCHER: {
                    if (model_is_test_output_active(model, RESISTORS_OUTPUT_INDEX) &&
                        !test_resistenze_in_sicurezza(model, pdata)) {
                        view_get_protocol(handle)->clear_outputs(handle);
                    } else if (model_is_test_output_active(model, OUTPUT_COLD_WATER) ||
                               model_is_test_output_active(model, OUTPUT_WARM_WATER) ||
                               model_is_test_output_active(model, OUTPUT_DRAIN)) {
                        if (!test_acqua_in_sicurezza(model, pdata)) {
                            view_get_protocol(handle)->clear_outputs(handle);
                        }
                    }
                    update_page(model, pdata);
                    break;
                }

                default:
                    break;
            }
            break;
        }

        case PMAN_EVENT_TAG_LVGL: {
            lv_obj_t           *target   = lv_event_get_current_target_obj(event.as.lvgl);
            view_object_data_t *obj_data = lv_obj_get_user_data(target);

            switch (lv_event_get_code(event.as.lvgl)) {
                case LV_EVENT_CLICKED: {
                    switch (obj_data->id) {
                        case BTN_BACK_ID:
                            view_get_protocol(handle)->set_test_mode(handle, 0);
                            msg.stack_msg = PMAN_STACK_MSG_BACK();
                            break;

                        case BTN_NEXT_ID:
                            if (pdata->test_temperature) {
                                msg.stack_msg = PMAN_STACK_MSG_SWAP(&page_test_coins_digital);
                            } else {
                                msg.stack_msg = PMAN_STACK_MSG_SWAP_EXTRA(&page_test_level, (void *)(uintptr_t)1);
                            }
                            break;

                        case BTN_CALIBRATION_ID:
                            view_get_protocol(handle)->pressure_calibration(handle);
                            break;

                        case BTN_HEATING_ID: {
                            if (model_is_test_output_active(model, RESISTORS_OUTPUT_INDEX)) {
                                view_get_protocol(handle)->set_output_overlapping(handle, RESISTORS_OUTPUT_INDEX, 0);
                            } else {
                                view_get_protocol(handle)->set_output_overlapping(handle, RESISTORS_OUTPUT_INDEX, 1);
                            }
                            update_page(model, pdata);
                            break;
                        }

                        case BTN_WARM_WATER_ID: {
                            if (model_is_test_output_active(model, OUTPUT_WARM_WATER)) {
                                view_get_protocol(handle)->set_output_overlapping(handle, OUTPUT_WARM_WATER, 0);
                            } else {
                                view_get_protocol(handle)->set_output_overlapping(handle, OUTPUT_WARM_WATER, 1);
                            }
                            update_page(model, pdata);
                            break;
                        }

                        case BTN_COLD_WATER_ID: {
                            if (model_is_test_output_active(model, OUTPUT_COLD_WATER)) {
                                view_get_protocol(handle)->set_output_overlapping(handle, OUTPUT_COLD_WATER, 0);
                            } else {
                                view_get_protocol(handle)->set_output_overlapping(handle, OUTPUT_COLD_WATER, 1);
                            }
                            update_page(model, pdata);
                            break;
                        }

                        case BTN_DRAIN_ID: {
                            if (model_is_test_output_active(model, OUTPUT_DRAIN)) {
                                view_get_protocol(handle)->set_output_overlapping(handle, OUTPUT_DRAIN, 0);
                            } else {
                                view_get_protocol(handle)->set_output_overlapping(handle, OUTPUT_DRAIN, 1);
                            }
                            update_page(model, pdata);
                            break;
                        }

                        case BTN_LOCK_ID: {
                            view_get_protocol(handle)->toggle_lock(handle);
                            break;
                        }
                    }
                    break;
                }

                case LV_EVENT_LONG_PRESSED: {
                    switch (obj_data->id) {
                        case BTN_NEXT_ID:
                            if (pdata->test_temperature) {
                                msg.stack_msg = PMAN_STACK_MSG_SWAP_EXTRA(&page_test_level, (void *)(uintptr_t)0);
                            } else {
                                msg.stack_msg = PMAN_STACK_MSG_SWAP_EXTRA(&page_test_drum, (void *)(uintptr_t)2);
                            }
                            break;
                    }
                    break;
                }

                default:
                    break;
            }
            break;
        }

        default:
            break;
    }

    return msg;
}


static int test_acqua_in_sicurezza(model_t *model, struct page_data *data) {
    (void)data;
    return model_is_emergency_ok(model) && model_oblo_serrato(model);
}


static int test_resistenze_in_sicurezza(model_t *model, struct page_data *data) {
    (void)data;
    return (
        model_is_emergency_ok(model) && model->run.macchina.temperatura < model->prog.parmac.temperatura_sicurezza &&
        model_get_livello_centimetri(model) >= model_get_minimo_livello_riscaldo(model) && model_oblo_serrato(model));
}


static void update_page(model_t *model, struct page_data *pdata) {
    (void)model;
    (void)pdata;

    uint16_t adc = 0;
    if (model->test.adc_press > model->test.offset_press) {
        adc = model->test.adc_press - model->test.offset_press;
    }

    if (pdata->test_temperature) {
        view_common_set_hidden(pdata->button_calibration, 1);
        lv_label_set_text_fmt(pdata->label_data, "%s: %3i C [%04i]", view_intl_get_string(model, STRINGS_TEMPERATURA),
                              model->run.macchina.temperatura, model->test.adc_temp);
    } else {
        view_common_set_hidden(pdata->button_calibration, 0);
        lv_label_set_text_fmt(pdata->label_data, "%s: %3i cm [%04i]\nOffset: %4i [%04i]",
                              view_intl_get_string(model, STRINGS_LIVELLO), model_get_livello_centimetri(model), adc,
                              model->test.offset_press, model->test.adc_press);
    }

    if (model_is_test_output_active(model, OUTPUT_WARM_WATER)) {
        lv_obj_add_state(pdata->button_warm_water, LV_STATE_CHECKED);
    } else {
        lv_obj_remove_state(pdata->button_warm_water, LV_STATE_CHECKED);
    }

    if (model_is_test_output_active(model, OUTPUT_COLD_WATER)) {
        lv_obj_add_state(pdata->button_cold_water, LV_STATE_CHECKED);
    } else {
        lv_obj_remove_state(pdata->button_cold_water, LV_STATE_CHECKED);
    }

    if (model_is_test_output_active(model, OUTPUT_DRAIN)) {
        lv_obj_add_state(pdata->button_drain, LV_STATE_CHECKED);
    } else {
        lv_obj_remove_state(pdata->button_drain, LV_STATE_CHECKED);
    }

    if (model_is_emergency_ok(model)) {
        lv_led_off(pdata->led_emergency);
    } else {
        lv_led_on(pdata->led_emergency);
    }

    if (model_oblo_serrato(model)) {
        lv_led_off(pdata->led_porthole);
    } else {
        lv_led_on(pdata->led_porthole);
    }

    if (model->run.macchina.temperatura >= model->prog.parmac.temperatura_sicurezza) {
        lv_led_off(pdata->led_temperature);
    } else {
        lv_led_on(pdata->led_temperature);
    }

    if (model_get_livello_centimetri(model) < model->prog.parmac.centimetri_max_livello) {
        lv_led_off(pdata->led_warm_water);
    } else {
        lv_led_on(pdata->led_warm_water);
    }

    if (model_is_test_output_active(model, RESISTORS_OUTPUT_INDEX)) {
        lv_led_on(pdata->led_heating);
    } else {
        lv_led_off(pdata->led_heating);
    }
}


const pman_page_t page_test_level = {
    .create        = create_page,
    .destroy       = pman_destroy_all,
    .open          = open_page,
    .close         = pman_close_all,
    .process_event = page_event,
};
