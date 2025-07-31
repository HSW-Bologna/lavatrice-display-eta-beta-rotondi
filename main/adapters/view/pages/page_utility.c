#include "lvgl.h"
#include "model/model.h"
#include "../view.h"
#include "../theme/style.h"
#include "../common.h"
#include "config/app_config.h"
#include "src/widgets/led/lv_led.h"
#include "../intl/intl.h"


#define BUTTON_WIDTH 150


struct page_data {
    lv_obj_t *dropdown;

    lv_obj_t *led_user_menu;
    lv_obj_t *led_opl;
    lv_obj_t *led_exclude_detergent;
    lv_obj_t *led_datetime;
};


enum {
    BTN_BACK_ID,
    BTN_PROGRAMMED_WASH_ID,
    BTN_USER_MENU_ID,
    BTN_DETERGENT_EXCLUSION_ID,
    BTN_DETERGENT_MENU_ID,
    BTN_DATETIME_ID,
};


static void update_page(model_t *model, struct page_data *pdata);


static void *create_page(pman_handle_t handle, void *extra) {
    (void)handle;
    (void)extra;

    struct page_data *pdata = lv_malloc(sizeof(struct page_data));
    assert(pdata != NULL);

    return pdata;
}


static void open_page(pman_handle_t handle, void *state) {
    struct page_data *pdata = state;

    model_t *model = view_get_model(handle);

    view_common_create_title(lv_scr_act(), view_intl_get_string(model, STRINGS_UTILITA), BTN_BACK_ID, -1);

    lv_obj_t *cont = lv_obj_create(lv_scr_act());
    lv_obj_set_style_pad_column(cont, 3, LV_STATE_DEFAULT);
    lv_obj_set_style_pad_row(cont, 4, LV_STATE_DEFAULT);
    lv_obj_set_style_pad_hor(cont, 0, LV_STATE_DEFAULT);
    lv_obj_set_size(cont, LV_HOR_RES, LV_VER_RES - 56);
    lv_obj_set_layout(cont, LV_LAYOUT_FLEX);
    lv_obj_set_flex_flow(cont, LV_FLEX_FLOW_ROW_WRAP);
    lv_obj_set_flex_align(cont, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_align(cont, LV_ALIGN_BOTTOM_MID, 0, 0);

    {
        lv_obj_t *btn = lv_btn_create(cont);
        lv_obj_set_width(btn, BUTTON_WIDTH * 2);
        lv_obj_t *lbl = lv_label_create(btn);
        lv_label_set_text(lbl, view_intl_get_string(model, STRINGS_MENU_UTENTE));
        lv_obj_set_style_text_font(lbl, STYLE_FONT_SMALL, LV_STATE_DEFAULT);
        lv_obj_align(lbl, LV_ALIGN_LEFT_MID, 16, 0);

        lv_obj_t *led = lv_led_create(btn);
        lv_obj_set_size(led, 28, 28);
        lv_led_set_color(led, VIEW_STYLE_COLOR_RED);
        lv_obj_align(lbl, LV_ALIGN_RIGHT_MID, -16, 0);
        pdata->led_user_menu = led;

        view_register_object_default_callback(btn, BTN_USER_MENU_ID);
    }

    {
        lv_obj_t *btn = lv_btn_create(cont);
        lv_obj_set_width(btn, BUTTON_WIDTH * 2);
        lv_obj_t *lbl = lv_label_create(btn);
        lv_label_set_text(lbl, view_intl_get_string(model, STRINGS_ESCLUSIONE_SAPONE));
        lv_obj_set_style_text_font(lbl, STYLE_FONT_SMALL, LV_STATE_DEFAULT);
        lv_obj_align(lbl, LV_ALIGN_LEFT_MID, 16, 0);

        lv_obj_t *led = lv_led_create(btn);
        lv_obj_set_size(led, 28, 28);
        lv_led_set_color(led, VIEW_STYLE_COLOR_RED);
        lv_obj_align(lbl, LV_ALIGN_RIGHT_MID, -16, 0);
        pdata->led_exclude_detergent = led;

        view_register_object_default_callback(btn, BTN_DETERGENT_EXCLUSION_ID);
    }

    {
        lv_obj_t *btn = lv_btn_create(cont);
        lv_obj_set_width(btn, BUTTON_WIDTH);
        lv_obj_t *lbl = lv_label_create(btn);
        lv_label_set_text(lbl, view_intl_get_string(model, STRINGS_MENU_SAPONI));
        lv_obj_set_style_text_font(lbl, STYLE_FONT_SMALL, LV_STATE_DEFAULT);
        lv_obj_align(lbl, LV_ALIGN_CENTER, 0, 0);

        view_register_object_default_callback(btn, BTN_DETERGENT_MENU_ID);
    }

    {
        lv_obj_t *btn = lv_btn_create(cont);
        lv_obj_set_width(btn, BUTTON_WIDTH * 2);
        lv_obj_t *lbl = lv_label_create(btn);
        lv_label_set_text(lbl, view_intl_get_string(model, STRINGS_DATA_ORA));
        lv_obj_set_style_text_font(lbl, STYLE_FONT_SMALL, LV_STATE_DEFAULT);
        lv_obj_align(lbl, LV_ALIGN_LEFT_MID, 16, 0);

        lv_obj_t *led = lv_led_create(btn);
        lv_obj_set_size(led, 28, 28);
        lv_led_set_color(led, VIEW_STYLE_COLOR_RED);
        lv_obj_align(lbl, LV_ALIGN_RIGHT_MID, -16, 0);
        pdata->led_datetime = led;

        view_register_object_default_callback(btn, BTN_DATETIME_ID);
    }

    if (model->prog.parmac.abilitazione_lavaggio_programmato) {
        lv_obj_t *btn = lv_btn_create(cont);
        lv_obj_set_width(btn, BUTTON_WIDTH * 2);
        lv_obj_t *lbl = lv_label_create(btn);
        lv_obj_set_width(lbl, LV_PCT(100));
        lv_label_set_long_mode(lbl, LV_LABEL_LONG_SCROLL);
        lv_obj_set_style_text_font(lbl, STYLE_FONT_SMALL, LV_STATE_DEFAULT);
        lv_label_set_text(lbl, view_intl_get_string(model, STRINGS_LAVAGGIO_PROGRAMMATO));
        lv_obj_center(lbl);
        view_register_object_default_callback(btn, BTN_PROGRAMMED_WASH_ID);
    }

    update_page(model, pdata);
}


static pman_msg_t page_event(pman_handle_t handle, void *state, pman_event_t event) {
    struct page_data *pdata = state;

    mut_model_t *model = view_get_model(handle);

    pman_msg_t msg = PMAN_MSG_NULL;

    switch (event.tag) {
        case PMAN_EVENT_TAG_OPEN:
            break;

        case PMAN_EVENT_TAG_USER: {
            view_event_t *view_event = event.as.user;
            switch (view_event->tag) {
                case VIEW_EVENT_TAG_PAGE_WATCHER: {
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
                            view_get_protocol(handle)->save_parmac(handle);
                            msg.stack_msg = PMAN_STACK_MSG_BACK();
                            break;

                        case BTN_PROGRAMMED_WASH_ID:
                            msg.stack_msg = PMAN_STACK_MSG_PUSH_PAGE(&page_pick_program);
                            break;

                        case BTN_USER_MENU_ID:
                            model->prog.parmac.visualizzazione_menu = !model->prog.parmac.visualizzazione_menu;
                            update_page(model, pdata);
                            break;

                        case BTN_DETERGENT_EXCLUSION_ID:
                            model->prog.parmac.visualizzazione_esclusione_sapone =
                                !model->prog.parmac.visualizzazione_esclusione_sapone;
                            update_page(model, pdata);
                            break;

                        case BTN_DATETIME_ID:
                            model->prog.parmac.visualizzazione_data_ora = !model->prog.parmac.visualizzazione_data_ora;
                            update_page(model, pdata);
                            break;

                        case BTN_DETERGENT_MENU_ID:
                            msg.stack_msg = PMAN_STACK_MSG_PUSH_PAGE_EXTRA(&page_work_parameters, (void *)(uintptr_t)1);
                            break;

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
    (void)pdata;

    if (model->prog.parmac.visualizzazione_menu) {
        lv_led_on(pdata->led_user_menu);
    } else {
        lv_led_off(pdata->led_user_menu);
    }

    if (model->prog.parmac.visualizzazione_esclusione_sapone) {
        lv_led_on(pdata->led_exclude_detergent);
    } else {
        lv_led_off(pdata->led_exclude_detergent);
    }

    if (model->prog.parmac.visualizzazione_data_ora) {
        lv_led_on(pdata->led_datetime);
    } else {
        lv_led_off(pdata->led_datetime);
    }
}


const pman_page_t page_utility = {
    .create        = create_page,
    .destroy       = pman_destroy_all,
    .open          = open_page,
    .close         = pman_close_all,
    .process_event = page_event,
};
