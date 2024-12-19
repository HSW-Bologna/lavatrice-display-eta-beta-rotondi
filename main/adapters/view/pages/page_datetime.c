#include <assert.h>
#include <stdlib.h>
#include "lvgl.h"
#include "model/model.h"
#include "../view.h"
#include "../theme/style.h"
#include "../intl/intl.h"
#include "../common.h"
#include <esp_log.h>
#include "services/system_time.h"


#define PARAM_WINDOW_WIDTH 230


enum {
    BTN_BACK_ID,
    BTN_HOUR_ID,
    BTN_MINUTE_ID,
    BTN_DAY_ID,
    BTN_MONTH_ID,
    BTN_YEAR_ID,
    BTN_CONFIRM_ID,
};


struct page_data {
    lv_obj_t *label_hours;
    lv_obj_t *label_minutes;
    lv_obj_t *label_day;
    lv_obj_t *label_month;
    lv_obj_t *label_year;

    lv_obj_t *button_confirm;

    struct tm current_time;

    uint8_t modified;
    uint8_t program_wash_cycle;
};


static void      update_page(model_t *model, struct page_data *pdata);
static lv_obj_t *param_window_create(lv_obj_t *parent, lv_obj_t **label);
static lv_obj_t *icon_button_create(lv_obj_t *parent, const char *text, uint16_t id, uint16_t number);


static const char *TAG = __FILE_NAME__;


static void *create_page(pman_handle_t handle, void *extra) {
    (void)handle;
    (void)extra;

    struct page_data *pdata = lv_malloc(sizeof(struct page_data));
    assert(pdata != NULL);

    pdata->program_wash_cycle = (uint8_t)(uintptr_t)extra;
    pdata->modified           = 0;
    pdata->current_time       = system_time_get();

    return pdata;
}


