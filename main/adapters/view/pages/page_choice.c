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


enum {
    BTN_BACK_ID,
    BTN_START_ID,
};


static const char *TAG = "PageMain";


struct page_data {
    lv_obj_t *lbl_status;
    lv_obj_t *lbl_credit;
    lv_obj_t *lbl_price;

    lv_obj_t *button_start;

    uint16_t allarme;
    uint16_t number;
    uint16_t previous_credit;

    pman_timer_t *timer;
};


static void update_page(model_t *pmodel, struct page_data *pdata);


static void *create_page(pman_handle_t handle, void *extra) {
    model_t *model = view_get_model(handle);

    struct page_data *pdata = lv_malloc(sizeof(struct page_data));
    assert(pdata != NULL);

    pdata->number = (uint16_t)(uintptr_t)extra;
    pdata->timer  = PMAN_REGISTER_TIMER_ID(handle, model->prog.parmac.tempo_out_pagine * 1000UL, 0);

    return pdata;
}


static void open_page(pman_handle_t handle, void *state) {
    struct page_data *pdata = state;
    pman_timer_reset(pdata->timer);
    pman_timer_resume(pdata->timer);

    model_t *model = view_get_model(handle);

    const programma_preview_t *preview = model_get_preview(model, pdata->number);

    view_title_t title = view_common_create_title(lv_screen_active(), preview->name, BTN_BACK_ID, -1);
    lv_label_set_long_mode(title.label_title, LV_LABEL_LONG_SCROLL_CIRCULAR);

    lv_obj_t *cont = lv_obj_create(lv_scr_act());
    lv_obj_add_style(cont, &style_padless_cont, LV_STATE_DEFAULT);
    lv_obj_set_style_pad_column(cont, 12, LV_STATE_DEFAULT);
    lv_obj_set_style_pad_row(cont, 12, LV_STATE_DEFAULT);
    lv_obj_set_size(cont, LV_HOR_RES, LV_VER_RES - 56);
    lv_obj_set_layout(cont, LV_LAYOUT_FLEX);
    lv_obj_set_flex_flow(cont, LV_FLEX_FLOW_ROW_WRAP);
    lv_obj_set_flex_align(cont, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_align(cont, LV_ALIGN_BOTTOM_MID, 0, 0);

    pdata->allarme         = 0;
    pdata->previous_credit = model_get_credito(model);

    lv_obj_t *lbl = lv_label_create(lv_scr_act());
    lv_label_set_long_mode(lbl, LV_LABEL_LONG_SCROLL_CIRCULAR);
    lv_obj_set_width(lbl, LV_PCT(95));
    lv_obj_set_style_text_align(lbl, LV_TEXT_ALIGN_CENTER, LV_STATE_DEFAULT);
    lv_obj_align(lbl, LV_ALIGN_BOTTOM_MID, 0, 0);
    pdata->lbl_status = lbl;

    lbl             = lv_label_create(lv_scr_act());
    char string[32] = {0};
    model_formatta_prezzo(string, model, preview->prezzo);
    lv_label_set_text(lbl, string);
    lv_obj_align(lbl, LV_ALIGN_CENTER, 0, -10);
    pdata->lbl_price = lbl;

    lbl = lv_label_create(lv_scr_act());
    lv_obj_set_style_text_align(lbl, LV_TEXT_ALIGN_CENTER, LV_STATE_DEFAULT);
    lv_obj_align(lbl, LV_ALIGN_CENTER, 0, 10);
    pdata->lbl_credit = lbl;

    {
        lv_obj_t *btn = lv_button_create(lv_screen_active());
        lv_obj_set_size(btn, 64, 64);
        lv_obj_t *lbl = lv_label_create(btn);
        lv_label_set_text(lbl, LV_SYMBOL_PLAY);
        lv_obj_center(lbl);
        lv_obj_center(btn);
        view_register_object_default_callback(btn, BTN_START_ID);
        pdata->button_start = btn;
    }

    VIEW_ADD_WATCHED_VARIABLE(&model->run.macchina.codice_allarme, 0);
    VIEW_ADD_WATCHED_VARIABLE(&model->run.macchina.pagato, 0);
    VIEW_ADD_WATCHED_ARRAY(model->run.macchina.credito, LINEE_PAGAMENTO_GETTONIERA, 0);
    VIEW_ADD_WATCHED_VARIABLE(&model->run.credito, 0);
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
                    if (model_get_credito(model) != pdata->previous_credit) {
                        pman_timer_reset(pdata->timer);
                        pdata->previous_credit = model_get_credito(model);
                    }

                    if (!model_macchina_in_stop(model) && model_can_work(model)) {
                        msg.stack_msg = PMAN_STACK_MSG_PUSH_PAGE(&page_washing);
                    } else if (model_alarm_code(model) > 0) {
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

        case PMAN_EVENT_TAG_LVGL: {
            lv_obj_t           *target   = lv_event_get_target_obj(event.as.lvgl);
            view_object_data_t *obj_data = lv_obj_get_user_data(target);

            switch (lv_event_get_code(event.as.lvgl)) {
                case LV_EVENT_CLICKED: {
                    pman_timer_reset(pdata->timer);

                    switch (obj_data->id) {
                        case BTN_BACK_ID: {
                            msg.stack_msg = PMAN_STACK_MSG_BACK();
                            break;
                        }

                        case BTN_START_ID: {
                            if (model_lavaggio_pagato(model, pdata->number)) {
                                view_get_protocol(handle)->start_program(handle);
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

            break;
        }

        default:
            break;
    }

    return msg;
}


static void update_page(model_t *model, struct page_data *pdata) {
    if (model_alarm_code(model) > 0) {
        if (pdata->allarme != model_alarm_code(model)) {
            pdata->allarme = model_alarm_code(model);
            lv_label_set_text(pdata->lbl_status, view_common_alarm_description(model));
        }
        view_common_set_hidden(pdata->lbl_credit, 0);
        view_common_set_hidden(pdata->lbl_price, 0);
        view_common_set_hidden(pdata->button_start, 1);
    } else if (model_lavaggio_pagato(model, pdata->number)) {
        lv_label_set_text(pdata->lbl_status, view_intl_get_string_in_language(model_get_temporary_language(model),
                                                                              STRINGS_SCELTA_PROGRAMMA));
        view_common_set_hidden(pdata->lbl_credit, 1);
        view_common_set_hidden(pdata->lbl_price, 1);
        view_common_set_hidden(pdata->button_start, 0);
    } else {
        lv_label_set_text(pdata->lbl_status, view_require_payment_string(model, model_get_temporary_language(model)));
        view_common_set_hidden(pdata->lbl_credit, 0);
        view_common_set_hidden(pdata->lbl_price, 0);
        view_common_set_hidden(pdata->button_start, 1);
    }

    char string[32] = {0};
    model_formatta_prezzo(string, model, model_get_credito(model));
    lv_label_set_text(pdata->lbl_credit, string);
}


static void destroy_page(void *state, void *extra) {
    struct page_data *pdata = state;
    (void)extra;
    pman_timer_delete(pdata->timer);
    lv_free(pdata);
}


static void close_page(void *state) {
    struct page_data *pdata = state;
    pman_timer_pause(pdata->timer);
    lv_obj_clean(lv_scr_act());
}


const pman_page_t page_choice = {
    .create        = create_page,
    .destroy       = destroy_page,
    .open          = open_page,
    .close         = close_page,
    .process_event = page_event,
};
