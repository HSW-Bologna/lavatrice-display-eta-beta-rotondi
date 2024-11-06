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


LV_IMAGE_DECLARE(img_start);
LV_IMAGE_DECLARE(img_stop);
LV_IMAGE_DECLARE(img_pause);
LV_IMAGE_DECLARE(img_italiano);
LV_IMAGE_DECLARE(img_english);


#define ALLARME_CHIAVISTELLO 15
#define ALLARME_SCARICO      32


enum {
    BTN_STOP_ID,
    BTN_START_ID,
    BTN_LEFT_ID,
    BTN_RIGHT_ID,
};


static const char *TAG = "PageMain";


struct page_data {
    lv_obj_t *label_program;
    lv_obj_t *label_status;
    lv_obj_t *label_step;
    lv_obj_t *label_remaining;
    lv_obj_t *lbl_alarm;
    lv_obj_t *lbl_alarm_code;

    lv_obj_t *button_start;
    lv_obj_t *button_stop;
    lv_obj_t *button_left;
    lv_obj_t *button_right;

    uint8_t  scarico_fallito;
    uint16_t allarme;
};


static void update_page(model_t *model, struct page_data *pdata);


static void *create_page(pman_handle_t handle, void *extra) {
    (void)handle;
    (void)extra;

    struct page_data *pdata = lv_malloc(sizeof(struct page_data));
    assert(pdata != NULL);

    pdata->scarico_fallito = 0;
    pdata->allarme         = -1;

    return pdata;
}


