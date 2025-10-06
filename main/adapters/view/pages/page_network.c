#include <stdio.h>
#include "lvgl.h"
#include "model/model.h"
#include "../view.h"
#include "../theme/style.h"
#include "../common.h"
#include "config/app_config.h"
#include "src/widgets/led/lv_led.h"
#include "../intl/intl.h"
#include "model/descriptions/AUTOGEN_FILE_pars.h"
#include <esp_log.h>


struct page_data {
    lv_obj_t *button_networks[MAX_AP_SCAN_LIST_SIZE];

    lv_obj_t *textarea;

    lv_obj_t *blanket;

    lv_obj_t *label_status;

    bool insert_password;

    char ssid[33];
};


enum {
    BTN_BACK_ID,
    BTN_SCAN_ID,
    BTN_CONNECT_ID,
    PASSWORD_KB_ID,
};


static void update_page(model_t *model, struct page_data *pdata);


static const char *TAG = __FILE_NAME__;


static void *create_page(pman_handle_t handle, void *extra) {
    (void)handle;
    (void)extra;

    struct page_data *pdata = lv_malloc(sizeof(struct page_data));
    assert(pdata != NULL);
    pdata->insert_password = false;

    return pdata;
}


static void open_page(pman_handle_t handle, void *state) {
    struct page_data *pdata = state;

    model_t *model = view_get_model(handle);

    view_common_create_title(lv_scr_act(), view_intl_get_string(model, STRINGS_RETE), BTN_BACK_ID, -1);

    {
        lv_obj_t *button = lv_button_create(lv_screen_active());
        lv_obj_add_style(button, (lv_style_t *)&style_config_btn, LV_STATE_DEFAULT);
        lv_obj_add_style(button, (lv_style_t *)&style_icon_button, LV_STATE_DEFAULT);
        lv_obj_set_size(button, 48, 48);
        lv_obj_t *label = lv_label_create(button);
        lv_label_set_text(label, LV_SYMBOL_REFRESH);
        lv_obj_center(label);
        lv_obj_align(button, LV_ALIGN_TOP_RIGHT, -4, 0);
        view_register_object_default_callback(button, BTN_SCAN_ID);
    }

    {
        lv_obj_t *label = lv_label_create(lv_screen_active());
        lv_obj_align(label, LV_ALIGN_TOP_MID, 0, 56);
        lv_obj_set_style_text_font(label, STYLE_FONT_SMALL, LV_STATE_DEFAULT);
        pdata->label_status = label;
    }

    lv_obj_t *cont = lv_obj_create(lv_scr_act());
    lv_obj_set_size(cont, LV_HOR_RES, LV_VER_RES - 56 - 24);
    lv_obj_set_layout(cont, LV_LAYOUT_FLEX);
    lv_obj_set_flex_flow(cont, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(cont, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_align(cont, LV_ALIGN_BOTTOM_MID, 0, 0);
    lv_obj_add_flag(cont, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_scrollbar_mode(cont, LV_SCROLLBAR_MODE_ON);

    for (size_t i = 0; i < MAX_AP_SCAN_LIST_SIZE; i++) {
        lv_obj_t *button = lv_button_create(cont);
        lv_obj_set_width(button, LV_PCT(100));

        lv_obj_t *label = lv_label_create(button);
        lv_obj_set_style_text_font(label, STYLE_FONT_MEDIUM, LV_STATE_DEFAULT);

        view_register_object_default_callback_with_number(button, BTN_CONNECT_ID, i);
        pdata->button_networks[i] = button;
    }

    {
        lv_obj_t *cont = lv_obj_create(lv_scr_act());
        lv_obj_set_size(cont, LV_HOR_RES, LV_VER_RES);
        lv_obj_add_style(cont, (lv_style_t *)&style_transparent_cont, LV_STATE_DEFAULT);
        lv_obj_set_style_bg_opa(cont, LV_OPA_70, LV_STATE_DEFAULT);
        lv_obj_align(cont, LV_ALIGN_BOTTOM_MID, 0, 0);
        lv_obj_remove_flag(cont, LV_OBJ_FLAG_SCROLLABLE);

        lv_obj_t *ta = lv_textarea_create(cont);
        lv_textarea_set_one_line(ta, 1);
        lv_textarea_set_password_mode(ta, 1);
        lv_textarea_set_text(ta, "");
        lv_textarea_set_max_length(ta, 64);
        lv_obj_set_width(ta, LV_PCT(50));
        lv_obj_remove_flag(ta, LV_OBJ_FLAG_SCROLLABLE);
        lv_obj_align(ta, LV_ALIGN_TOP_MID, 0, 20);
        lv_obj_set_style_text_font(ta, STYLE_FONT_MEDIUM, LV_STATE_DEFAULT);
        lv_obj_set_style_text_align(ta, LV_TEXT_ALIGN_CENTER, LV_STATE_DEFAULT);
        lv_obj_add_state(ta, LV_STATE_FOCUSED);
        pdata->textarea = ta;

        lv_obj_t *kb = lv_keyboard_create(cont);
        lv_keyboard_set_textarea(kb, ta);
        lv_obj_add_style(kb, (lv_style_t *)&style_config_btn, LV_PART_ITEMS);
        lv_obj_set_style_text_font(kb, STYLE_FONT_SMALL, LV_PART_ITEMS);
        lv_obj_set_size(kb, 320, 180);
        lv_obj_align(kb, LV_ALIGN_BOTTOM_MID, 0, 0);
        view_register_object_default_callback(kb, PASSWORD_KB_ID);

        pdata->blanket = cont;
    }

    VIEW_ADD_WATCHED_VARIABLE(&model->run.ssid_num, 0);
    view_add_watched_variable((void *)model->run.ssids, sizeof(model->run.ssids), 0);
    VIEW_ADD_WATCHED_VARIABLE(&model->run.wifi_state, 0);

    update_page(model, pdata);
}


static pman_msg_t page_event(pman_handle_t handle, void *state, pman_event_t event) {
    struct page_data *pdata = state;

    mut_model_t *model = view_get_model(handle);

    pman_msg_t msg = PMAN_MSG_NULL;

    switch (event.tag) {
        case PMAN_EVENT_TAG_OPEN:
            view_get_protocol(handle)->scan_wifi(handle);
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
                            msg.stack_msg = PMAN_STACK_MSG_BACK();
                            break;

                        case BTN_CONNECT_ID: {
                            snprintf(pdata->ssid, sizeof(pdata->ssid), "%s", model->run.ssids[obj_data->number]);
                            pdata->insert_password = true;
                            update_page(model, pdata);
                            break;
                        }

                        case BTN_SCAN_ID:
                            view_get_protocol(handle)->scan_wifi(handle);
                            break;
                    }
                    break;
                }

                case LV_EVENT_READY: {
                    view_get_protocol(handle)->connect_to_wifi(handle, pdata->ssid,
                                                               lv_textarea_get_text(pdata->textarea));
                    pdata->insert_password = false;
                    update_page(model, pdata);
                    break;
                }

                case LV_EVENT_CANCEL: {
                    pdata->insert_password = false;
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

    return msg;
}


static void update_page(model_t *model, struct page_data *pdata) {
    switch (model->run.wifi_state) {
        case WIFI_STATE_CONNECTED: {
            lv_label_set_text_fmt(pdata->label_status, "%s: %i.%i.%i.%i", model->run.connected_ssid,
                                  (int)(model->run.ip_addr >> 24) & 0xFF, (int)(model->run.ip_addr >> 16) & 0xFF,
                                  (int)(model->run.ip_addr >> 8) & 0xFF, (int)(model->run.ip_addr >> 0) & 0xFF);
            break;
        }

        case WIFI_STATE_CONNECTING: {
            lv_label_set_text(pdata->label_status, view_intl_get_string(model, STRINGS_CONNESSIONE_IN_CORSO));
            break;
        }

        case WIFI_STATE_DISCONNECTED: {
            lv_label_set_text(pdata->label_status, view_intl_get_string(model, STRINGS_NON_CONNESSO));
            break;
        }
    }

    view_common_set_hidden(pdata->blanket, !pdata->insert_password);

    for (size_t i = 0; i < MAX_AP_SCAN_LIST_SIZE; i++) {
        if (i < model->run.ssid_num) {
            view_common_set_hidden(pdata->button_networks[i], false);
            lv_label_set_text(lv_obj_get_child(pdata->button_networks[i], 0), model->run.ssids[i]);
        } else {
            view_common_set_hidden(pdata->button_networks[i], true);
        }
    }
}


const pman_page_t page_network = {
    .create        = create_page,
    .destroy       = pman_destroy_all,
    .open          = open_page,
    .close         = pman_close_all,
    .process_event = page_event,
};
