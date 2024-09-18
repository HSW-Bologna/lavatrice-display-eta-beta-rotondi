#include <assert.h>
#include <stdlib.h>
#include "lvgl.h"
#include "model/model.h"
#include "../view.h"
#include "../theme/style.h"
#include "../intl/intl.h"
#include "../common.h"
#include <esp_log.h>
#include "bsp/tft/display.h"
#include "config/app_config.h"


#define ALLARME_CHIAVISTELLO 15
#define ALLARME_SCARICO      32


enum {
    BTN_STOP_ID,
    BTN_START_ID,
};


static const char *TAG = "PageMain";


struct page_data {
    lv_obj_t *label_status;
    lv_obj_t *button_start;
    lv_obj_t *button_stop;

    uint8_t scarico_fallito;
};


static void update_page(model_t *pmodel, struct page_data *pdata);


static void *create_page(pman_handle_t handle, void *extra) {
    (void)handle;
    (void)extra;

    struct page_data *pdata = lv_malloc(sizeof(struct page_data));
    assert(pdata != NULL);

    pdata->scarico_fallito = 0;

    return pdata;
}


static void open_page(pman_handle_t handle, void *state) {
    model_t          *model = view_get_model(handle);
    struct page_data *pdata = state;

    lv_obj_t *lbl = lv_label_create(lv_screen_active());
    lv_label_set_text(lbl, "Lavaggio");
    lv_obj_align(lbl, LV_ALIGN_TOP_MID, 0, 0);

    {
        lv_obj_t *btn = lv_button_create(lv_screen_active());
        lv_obj_align(btn, LV_ALIGN_CENTER, +72, 0);
        lbl = lv_label_create(btn);
        lv_label_set_text(lbl, "Start");
        lv_obj_center(lbl);
        view_register_object_default_callback(btn, BTN_START_ID);
        pdata->button_start = btn;
    }

    {
        lv_obj_t *btn = lv_button_create(lv_screen_active());
        lv_obj_align(btn, LV_ALIGN_CENTER, -72, 0);
        lbl = lv_label_create(btn);
        lv_label_set_text(lbl, "Stop");
        lv_obj_center(lbl);
        view_register_object_default_callback(btn, BTN_STOP_ID);
        pdata->button_stop = btn;
    }

    VIEW_ADD_WATCHED_VARIABLE(&model->run.macchina.stato, 0);
    VIEW_ADD_WATCHED_VARIABLE(&model->run.macchina.sottostato, 0);

    update_page(model, pdata);
    ESP_LOGI(TAG, "Open");
}


static pman_msg_t page_event(pman_handle_t handle, void *state, pman_event_t event) {
    model_t *model = view_get_model(handle);

    pman_msg_t msg = PMAN_MSG_NULL;

    struct page_data *pdata = state;

    msg.user_msg = NULL;

    switch (event.tag) {
        case PMAN_EVENT_TAG_TIMER: {
            msg.stack_msg = PMAN_STACK_MSG_BACK();
            break;
        }

        case PMAN_EVENT_TAG_USER: {
            view_event_t *view_event = event.as.user;
            switch (view_event->tag) {
                case VIEW_EVENT_TAG_PAGE_WATCHER: {
                    if (model_macchina_in_stop(model) || !model_can_work(model)) {
                        model->run.done = 1;
                        msg.stack_msg   = PMAN_STACK_MSG_REBASE(&page_main);
                    }

                    if (model_alarm_code(model) == ALLARME_SCARICO || model_alarm_code(model) == ALLARME_CHIAVISTELLO) {
                        if (pdata->scarico_fallito == 0) {
                            ESP_LOGI(TAG, "Scarico fallito");
                            pdata->scarico_fallito = 1;
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
            lv_obj_t           *target   = lv_event_get_target_obj(event.as.lvgl);
            view_object_data_t *obj_data = lv_obj_get_user_data(target);

            switch (lv_event_get_code(event.as.lvgl)) {
                case LV_EVENT_CLICKED: {
                    switch (obj_data->id) {
                        case BTN_START_ID: {
                            view_get_protocol(handle)->start_program(handle);
                            msg.stack_msg = PMAN_STACK_MSG_BACK();
                            break;
                        }

                        case BTN_STOP_ID: {
                            if (model_macchina_in_pausa(model)) {
                                ESP_LOGI(TAG, "Requesting stop");
                                view_get_protocol(handle)->stop_program(handle);
                            } else if (model_macchina_in_marcia(model)) {
                                ESP_LOGI(TAG, "Requesting pause");
                                view_get_protocol(handle)->pause_program(handle);
                            }
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
    lv_obj_t *lbl = lv_obj_get_child(pdata->button_stop, 0);

    if (model_macchina_in_stop(model)) {
        view_common_set_hidden(pdata->button_stop, 1);
        view_common_set_hidden(pdata->button_start, 0);
    } else if (model_macchina_in_pausa(model)) {
        view_common_set_hidden(pdata->button_stop, 0);
        view_common_set_hidden(pdata->button_start, 0);
        lv_label_set_text(lbl, "Stop");
    } else {
        view_common_set_hidden(pdata->button_stop, 0);
        view_common_set_hidden(pdata->button_start, 1);
        lv_label_set_text(lbl, "Pausa");
    }
}


const pman_page_t page_washing = {
    .create        = create_page,
    .destroy       = pman_destroy_all,
    .open          = open_page,
    .close         = pman_close_all,
    .process_event = page_event,
};
