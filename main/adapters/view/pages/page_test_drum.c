#include "lvgl.h"
#include "model/model.h"
#include "../view.h"
#include "../theme/style.h"
#include "../common.h"
#include "config/app_config.h"
#include "src/widgets/led/lv_led.h"
#include "../intl/intl.h"
#include "esp_log.h"


#define DA2RPM(da)   ((da * 1000) / 100)
#define OUT_AVANTI   14
#define OUT_INDIETRO 13


struct page_data {
    lv_obj_t *label_da;
    lv_obj_t *label_values;
    lv_obj_t *label_signal;
    lv_obj_t *label_run;
    lv_obj_t *label_level;
    lv_obj_t *label_control;
    lv_obj_t *label_proximity;
    lv_obj_t *label_accelerometer_off;

    lv_obj_t *chart;

    lv_obj_t *scale;

    lv_obj_t *button_control;
    lv_obj_t *button_clear;
    lv_obj_t *button_clear_alarms;

    lv_obj_t *led_porthole;
    lv_obj_t *led_inverter;
    lv_obj_t *led_lurching;
    lv_obj_t *led_emergency;
    lv_obj_t *led_water;

    lv_chart_series_t *ser_x;
    lv_chart_series_t *ser_y;
    lv_chart_series_t *ser_z;

    uint8_t                           da_value;
    uint8_t                           run;
    view_common_page_test_drum_type_t page_type;
};


enum {
    BTN_BACK_ID,
    BTN_NEXT_ID,
    BTN_MINUS_ID,
    BTN_PLUS_ID,
    BTN_CONTROL_ID,
    BTN_CLEAR_ID,
    BTN_CLEAR_ALARMS_ID,
    BTN_LOCK_ID,
};


static void      update_page(model_t *model, struct page_data *pdata);
static lv_obj_t *alarm_led_create(lv_obj_t *parent, lv_obj_t **led, const char *text);


static const char *TAG = "PageTestDrum";


static void *create_page(pman_handle_t handle, void *extra) {
    (void)handle;
    (void)extra;

    struct page_data *pdata = lv_malloc(sizeof(struct page_data));
    assert(pdata != NULL);

    pdata->da_value  = 0;
    pdata->run       = 0;
    pdata->page_type = (view_common_page_test_drum_type_t)(uintptr_t)extra;

    return pdata;
}


