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
LV_IMAGE_DECLARE(img_left);
LV_IMG_DECLARE(img_marble_italiano);
LV_IMG_DECLARE(img_marble_english);


enum {
    BTN_PLACEHOLDER_ID,
    BTN_START_ID,
    BTN_STOP_ID,
    BTN_LANGUAGE_ID,
    BTN_LEFT_ID,
    BTN_RIGHT_ID,
    BTN_MENU_ID,
};


static const char *TAG = __FILE_NAME__;


struct page_data {
    lv_obj_t *button_start;
    lv_obj_t *button_stop;
    lv_obj_t *button_left;
    lv_obj_t *button_right;

    lv_obj_t *label_remaining;
    lv_obj_t *label_status;
    lv_obj_t *label_step;
    lv_obj_t *label_level_speed;

    lv_obj_t *image_language;

    lv_obj_t *arc;

    uint8_t  scarico_fallito;
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
        lv_obj_t *arc = lv_arc_create(lv_screen_active());
        lv_arc_set_rotation(arc, 270);
        lv_arc_set_start_angle(arc, 0);
        lv_arc_set_end_angle(arc, 360);
        lv_arc_set_bg_angles(arc, 0, 360);
        lv_obj_remove_style(arc, NULL, LV_PART_KNOB);
        lv_obj_remove_flag(arc, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_set_style_arc_width(arc, 26, LV_PART_MAIN);
        lv_obj_set_style_arc_rounded(arc, 0, LV_PART_MAIN);
        lv_obj_set_style_arc_color(arc, VIEW_STYLE_COLOR_BACKGROUND, LV_PART_MAIN);
        lv_obj_set_style_arc_color(arc, VIEW_STYLE_COLOR_GREEN, LV_PART_INDICATOR);
        lv_obj_set_style_arc_width(arc, 24, LV_PART_INDICATOR);
        lv_obj_set_style_arc_rounded(arc, 0, LV_PART_INDICATOR);
        lv_obj_align(arc, LV_ALIGN_BOTTOM_MID, 0, -48);
        lv_arc_set_range(arc, 0, preview->durata);
        lv_obj_set_size(arc, 180, 180);
        pdata->arc = arc;

        lv_obj_t *lbl = lv_label_create(cont);
        lv_obj_set_style_text_align(lbl, LV_TEXT_ALIGN_CENTER, LV_STATE_DEFAULT);
        lv_label_set_long_mode(lbl, LV_LABEL_LONG_SCROLL_CIRCULAR);
        lv_obj_set_style_text_color(lbl, lv_color_darken(VIEW_STYLE_COLOR_BACKGROUND, LV_OPA_60), LV_STATE_DEFAULT);
        lv_obj_set_style_text_font(lbl, STYLE_FONT_BIG, LV_STATE_DEFAULT);
        lv_obj_set_width(lbl, LV_PCT(95));
        lv_obj_align_to(lbl, arc, LV_ALIGN_CENTER, 0, -8);
        pdata->label_remaining = lbl;

        lv_obj_t *lbl_lavaggio = lv_label_create(cont);
        lv_obj_set_style_text_font(lbl_lavaggio, STYLE_FONT_SMALL, LV_STATE_DEFAULT);
        lv_obj_set_style_text_color(lbl_lavaggio, VIEW_STYLE_COLOR_BLACK, LV_STATE_DEFAULT);
        lv_obj_set_style_text_align(lbl_lavaggio, LV_TEXT_ALIGN_CENTER, LV_STATE_DEFAULT);
        lv_label_set_long_mode(lbl_lavaggio, LV_LABEL_LONG_SCROLL_CIRCULAR);
        lv_obj_set_width(lbl_lavaggio, 128);
        lv_label_set_text(lbl_lavaggio,
                          view_intl_get_string_in_language(model_get_temporary_language(model), STRINGS_LAVAGGIO));
        lv_obj_align_to(lbl_lavaggio, lbl, LV_ALIGN_BOTTOM_MID, 0, 16);
        pdata->label_step = lbl_lavaggio;

        lv_obj_t *label_level_speed = lv_label_create(cont);
        lv_obj_set_style_text_font(label_level_speed, STYLE_FONT_SMALL, LV_STATE_DEFAULT);
        lv_obj_set_style_text_color(label_level_speed, VIEW_STYLE_COLOR_BLACK, LV_STATE_DEFAULT);
        lv_obj_align(label_level_speed, LV_ALIGN_TOP_MID, 0, 48);
        pdata->label_level_speed = label_level_speed;
    }

