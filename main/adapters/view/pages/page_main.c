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


#define MAX_IMAGES    9
#define BUTTON_WIDTH  80
#define BUTTON_HEIGHT 64


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


enum {
    BTN_SETTINGS_ID,
    BTN_PROGRAM_ID,
    BTN_LEFT_ID,
    BTN_RIGHT_ID,
};


static const char *TAG = "PageMain";


struct page_data {
    lv_obj_t *button_left;
    lv_obj_t *button_right;

    lv_obj_t *images[MAX_IMAGES];
    lv_obj_t *buttons[MAX_IMAGES];

    uint16_t program_window_index;

    lv_style_t style_pr;
};


static uint16_t get_max_images(model_t *model);


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


    static lv_style_prop_t tr_prop[] = {LV_STYLE_TRANSLATE_X, LV_STYLE_TRANSLATE_Y, LV_STYLE_TRANSFORM_SCALE_X,
                                        LV_STYLE_TRANSFORM_SCALE_Y, 0};

    static lv_style_transition_dsc_t tr;
    lv_style_transition_dsc_init(&tr, tr_prop, lv_anim_path_linear, 100, 0, NULL);

    /*Darken the button when pressed and make it wider*/
    lv_style_init(&pdata->style_pr);
    lv_style_set_image_recolor_opa(&pdata->style_pr, 10);
    lv_style_set_image_recolor(&pdata->style_pr, lv_color_black());
    lv_style_set_transform_scale(&pdata->style_pr, 280);
    lv_style_set_transform_pivot_x(&pdata->style_pr, LV_PCT(50));
    lv_style_set_transform_pivot_y(&pdata->style_pr, LV_PCT(50));
    lv_style_set_transition(&pdata->style_pr, &tr);

    pdata->program_window_index = 0;

    return pdata;
}


