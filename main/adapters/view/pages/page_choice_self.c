#include <stdio.h>
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


LV_IMG_DECLARE(img_lavaggio);
LV_IMG_DECLARE(img_colorati);
LV_IMG_DECLARE(img_lavaggio_molto_sporchi);
LV_IMG_DECLARE(img_lavaggio_sporchi);
LV_IMG_DECLARE(img_normali);
LV_IMG_DECLARE(img_molto_sporchi);
LV_IMG_DECLARE(img_sintetici);
LV_IMG_DECLARE(img_piumoni);
LV_IMG_DECLARE(img_delicati);
LV_IMG_DECLARE(img_freddo);
LV_IMG_DECLARE(img_lana);
LV_IMG_DECLARE(img_fibre_naturali);
LV_IMG_DECLARE(img_solo_centrifuga);
LV_IMG_DECLARE(img_centrifuga_delicati);
LV_IMG_DECLARE(img_igienizza_cesto);
LV_IMG_DECLARE(img_ammollo);
LV_IMG_DECLARE(img_prelavaggio_centrifuga);
LV_IMG_DECLARE(img_risciacquo_centrifuga);
LV_IMG_DECLARE(img_padlock_closed);
LV_IMG_DECLARE(img_padlock_open);
LV_IMG_DECLARE(img_italiano);
LV_IMG_DECLARE(img_english);
LV_IMAGE_DECLARE(img_play_released);
LV_IMAGE_DECLARE(img_stop_released);
LV_IMAGE_DECLARE(img_pause_released);
LV_IMAGE_DECLARE(img_pause_pressed);
LV_IMAGE_DECLARE(img_stop);
LV_IMG_DECLARE(img_marble_italiano);
LV_IMG_DECLARE(img_marble_english);


enum {
    BTN_PLACEHOLDER_ID,
    BTN_START_ID,
    BTN_STOP_ID,
    BTN_DETERGENT_ID,
    BTN_LANGUAGE_ID,
    TIMER_BACK_ID,
    TIMER_MINUTES_ID,
};


static const char *TAG = __FILE_NAME__;


struct page_data {
    lv_obj_t *button_start;
    lv_obj_t *button_stop;
    lv_obj_t *button_detergent;

    lv_obj_t *label_status;
    lv_obj_t *label_credit;
    lv_obj_t *label_price;

    lv_obj_t *image_language;

    pman_timer_t *timer;
    pman_timer_t *timer_minutes;

    uint8_t  scarico_fallito;
    uint16_t number;
    uint16_t previous_credit;
    uint16_t allarme;
};


static void update_page(model_t *model, struct page_data *pdata);


static const lv_img_dsc_t *icons[] = {
    &img_lavaggio_molto_sporchi,
    &img_lavaggio_sporchi,
    &img_molto_sporchi,
    &img_normali,
    &img_colorati,
    &img_sintetici,
    &img_piumoni,
    &img_freddo,
    &img_lana,
    &img_fibre_naturali,
    &img_solo_centrifuga,
    &img_centrifuga_delicati,
    &img_igienizza_cesto,
    &img_ammollo,
    &img_prelavaggio_centrifuga,
    &img_risciacquo_centrifuga,
};


static void *create_page(pman_handle_t handle, void *extra) {
    model_t *model = view_get_model(handle);
    (void)handle;
    (void)extra;

    struct page_data *pdata = lv_malloc(sizeof(struct page_data));
    assert(pdata != NULL);

    pdata->number          = (uint16_t)(uintptr_t)extra;
    pdata->scarico_fallito = 0;
    pdata->allarme         = -1;
    pdata->timer         = PMAN_REGISTER_TIMER_ID(handle, model->prog.parmac.tempo_out_pagine * 1000UL, TIMER_BACK_ID);
    pdata->timer_minutes = PMAN_REGISTER_TIMER_ID(handle, 30UL * 1000UL, TIMER_MINUTES_ID);

    return pdata;
}