    {
        lv_obj_t *img = lv_img_create(cont);
        lv_obj_add_flag(img, LV_OBJ_FLAG_CLICKABLE);
        lv_image_set_src(img, &img_play_released);
        lv_obj_align(img, LV_ALIGN_TOP_RIGHT, 0, 64);
        view_register_object_default_callback(img, BTN_START_ID);
        pdata->button_start = img;
    }

    {
        lv_obj_t *img = lv_img_create(cont);
        lv_obj_add_flag(img, LV_OBJ_FLAG_CLICKABLE);
        lv_image_set_src(img, &img_pause_released);
        lv_obj_align(img, LV_ALIGN_TOP_LEFT, 0, 64);
        view_register_object_default_callback(img, BTN_STOP_ID);
        pdata->button_stop = img;
    }

    {
        lv_obj_t *lbl = lv_label_create(lv_screen_active());
        lv_label_set_long_mode(lbl, LV_LABEL_LONG_SCROLL_CIRCULAR);
        lv_obj_set_style_text_color(lbl, VIEW_STYLE_COLOR_BLACK, LV_STATE_DEFAULT);
        lv_obj_set_width(lbl, LV_PCT(100));
        lv_obj_set_style_text_align(lbl, LV_TEXT_ALIGN_CENTER, LV_STATE_DEFAULT);
        lv_obj_align(lbl, LV_ALIGN_BOTTOM_MID, 0, 0);
        pdata->label_status = lbl;
    }

    {
        lv_obj_t *img = lv_image_create(cont);
        lv_obj_add_flag(img, LV_OBJ_FLAG_CLICKABLE);
        lv_image_set_scale(img, 180);
        lv_image_set_src(img, &img_marble_italiano);
        view_register_object_default_callback(img, BTN_LANGUAGE_ID);
        lv_obj_align(img, LV_ALIGN_LEFT_MID, -16, 18);
        pdata->image_language = img;
    }

    if (model->prog.parmac.visualizzazione_menu) {
        LV_IMG_DECLARE(img_user);
        lv_obj_t *btn = lv_btn_create(cont);
        lv_obj_add_style(btn, (lv_style_t *)&style_config_btn, LV_STATE_DEFAULT);
        lv_obj_add_style(btn, (lv_style_t *)&style_icon_button, LV_STATE_DEFAULT);
        lv_obj_set_size(btn, 64, 64);
        lv_obj_t *img = lv_img_create(btn);
        lv_image_set_scale(img, 200);
        lv_img_set_src(img, &img_user);
        lv_obj_center(img);
        lv_obj_align(btn, LV_ALIGN_RIGHT_MID, 4, 18);
        view_register_object_default_callback(btn, BTN_MENU_ID);
    }

    {
        lv_obj_t *img = lv_image_create(cont);
        lv_obj_add_flag(img, LV_OBJ_FLAG_CLICKABLE);
        lv_image_set_src(img, &img_left);
        lv_obj_align(img, LV_ALIGN_BOTTOM_LEFT, 0, -28);
        view_register_object_default_callback(img, BTN_LEFT_ID);
        pdata->button_left = img;
    }

    {
        lv_obj_t *img = lv_image_create(cont);
        lv_obj_add_flag(img, LV_OBJ_FLAG_CLICKABLE);
        lv_image_set_src(img, &img_left);
        lv_image_set_rotation(img, 1800);
        lv_obj_align(img, LV_ALIGN_BOTTOM_RIGHT, 0, -28);
        view_register_object_default_callback(img, BTN_RIGHT_ID);
        pdata->button_right = img;
    }

