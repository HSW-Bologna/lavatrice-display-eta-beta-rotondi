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


#define MAX_IMAGES    6
#define BUTTON_HEIGHT 56

#define SETTINGS_DRAG_WIDTH    96
#define SETTINGS_DRAG_HEIGHT   16
#define SETTINGS_DRAWER_WIDTH  96
#define SETTINGS_DRAWER_HEIGHT 96
#define SETTINGS_BTN_WIDTH     64

#define ALARM_NONE 0


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
LV_IMG_DECLARE(img_marble_italiano);
LV_IMG_DECLARE(img_marble_english);


enum {
    OBJ_SETTINGS_ID,
    BTN_PROGRAM_ID,
    BTN_ALARM_ID,
    BTN_MESSAGE_ID,
    BTN_LEFT_ID,
    BTN_RIGHT_ID,
    BTN_PADLOCK_ID,
    BTN_LANGUAGE_ID,
    WATCH_STATE_ID,
    WATCH_INFO_ID,
    TIMER_CHANGE_PAGE_ID,
    TIMER_RESTORE_LANGUAGE_ID,
};


struct page_data {
    lv_obj_t *button_left;
    lv_obj_t *button_right;
    lv_obj_t *button_padlock;
    lv_obj_t *buttons[MAX_IMAGES];
    lv_obj_t *images[MAX_IMAGES];
    lv_obj_t *labels[MAX_IMAGES];

    lv_obj_t *obj_handle;
    lv_obj_t *obj_drawer;

    lv_obj_t *image_language;

    pman_timer_t *timer_change_page;
    pman_timer_t *timer_restore_language;

    popup_t alarm_popup;
    popup_t message_popup;

    uint16_t program_window_index;
    uint8_t  alarm_pacified;
    int16_t  last_alarm;
};


static void    update_page(model_t *model, struct page_data *pdata);
static uint8_t should_display_alarm(model_t *model, struct page_data *pdata);
static void    handle_alarm(model_t *model, struct page_data *pdata);


static const char *TAG = "PageMainSelf";

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

    static lv_style_prop_t tr_prop[] = {LV_STYLE_TRANSLATE_X, LV_STYLE_TRANSLATE_Y, LV_STYLE_TRANSFORM_SCALE_X,
                                        LV_STYLE_TRANSFORM_SCALE_Y, 0};

    static lv_style_transition_dsc_t tr;
    lv_style_transition_dsc_init(&tr, tr_prop, lv_anim_path_linear, 100, 0, NULL);

    pdata->program_window_index = 0;
    pdata->alarm_pacified       = 0;
    pdata->last_alarm           = 0;

    pdata->timer_change_page      = pman_timer_create(handle, 250, (void *)(uintptr_t)TIMER_CHANGE_PAGE_ID);
    pdata->timer_restore_language = pman_timer_create(handle, 20000UL, (void *)(uintptr_t)TIMER_RESTORE_LANGUAGE_ID);

    return pdata;
}