static void open_page(pman_handle_t handle, void *state) {
    model_t          *model = view_get_model(handle);
    struct page_data *pdata = state;
    pman_timer_reset(pdata->timer);
    pman_timer_resume(pdata->timer);

    pman_timer_resume(pdata->timer_minutes);

    const programma_preview_t *preview = model_get_preview(model, model_get_program_num(model));

    lv_obj_t *cont = lv_obj_create(lv_scr_act());
    lv_obj_remove_flag(cont, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_border_width(cont, 0, LV_STATE_DEFAULT);
    lv_obj_set_style_radius(cont, 0, LV_STATE_DEFAULT);
    lv_obj_set_style_pad_all(cont, 8, LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(cont, VIEW_STYLE_COLOR_WHITE, LV_STATE_DEFAULT);
    lv_obj_set_size(cont, LV_HOR_RES, LV_VER_RES);
    lv_obj_align(cont, LV_ALIGN_TOP_MID, 0, 0);

    {
        lv_obj_t *image = lv_image_create(cont);
        if (preview->tipo < sizeof(icons) / sizeof(icons[0])) {
            lv_img_set_src(image, icons[preview->tipo]);
        } else {
            lv_img_set_src(image, &img_lavaggio);
        }
        lv_image_set_scale(image, 280);
        lv_obj_align(image, LV_ALIGN_TOP_LEFT, 4, 8);

        lv_obj_t *lbl = lv_label_create(cont);
        lv_obj_set_style_text_color(lbl, VIEW_STYLE_COLOR_BLACK, LV_STATE_DEFAULT);
        lv_obj_set_style_text_font(lbl, STYLE_FONT_MEDIUM, LV_STATE_DEFAULT);
        lv_obj_set_style_text_align(lbl, LV_TEXT_ALIGN_LEFT, LV_STATE_DEFAULT);
        lv_label_set_long_mode(lbl, LV_LABEL_LONG_WRAP);
        lv_obj_set_width(lbl, 230);
        lv_obj_align_to(lbl, image, LV_ALIGN_OUT_RIGHT_TOP, 20, -10);
        lv_label_set_text_fmt(lbl, "%i° %s", preview->temperatura, preview->name);
    }

    {
        lv_obj_t *img = lv_img_create(cont);
        lv_obj_add_flag(img, LV_OBJ_FLAG_CLICKABLE);
        lv_image_set_src(img, &img_play_released);
        lv_obj_align(img, LV_ALIGN_TOP_RIGHT, 0, 72);
        view_register_object_default_callback(img, BTN_START_ID);
        pdata->button_start = img;
    }

    {
        LV_IMG_DECLARE(img_door)

        lv_obj_t *btn = lv_button_create(cont);
        lv_obj_add_style(btn, (lv_style_t *)&style_config_btn, LV_STATE_DEFAULT);
        lv_obj_add_style(btn, (lv_style_t *)&style_icon_button, LV_STATE_DEFAULT);
        lv_obj_set_size(btn, 64, 64);
        lv_obj_t *img = lv_image_create(btn);
        lv_image_set_src(img, &img_door);
        lv_obj_add_style(img, &style_white_icon, LV_STATE_DEFAULT);
        lv_obj_center(img);
        lv_obj_align(btn, LV_ALIGN_TOP_LEFT, 0, 72);
        view_register_object_default_callback(btn, BTN_STOP_ID);

        pdata->button_stop = btn;
    }

    {
        LV_IMG_DECLARE(img_soap);
        lv_obj_t *btn = lv_btn_create(cont);
        lv_obj_add_style(btn, (lv_style_t *)&style_config_btn, LV_STATE_DEFAULT);
        lv_obj_add_style(btn, (lv_style_t *)&style_icon_button, LV_STATE_DEFAULT);
        lv_obj_set_size(btn, 64, 64);
        lv_obj_t *img = lv_img_create(btn);
        lv_img_set_src(img, &img_soap);
        lv_obj_center(img);
        lv_obj_align(btn, LV_ALIGN_BOTTOM_RIGHT, 0, -32);
        view_register_object_default_callback(btn, BTN_DETERGENT_ID);
        pdata->button_detergent = btn;
    }


    {
        lv_obj_t *lbl = lv_label_create(lv_screen_active());
        lv_label_set_long_mode(lbl, LV_LABEL_LONG_SCROLL_CIRCULAR);
        lv_obj_set_style_text_color(lbl, VIEW_STYLE_COLOR_BLACK, LV_STATE_DEFAULT);
        lv_obj_set_width(lbl, LV_PCT(100));
        lv_obj_set_style_text_align(lbl, LV_TEXT_ALIGN_CENTER, LV_STATE_DEFAULT);
        lv_obj_align(lbl, LV_ALIGN_BOTTOM_MID, 0, -2);
        pdata->label_status = lbl;
    }

    pdata->previous_credit = model_get_credito(model);

    lv_obj_t *lbl = lv_label_create(cont);
    lv_obj_set_style_text_color(lbl, VIEW_STYLE_COLOR_BLACK, LV_STATE_DEFAULT);
    lv_obj_align(lbl, LV_ALIGN_CENTER, 96, 8);
    pdata->label_credit = lbl;

    lbl = lv_label_create(cont);
    lv_obj_set_style_text_color(lbl, VIEW_STYLE_COLOR_BLACK, LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(lbl, LV_TEXT_ALIGN_CENTER, LV_STATE_DEFAULT);
    lv_obj_align(lbl, LV_ALIGN_CENTER, -96, 8);
    pdata->label_price = lbl;

    {
        lv_obj_t *img = lv_image_create(cont);
        lv_obj_add_flag(img, LV_OBJ_FLAG_CLICKABLE);
        lv_image_set_scale(img, 200);
        lv_image_set_src(img, &img_marble_italiano);
        view_register_object_default_callback(img, BTN_LANGUAGE_ID);
        lv_obj_align(img, LV_ALIGN_BOTTOM_LEFT, -8, -32);
        pdata->image_language = img;
    }

    VIEW_ADD_WATCHED_VARIABLE(&model->run.macchina, 0);

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
            int timer_id = (int)(uintptr_t)event.as.timer->user_data;

            switch (timer_id) {
                case TIMER_BACK_ID:
                    if (!model_lavaggio_programmato_impostato(model, NULL)) {
                        msg.stack_msg = PMAN_STACK_MSG_REBASE(view_common_main_page(model));
                    }
                    break;
                case TIMER_MINUTES_ID:
                    update_page(model, pdata);
                    break;
                default:
                    break;
            }

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
                        msg.stack_msg = PMAN_STACK_MSG_PUSH_PAGE(view_common_washing_page(model));
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
                        case BTN_START_ID: {
                            if (model_lavaggio_pagato(model, pdata->number)) {
                                view_get_protocol(handle)->start_program(handle);
                            }
                            break;
                        }

                        case BTN_STOP_ID: {
                            if (model_lavaggio_programmato_impostato(model, NULL)) {
                                model_cancella_lavaggio_programmato(model);
                            }
                            msg.stack_msg = PMAN_STACK_MSG_REBASE(view_common_main_page(model));
                            break;
                        }

                        case BTN_LANGUAGE_ID: {
                            model->run.lingua = (model->run.lingua + 1) % 2;
                            update_page(model, pdata);
                            break;
                        }

                        case BTN_DETERGENT_ID: {
                            view_get_protocol(handle)->toggle_exclude_detergent(handle);
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
    LV_IMG_DECLARE(img_nosoap);
    LV_IMG_DECLARE(img_soap);
    view_common_set_hidden(pdata->button_detergent, !model->prog.parmac.visualizzazione_esclusione_sapone);
    lv_img_set_src(lv_obj_get_child(pdata->button_detergent, 0),
                   model->run.detergent_exclusion ? &img_nosoap : &img_soap);

    const char *status_string =
        view_intl_get_string_in_language(model_get_temporary_language(model), STRINGS_SCELTA_PROGRAMMA);

    if (model_lavaggio_programmato_impostato(model, NULL)) {
        char programmed_wash_string[128] = {0};
        int  rimanenti                   = model_lavaggio_programmato_minuti_rimanenti(model);

        snprintf(programmed_wash_string, sizeof(programmed_wash_string), "%s: %02i:%02i",
                 view_intl_get_string_in_language(model_get_temporary_language(model), STRINGS_LAVAGGIO_PROGRAMMATO),
                 rimanenti / 60, rimanenti % 60);
        status_string = programmed_wash_string;
    }

    if (model_alarm_code(model) > 0) {
        if (pdata->allarme != model_alarm_code(model)) {
            pdata->allarme = model_alarm_code(model);
        }
        status_string = view_common_alarm_description(model);
        view_common_set_hidden(pdata->label_credit, 0);
        view_common_set_hidden(pdata->label_price, 0);
        view_common_set_hidden(pdata->button_start, 0);
    } else if (model_lavaggio_pagato(model, pdata->number)) {
        pdata->allarme = model_alarm_code(model);
        view_common_set_hidden(pdata->label_credit, 1);
        view_common_set_hidden(pdata->label_price, 1);
        view_common_set_hidden(pdata->button_start, 0);
    } else {
        pdata->allarme = model_alarm_code(model);
        status_string  = view_require_payment_string(model, model_get_temporary_language(model));
        view_common_set_hidden(pdata->label_credit, 0);
        view_common_set_hidden(pdata->label_price, 0);
        view_common_set_hidden(pdata->button_start, 1);
    }

    if (model->prog.parmac.tipo_gettoniera == PAGAMENTO_NESSUNO) {
        view_common_set_hidden(pdata->label_price, 1);
        view_common_set_hidden(pdata->label_credit, 1);
    }

    if (strcmp(lv_label_get_text(pdata->label_status), status_string) != 0) {
        lv_label_set_text(pdata->label_status, status_string);
    }

    char string[32] = {0};
    model_formatta_prezzo(string, model, model_get_credito(model));
    lv_label_set_text(pdata->label_credit, string);

    const programma_preview_t *preview = model_get_preview(model, pdata->number);
    if (model->prog.parmac.prezzo_unico) {
        model_formatta_prezzo(string, model, model->prog.parmac.valore_prezzo_unico);
    } else {
        model_formatta_prezzo(string, model, preview->prezzo);
    }
    lv_label_set_text(pdata->label_price, string);

    const lv_img_dsc_t *language_icons[2] = {&img_marble_italiano, &img_marble_english};
    lv_img_set_src(pdata->image_language, language_icons[model_get_temporary_language(model)]);
}


static void destroy_page(void *state, void *extra) {
    struct page_data *pdata = state;
    (void)extra;
    pman_timer_delete(pdata->timer);
    pman_timer_delete(pdata->timer_minutes);
    lv_free(pdata);
}


static void close_page(void *state) {
    struct page_data *pdata = state;
    pman_timer_pause(pdata->timer);
    pman_timer_pause(pdata->timer_minutes);
    lv_obj_clean(lv_scr_act());
}


const pman_page_t page_choice_self = {
    .create        = create_page,
    .destroy       = destroy_page,
    .open          = open_page,
    .close         = close_page,
    .process_event = page_event,
};