    VIEW_ADD_WATCHED_VARIABLE(&model->run.macchina, 0);
    VIEW_ADD_WATCHED_VARIABLE(&model->run.done, 0);

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
                        msg.stack_msg = PMAN_STACK_MSG_REBASE(view_common_main_page(model));
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

                        case BTN_LANGUAGE_ID: {
                            model->run.lingua = (model->run.lingua + 1) % 2;
                            update_page(model, pdata);
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

                        case BTN_MENU_ID: {
                            if (!model_macchina_in_stop(model)) {
                                msg.stack_msg = PMAN_STACK_MSG_PUSH_PAGE(&page_work_parameters);
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
    uint16_t rimanente = model_program_remaining(model);

    lv_label_set_text_fmt(pdata->label_level_speed, "liv %3i, vel %3i, t %3i, %i", model->run.macchina.livello,
                          model->run.macchina.velocita_rilevata, model->run.macchina.temperatura,
                          model->run.macchina.codice_allarme);

    uint16_t remaining_seconds = rimanente;
    uint16_t remaining_minutes = remaining_seconds / 60;
    lv_label_set_text_fmt(pdata->label_remaining, "%02i:%02i", remaining_minutes, remaining_seconds % 60);

    lv_arc_set_value(pdata->arc, remaining_seconds);

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
        lv_image_set_src(pdata->button_stop, &img_stop_released);
        lv_obj_set_style_image_recolor_opa(pdata->button_stop, LV_OPA_TRANSP, LV_STATE_DEFAULT);
    } else if (model_macchina_in_frenata(model) || model_macchina_in_scarico_forzato(model)) {
        view_common_set_hidden(pdata->button_stop, 1);
        view_common_set_hidden(pdata->button_start, 1);
        view_common_set_hidden(pdata->button_left, 1);
        view_common_set_hidden(pdata->button_right, 1);
    } else {
        view_common_set_hidden(pdata->button_stop, 0);
        view_common_set_hidden(pdata->button_start, 1);
        view_common_set_hidden(pdata->button_left, 1);
        view_common_set_hidden(pdata->button_right, 1);
        lv_obj_set_style_image_recolor_opa(pdata->button_stop, LV_OPA_TRANSP, LV_STATE_DEFAULT);
        lv_img_set_src(pdata->button_stop, &img_pause_released);
    }

    parametri_step_t *step = model_get_current_step(model);

    const char *string = view_intl_get_string_in_language(model_get_temporary_language(model), STRINGS_IN_MARCIA);
    ESP_LOGI(TAG, "Alarm %i", model_alarm_code(model));

    if (model_alarm_code(model) > 0) {
        string = view_common_alarm_description(model);
        view_common_set_hidden(pdata->button_start, 0);
    } else {
        if (model->run.f_richiedi_scarico) {
            string = view_intl_get_string_in_language(model_get_temporary_language(model), STRINGS_SCARICO_NECESSARIO);
        } else if (model_macchina_in_scarico_forzato(model)) {
            string = view_intl_get_string_in_language(model_get_temporary_language(model), STRINGS_SCARICO_FORZATO);
        } else if (model_macchina_in_frenata(model)) {
            string = view_intl_get_string_in_language(model_get_temporary_language(model), STRINGS_ATTESA_FRENATA);
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

    const programma_lavatrice_t *program = model_get_program(model);
    lv_label_set_text_fmt(pdata->label_step, "%s\n%02i/%02i", view_common_step2str(model, step->tipo),
                          model_get_current_step_number(model) + 1, (int)program->num_steps);

    const lv_img_dsc_t *language_icons[2] = {&img_marble_italiano, &img_marble_english};
    lv_img_set_src(pdata->image_language, language_icons[model_get_temporary_language(model)]);
}


const pman_page_t page_washing_self = {
    .create        = create_page,
    .destroy       = pman_destroy_all,
    .open          = open_page,
    .close         = pman_close_all,
    .process_event = page_event,
};