static void open_page(pman_handle_t handle, void *state) {
    struct page_data *pdata = state;
    model_t          *model = view_get_model(handle);
    // TODO: 3 pagine;
    // 1. niente scritta, tasti avanti indietro lock e lingua
    // 2. info, pagamento start
    // 3. lavaggio, stringa di stato

    lv_obj_t *cont = lv_obj_create(lv_screen_active());
    lv_obj_set_size(cont, LV_HOR_RES, LV_VER_RES);
    lv_obj_remove_flag(cont, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_add_style(cont, &style_padless_cont, LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(cont, 0, LV_STATE_DEFAULT);
    lv_obj_set_style_radius(cont, 0, LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(cont, VIEW_STYLE_COLOR_WHITE, LV_STATE_DEFAULT);

    lv_obj_t *top_cont = lv_obj_create(cont);
    lv_obj_remove_flag(top_cont, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_add_style(top_cont, &style_transparent_cont, LV_STATE_DEFAULT);
    lv_obj_set_size(top_cont, LV_HOR_RES, 160);
    lv_obj_align(top_cont, LV_ALIGN_TOP_MID, 0, -32);
    lv_obj_set_style_pad_column(top_cont, 8, LV_STATE_DEFAULT);
    lv_obj_set_style_pad_hor(top_cont, 8, LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(top_cont, 4, LV_STATE_DEFAULT);
    lv_obj_set_layout(top_cont, LV_LAYOUT_FLEX);
    lv_obj_set_flex_flow(top_cont, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(top_cont, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_START);

    for (size_t i = 0; i < MAX_IMAGES / 2; i++) {
        lv_obj_t *button = lv_button_create(top_cont);
        lv_obj_set_height(button, LV_PCT(100));
        lv_obj_set_width(button, 96);
        lv_obj_add_style(button, &style_tall_button, LV_STATE_DEFAULT);
        lv_obj_set_style_pad_top(button, 40, LV_STATE_DEFAULT);
        lv_obj_set_style_pad_bottom(button, 16, LV_STATE_DEFAULT);
        lv_obj_set_style_radius(button, 24, 0);
        view_register_object_default_callback_with_number(button, BTN_PROGRAM_ID, i);

        lv_obj_t *image = lv_image_create(button);
        lv_image_set_src(image, &img_lavaggio);
        lv_image_set_scale(image, 280);
        lv_obj_align(image, LV_ALIGN_BOTTOM_MID, 0, 0);

        lv_obj_t *label = lv_label_create(button);
        lv_obj_set_style_text_font(label, STYLE_FONT_BIG, LV_STATE_DEFAULT);
        lv_obj_align(label, LV_ALIGN_TOP_MID, 0, 0);

        pdata->buttons[i] = button;
        pdata->images[i]  = image;
        pdata->labels[i]  = label;
    }

    lv_obj_t *bottom_cont = lv_obj_create(cont);
    lv_obj_remove_flag(bottom_cont, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_add_style(bottom_cont, &style_transparent_cont, LV_STATE_DEFAULT);
    lv_obj_set_size(bottom_cont, LV_HOR_RES, 160);
    lv_obj_align(bottom_cont, LV_ALIGN_BOTTOM_MID, 0, 32);
    lv_obj_set_style_pad_column(bottom_cont, 8, LV_STATE_DEFAULT);
    lv_obj_set_style_pad_hor(bottom_cont, 8, LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(bottom_cont, 4, LV_STATE_DEFAULT);
    lv_obj_set_layout(bottom_cont, LV_LAYOUT_FLEX);
    lv_obj_set_flex_flow(bottom_cont, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(bottom_cont, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_START);

    for (size_t i = MAX_IMAGES / 2; i < MAX_IMAGES; i++) {
        lv_obj_t *button = lv_button_create(bottom_cont);
        lv_obj_set_width(button, 96);
        lv_obj_set_height(button, LV_PCT(100));
        lv_obj_add_style(button, &style_tall_button, LV_STATE_DEFAULT);
        lv_obj_set_style_pad_bottom(button, 40, LV_STATE_DEFAULT);
        lv_obj_set_style_pad_top(button, 16, LV_STATE_DEFAULT);
        lv_obj_set_style_radius(button, 24, 0);
        view_register_object_default_callback_with_number(button, BTN_PROGRAM_ID, i);

        lv_obj_t *image = lv_image_create(button);
        lv_image_set_src(image, &img_lavaggio);
        lv_image_set_scale(image, 280);
        lv_obj_align(image, LV_ALIGN_TOP_MID, 0, 0);

        lv_obj_t *label = lv_label_create(button);
        lv_obj_set_style_text_font(label, STYLE_FONT_BIG, LV_STATE_DEFAULT);
        lv_obj_align(image, LV_ALIGN_BOTTOM_MID, 0, 0);

        pdata->buttons[i] = button;
        pdata->images[i]  = image;
        pdata->labels[i]  = label;
    }

    {
        lv_obj_t *buttons_cont = lv_obj_create(cont);
        lv_obj_add_style(buttons_cont, &style_transparent_cont, LV_STATE_DEFAULT);
        lv_obj_clear_flag(buttons_cont, LV_OBJ_FLAG_SCROLLABLE);
        lv_obj_set_style_pad_column(buttons_cont, 12, LV_STATE_DEFAULT);
        lv_obj_set_style_pad_row(buttons_cont, 12, LV_STATE_DEFAULT);
        lv_obj_set_style_pad_ver(buttons_cont, 0, LV_STATE_DEFAULT);
        lv_obj_set_style_pad_hor(buttons_cont, 4, LV_STATE_DEFAULT);
        lv_obj_set_size(buttons_cont, LV_HOR_RES, 72);
        lv_obj_set_layout(buttons_cont, LV_LAYOUT_FLEX);
        lv_obj_set_flex_flow(buttons_cont, LV_FLEX_FLOW_ROW);
        lv_obj_set_flex_align(buttons_cont, LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
        lv_obj_set_style_bg_color(buttons_cont, VIEW_STYLE_COLOR_BACKGROUND, LV_STATE_DEFAULT);
        lv_obj_align(buttons_cont, LV_ALIGN_CENTER, 0, 0);

        {
            lv_obj_t *button = lv_button_create(buttons_cont);
            lv_obj_add_style(button, &style_work_button, LV_STATE_DEFAULT);
            lv_obj_set_size(button, BUTTON_HEIGHT, BUTTON_HEIGHT);
            view_register_object_default_callback(button, BTN_LEFT_ID);

            lv_obj_t *lbl = lv_label_create(button);
            lv_label_set_text(lbl, LV_SYMBOL_LEFT);
            lv_obj_set_style_text_color(lbl, VIEW_STYLE_COLOR_BLACK, LV_STATE_DEFAULT);
            lv_obj_center(lbl);

            pdata->button_left = button;
        }

        {
            lv_obj_t *btn = lv_btn_create(buttons_cont);
            lv_obj_add_style(btn, &style_work_button, LV_STATE_DEFAULT);
            lv_obj_set_size(btn, BUTTON_HEIGHT, BUTTON_HEIGHT);
            view_register_object_default_callback(btn, BTN_PADLOCK_ID);
            pdata->button_padlock = btn;

            lv_obj_t *img = lv_image_create(btn);
            lv_obj_add_style(img, &style_black_icon, LV_STATE_DEFAULT);
            lv_image_set_scale(img, 200);
            lv_image_set_src(img, &img_padlock_closed);
            lv_obj_center(img);
        }

        {
            lv_obj_t *img = lv_image_create(buttons_cont);
            lv_obj_add_flag(img, LV_OBJ_FLAG_CLICKABLE);
            lv_image_set_scale(img, 150);
            lv_image_set_src(img, &img_marble_italiano);
            view_register_object_default_callback(img, BTN_LANGUAGE_ID);
            pdata->image_language = img;
        }

        {
            lv_obj_t *button = lv_button_create(buttons_cont);
            lv_obj_add_style(button, &style_work_button, LV_STATE_DEFAULT);
            lv_obj_set_size(button, BUTTON_HEIGHT, BUTTON_HEIGHT);
            view_register_object_default_callback(button, BTN_RIGHT_ID);
            pdata->button_right = button;

            lv_obj_t *lbl = lv_label_create(button);
            lv_obj_set_style_text_color(lbl, VIEW_STYLE_COLOR_BLACK, LV_STATE_DEFAULT);
            lv_label_set_text(lbl, LV_SYMBOL_RIGHT);
            lv_obj_center(lbl);
        }
    }

    {
        lv_obj_t *obj = lv_obj_create(cont);
        lv_obj_set_size(obj, SETTINGS_DRAG_WIDTH, SETTINGS_DRAG_HEIGHT);
        lv_obj_align(obj, LV_ALIGN_TOP_MID, 0, 0);
        lv_obj_add_style(obj, (lv_style_t *)&style_transparent_cont, LV_STATE_DEFAULT);
        lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
        view_register_object_default_callback(obj, OBJ_SETTINGS_ID);
        pdata->obj_handle = obj;

        lv_obj_t *drawer = lv_obj_create(cont);
        lv_obj_set_size(drawer, SETTINGS_DRAWER_WIDTH, SETTINGS_DRAWER_HEIGHT);
        lv_obj_align_to(drawer, obj, LV_ALIGN_OUT_TOP_MID, 0, 0);
        pdata->obj_drawer = drawer;

        lv_obj_t *btn = lv_btn_create(drawer);
        lv_obj_set_size(btn, SETTINGS_BTN_WIDTH, SETTINGS_BTN_WIDTH);
        lv_obj_t *lbl = lv_label_create(btn);
        lv_label_set_text(lbl, LV_SYMBOL_SETTINGS);
        lv_obj_center(lbl);
        lv_obj_align(btn, LV_ALIGN_CENTER, 0, 0);
    }

    pdata->message_popup = view_common_popup_create(
        cont, view_intl_get_string(model, STRINGS_LAVAGGIO_CONCLUSO_APRIRE_OBLO), BTN_MESSAGE_ID, -1);
    lv_obj_set_size(pdata->message_popup.blanket, LV_HOR_RES, LV_VER_RES);

    pdata->alarm_popup = view_common_alarm_popup_create(cont, BTN_ALARM_ID);
    lv_obj_set_size(pdata->alarm_popup.blanket, LV_HOR_RES, LV_VER_RES);

    VIEW_ADD_WATCHED_VARIABLE(&model->run.macchina.stato, WATCH_STATE_ID);
    VIEW_ADD_WATCHED_VARIABLE(&model->run.macchina.sottostato, WATCH_STATE_ID);
    VIEW_ADD_WATCHED_VARIABLE(&model->run.macchina.sottostato_step, WATCH_STATE_ID);
    VIEW_ADD_WATCHED_VARIABLE(&model->run.macchina.oblo_aperto_chiuso, WATCH_INFO_ID);
    VIEW_ADD_WATCHED_VARIABLE(&model->run.macchina.chiavistello_aperto, WATCH_INFO_ID);
    VIEW_ADD_WATCHED_VARIABLE(&model->run.macchina.chiavistello_chiuso, WATCH_INFO_ID);
    VIEW_ADD_WATCHED_VARIABLE(&model->run.macchina.codice_allarme, WATCH_INFO_ID);

    update_page(model, pdata);
}


static pman_msg_t page_event(pman_handle_t handle, void *state, pman_event_t event) {
    model_t *model = view_get_model(handle);

    pman_msg_t msg = PMAN_MSG_NULL;

    struct page_data *pdata = state;

    msg.user_msg = NULL;

    switch (event.tag) {
        case PMAN_EVENT_TAG_OPEN: {
            handle_alarm(model, pdata);
            update_page(model, pdata);

            if (!model_macchina_in_stop(model)) {
                msg.stack_msg = PMAN_STACK_MSG_PUSH_PAGE(view_common_washing_page(model));
            }
            break;
        }

        case PMAN_EVENT_TAG_USER: {
            view_event_t *view_event = event.as.user;
            switch (view_event->tag) {
                case VIEW_EVENT_TAG_PAGE_WATCHER: {
                    switch (view_event->as.page_watcher.code) {
                        case WATCH_STATE_ID:
                            if (model_macchina_in_marcia(model)) {
                                msg.stack_msg = PMAN_STACK_MSG_PUSH_PAGE(view_common_washing_page(model));
                            }
                            break;

                        case WATCH_INFO_ID:
                            handle_alarm(model, pdata);
                            update_page(model, pdata);
                            break;

                        default:
                            break;
                    }

                    if (!model_macchina_in_stop(model) && !model->run.f_richiedi_scarico) {
                        msg.stack_msg = PMAN_STACK_MSG_PUSH_PAGE(view_common_washing_page(model));
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
                case TIMER_CHANGE_PAGE_ID: {
                    pman_stack_msg_t         pw_msg = PMAN_STACK_MSG_SWAP(&page_menu);
                    password_page_options_t *opts =
                        view_common_default_password_page_options(pw_msg, (const char *)model->prog.password);
                    msg.stack_msg = PMAN_STACK_MSG_PUSH_PAGE_EXTRA(&page_password, opts);
                    break;
                }

                case TIMER_RESTORE_LANGUAGE_ID: {
                    model_reset_temporary_language(model);
                    pman_timer_pause(pdata->timer_restore_language);
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
                case LV_EVENT_PRESSED: {
                    switch (obj_data->id) {
                        case BTN_PROGRAM_ID: {
                            // lv_obj_add_style(pdata->images[obj_data->number], &pdata->style_pr, LV_STATE_DEFAULT);
                            break;
                        }
                        default:
                            break;
                    }
                    break;
                }

                case LV_EVENT_PRESSING: {
                    switch (obj_data->id) {
                        case OBJ_SETTINGS_ID: {
                            lv_indev_t *indev = lv_indev_get_act();
                            if (indev != NULL) {
                                lv_point_t vect;
                                lv_indev_get_vect(indev, &vect);

                                lv_coord_t y = lv_obj_get_y_aligned(target) + vect.y;

                                if (y > 0 && y < SETTINGS_DRAWER_HEIGHT) {
                                    lv_obj_set_y(target, y);
                                    lv_obj_align_to(pdata->obj_drawer, target, LV_ALIGN_OUT_TOP_MID, 0, 0);
                                }
                            }
                            break;
                        }

                        default:
                            break;
                    }
                    break;
                }

                case LV_EVENT_RELEASED: {
                    switch (obj_data->id) {
                        case BTN_PROGRAM_ID: {
                            // lv_obj_remove_style(pdata->images[obj_data->number], &pdata->style_pr, LV_STATE_DEFAULT);
                            break;
                        }

                        case OBJ_SETTINGS_ID: {
                            lv_coord_t y = lv_obj_get_y_aligned(target);

                            if (y > SETTINGS_DRAWER_HEIGHT / 2) {
                                lv_obj_align(target, LV_ALIGN_TOP_MID, 0, SETTINGS_DRAWER_HEIGHT);
                                pman_timer_reset(pdata->timer_change_page);
                                pman_timer_resume(pdata->timer_change_page);
                            } else {
                                lv_obj_align(target, LV_ALIGN_TOP_MID, 0, 0);
                            }
                            lv_obj_align_to(pdata->obj_drawer, target, LV_ALIGN_OUT_TOP_MID, 0, 0);
                            break;
                        }

                        default:
                            break;
                    }
                    break;
                }

                case LV_EVENT_CLICKED: {
                    pman_timer_reset(pdata->timer_restore_language);

                    switch (obj_data->id) {
                        case BTN_PROGRAM_ID: {
                            uint16_t absolute_index = pdata->program_window_index * MAX_IMAGES + obj_data->number;
                            if (view_get_protocol(handle)->load_program(handle, absolute_index) == ALARM_NONE) {
                                msg.stack_msg = PMAN_STACK_MSG_PUSH_PAGE_EXTRA(
                                    view_common_choice_page(model), (void *)(uintptr_t)model_get_program_num(model));
                            }
                            break;
                        }

                        case BTN_LANGUAGE_ID: {
                            model->run.lingua = (model->run.lingua + 1) % 2;
                            pman_timer_resume(pdata->timer_restore_language);
                            update_page(model, pdata);
                            break;
                        }

                        case BTN_MESSAGE_ID:
                            model->run.done = 0;
                            update_page(model, pdata);
                            break;

                        case BTN_ALARM_ID: {
                            if (model_alarm_code(model) == ALARM_NONE) {
                                pdata->alarm_pacified = 0;
                                pdata->last_alarm     = model_alarm_code(model);
                            } else {
                                pdata->alarm_pacified = 1;
                            }

                            update_page(model, pdata);
                            break;
                        }

                        case BTN_LEFT_ID: {
                            uint16_t max_images = MAX_IMAGES;

                            if (pdata->program_window_index > 0) {
                                pdata->program_window_index--;
                            } else {
                                if (model->prog.num_programmi <= max_images) {
                                    pdata->program_window_index = 0;
                                } else {
                                    int16_t extra_index = (model->prog.num_programmi % max_images) != 0 ? 1 : 0;
                                    pdata->program_window_index =
                                        (model->prog.num_programmi / max_images) + extra_index - 1;
                                }
                            }
                            update_page(model, pdata);
                            break;
                        }

                        case BTN_RIGHT_ID: {
                            pdata->program_window_index++;
                            if (pdata->program_window_index * MAX_IMAGES > model->prog.num_programmi) {
                                pdata->program_window_index = 0;
                            }
                            update_page(model, pdata);
                            break;
                        }

                        case BTN_PADLOCK_ID:
                            view_get_protocol(handle)->toggle_lock(handle);
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
    uint16_t max_images = MAX_IMAGES;

    for (size_t i = 0; i < max_images; i++) {
        size_t program_index = pdata->program_window_index * max_images + i;

        if (program_index < model->prog.num_programmi) {
            const programma_preview_t *preview = model_get_preview(model, program_index);

            if (preview->tipo < sizeof(icons) / sizeof(icons[0])) {
                lv_image_set_src(pdata->images[i], icons[preview->tipo]);
            } else {
                lv_image_set_src(pdata->images[i], &img_lavaggio);
            }

            lv_label_set_text_fmt(pdata->labels[i], "%i°", preview->temperatura);

            view_common_set_hidden(pdata->buttons[i], 0);
        } else {
            view_common_set_hidden(pdata->buttons[i], 1);
        }
    }

    if (should_display_alarm(model, pdata)) {
        view_common_set_hidden(pdata->alarm_popup.blanket, 0);
        view_common_alarm_popup_update(&pdata->alarm_popup, model->run.lingua, pdata->last_alarm);
    } else {
        view_common_set_hidden(pdata->alarm_popup.blanket, 1);
    }

    view_common_set_hidden(pdata->message_popup.blanket, !model->run.done);

    const lv_img_dsc_t *language_icons[2] = {&img_marble_italiano, &img_marble_english};
    lv_img_set_src(pdata->image_language, language_icons[model_get_temporary_language(model)]);
}


static void destroy_page(void *state, void *extra) {
    (void)extra;
    struct page_data *pdata = state;
    pman_timer_delete(pdata->timer_change_page);
    pman_timer_delete(pdata->timer_restore_language);
    lv_free(state);
}


static void close_page(void *state) {
    struct page_data *pdata = state;
    pman_timer_pause(pdata->timer_change_page);
    pman_timer_pause(pdata->timer_restore_language);
    lv_obj_clean(lv_screen_active());
}


static uint8_t should_display_alarm(model_t *model, struct page_data *pdata) {
    (void)model;
    if (pdata->last_alarm != ALARM_NONE) {
        return !pdata->alarm_pacified;
    } else {
        return 0;
    }
}


static void handle_alarm(model_t *model, struct page_data *pdata) {
    ESP_LOGI(TAG, "Alarm %i %i %i", pdata->last_alarm, model_alarm_code(model), pdata->alarm_pacified);

    if (pdata->alarm_pacified && (model_alarm_code(model) == ALARM_NONE)) {
        pdata->last_alarm = model_alarm_code(model);
    } else if (pdata->last_alarm != model_alarm_code(model)) {
        pdata->alarm_pacified = 0;
    }
    if (model_alarm_code(model) != ALARM_NONE) {
        pdata->last_alarm = model_alarm_code(model);
    }
}


const pman_page_t page_main_self = {
    .create        = create_page,
    .destroy       = destroy_page,
    .open          = open_page,
    .close         = close_page,
    .process_event = page_event,
};