static void open_page(pman_handle_t handle, void *state) {
    struct page_data *pdata = state;

    model_t *model = pman_get_user_data(handle);

    view_title_t title = view_common_create_title(
        lv_scr_act(),
        view_intl_get_string(model, pdata->program_wash_cycle ? STRINGS_LAVAGGIO_PROGRAMMATO : STRINGS_ORA_DATA),
        BTN_BACK_ID, pdata->program_wash_cycle ? BTN_CONFIRM_ID : -1);

    pdata->button_confirm = title.button_next;
    if (pdata->button_confirm) {
        lv_obj_t *label_next = lv_obj_get_child(title.button_next, 0);
        lv_label_set_text(label_next, LV_SYMBOL_OK);
    }

    lv_obj_t *cont = lv_obj_create(lv_scr_act());
    lv_obj_remove_flag(cont, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_pad_column(cont, 8, LV_STATE_DEFAULT);
    lv_obj_set_size(cont, LV_HOR_RES, LV_VER_RES - 56);
    lv_obj_align(cont, LV_ALIGN_BOTTOM_MID, 0, 0);

    lv_obj_t *param        = param_window_create(cont, &pdata->label_hours);
    lv_obj_t *button_left  = icon_button_create(cont, LV_SYMBOL_MINUS, BTN_HOUR_ID, -1);
    lv_obj_t *button_right = icon_button_create(cont, LV_SYMBOL_PLUS, BTN_HOUR_ID, +1);
    lv_obj_t *param_prev   = param;

    lv_obj_align(param, LV_ALIGN_TOP_MID, 0, 0);
    lv_obj_align_to(button_left, param, LV_ALIGN_OUT_LEFT_MID, -2, 0);
    lv_obj_align_to(button_right, param, LV_ALIGN_OUT_RIGHT_MID, +2, 0);


    param        = param_window_create(cont, &pdata->label_minutes);
    button_left  = icon_button_create(cont, LV_SYMBOL_MINUS, BTN_MINUTE_ID, -1);
    button_right = icon_button_create(cont, LV_SYMBOL_PLUS, BTN_MINUTE_ID, +1);

    lv_obj_align_to(param, param_prev, LV_ALIGN_OUT_BOTTOM_MID, 0, 4);
    lv_obj_align_to(button_left, param, LV_ALIGN_OUT_LEFT_MID, -2, 0);
    lv_obj_align_to(button_right, param, LV_ALIGN_OUT_RIGHT_MID, +2, 0);
    param_prev = param;


    param        = param_window_create(cont, &pdata->label_day);
    button_left  = icon_button_create(cont, LV_SYMBOL_MINUS, BTN_DAY_ID, -1);
    button_right = icon_button_create(cont, LV_SYMBOL_PLUS, BTN_DAY_ID, +1);

    lv_obj_align_to(param, param_prev, LV_ALIGN_OUT_BOTTOM_MID, 0, 4);
    lv_obj_align_to(button_left, param, LV_ALIGN_OUT_LEFT_MID, -2, 0);
    lv_obj_align_to(button_right, param, LV_ALIGN_OUT_RIGHT_MID, +2, 0);
    param_prev = param;


    param        = param_window_create(cont, &pdata->label_month);
    button_left  = icon_button_create(cont, LV_SYMBOL_MINUS, BTN_MONTH_ID, -1);
    button_right = icon_button_create(cont, LV_SYMBOL_PLUS, BTN_MONTH_ID, +1);

    lv_obj_align_to(param, param_prev, LV_ALIGN_OUT_BOTTOM_MID, 0, 4);
    lv_obj_align_to(button_left, param, LV_ALIGN_OUT_LEFT_MID, -2, 0);
    lv_obj_align_to(button_right, param, LV_ALIGN_OUT_RIGHT_MID, +2, 0);
    param_prev = param;


    param        = param_window_create(cont, &pdata->label_year);
    button_left  = icon_button_create(cont, LV_SYMBOL_MINUS, BTN_YEAR_ID, -1);
    button_right = icon_button_create(cont, LV_SYMBOL_PLUS, BTN_YEAR_ID, +1);

    lv_obj_align_to(param, param_prev, LV_ALIGN_OUT_BOTTOM_MID, 0, 4);
    lv_obj_align_to(button_left, param, LV_ALIGN_OUT_LEFT_MID, -2, 0);
    lv_obj_align_to(button_right, param, LV_ALIGN_OUT_RIGHT_MID, +2, 0);
    param_prev = param;


    update_page(model, pdata);

    ESP_LOGI(TAG, "Open");
}


static pman_msg_t page_event(pman_handle_t handle, void *state, pman_event_t event) {
    (void)handle;

    pman_msg_t msg = PMAN_MSG_NULL;

    struct page_data *pdata = state;

    mut_model_t *model = pman_get_user_data(handle);

    switch (event.tag) {
        case PMAN_EVENT_TAG_TIMER: {
            update_page(model, pdata);
            break;
        }

        case PMAN_EVENT_TAG_USER: {
            view_event_t *view_event = event.as.user;
            switch (view_event->tag) {
                case VIEW_EVENT_TAG_PAGE_WATCHER: {
                    break;
                }
            }
            break;
        }

        case PMAN_EVENT_TAG_LVGL: {
            lv_event_t         *lv_event = event.as.lvgl;
            lv_obj_t           *target   = lv_event_get_current_target_obj(lv_event);
            view_object_data_t *obj_data = lv_obj_get_user_data(target);

            switch (lv_event_get_code(lv_event)) {
                case LV_EVENT_CLICKED: {
                    switch (obj_data->id) {
                        case BTN_BACK_ID: {
                            if (!pdata->program_wash_cycle && pdata->modified) {
                                view_get_protocol(handle)->set_time(handle, pdata->current_time);
                            }
                            msg.stack_msg = PMAN_STACK_MSG_BACK();
                            break;
                        }

                        case BTN_CONFIRM_ID: {
                            model->prog.programmed_wash.start      = mktime(&pdata->current_time);
                            model->prog.programmed_wash.configured = 1;
                            view_get_protocol(handle)->load_program(handle, model->prog.programmed_wash.program_index);
                            msg.stack_msg = PMAN_STACK_MSG_REBASE(view_common_choice_page(model));
                            break;
                        }

                        case BTN_HOUR_ID: {
                            pdata->modified = 1;
                            if (obj_data->number > 0) {
                                pdata->current_time.tm_hour = (pdata->current_time.tm_hour + 1) % 24;
                            } else if (obj_data->number < 0) {
                                if (pdata->current_time.tm_hour > 0) {
                                    pdata->current_time.tm_hour--;
                                } else {
                                    pdata->current_time.tm_hour = 23;
                                }
                            }
                            update_page(model, pdata);
                            break;
                        }

                        case BTN_MINUTE_ID: {
                            pdata->modified = 1;
                            if (obj_data->number > 0) {
                                pdata->current_time.tm_min = (pdata->current_time.tm_min + 1) % 60;
                            } else if (obj_data->number < 0) {
                                if (pdata->current_time.tm_min > 0) {
                                    pdata->current_time.tm_min--;
                                } else {
                                    pdata->current_time.tm_min = 59;
                                }
                            }
                            update_page(model, pdata);
                            break;
                        }

                        case BTN_DAY_ID: {
                            pdata->modified = 1;

                            if (obj_data->number > 0) {
                                pdata->current_time.tm_mday++;
                            } else if (obj_data->number < 0) {
                                if (pdata->current_time.tm_mday > 1) {
                                    pdata->current_time.tm_mday--;
                                }
                            }

                            time_t correct_time = mktime(&pdata->current_time);
                            pdata->current_time = *localtime(&correct_time);

                            update_page(model, pdata);
                            break;
                        }

                        case BTN_MONTH_ID: {
                            pdata->modified = 1;
                            if (obj_data->number > 0) {
                                pdata->current_time.tm_mon = (pdata->current_time.tm_mon + 1) % 12;
                            } else if (obj_data->number < 0) {
                                if (pdata->current_time.tm_mon > 0) {
                                    pdata->current_time.tm_mon--;
                                } else {
                                    pdata->current_time.tm_mon = 11;
                                }
                            }

                            time_t correct_time = mktime(&pdata->current_time);
                            pdata->current_time = *localtime(&correct_time);

                            update_page(model, pdata);
                            break;
                        }


                        case BTN_YEAR_ID: {
                            pdata->modified = 1;
                            if (obj_data->number > 0) {
                                pdata->current_time.tm_year++;
                            } else if (obj_data->number < 0) {
                                if (pdata->current_time.tm_year > 124) {
                                    pdata->current_time.tm_year--;
                                }
                            }

                            time_t correct_time = mktime(&pdata->current_time);
                            pdata->current_time = *localtime(&correct_time);

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

        default:
            break;
    }

    return msg;
}


static void update_page(model_t *model, struct page_data *pdata) {
    (void)model;
    lv_label_set_text_fmt(pdata->label_hours, "%s: %02i", view_intl_get_string(model, STRINGS_ORE),
                          pdata->current_time.tm_hour);
    lv_label_set_text_fmt(pdata->label_minutes, "%s: %02i", view_intl_get_string(model, STRINGS_MINUTI),
                          pdata->current_time.tm_min);
    lv_label_set_text_fmt(pdata->label_day, "%s: %02i", view_intl_get_string(model, STRINGS_GIORNO),
                          pdata->current_time.tm_mday);
    lv_label_set_text_fmt(pdata->label_month, "%s: %02i", view_intl_get_string(model, STRINGS_MESE),
                          pdata->current_time.tm_mon + 1);
    lv_label_set_text_fmt(pdata->label_year, "%s: %02i", view_intl_get_string(model, STRINGS_ANNO),
                          1900 + pdata->current_time.tm_year);

    if (pdata->button_confirm) {
        time_t now  = time(NULL);
        time_t then = mktime(&pdata->current_time);

        if (now >= then) {
            lv_obj_add_state(pdata->button_confirm, LV_STATE_DISABLED);
        } else {
            lv_obj_remove_state(pdata->button_confirm, LV_STATE_DISABLED);
        }
    }
}


static lv_obj_t *param_window_create(lv_obj_t *parent, lv_obj_t **label) {
    lv_obj_t *cont = lv_obj_create(parent);
    lv_obj_set_size(cont, PARAM_WINDOW_WIDTH, 40);
    lv_obj_remove_flag(cont, LV_OBJ_FLAG_SCROLLABLE);

    *label = lv_label_create(cont);
    lv_label_set_long_mode(*label, LV_LABEL_LONG_CLIP);
    lv_obj_set_style_text_font(*label, STYLE_FONT_MEDIUM, LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(*label, LV_TEXT_ALIGN_CENTER, LV_STATE_DEFAULT);
    lv_obj_set_width(*label, PARAM_WINDOW_WIDTH - 10);
    lv_obj_center(*label);

    return cont;
}


static lv_obj_t *icon_button_create(lv_obj_t *parent, const char *text, uint16_t id, uint16_t number) {
    lv_obj_t *button = lv_button_create(parent);
    lv_obj_set_size(button, 40, 40);
    lv_obj_set_style_shadow_width(button, 0, LV_STATE_DEFAULT);
    lv_obj_remove_flag(button, LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_t *label = lv_label_create(button);
    lv_label_set_text(label, text);
    lv_obj_center(label);

    view_register_object_default_callback_with_number(button, id, number);

    return button;
}



const pman_page_t page_datetime = {
    .create        = create_page,
    .destroy       = pman_destroy_all,
    .open          = open_page,
    .close         = pman_close_all,
    .process_event = page_event,
};
