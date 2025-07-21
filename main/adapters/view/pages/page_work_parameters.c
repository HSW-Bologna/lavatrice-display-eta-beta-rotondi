#include <assert.h>
#include <stdlib.h>
#include "lvgl.h"
#include "model/model.h"
#include "../view.h"
#include "../theme/style.h"
#include "../intl/intl.h"
#include <esp_log.h>
#include "bsp/tft/display.h"
#include "../theme/style.h"
#include "../common.h"
#include "config/app_config.h"
#include "services/timestamp.h"
#include "stopwatch.h"


typedef enum {
    WORK_PARAMETER_TEMPERATURE = 0,
    WORK_PARAMETER_LEVEL,
    WORK_PARAMETER_SPEED,
    WORK_PARAMETER_DURATION,
    WORK_PARAMETER_DETERGENT,
#define WORK_PARAMETERS_NUM 5
} work_parameter_t;


enum {
    BTN_BACK_ID,
    BTN_TEMPERATURE_ID,
    BTN_LEVEL_ID,
    BTN_SPEED_ID,
    BTN_DURATION_ID,
    BTN_MODIFY_ID,
    BTN_DETERGENT_ID,
    BTN_DETERGENT_TOGGLE_ID,
    BTN_DETERGENT_INSTANT_ID,
    TIMER_BACK_ID,
    TIMER_HALFSECOND_ID,
};


static const char *TAG = __FILE_NAME__;


struct page_data {
    lv_obj_t *buttons[WORK_PARAMETERS_NUM];
    lv_obj_t *button_minus;
    lv_obj_t *button_plus;
    lv_obj_t *button_back;
    lv_obj_t *button_instant_detergent;
    lv_obj_t *button_detergent;

    lv_obj_t *led_detergent;

    lv_obj_t *labels[WORK_PARAMETERS_NUM];
    lv_obj_t *label_originals[WORK_PARAMETERS_NUM];
    lv_obj_t *label_parameter;
    lv_obj_t *label_detergent_time;

    stopwatch_t stopwatches[10];

    int      selected_parameter;
    uint16_t selected_detergent;
    uint8_t  fixed_detergent;
    uint8_t  sapone_on;

    pman_timer_t *timer;
    pman_timer_t *timer_halfsec;
};


static void      update_page(model_t *model, struct page_data *pdata);
static lv_obj_t *button_parameter_create(lv_obj_t *parent, lv_obj_t **label_original, lv_obj_t **label_value,
                                         const char *description, int id, uint8_t top);
static void      turn_off_detergent(pman_handle_t handle, struct page_data *pdata);


static void *create_page(pman_handle_t handle, void *extra) {
    (void)handle;
    (void)extra;

    struct page_data *pdata = lv_malloc(sizeof(struct page_data));
    assert(pdata != NULL);

    // pdata->timer              = PMAN_REGISTER_TIMER_ID(handle, model->config.parmac.reset_page_time * 1000UL, 0);
    pdata->timer              = PMAN_REGISTER_TIMER_ID(handle, 30 * 1000UL, TIMER_BACK_ID);
    pdata->timer_halfsec      = PMAN_REGISTER_TIMER_ID(handle, 500UL, TIMER_HALFSECOND_ID);
    pdata->selected_detergent = 0;
    pdata->fixed_detergent    = (uint8_t)(uintptr_t)extra;
    if (pdata->fixed_detergent) {
        pdata->selected_parameter = WORK_PARAMETER_DETERGENT;
    } else {
        pdata->selected_parameter = -1;
    }

    for (size_t i = 0; i < 10; i++) {
        stopwatch_init(&pdata->stopwatches[i]);
    }

    pdata->sapone_on = 0;

    return pdata;
}