static void open_page(pman_handle_t handle, void *state) {
    struct page_data *pdata = state;
    model_t          *model = view_get_model(handle);

    {
        lv_obj_t *cont = lv_obj_create(lv_scr_act());
        lv_obj_add_style(cont, &style_padless_cont, LV_STATE_DEFAULT);
        lv_obj_set_style_pad_column(cont, 20, LV_STATE_DEFAULT);
        lv_obj_set_style_pad_row(cont, 12, LV_STATE_DEFAULT);
        lv_obj_set_style_pad_top(cont, 10, LV_STATE_DEFAULT);
        lv_obj_set_size(cont, LV_HOR_RES, LV_VER_RES * 3 / 4);
        lv_obj_set_layout(cont, LV_LAYOUT_FLEX);
        lv_obj_set_flex_flow(cont, LV_FLEX_FLOW_ROW_WRAP);
        lv_obj_set_flex_align(cont, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_START);
        lv_obj_set_style_bg_color(cont, VIEW_STYLE_COLOR_BACKGROUND, LV_STATE_DEFAULT);
        lv_obj_align(cont, LV_ALIGN_TOP_MID, 0, 0);

        for (size_t i = 0; i < get_max_images(model); i++) {
            lv_obj_t *button = lv_button_create(cont);



            lv_obj_t *img = lv_image_create(button);

            lv_img_set_src(img, &img_lavaggio);

            lv_obj_set_size(button, BUTTON_WIDTH, BUTTON_HEIGHT);
            lv_obj_align(img, LV_ALIGN_CENTER, 0, 2);
            lv_obj_set_style_radius(button, LV_RADIUS_CIRCLE, LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(button, lv_color_white(), LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(button, 0, LV_STATE_DEFAULT);

            pdata->buttons[i] = button;
            pdata->images[i]  = img;

            view_register_object_default_callback_with_number(button, BTN_PROGRAM_ID, i);
        }

        {
            lv_obj_t *button = lv_button_create(cont);
            lv_obj_t *lbl    = lv_label_create(button);
            lv_label_set_text(lbl, LV_SYMBOL_LEFT);
            lv_obj_center(lbl);

            lv_obj_set_size(button, BUTTON_HEIGHT, BUTTON_HEIGHT);
            lv_obj_set_style_radius(button, LV_RADIUS_CIRCLE, LV_STATE_DEFAULT);
            lv_obj_add_flag(button, LV_OBJ_FLAG_IGNORE_LAYOUT);
            lv_obj_align(button, LV_ALIGN_BOTTOM_LEFT, 26, -10);

            view_register_object_default_callback(button, BTN_LEFT_ID);

            pdata->button_left = button;
        }

        {
            lv_obj_t *button = lv_button_create(cont);
            lv_obj_t *lbl    = lv_label_create(button);
            lv_label_set_text(lbl, LV_SYMBOL_RIGHT);
            lv_obj_center(lbl);

            lv_obj_set_size(button, BUTTON_HEIGHT, BUTTON_HEIGHT);
            lv_obj_set_style_radius(button, LV_RADIUS_CIRCLE, LV_STATE_DEFAULT);
            lv_obj_add_flag(button, LV_OBJ_FLAG_IGNORE_LAYOUT);
            lv_obj_align(button, LV_ALIGN_BOTTOM_RIGHT, -26, -10);

            view_register_object_default_callback(button, BTN_RIGHT_ID);

            pdata->button_right = button;
        }
    }

    {
        lv_obj_t *cont = lv_obj_create(lv_scr_act());
        lv_obj_add_style(cont, &style_padless_cont, LV_STATE_DEFAULT);
        lv_obj_set_style_pad_column(cont, 12, LV_STATE_DEFAULT);
        lv_obj_set_style_pad_row(cont, 12, LV_STATE_DEFAULT);
        lv_obj_set_size(cont, LV_HOR_RES, LV_VER_RES * 1 / 4);
        lv_obj_set_layout(cont, LV_LAYOUT_FLEX);
        lv_obj_set_flex_flow(cont, LV_FLEX_FLOW_ROW);
        lv_obj_set_flex_align(cont, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
        lv_obj_set_style_bg_color(cont, VIEW_STYLE_COLOR_BACKGROUND, LV_STATE_DEFAULT);
        lv_obj_align(cont, LV_ALIGN_BOTTOM_MID, 0, 0);

        {
            lv_obj_t *btn = lv_btn_create(cont);
            lv_obj_set_size(btn, 64, 64);
            lv_obj_t *lbl = lv_label_create(btn);
            lv_label_set_text(lbl, LV_SYMBOL_SETTINGS);
            view_register_object_default_callback(btn, BTN_SETTINGS_ID);
            lv_obj_center(lbl);
        }
    }

    ESP_LOGI(TAG, "Open");

    update_page(model, pdata);
}


static pman_msg_t page_event(pman_handle_t handle, void *state, pman_event_t event) {
    model_t *model = view_get_model(handle);

    pman_msg_t msg = PMAN_MSG_NULL;

    struct page_data *pdata = state;

    msg.user_msg = NULL;

    switch (event.tag) {
        case PMAN_EVENT_TAG_LVGL: {
            lv_obj_t           *target   = lv_event_get_target_obj(event.as.lvgl);
            view_object_data_t *obj_data = lv_obj_get_user_data(target);

            switch (lv_event_get_code(event.as.lvgl)) {
                case LV_EVENT_PRESSED: {
                    switch (obj_data->id) {
                        case BTN_PROGRAM_ID: {
                            lv_obj_add_style(pdata->images[obj_data->number], &pdata->style_pr, LV_STATE_DEFAULT);
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
                            lv_obj_remove_style(pdata->images[obj_data->number], &pdata->style_pr, LV_STATE_DEFAULT);
                            break;
                        }
                        default:
                            break;
                    }
                    break;
                }

                case LV_EVENT_CLICKED: {
                    switch (obj_data->id) {
                        case BTN_SETTINGS_ID: {
                            pman_stack_msg_t         pw_msg = PMAN_STACK_MSG_SWAP(&page_menu);
                            password_page_options_t *opts =
                                view_common_default_password_page_options(pw_msg, (const char *)APP_CONFIG_PASSWORD);
                            msg.stack_msg = PMAN_STACK_MSG_PUSH_PAGE_EXTRA(&page_password, opts);
                            break;
                        }

                        case BTN_PROGRAM_ID: {
                            if (view_get_protocol(handle)->load_program(handle, obj_data->number) == 0) {
                                msg.stack_msg = PMAN_STACK_MSG_PUSH_PAGE_EXTRA(
                                    &page_choice, (void *)(uintptr_t)model_get_program_num(model));
                            }
                            break;
                        }

                        case BTN_LEFT_ID: {
                            uint16_t max_images = get_max_images(model);

                            if (pdata->program_window_index > 0) {
                                pdata->program_window_index--;
                            } else {
                                if (model->prog.num_programmi <= max_images) {
                                    pdata->program_window_index = 0;
                                } else {
                                    int16_t extra_index = (model->prog.num_programmi % max_images) != 0 ? 1 : 0;
                                    pdata->program_window_index =
                                        (model->prog.num_programmi / max_images) + extra_index - 1;
                                    ESP_LOGI(TAG, "%zu %zu %i %i", model->prog.num_programmi,
                                             model->prog.num_programmi / max_images, extra_index,
                                             pdata->program_window_index);
                                }
                            }
                            update_page(model, pdata);
                            break;
                        }

                        case BTN_RIGHT_ID: {
                            pdata->program_window_index++;
                            if (pdata->program_window_index * get_max_images(model) > model->prog.num_programmi) {
                                pdata->program_window_index = 0;
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

        default:
            break;
    }

    return msg;
}


static void update_page(model_t *model, struct page_data *pdata) {
    uint16_t max_images = get_max_images(model);

    for (size_t i = 0; i < max_images; i++) {
        size_t program_index = pdata->program_window_index * max_images + i;

        if (program_index < model->prog.num_programmi) {
            const programma_preview_t *preview = model_get_preview(model, program_index);

            if (preview->tipo < sizeof(icons) / sizeof(icons[0])) {
                lv_img_set_src(pdata->images[i], icons[preview->tipo]);
            } else {
                lv_img_set_src(pdata->images[i], &img_lavaggio);
            }

            view_common_set_hidden(pdata->buttons[i], 0);
        } else {
            view_common_set_hidden(pdata->buttons[i], 1);
        }
    }

    view_common_set_hidden(pdata->button_left, model->prog.num_programmi <= max_images);
    view_common_set_hidden(pdata->button_right, model->prog.num_programmi <= max_images);
}


static uint16_t get_max_images(model_t *model) {
    const uint16_t limited_images  = 7;
    const uint16_t one_page_images = 9;
    return model->prog.num_programmi <= one_page_images ? one_page_images : limited_images;
}


const pman_page_t page_main = {
    .create        = create_page,
    .destroy       = pman_destroy_all,
    .open          = open_page,
    .close         = pman_close_all,
    .process_event = page_event,
};