static void open_page(pman_handle_t handle, void *state) {
    struct page_data *pdata = state;

    model_t *model = view_get_model(handle);

    switch (pdata->page_type) {
        case VIEW_COMMON_PAGE_TEST_DRUM_TYPE_BACKWARD:
            view_common_create_title(lv_scr_act(), view_intl_get_string(model, STRINGS_MOTORE_INDIETRO), BTN_BACK_ID,
                                     BTN_NEXT_ID);
            break;
        case VIEW_COMMON_PAGE_TEST_DRUM_TYPE_FORWARD:
            view_common_create_title(lv_scr_act(), view_intl_get_string(model, STRINGS_MOTORE_AVANTI), BTN_BACK_ID,
                                     BTN_NEXT_ID);
            break;
        default:
            view_common_create_title(lv_scr_act(), view_intl_get_string(model, STRINGS_ACCELEROMETRO), BTN_BACK_ID,
                                     BTN_NEXT_ID);
            break;
    }

    lv_obj_t *cont = lv_obj_create(lv_scr_act());
    lv_obj_set_style_pad_top(cont, 6, LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(cont, 0, LV_STATE_DEFAULT);
    lv_obj_clear_flag(cont, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_size(cont, LV_HOR_RES, LV_VER_RES - 56);
    lv_obj_align(cont, LV_ALIGN_BOTTOM_MID, 0, 0);

    {
        lv_obj_t *btn = lv_button_create(cont);
        lv_obj_set_size(btn, 80, 40);
        lv_obj_t *lbl = lv_label_create(btn);
        lv_obj_set_style_text_font(lbl, STYLE_FONT_SMALL, LV_STATE_DEFAULT);
        lv_label_set_text(lbl, LV_SYMBOL_MINUS);
        lv_obj_center(lbl);
        view_register_object_default_callback(btn, BTN_MINUS_ID);
        lv_obj_align(btn, LV_ALIGN_TOP_LEFT, 0, 0);
    }

    {
        lv_obj_t *btn = lv_button_create(cont);
        lv_obj_set_size(btn, 80, 40);
        lv_obj_t *lbl = lv_label_create(btn);
        lv_obj_set_style_text_font(lbl, STYLE_FONT_SMALL, LV_STATE_DEFAULT);
        lv_label_set_text(lbl, LV_SYMBOL_PLUS);
        lv_obj_center(lbl);
        view_register_object_default_callback(btn, BTN_PLUS_ID);
        lv_obj_align(btn, LV_ALIGN_TOP_RIGHT, 0, 0);
    }

    {
        lv_obj_t *btn = lv_button_create(cont);
        lv_obj_set_size(btn, 80, 40);
        lv_obj_t *lbl = lv_label_create(btn);
        lv_obj_set_style_text_font(lbl, STYLE_FONT_SMALL, LV_STATE_DEFAULT);
        lv_label_set_text(lbl, "CLEAR");
        lv_obj_center(lbl);
        view_register_object_default_callback(btn, BTN_CLEAR_ID);
        lv_obj_align(btn, LV_ALIGN_RIGHT_MID, 0, 24);
        pdata->button_clear = btn;
    }

    {
        lv_obj_t *btn = lv_button_create(cont);
        lv_obj_set_size(btn, 96, 40);
        lv_obj_t *lbl = lv_label_create(btn);
        lv_obj_set_style_text_font(lbl, STYLE_FONT_SMALL, LV_STATE_DEFAULT);
        lv_obj_center(lbl);
        view_register_object_default_callback(btn, BTN_CONTROL_ID);
        lv_obj_align(btn, LV_ALIGN_TOP_MID, 0, 0);
        pdata->label_control  = lbl;
        pdata->button_control = btn;
    }

    {
        lv_obj_t *lbl = lv_label_create(cont);
        lv_obj_set_style_text_font(lbl, STYLE_FONT_SMALL, LV_STATE_DEFAULT);
        lv_obj_align(lbl, LV_ALIGN_TOP_LEFT, 0, 52);
        pdata->label_da = lbl;
    }
    /*Create a chart*/
    {
        lv_obj_t *chart = lv_chart_create(cont);
        lv_obj_set_size(chart, LV_HOR_RES - 40, 120);
        lv_obj_align(chart, LV_ALIGN_CENTER, 12, 12);
        lv_chart_set_type(chart, LV_CHART_TYPE_LINE);
        lv_chart_set_point_count(chart, MAX_LOG_ACCELEROMETRO);
        lv_chart_set_range(chart, LV_CHART_AXIS_PRIMARY_Y, 0, 512 + 128);
        lv_chart_set_div_line_count(chart, 4, MAX_LOG_ACCELEROMETRO);
        lv_obj_set_style_width(chart, 0, LV_PART_ITEMS);
        lv_obj_set_style_height(chart, 0, LV_PART_ITEMS);
        lv_obj_set_style_width(chart, 0, LV_PART_INDICATOR);
        lv_obj_set_style_height(chart, 0, LV_PART_INDICATOR);

        lv_obj_t *label = lv_label_create(cont);
        lv_obj_align(label, LV_ALIGN_CENTER, 0, -16);
        lv_label_set_text(label, view_intl_get_string(model, STRINGS_ALLARME_ACCELEROMETRO));
        lv_obj_set_style_text_font(label, STYLE_FONT_SMALL, LV_STATE_DEFAULT);
        pdata->label_accelerometer_off = label;

        {
            lv_obj_t *button = lv_button_create(cont);
            lv_obj_set_size(button, 96, 48);
            lv_obj_t *label = lv_label_create(button);
            lv_obj_center(label);
            lv_obj_set_style_text_font(label, STYLE_FONT_SMALL, LV_STATE_DEFAULT);
            lv_label_set_text(label, view_intl_get_string(model, STRINGS_AZZERA));
            lv_obj_align(button, LV_ALIGN_CENTER, 0, 24);
            view_register_object_default_callback(button, BTN_CLEAR_ALARMS_ID);
            pdata->button_clear_alarms = button;
        }

        lv_obj_t *scale = lv_scale_create(cont);
        lv_scale_set_mode(scale, LV_SCALE_MODE_VERTICAL_LEFT);
        lv_obj_set_size(scale, 16, 120);
        lv_scale_set_major_tick_every(scale, 1);
        lv_obj_set_style_pad_hor(scale, lv_chart_get_first_point_center_offset(chart), 0);
        lv_obj_set_style_text_font(scale, STYLE_FONT_SMALL, LV_PART_INDICATOR);
        lv_obj_set_style_line_width(scale, 0, LV_PART_MAIN);
        lv_obj_align_to(scale, chart, LV_ALIGN_OUT_LEFT_MID, 0, 0);
        pdata->scale = scale;

        switch (model->prog.parmac.scala_accelerometro) {
            case SCALA_ACCELEROMETRO_4G:
            case SCALA_ACCELEROMETRO_4G_OFFSET: {
                lv_scale_set_total_tick_count(scale, 6);
                static const char *ticks[] = {"", "1G", "2G", "3G", "4G", "5G", NULL};
                lv_scale_set_text_src(scale, ticks);
                break;
            }

            case SCALA_ACCELEROMETRO_2G:
            case SCALA_ACCELEROMETRO_2G_OFFSET: {
                lv_scale_set_total_tick_count(scale, 3);
                static const char *ticks[] = {"", "1G", "2G", NULL};
                lv_scale_set_text_src(scale, ticks);
                break;
            }

            case SCALA_ACCELEROMETRO_8G:
            case SCALA_ACCELEROMETRO_8G_OFFSET: {
                lv_scale_set_total_tick_count(scale, 6);
                static const char *ticks[] = {"", "2G", "4G", "6G", "8G", "10G", NULL};
                lv_scale_set_text_src(scale, ticks);
                break;
            }
        }

        lv_chart_series_t *ser1 =
            lv_chart_add_series(chart, lv_palette_main(LV_PALETTE_GREEN), LV_CHART_AXIS_PRIMARY_Y);
        lv_chart_series_t *ser2 = lv_chart_add_series(chart, lv_palette_main(LV_PALETTE_RED), LV_CHART_AXIS_PRIMARY_Y);
        lv_chart_series_t *ser3 = lv_chart_add_series(chart, lv_palette_main(LV_PALETTE_BLUE), LV_CHART_AXIS_PRIMARY_Y);

        pdata->ser_x = ser1;
        pdata->ser_y = ser2;
        pdata->ser_z = ser3;

        pdata->chart = chart;
    }

    {
        lv_obj_t *lbl = lv_label_create(cont);
        lv_obj_set_style_text_font(lbl, STYLE_FONT_SMALL, LV_STATE_DEFAULT);
        lv_obj_align(lbl, LV_ALIGN_TOP_LEFT, 64, 96);
        pdata->label_values = lbl;
    }

    {
        lv_obj_t *lbl = lv_label_create(cont);
        lv_obj_set_style_text_font(lbl, STYLE_FONT_SMALL, LV_STATE_DEFAULT);
        lv_obj_align(lbl, LV_ALIGN_TOP_LEFT, 0, 56 + 24);
        pdata->label_signal = lbl;
    }

    {
        lv_obj_t *lbl = lv_label_create(cont);
        lv_obj_set_style_text_font(lbl, STYLE_FONT_SMALL, LV_STATE_DEFAULT);
        lv_obj_align(lbl, LV_ALIGN_TOP_LEFT, 0, 56 + 48);
        pdata->label_proximity = lbl;
    }

    {
        lv_obj_t *lbl = lv_label_create(cont);
        lv_obj_set_style_text_font(lbl, STYLE_FONT_SMALL, LV_STATE_DEFAULT);
        lv_obj_align(lbl, LV_ALIGN_TOP_LEFT, 0, 56 + 72);
        pdata->label_run = lbl;
    }

    {
        lv_obj_t *lbl = lv_label_create(cont);
        lv_obj_set_style_text_font(lbl, STYLE_FONT_SMALL, LV_STATE_DEFAULT);
        lv_obj_align(lbl, LV_ALIGN_TOP_LEFT, 0, 56 + 96);
        pdata->label_level = lbl;
    }

    {
        lv_obj_t *bottom = lv_obj_create(cont);
        lv_obj_remove_flag(bottom, LV_OBJ_FLAG_SCROLLABLE);
        lv_obj_add_style(bottom, &style_transparent_cont, LV_STATE_DEFAULT);
        lv_obj_set_style_pad_column(bottom, 8, LV_STATE_DEFAULT);
        lv_obj_set_size(bottom, LV_HOR_RES, 52);
        lv_obj_set_layout(bottom, LV_LAYOUT_FLEX);
        lv_obj_set_flex_flow(bottom, LV_FLEX_FLOW_ROW);
        lv_obj_set_flex_align(bottom, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
        lv_obj_set_style_pad_bottom(bottom, 2, LV_STATE_DEFAULT);
        lv_obj_align(bottom, LV_ALIGN_BOTTOM_MID, 0, 0);

        {
            lv_obj_t *obj = lv_button_create(bottom);
            lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
            lv_obj_set_style_radius(obj, 4, LV_STATE_DEFAULT);
            view_register_object_default_callback(obj, BTN_LOCK_ID);
            lv_obj_set_size(obj, 56, 48);

            lv_obj_t *led = lv_led_create(obj);
            lv_led_set_color(led, VIEW_STYLE_COLOR_RED);
            lv_obj_set_size(led, 28, 28);
            lv_obj_center(led);
            lv_obj_add_flag(led, LV_OBJ_FLAG_EVENT_BUBBLE);

            lv_obj_t *lbl = lv_label_create(obj);
            lv_obj_set_style_text_font(lbl, STYLE_FONT_SMALL, LV_STATE_DEFAULT);
            lv_label_set_text(lbl, "OBL.");
            lv_obj_center(lbl);
            pdata->led_porthole = led;
        }
        alarm_led_create(bottom, &pdata->led_water, "H2O");
        alarm_led_create(bottom, &pdata->led_inverter, "INV.");
        alarm_led_create(bottom, &pdata->led_lurching, "SBL.");
        alarm_led_create(bottom, &pdata->led_emergency, "EMR.");
    }

    VIEW_ADD_WATCHED_VARIABLE(&model->test, 0);
    VIEW_ADD_WATCHED_VARIABLE(&model->run.macchina.livello, 0);
    VIEW_ADD_WATCHED_VARIABLE(&model->run.macchina.livello_litri, 0);

    update_page(model, pdata);
}


static pman_msg_t page_event(pman_handle_t handle, void *state, pman_event_t event) {
    struct page_data *pdata = state;

    model_t *model = view_get_model(handle);

    pman_msg_t msg = PMAN_MSG_NULL;

    switch (event.tag) {
        case PMAN_EVENT_TAG_OPEN:
            view_get_protocol(handle)->set_test_mode(handle, 1);
            view_get_protocol(handle)->test_dac(handle, pdata->da_value);
            break;

        case PMAN_EVENT_TAG_USER: {
            view_event_t *view_event = event.as.user;
            switch (view_event->tag) {
                case VIEW_EVENT_TAG_PAGE_WATCHER: {
                    if (pdata->run && !model_test_cesto_in_sicurezza(model)) {
                        pdata->run = 0;
                        view_get_protocol(handle)->clear_outputs(handle);
                    } else if (model_alarm_code(model) > 0 && pdata->run > 0) {
                        pdata->run = 0;
                        view_get_protocol(handle)->clear_outputs(handle);
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
                        case BTN_MINUS_ID: {
                            if (pdata->da_value > 5) {
                                pdata->da_value -= 5;
                            } else if (pdata->da_value > 0) {
                                pdata->da_value = 0;
                            } else {
                                pdata->da_value = 100;
                            }

                            view_get_protocol(handle)->test_dac(handle, pdata->da_value);
                            update_page(model, pdata);
                            break;
                        }

                        case BTN_PLUS_ID: {
                            if (pdata->da_value >= 100) {
                                pdata->da_value = 0;
                            } else if (pdata->da_value >= 95) {
                                pdata->da_value = 100;
                            } else {
                                pdata->da_value = (pdata->da_value + 5) % 100;
                            }

                            view_get_protocol(handle)->test_dac(handle, pdata->da_value);
                            update_page(model, pdata);
                            break;
                        }

                        case BTN_BACK_ID:
                            view_get_protocol(handle)->set_test_mode(handle, 0);
                            msg.stack_msg = PMAN_STACK_MSG_BACK();
                            break;

                        case BTN_NEXT_ID:
                            switch (pdata->page_type) {
                                case VIEW_COMMON_PAGE_TEST_DRUM_TYPE_FORWARD:
                                    msg.stack_msg = PMAN_STACK_MSG_SWAP_EXTRA(
                                        &page_test_drum, (void *)(uintptr_t)VIEW_COMMON_PAGE_TEST_DRUM_TYPE_BACKWARD);
                                    break;
                                case VIEW_COMMON_PAGE_TEST_DRUM_TYPE_BACKWARD:
                                    msg.stack_msg = PMAN_STACK_MSG_SWAP_EXTRA(
                                        &page_test_drum,
                                        (void *)(uintptr_t)VIEW_COMMON_PAGE_TEST_DRUM_TYPE_ACCELEROMETER);
                                    break;
                                default:
                                    msg.stack_msg = PMAN_STACK_MSG_SWAP(&page_test_level);
                                    break;
                            }
                            break;

                        case BTN_CLEAR_ID: {
                            model_clear_test_data(model);
                            update_page(model, pdata);
                            break;
                        }

                        case BTN_CONTROL_ID: {
                            if (pdata->run == 0 && !model_test_cesto_in_sicurezza(model)) {
                                ESP_LOGW(TAG, "Cannot start");
                                break;
                            }
                            pdata->run = !pdata->run;
                            view_get_protocol(handle)->set_output(
                                handle,
                                pdata->page_type == VIEW_COMMON_PAGE_TEST_DRUM_TYPE_BACKWARD ? OUT_INDIETRO
                                                                                             : OUT_AVANTI,
                                pdata->run);
                            update_page(model, pdata);
                            break;
                        }

                        case BTN_LOCK_ID: {
                            view_get_protocol(handle)->toggle_lock(handle);
                            break;
                        }

                        case BTN_CLEAR_ALARMS_ID: {
                            view_get_protocol(handle)->clear_alarms(handle);
                            break;
                        }

                        default:
                            break;
                    }
                    break;
                }

                case LV_EVENT_LONG_PRESSED: {
                    switch (obj_data->id) {
                        case BTN_NEXT_ID:
                            switch (pdata->page_type) {
                                case VIEW_COMMON_PAGE_TEST_DRUM_TYPE_FORWARD:
                                    msg.stack_msg = PMAN_STACK_MSG_SWAP_EXTRA(
                                        &page_test_drum, (void *)(uintptr_t)VIEW_COMMON_PAGE_TEST_DRUM_TYPE_FORWARD);
                                    break;
                                case VIEW_COMMON_PAGE_TEST_DRUM_TYPE_BACKWARD:
                                    msg.stack_msg = PMAN_STACK_MSG_SWAP_EXTRA(
                                        &page_test_drum, (void *)(uintptr_t)VIEW_COMMON_PAGE_TEST_DRUM_TYPE_FORWARD);
                                    break;
                                default:
                                    msg.stack_msg = PMAN_STACK_MSG_SWAP(&page_test_outputs);
                                    break;
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


static void update_page(model_t *model, struct page_data *pdata) {
    view_common_set_disabled(pdata->button_control, !model_test_cesto_in_sicurezza(model));

    lv_label_set_text_fmt(pdata->label_da, "DA=[%04i] %4irpm", pdata->da_value, DA2RPM(pdata->da_value));
    lv_label_set_text_fmt(pdata->label_signal, "Output signal=%02i.%02iV", DA2RPM(pdata->da_value) / 100,
                          DA2RPM(pdata->da_value) % 100);
    lv_label_set_text_fmt(pdata->label_run, "[marcia] %s %s", pdata->run ? "on " : "off",
                          model_test_cesto_in_sicurezza(model) ? "ok" : "no");
    lv_label_set_text_fmt(pdata->label_proximity, "m=%04" PRIuLEAST32 "    M=%04" PRIuLEAST32 "    %04i",
                          model->test.min_proximity_interval, model->test.max_proximity_interval,
                          model->run.macchina.velocita_rilevata);

    lv_led_on(pdata->led_water);
    if (model_get_livello_centimetri(model) > 0) {
        lv_led_set_color(pdata->led_water, VIEW_STYLE_COLOR_ORANGE);
    } else {
        lv_led_set_color(pdata->led_water, VIEW_STYLE_COLOR_GREEN);
    }

    lv_led_on(pdata->led_emergency);
    if ((model->test.inputs & 1) > 0) {
        lv_led_set_color(pdata->led_emergency, VIEW_STYLE_COLOR_GREEN);
    } else {
        lv_led_set_color(pdata->led_emergency, VIEW_STYLE_COLOR_RED);
    }

    lv_led_on(pdata->led_porthole);
    if (model_oblo_serrato(model)) {
        lv_led_set_color(pdata->led_porthole, VIEW_STYLE_COLOR_GREEN);
    } else {
        lv_led_set_color(pdata->led_porthole, VIEW_STYLE_COLOR_RED);
    }

    lv_led_on(pdata->led_lurching);
    if ((model->test.inputs & 2) > 0) {
        lv_led_set_color(pdata->led_lurching, VIEW_STYLE_COLOR_GREEN);
    } else {
        lv_led_set_color(pdata->led_lurching, VIEW_STYLE_COLOR_RED);
    }

    lv_led_on(pdata->led_inverter);
    if ((model->test.inputs & (1 << 10)) > 0) {
        lv_led_set_color(pdata->led_inverter, VIEW_STYLE_COLOR_GREEN);
    } else {
        lv_led_set_color(pdata->led_inverter, VIEW_STYLE_COLOR_RED);
    }

    lv_label_set_text_fmt(pdata->label_level, "%s: %s", view_intl_get_string(model, STRINGS_LIVELLO),
                          model_get_livello(model) == 0 ? "ok" : "no");

    if (pdata->run) {
        lv_label_set_text(pdata->label_control, "STOP");
    } else {
        lv_label_set_text(pdata->label_control, "START");
    }

    switch (pdata->page_type) {
        case VIEW_COMMON_PAGE_TEST_DRUM_TYPE_BACKWARD:
        case VIEW_COMMON_PAGE_TEST_DRUM_TYPE_FORWARD:
            view_common_set_hidden(pdata->label_signal, 0);
            view_common_set_hidden(pdata->label_run, 0);
            view_common_set_hidden(pdata->label_proximity, 0);
            view_common_set_hidden(pdata->label_level, 0);
            view_common_set_hidden(pdata->chart, 1);
            view_common_set_hidden(pdata->scale, 1);
            view_common_set_hidden(pdata->button_clear, 0);
            view_common_set_hidden(pdata->label_accelerometer_off, 1);
            view_common_set_hidden(pdata->button_clear_alarms, 1);
            view_common_set_hidden(pdata->label_values, 1);
            break;
        default: {
            if (model->test.accelerometro_ok) {
                view_common_set_hidden(pdata->label_values, 0);
                view_common_set_hidden(pdata->chart, 0);
                view_common_set_hidden(pdata->scale, 0);
                view_common_set_hidden(pdata->label_accelerometer_off, 1);
                view_common_set_hidden(pdata->button_clear_alarms, 1);
                int32_t *ser_x_points = lv_chart_get_y_array(pdata->chart, pdata->ser_x);
                int32_t *ser_y_points = lv_chart_get_y_array(pdata->chart, pdata->ser_y);
                int32_t *ser_z_points = lv_chart_get_y_array(pdata->chart, pdata->ser_z);

                for (uint16_t i = 0; i < MAX_LOG_ACCELEROMETRO; i++) {
                    uint16_t j      = (model->test.log_index + i) % MAX_LOG_ACCELEROMETRO;
                    ser_x_points[i] = model->test.log_accelerometro[j][0];
                    ser_y_points[i] = model->test.log_accelerometro[j][1];
                    ser_z_points[i] = model->test.log_accelerometro[j][2];
                }
                lv_chart_refresh(pdata->chart);

                lv_label_set_text_fmt(pdata->label_values, "x=%4" PRIi32 " y=%4" PRIi32 " z=%4" PRIi32,
                                      ser_x_points[MAX_LOG_ACCELEROMETRO - 1], ser_y_points[MAX_LOG_ACCELEROMETRO - 1],
                                      ser_z_points[MAX_LOG_ACCELEROMETRO - 1]);
            } else {
                view_common_set_hidden(pdata->label_values, 1);
                view_common_set_hidden(pdata->chart, 1);
                view_common_set_hidden(pdata->scale, 1);
                view_common_set_hidden(pdata->label_accelerometer_off, 0);
                view_common_set_hidden(pdata->button_clear_alarms, 0);
            }

            view_common_set_hidden(pdata->label_signal, 1);
            view_common_set_hidden(pdata->label_run, 1);
            view_common_set_hidden(pdata->label_proximity, 1);
            view_common_set_hidden(pdata->label_level, 1);
            view_common_set_hidden(pdata->button_clear, 1);
            break;
        }
    }
}


static lv_obj_t *alarm_led_create(lv_obj_t *parent, lv_obj_t **led, const char *text) {
    lv_obj_t *obj = lv_obj_create(parent);
    lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_bg_color(obj, lv_obj_get_style_border_color(obj, LV_STATE_DEFAULT), LV_STATE_DEFAULT);
    lv_obj_set_style_radius(obj, 4, LV_STATE_DEFAULT);
    lv_obj_set_size(obj, 50, 48);

    *led = lv_led_create(obj);
    lv_led_set_color(*led, VIEW_STYLE_COLOR_RED);
    lv_obj_set_size(*led, 24, 24);
    lv_obj_center(*led);

    lv_obj_t *lbl = lv_label_create(obj);
    lv_obj_set_style_text_font(lbl, STYLE_FONT_SMALL, LV_STATE_DEFAULT);
    lv_label_set_text(lbl, text);
    lv_obj_center(lbl);

    return obj;
}


const pman_page_t page_test_drum = {
    .create        = create_page,
    .destroy       = pman_destroy_all,
    .open          = open_page,
    .close         = pman_close_all,
    .process_event = page_event,
};