static void open_page(pman_handle_t handle, void *state) {
    model_t          *model = view_get_model(handle);
    struct page_data *pdata = state;

    lv_obj_t *cont = lv_obj_create(lv_scr_act());
    lv_obj_set_style_pad_all(cont, 8, LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(cont, VIEW_STYLE_COLOR_BACKGROUND, LV_STATE_DEFAULT);
    lv_obj_set_size(cont, LV_HOR_RES, LV_VER_RES - 56);
    lv_obj_align(cont, LV_ALIGN_TOP_MID, 0, 0);

    {
        lv_obj_t *btn = lv_button_create(cont);
        lv_obj_set_size(btn, 64, 64);
        lv_obj_set_style_radius(btn, 16, LV_STATE_DEFAULT);
        lv_obj_set_style_bg_color(btn, VIEW_STYLE_COLOR_START, LV_STATE_DEFAULT);
        lv_obj_t *img = lv_img_create(btn);
        lv_image_set_scale(img, 170);
        lv_image_set_src(img, &img_start);
        lv_obj_add_style(img, &style_white_icon, LV_STATE_DEFAULT);
        lv_obj_center(img);
        lv_obj_align(btn, LV_ALIGN_TOP_RIGHT, 0, 0);
        view_register_object_default_callback(btn, BTN_START_ID);
        pdata->button_start = btn;
    }

    {
        lv_obj_t *btn = lv_button_create(cont);
        lv_obj_set_size(btn, 64, 64);
        lv_obj_set_style_radius(btn, 16, LV_STATE_DEFAULT);
        lv_obj_set_style_bg_color(btn, VIEW_STYLE_COLOR_PAUSE, LV_STATE_DEFAULT);
        lv_obj_t *img = lv_img_create(btn);
        lv_image_set_scale(img, 170);
        lv_image_set_src(img, &img_pause);
        lv_obj_add_style(img, &style_white_icon, LV_STATE_DEFAULT);
        lv_obj_center(img);
        lv_obj_align(btn, LV_ALIGN_TOP_LEFT, 0, 0);
        view_register_object_default_callback(btn, BTN_STOP_ID);
        pdata->button_stop = btn;
    }

    {
        lv_obj_t *btn = lv_button_create(cont);
        lv_obj_set_size(btn, 64, 64);
        lv_obj_set_style_radius(btn, 16, LV_STATE_DEFAULT);
        lv_obj_set_style_bg_color(btn, VIEW_STYLE_PRIMARY, LV_STATE_DEFAULT);
        lv_obj_t *lbl = lv_label_create(btn);
        lv_obj_set_style_text_font(lbl, STYLE_FONT_HUGE, LV_STATE_DEFAULT);
        lv_label_set_text(lbl, LV_SYMBOL_LEFT);
        lv_obj_align(lbl, LV_ALIGN_CENTER, -2, 0);
        lv_obj_align(btn, LV_ALIGN_BOTTOM_LEFT, 0, 0);
        view_register_object_default_callback(btn, BTN_LEFT_ID);
        pdata->button_left = btn;
    }

    {
        lv_obj_t *btn = lv_button_create(cont);
        lv_obj_set_size(btn, 64, 64);
        lv_obj_set_style_radius(btn, 16, LV_STATE_DEFAULT);
        lv_obj_set_style_bg_color(btn, VIEW_STYLE_PRIMARY, LV_STATE_DEFAULT);
        lv_obj_t *lbl = lv_label_create(btn);
        lv_obj_set_style_text_font(lbl, STYLE_FONT_HUGE, LV_STATE_DEFAULT);
        lv_label_set_text(lbl, LV_SYMBOL_RIGHT);
        lv_obj_align(lbl, LV_ALIGN_CENTER, 2, 0);
        lv_obj_align(btn, LV_ALIGN_BOTTOM_RIGHT, 0, 0);
        view_register_object_default_callback(btn, BTN_RIGHT_ID);
        pdata->button_right = btn;
    }

    {
        lv_obj_t *lbl = lv_label_create(cont);
        lv_obj_set_style_text_font(lbl, STYLE_FONT_SMALL, LV_STATE_DEFAULT);
        lv_obj_set_style_text_align(lbl, LV_TEXT_ALIGN_CENTER, LV_STATE_DEFAULT);
        lv_label_set_long_mode(lbl, LV_LABEL_LONG_SCROLL_CIRCULAR);
        lv_obj_set_width(lbl, LV_PCT(95));
        lv_obj_align(lbl, LV_ALIGN_TOP_MID, 0, 96);
        pdata->label_program = lbl;
    }

    {
        lv_obj_t *lbl = lv_label_create(cont);
        lv_obj_set_style_text_font(lbl, STYLE_FONT_MEDIUM, LV_STATE_DEFAULT);
        lv_obj_set_style_text_align(lbl, LV_TEXT_ALIGN_CENTER, LV_STATE_DEFAULT);
        lv_label_set_long_mode(lbl, LV_LABEL_LONG_SCROLL_CIRCULAR);
        lv_obj_set_width(lbl, LV_HOR_RES - 64 * 2 - 32);
        lv_obj_align(lbl, LV_ALIGN_BOTTOM_MID, 0, -2);
        pdata->label_step = lbl;
    }

    {
        lv_obj_t *lbl = lv_label_create(cont);
        lv_obj_set_style_text_font(lbl, STYLE_FONT_SMALL, LV_STATE_DEFAULT);
        lv_obj_set_style_text_align(lbl, LV_TEXT_ALIGN_CENTER, LV_STATE_DEFAULT);
        lv_label_set_long_mode(lbl, LV_LABEL_LONG_SCROLL_CIRCULAR);
        lv_obj_set_width(lbl, LV_PCT(95));
        lv_obj_align(lbl, LV_ALIGN_TOP_MID, 0, 96 + 24);
        pdata->label_remaining = lbl;
    }

    {
        lv_obj_t *status_cont = lv_obj_create(lv_scr_act());
        lv_obj_add_style(status_cont, &style_padless_cont, LV_STATE_DEFAULT);
        lv_obj_set_size(status_cont, LV_HOR_RES, 56);
        lv_obj_set_style_bg_color(status_cont, VIEW_STYLE_COLOR_BACKGROUND, LV_STATE_DEFAULT);
        lv_obj_align(status_cont, LV_ALIGN_BOTTOM_MID, 0, 0);

        lv_obj_t *lbl = lv_label_create(status_cont);
        lv_label_set_long_mode(lbl, LV_LABEL_LONG_SCROLL_CIRCULAR);
        lv_obj_set_width(lbl, LV_PCT(95));
        lv_obj_set_style_text_align(lbl, LV_TEXT_ALIGN_CENTER, LV_STATE_DEFAULT);
        lv_obj_align(lbl, LV_ALIGN_CENTER, 0, 0);
        pdata->label_status = lbl;
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
            break;
        }

        case PMAN_EVENT_TAG_USER: {
            view_event_t *view_event = event.as.user;
            switch (view_event->tag) {
                case VIEW_EVENT_TAG_PAGE_WATCHER: {
                    if (model_macchina_in_stop(model) || !model_can_work(model)) {
                        model->run.done = 1;
                        msg.stack_msg   = PMAN_STACK_MSG_REBASE(view_common_main_page(model));
                    }

                    if (model_alarm_code(model) == ALLARME_SCARICO || model_alarm_code(model) == ALLARME_CHIAVISTELLO) {
                        if (pdata->scarico_fallito == 0) {
                            ESP_LOGI(TAG, "Scarico fallito");
                            pdata->scarico_fallito = 1;
                        }
                    }

                    if (model_alarm_code(model) > 0 && pdata->allarme != model_alarm_code(model)) {
                        pdata->allarme = model_alarm_code(model);
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
                            break;
                        }

                        case BTN_LEFT_ID: {
                            if (model_macchina_in_pausa(model)) {
                                model_arretra_step(model);
                                update_page(model, pdata);
                            }
                            break;
                        }

                        case BTN_RIGHT_ID: {
                            if (model_macchina_in_pausa(model)) {
                                model_avanza_step(model);
                                update_page(model, pdata);
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
    lv_obj_t *img = lv_obj_get_child(pdata->button_stop, 0);

    if (model_macchina_in_stop(model)) {
        view_common_set_hidden(pdata->button_stop, 1);
        view_common_set_hidden(pdata->button_start, 0);
        view_common_set_hidden(pdata->button_left, 1);
        view_common_set_hidden(pdata->button_right, 1);
    } else if (model_macchina_in_pausa(model)) {
        view_common_set_hidden(pdata->button_stop, 0);
        view_common_set_hidden(pdata->button_start, 0);
        view_common_set_hidden(pdata->button_left, 0);
        view_common_set_hidden(pdata->button_right, 0);
        lv_obj_set_style_bg_color(pdata->button_stop, VIEW_STYLE_COLOR_STOP, LV_STATE_DEFAULT);
        lv_img_set_src(img, &img_stop);
    } else {
        view_common_set_hidden(pdata->button_stop, 0);
        view_common_set_hidden(pdata->button_start, 1);
        view_common_set_hidden(pdata->button_left, 1);
        view_common_set_hidden(pdata->button_right, 1);
        lv_obj_set_style_bg_color(pdata->button_stop, VIEW_STYLE_COLOR_PAUSE, LV_STATE_DEFAULT);
        lv_img_set_src(img, &img_pause);
    }

    const programma_lavatrice_t *program = model_get_program(model);
    const char                  *nome    = program->nomi[model_get_language(model)];
    if (strcmp(nome, lv_label_get_text(pdata->label_program))) {
        lv_label_set_text(pdata->label_program, nome);
    }

    parametri_step_t *step = model_get_current_step(model);
    lv_label_set_text_fmt(pdata->label_step, "%s\n%02i/%02i", view_common_step2str(model, step->tipo),
                          model_get_current_step_number(model) + 1, (int)program->num_steps);

    uint16_t step_rimanente = model->run.macchina.rimanente;
    uint16_t rimanente      = model_program_remaining(model);
    lv_label_set_text_fmt(pdata->label_remaining, "%02im%02is - %02im%02is", step_rimanente / 60, step_rimanente % 60,
                          rimanente / 60, rimanente % 60);

    const char *string = view_intl_get_string_in_language(model_get_temporary_language(model), STRINGS_IN_MARCIA);
    if (model_alarm_code(model) > 0) {
        string = view_common_alarm_description(model);
        view_common_set_hidden(pdata->button_start, 0);
    } else {
        lv_label_set_text(pdata->label_status, "");

        if (model->run.f_richiedi_scarico) {
            string = view_intl_get_string_in_language(model_get_temporary_language(model), STRINGS_SCARICO_NECESSARIO);
        } else if (model_macchina_in_scarico_forzato(model)) {
            string = view_intl_get_string_in_language(model_get_temporary_language(model), STRINGS_SCARICO_FORZATO);
        } else if (model_macchina_in_pausa(model)) {
            string = view_intl_get_string_in_language(model_get_temporary_language(model), STRINGS_PAUSA_LAVAGGIO);
        } else if (model->run.macchina.descrizione_pedante == 0) {
            string = view_common_step2str(model, step->tipo);
        } else {
            string = view_common_pedantic_string(model);
        }
    }
    if (strcmp(lv_label_get_text(pdata->label_status), string) != 0) {
        lv_label_set_text(pdata->label_status, string);
    }
}


const pman_page_t page_washing = {
    .create        = create_page,
    .destroy       = pman_destroy_all,
    .open          = open_page,
    .close         = pman_close_all,
    .process_event = page_event,
};