static void open_page(pman_handle_t handle, void *state) {
    struct page_data *pdata = state;

    model_t *model = pman_get_user_data(handle);

    model->run.speed_setpoint = model->run.macchina.velocita_rpm;
    model->run.duration       = model->run.macchina.rimanente;

    pman_timer_reset(pdata->timer);
    pman_timer_resume(pdata->timer);
    pman_timer_resume(pdata->timer_halfsec);

    lv_obj_remove_flag(lv_screen_active(), LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_t *cont = lv_obj_create(lv_screen_active());
    lv_obj_remove_flag(cont, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_size(cont, LV_HOR_RES, LV_VER_RES);
    lv_obj_add_style(cont, &style_padless_cont, LV_STATE_DEFAULT);
    lv_obj_add_style(cont, &style_borderless_cont, LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(cont, VIEW_STYLE_COLOR_WHITE, LV_STATE_DEFAULT);

    lv_obj_t *top_cont = lv_obj_create(cont);
    lv_obj_remove_flag(top_cont, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_add_style(top_cont, &style_transparent_cont, LV_STATE_DEFAULT);
    lv_obj_set_size(top_cont, LV_HOR_RES, 120);
    lv_obj_align(top_cont, LV_ALIGN_TOP_MID, 0, -28);
    lv_obj_set_style_pad_column(top_cont, 8, LV_STATE_DEFAULT);
    lv_obj_set_style_pad_hor(top_cont, 8, LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(top_cont, 4, LV_STATE_DEFAULT);
    lv_obj_set_layout(top_cont, LV_LAYOUT_FLEX);
    lv_obj_set_flex_flow(top_cont, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(top_cont, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_START);

    pdata->buttons[WORK_PARAMETER_TEMPERATURE] = button_parameter_create(
        top_cont, &pdata->label_originals[WORK_PARAMETER_TEMPERATURE], &pdata->labels[WORK_PARAMETER_TEMPERATURE],
        view_intl_get_string(model, STRINGS_TEMPERATURA), BTN_TEMPERATURE_ID, 1);
    pdata->buttons[WORK_PARAMETER_LEVEL] = button_parameter_create(
        top_cont, &pdata->label_originals[WORK_PARAMETER_LEVEL], &pdata->labels[WORK_PARAMETER_LEVEL],
        view_intl_get_string(model, STRINGS_LIVELLO), BTN_LEVEL_ID, 1);

    lv_obj_t *bottom_cont = lv_obj_create(cont);
    lv_obj_remove_flag(bottom_cont, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_add_style(bottom_cont, &style_transparent_cont, LV_STATE_DEFAULT);
    lv_obj_set_size(bottom_cont, LV_HOR_RES, 120);
    lv_obj_align(bottom_cont, LV_ALIGN_BOTTOM_MID, 0, 28);
    lv_obj_set_style_pad_column(bottom_cont, 8, LV_STATE_DEFAULT);
    lv_obj_set_style_pad_hor(bottom_cont, 8, LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(bottom_cont, 4, LV_STATE_DEFAULT);
    lv_obj_set_layout(bottom_cont, LV_LAYOUT_FLEX);
    lv_obj_set_flex_flow(bottom_cont, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(bottom_cont, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_START);

    pdata->buttons[WORK_PARAMETER_SPEED] = button_parameter_create(
        bottom_cont, &pdata->label_originals[WORK_PARAMETER_SPEED], &pdata->labels[WORK_PARAMETER_SPEED],
        view_intl_get_string(model, STRINGS_VELOCITA), BTN_SPEED_ID, 0);

    pdata->buttons[WORK_PARAMETER_DURATION] = button_parameter_create(
        bottom_cont, &pdata->label_originals[WORK_PARAMETER_DURATION], &pdata->labels[WORK_PARAMETER_DURATION],
        view_intl_get_string(model, STRINGS_DURATA), BTN_DURATION_ID, 0);

    lv_obj_t *lbl = lv_label_create(cont);
    lv_obj_set_style_text_font(lbl, STYLE_FONT_MEDIUM, LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(lbl, lv_color_black(), LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(lbl, LV_TEXT_ALIGN_CENTER, LV_STATE_DEFAULT);
    lv_obj_align(lbl, LV_ALIGN_CENTER, 0, 40);
    pdata->label_parameter = lbl;

    {
        lv_obj_t *btn = lv_button_create(cont);
        lv_obj_set_size(btn, 48, 48);
        lv_obj_t *lbl = lv_label_create(btn);
        lv_label_set_text(lbl, LV_SYMBOL_MINUS);
        lv_obj_center(lbl);
        view_register_object_default_callback_with_number(btn, BTN_MODIFY_ID, -1);
        lv_obj_align(btn, LV_ALIGN_LEFT_MID, 8, 40);
        pdata->button_minus = btn;
    }

    {
        lv_obj_t *btn = lv_button_create(cont);
        lv_obj_set_size(btn, 48, 48);
        lv_obj_t *lbl = lv_label_create(btn);
        lv_label_set_text(lbl, LV_SYMBOL_PLUS);
        lv_obj_center(lbl);
        view_register_object_default_callback_with_number(btn, BTN_MODIFY_ID, +1);
        lv_obj_align(btn, LV_ALIGN_RIGHT_MID, -8, 40);
        pdata->button_plus = btn;
    }

    {
        LV_IMG_DECLARE(img_door);
        lv_obj_t *btn = lv_button_create(cont);
        lv_obj_add_style(btn, (lv_style_t *)&style_config_btn, LV_STATE_DEFAULT);
        lv_obj_set_style_radius(btn, LV_RADIUS_CIRCLE, LV_STATE_DEFAULT);
        lv_obj_set_size(btn, 56, 56);
        lv_obj_t *img = lv_image_create(btn);
        lv_image_set_src(img, &img_door);
        lv_obj_add_style(img, &style_white_icon, LV_STATE_DEFAULT);
        lv_obj_center(img);
        lv_obj_align(btn, LV_ALIGN_CENTER, 0, pdata->fixed_detergent ? 120 : 40);
        view_register_object_default_callback(btn, BTN_BACK_ID);
        pdata->button_back = btn;
    }

    {
        LV_IMG_DECLARE(img_detergent);
        lv_obj_t *btn = lv_button_create(cont);
        lv_obj_add_flag(btn, LV_OBJ_FLAG_CHECKABLE);
        lv_obj_add_style(btn, (lv_style_t *)&style_config_btn, LV_STATE_DEFAULT);
        lv_obj_set_style_radius(btn, LV_RADIUS_CIRCLE, LV_STATE_DEFAULT);
        lv_obj_set_size(btn, 56, 56);
        lv_obj_t *img = lv_image_create(btn);
        lv_image_set_src(img, &img_detergent);
        lv_obj_add_style(img, &style_white_icon, LV_STATE_DEFAULT);
        lv_obj_center(img);
        lv_obj_align(btn, LV_ALIGN_TOP_MID, 0, 90);
        view_register_object_default_callback(btn, BTN_DETERGENT_ID);
        pdata->buttons[WORK_PARAMETER_DETERGENT] = btn;
    }

    {
        lv_obj_t *led = lv_led_create(cont);
        lv_led_set_color(led, lv_color_make(0x80, 0xFF, 0));
        lv_obj_align(led, LV_ALIGN_TOP_LEFT, 5, 110);
        pdata->led_detergent = led;
    }

    {
        lv_obj_t *label = lv_label_create(cont);
        lv_obj_align(label, LV_ALIGN_TOP_LEFT, 35, 105);
        lv_obj_set_style_text_color(label, lv_color_black(), LV_STATE_DEFAULT);
        pdata->label_detergent_time = label;
    }

    {
        lv_obj_t *btn = lv_button_create(cont);
        lv_obj_add_flag(btn, LV_OBJ_FLAG_CHECKABLE);
        lv_obj_add_style(btn, (lv_style_t *)&style_config_btn, LV_STATE_DEFAULT);
        lv_obj_set_style_radius(btn, LV_RADIUS_CIRCLE, LV_STATE_DEFAULT);
        lv_obj_set_size(btn, 48, 48);
        lv_obj_t *label = lv_label_create(btn);
        lv_label_set_text(label, LV_SYMBOL_DOWN);
        lv_obj_center(label);
        lv_obj_align(btn, LV_ALIGN_TOP_RIGHT, -64, 100);
        view_register_object_default_callback(btn, BTN_DETERGENT_TOGGLE_ID);
        pdata->button_detergent = btn;
    }

    {
        lv_obj_t *btn = lv_button_create(cont);
        lv_obj_add_style(btn, (lv_style_t *)&style_config_btn, LV_STATE_DEFAULT);
        lv_obj_set_style_radius(btn, LV_RADIUS_CIRCLE, LV_STATE_DEFAULT);
        lv_obj_set_size(btn, 48, 48);
        lv_obj_t *label = lv_label_create(btn);
        lv_label_set_text(label, LV_SYMBOL_CHARGE);
        lv_obj_center(label);
        lv_obj_align(btn, LV_ALIGN_TOP_RIGHT, -4, 100);
        view_register_object_default_callback(btn, BTN_DETERGENT_INSTANT_ID);
        pdata->button_instant_detergent = btn;
    }

    ESP_LOGI(TAG, "Open");

    VIEW_ADD_WATCHED_VARIABLE(&model->run.macchina.rimanente, 0);
    VIEW_ADD_WATCHED_ARRAY(&model->run.macchina.out_saponi, 10, 0);

    update_page(model, pdata);
}


static pman_msg_t page_event(pman_handle_t handle, void *state, pman_event_t event) {
    (void)handle;

    pman_msg_t msg = PMAN_MSG_NULL;

    struct page_data *pdata = state;
    mut_model_t      *model = pman_get_user_data(handle);

    for (size_t i = 0; i < 10; i++) {
        if (model->run.macchina.out_saponi[i]) {
            stopwatch_resume(&pdata->stopwatches[i], timestamp_get());
        } else {
            stopwatch_pause(&pdata->stopwatches[i], timestamp_get());
        }
    }

    switch (event.tag) {
        case PMAN_EVENT_TAG_USER: {
            view_event_t *view_event = event.as.user;
            switch (view_event->tag) {
                case VIEW_EVENT_TAG_PAGE_WATCHER: {
                    if (model_macchina_in_stop(model) && !pdata->fixed_detergent) {
                        msg.stack_msg = PMAN_STACK_MSG_BACK();
                    } else {
                        update_page(model, pdata);
                    }
                    break;
                }

                default:
                    break;
            }

            break;
        }

        case PMAN_EVENT_TAG_TIMER: {
            switch ((int)(uintptr_t)event.as.timer->user_data) {
                case TIMER_BACK_ID:
                    msg.stack_msg = PMAN_STACK_MSG_BACK();
                    break;

                case TIMER_HALFSECOND_ID:
                    update_page(model, pdata);
                    break;

                default:
                    break;
            }
            break;
        }

        case PMAN_EVENT_TAG_LVGL: {
            lv_obj_t           *target   = lv_event_get_target(event.as.lvgl);
            view_object_data_t *obj_data = lv_obj_get_user_data(target);

            if (obj_data == NULL) {
                break;
            }

            switch (lv_event_get_code(event.as.lvgl)) {
                case LV_EVENT_CLICKED: {
                    pman_timer_reset(pdata->timer);

                    switch (obj_data->id) {
                        case BTN_BACK_ID: {
                            msg.stack_msg = PMAN_STACK_MSG_BACK();
                            break;
                        }

                        case BTN_DETERGENT_TOGGLE_ID: {
                            if (model->run.macchina.out_saponi[pdata->selected_detergent] == 0) {
                                pdata->sapone_on = 1;
                                view_get_protocol(handle)->colpo_sapone(handle, pdata->selected_detergent);
                            } else {
                                pdata->sapone_on = 0;
                                view_get_protocol(handle)->controllo_sapone(handle, pdata->selected_detergent, 0);
                            }
                            break;
                        }

                        case BTN_MODIFY_ID: {
                            switch (pdata->selected_parameter) {
                                case WORK_PARAMETER_TEMPERATURE: {
                                    model_modify_temperature_setpoint(model, obj_data->number);
                                    update_page(model, pdata);
                                    break;
                                }
                                case WORK_PARAMETER_LEVEL: {
                                    model_modify_level_setpoint(model, obj_data->number);
                                    update_page(model, pdata);
                                    break;
                                }
                                case WORK_PARAMETER_SPEED: {
                                    model_modify_speed_setpoint(model, obj_data->number);
                                    update_page(model, pdata);
                                    break;
                                }
                                case WORK_PARAMETER_DURATION: {
                                    model->run.duration += obj_data->number * 10;
                                    model->run.duration = (model->run.duration / 10) * 10;
                                    update_page(model, pdata);
                                    break;
                                }
                                case WORK_PARAMETER_DETERGENT: {
                                    turn_off_detergent(handle, pdata);
                                    if (obj_data->number > 0) {
                                        pdata->selected_detergent = (pdata->selected_detergent + 1) %
                                                                    model->prog.parmac.numero_saponi_utilizzabili;
                                    } else if (pdata->selected_detergent > 0) {
                                        pdata->selected_detergent--;
                                    } else {
                                        pdata->selected_detergent = model->prog.parmac.numero_saponi_utilizzabili - 1;
                                    }
                                    update_page(model, pdata);
                                    break;
                                }

                                default:
                                    break;
                            }
                            break;
                        }

                        case BTN_TEMPERATURE_ID: {
                            turn_off_detergent(handle, pdata);
                            if (pdata->selected_parameter == WORK_PARAMETER_TEMPERATURE) {
                                pdata->selected_parameter = -1;
                            } else {
                                pdata->selected_parameter = WORK_PARAMETER_TEMPERATURE;
                            }
                            update_page(model, pdata);
                            break;
                        }

                        case BTN_LEVEL_ID: {
                            turn_off_detergent(handle, pdata);
                            if (pdata->selected_parameter == WORK_PARAMETER_LEVEL) {
                                pdata->selected_parameter = -1;
                            } else {
                                pdata->selected_parameter = WORK_PARAMETER_LEVEL;
                            }
                            update_page(model, pdata);
                            break;
                        }

                        case BTN_SPEED_ID: {
                            turn_off_detergent(handle, pdata);
                            if (pdata->selected_parameter == WORK_PARAMETER_SPEED) {
                                pdata->selected_parameter = -1;
                            } else {
                                pdata->selected_parameter = WORK_PARAMETER_SPEED;
                            }
                            update_page(model, pdata);
                            break;
                        }

                        case BTN_DURATION_ID: {
                            turn_off_detergent(handle, pdata);
                            if (pdata->selected_parameter == WORK_PARAMETER_DURATION) {
                                pdata->selected_parameter = -1;
                            } else {
                                pdata->selected_parameter = WORK_PARAMETER_DURATION;
                            }
                            update_page(model, pdata);
                            break;
                        }

                        case BTN_DETERGENT_ID: {
                            if (!pdata->fixed_detergent) {
                                turn_off_detergent(handle, pdata);
                                if (pdata->selected_parameter == WORK_PARAMETER_DETERGENT) {
                                    pdata->selected_parameter = -1;
                                } else {
                                    pdata->selected_parameter = WORK_PARAMETER_DETERGENT;
                                }
                                update_page(model, pdata);
                            }
                            break;
                        }

                        default:
                            break;
                    }
                    break;
                }

                case LV_EVENT_LONG_PRESSED_REPEAT: {
                    pman_timer_reset(pdata->timer);

                    switch (obj_data->id) {
                        case BTN_MODIFY_ID: {
                            switch (pdata->selected_parameter) {
                                case WORK_PARAMETER_TEMPERATURE: {
                                    model_modify_temperature_setpoint(model, obj_data->number);
                                    update_page(model, pdata);
                                    break;
                                }
                                case WORK_PARAMETER_LEVEL: {
                                    model_modify_level_setpoint(model, obj_data->number);
                                    update_page(model, pdata);
                                    break;
                                }
                                case WORK_PARAMETER_SPEED: {
                                    model_modify_speed_setpoint(model, obj_data->number);
                                    update_page(model, pdata);
                                    break;
                                }
                                case WORK_PARAMETER_DURATION: {
                                    model->run.duration += obj_data->number * 10;
                                    model->run.duration = (model->run.duration / 10) * 10;
                                    update_page(model, pdata);
                                    break;
                                }
                                case WORK_PARAMETER_DETERGENT: {
                                    if (obj_data->number > 0) {
                                        pdata->selected_detergent = (pdata->selected_detergent + 1) %
                                                                    model->prog.parmac.numero_saponi_utilizzabili;
                                    } else if (pdata->selected_detergent > 0) {
                                        pdata->selected_detergent--;
                                    } else {
                                        pdata->selected_detergent = model->prog.parmac.numero_saponi_utilizzabili;
                                    }
                                    update_page(model, pdata);
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
                    break;
                }

                case LV_EVENT_PRESSED: {
                    switch (obj_data->id) {
                        case BTN_DETERGENT_INSTANT_ID: {
                            view_get_protocol(handle)->controllo_sapone(handle, pdata->selected_detergent, 1);
                            break;
                        }

                        default:
                            break;
                    }
                    break;
                }

                case LV_EVENT_RELEASED: {
                    switch (obj_data->id) {
                        case BTN_MODIFY_ID: {
                            view_get_protocol(handle)->update_work_parameters(handle);
                            break;
                        }

                        case BTN_DETERGENT_INSTANT_ID: {
                            view_get_protocol(handle)->controllo_sapone(handle, pdata->selected_detergent, 0);
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
            break;
        }

        default:
            break;
    }

    return msg;
}


static void update_page(model_t *model, struct page_data *pdata) {
    char *fmt = "%i";

    switch (model->prog.parmac.tipo_livello) {
        case LIVELLO_CENTIMETRI:
            fmt = "%i cm";
            break;
        default:
            fmt = "%i lt";
            break;
    }

    lv_label_set_text_fmt(pdata->labels[WORK_PARAMETER_TEMPERATURE], "%i °C", model->run.temperature_setpoint);
    lv_label_set_text_fmt(pdata->label_originals[WORK_PARAMETER_TEMPERATURE], "%i °C", model->run.macchina.temperatura);
    lv_label_set_text_fmt(pdata->labels[WORK_PARAMETER_LEVEL], fmt, model->run.level_setpoint);
    lv_label_set_text_fmt(pdata->label_originals[WORK_PARAMETER_LEVEL], fmt, model->run.macchina.livello);

    if (pdata->selected_parameter == -1) {
        view_common_set_hidden(pdata->label_parameter, 1);
        view_common_set_hidden(pdata->button_minus, 1);
        view_common_set_hidden(pdata->button_plus, 1);
        view_common_set_hidden(pdata->button_back, 0);
        view_common_set_hidden(pdata->led_detergent, 1);
        view_common_set_hidden(pdata->label_detergent_time, 1);
        view_common_set_hidden(pdata->button_detergent, 1);
        view_common_set_hidden(pdata->button_instant_detergent, 1);
    } else {
        view_common_set_hidden(pdata->label_parameter, 0);
        view_common_set_hidden(pdata->button_minus, 0);
        view_common_set_hidden(pdata->button_plus, 0);
        view_common_set_hidden(pdata->button_back, !pdata->fixed_detergent);

        switch (pdata->selected_parameter) {
            case WORK_PARAMETER_TEMPERATURE:
                lv_label_set_text_fmt(pdata->label_parameter, "%i °C", model->run.temperature_setpoint);
                view_common_set_hidden(pdata->led_detergent, 1);
                view_common_set_hidden(pdata->label_detergent_time, 1);
                view_common_set_hidden(pdata->button_detergent, 1);
                view_common_set_hidden(pdata->button_instant_detergent, 1);
                break;

            case WORK_PARAMETER_LEVEL: {
                lv_label_set_text_fmt(pdata->label_parameter, fmt, model->run.level_setpoint);
                view_common_set_hidden(pdata->led_detergent, 1);
                view_common_set_hidden(pdata->label_detergent_time, 1);
                view_common_set_hidden(pdata->button_detergent, 1);
                view_common_set_hidden(pdata->button_instant_detergent, 1);
                break;
            }

            case WORK_PARAMETER_SPEED:
                lv_label_set_text_fmt(pdata->label_parameter, "%i rpm", model->run.speed_setpoint);
                view_common_set_hidden(pdata->led_detergent, 1);
                view_common_set_hidden(pdata->label_detergent_time, 1);
                view_common_set_hidden(pdata->button_detergent, 1);
                view_common_set_hidden(pdata->button_instant_detergent, 1);
                break;

            case WORK_PARAMETER_DURATION:
                lv_label_set_text_fmt(pdata->label_parameter, "%02i:%02i", model->run.duration / 60,
                                      model->run.duration % 60);
                view_common_set_hidden(pdata->led_detergent, 1);
                view_common_set_hidden(pdata->label_detergent_time, 1);
                view_common_set_hidden(pdata->button_detergent, 1);
                view_common_set_hidden(pdata->button_instant_detergent, 1);
                break;

            case WORK_PARAMETER_DETERGENT:
                lv_label_set_text_fmt(pdata->label_parameter, "%s %i", view_intl_get_string(model, STRINGS_SAPONE),
                                      pdata->selected_detergent + 1);
                view_common_set_hidden(pdata->led_detergent, 0);
                view_common_set_hidden(pdata->label_detergent_time, 0);
                view_common_set_hidden(pdata->button_detergent, 0);
                view_common_set_hidden(pdata->button_instant_detergent, 0);
                if (model->run.macchina.out_saponi[pdata->selected_detergent]) {
                    lv_led_on(pdata->led_detergent);
                } else {
                    lv_led_off(pdata->led_detergent);
                }

                if (model->run.macchina.out_saponi[pdata->selected_detergent] && pdata->sapone_on) {
                    lv_obj_add_state(pdata->button_detergent, LV_STATE_CHECKED);
                } else {
                    lv_obj_remove_state(pdata->button_detergent, LV_STATE_CHECKED);
                }

                int seconds =
                    stopwatch_get_elapsed(&pdata->stopwatches[pdata->selected_detergent], timestamp_get()) / 1000;
                lv_label_set_text_fmt(pdata->label_detergent_time, "%02i:%02i", seconds / 60, seconds % 60);
                break;

            default:
                break;
        }
    }

    lv_label_set_text_fmt(pdata->labels[WORK_PARAMETER_SPEED], "%i rpm", model->run.speed_setpoint);
    lv_label_set_text_fmt(pdata->label_originals[WORK_PARAMETER_SPEED], fmt, model->run.macchina.velocita_rpm);
    lv_label_set_text_fmt(pdata->labels[WORK_PARAMETER_DURATION], "%02i:%02i", model->run.duration / 60,
                          model->run.duration % 60);
    view_common_set_hidden(pdata->label_originals[WORK_PARAMETER_DURATION], 1);

    for (work_parameter_t parameter_index = 0; parameter_index < WORK_PARAMETERS_NUM; parameter_index++) {
        if (pdata->fixed_detergent && parameter_index != WORK_PARAMETER_DETERGENT) {
            view_common_set_hidden(pdata->buttons[parameter_index], 1);
        } else {
            view_common_set_hidden(pdata->buttons[parameter_index], 0);
        }

        if ((int)parameter_index == pdata->selected_parameter) {
            lv_obj_add_state(pdata->buttons[parameter_index], LV_STATE_CHECKED);
        } else {
            lv_obj_remove_state(pdata->buttons[parameter_index], LV_STATE_CHECKED);
        }
    }
}


static void close_page(void *state) {
    struct page_data *pdata = state;
    pman_timer_pause(pdata->timer);
    pman_timer_pause(pdata->timer_halfsec);
    lv_obj_clean(lv_screen_active());
}


static void destroy_page(void *state, void *extra) {
    (void)extra;
    struct page_data *pdata = state;
    pman_timer_delete(pdata->timer);
    pman_timer_delete(pdata->timer_halfsec);
    lv_free(state);
}


static lv_obj_t *button_parameter_create(lv_obj_t *parent, lv_obj_t **label_original, lv_obj_t **label_value,
                                         const char *description, int id, uint8_t top) {
    lv_obj_t *button = lv_button_create(parent);
    lv_obj_set_flex_grow(button, 1);
    lv_obj_set_height(button, LV_PCT(100));
    lv_obj_add_style(button, &style_tall_button, LV_STATE_DEFAULT);
    lv_obj_add_style(button, &style_tall_button_checked, LV_STATE_CHECKED);
    lv_obj_set_style_pad_top(button, 28, LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(button, 12, LV_STATE_DEFAULT);
    lv_obj_set_style_radius(button, 24, 0);
    view_register_object_default_callback(button, id);

    lv_obj_t *label_description = lv_label_create(button);
    lv_label_set_long_mode(label_description, LV_LABEL_LONG_CLIP);
    lv_obj_set_width(label_description, LV_HOR_RES / 2 - 32);
    lv_obj_set_style_text_font(label_description, STYLE_FONT_COMPACT, LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(label_description, LV_TEXT_ALIGN_CENTER, LV_STATE_DEFAULT);
    lv_label_set_text(label_description, description);
    if (top) {
        lv_obj_align(label_description, LV_ALIGN_TOP_MID, 0, 4);
    } else {
        lv_obj_align(label_description, LV_ALIGN_BOTTOM_MID, 0, -22);
    }

    LV_FONT_DECLARE(font_montserrat_bold_20)
    lv_obj_t *local_label_original = lv_label_create(button);
    lv_obj_set_style_text_font(local_label_original, STYLE_FONT_MEDIUM, LV_STATE_DEFAULT);
    if (top) {
        lv_obj_align(local_label_original, LV_ALIGN_BOTTOM_MID, 0, -20);
    } else {
        lv_obj_align(local_label_original, LV_ALIGN_TOP_MID, 0, 4);
    }
    *label_original = local_label_original;

    lv_obj_t *local_label_value = lv_label_create(button);
    lv_obj_set_style_text_font(local_label_value, &font_montserrat_bold_20, LV_STATE_DEFAULT);
    if (top) {
        lv_obj_align(local_label_value, LV_ALIGN_BOTTOM_MID, 0, 2);
    } else {
        lv_obj_align(local_label_value, LV_ALIGN_TOP_MID, 0, -20);
    }
    *label_value = local_label_value;

    return button;
}


static void turn_off_detergent(pman_handle_t handle, struct page_data *pdata) {
    if (pdata->selected_parameter == WORK_PARAMETER_DETERGENT) {
        view_get_protocol(handle)->controllo_sapone(handle, pdata->selected_detergent, 0);
        pdata->sapone_on = 0;
    }
}


const pman_page_t page_work_parameters = {
    .create        = create_page,
    .destroy       = destroy_page,
    .open          = open_page,
    .close         = close_page,
    .process_event = page_event,
};
