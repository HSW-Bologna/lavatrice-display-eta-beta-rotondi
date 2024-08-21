#include <assert.h>
#include <stdlib.h>
#include "lvgl.h"
#include "model/model.h"
#include "../view.h"
#include "../theme/style.h"
#include "../intl/intl.h"
#include <esp_log.h>
#include "bsp/tft/display.h"


enum {
    BTN_BLACK_ID,
    BTN_WHITE_ID,
    BTN_PLUS_ID,
    BTN_MINUS_ID,
};


static const char *TAG = "PageMain";


struct page_data {
    lv_obj_t *screen;
    lv_obj_t *lbl_message;
    uint8_t   white;
    uint8_t   percentage;

    view_controller_msg_t cmsg;
};


static void update_page(struct page_data *pdata);


static void *create_page(pman_handle_t handle, void *extra) {
    (void)handle;
    (void)extra;

    struct page_data *pdata = lv_malloc(sizeof(struct page_data));
    assert(pdata != NULL);
    pdata->white      = 0;
    pdata->percentage = 100;
    return pdata;
}


static void open_page(pman_handle_t handle, void *state) {
    struct page_data *pdata = state;
    (void)pdata;

    model_t *p_model = pman_get_user_data(handle);

    pdata->screen = lv_screen_active();

    {
        lv_obj_t *btn = lv_btn_create(lv_scr_act());
        lv_obj_t *lbl = lv_label_create(btn);
        lv_label_set_text(lbl, view_intl_get_string(p_model, STRINGS_HELLO_WORLD));
        lv_obj_center(lbl);
        lv_obj_align(btn, LV_ALIGN_CENTER, 0, 0);
        pdata->lbl_message = lbl;
    }

    {
        lv_obj_t *btn = lv_btn_create(lv_scr_act());
        lv_obj_set_size(btn, 64, 64);
        lv_obj_align(btn, LV_ALIGN_BOTTOM_LEFT, 16, -16);
        lv_obj_t *lbl = lv_label_create(btn);
        lv_label_set_text(lbl, "N");
        lv_obj_center(lbl);
        view_register_object_default_callback(btn, BTN_BLACK_ID);
    }

    {
        lv_obj_t *btn = lv_btn_create(lv_scr_act());
        lv_obj_set_size(btn, 64, 64);
        lv_obj_align(btn, LV_ALIGN_BOTTOM_RIGHT, -16, -16);
        lv_obj_t *lbl = lv_label_create(btn);
        lv_label_set_text(lbl, LV_SYMBOL_MINUS);
        lv_obj_center(lbl);
        view_register_object_default_callback(btn, BTN_MINUS_ID);
    }

    {
        lv_obj_t *btn = lv_btn_create(lv_scr_act());
        lv_obj_set_size(btn, 64, 64);
        lv_obj_align(btn, LV_ALIGN_TOP_LEFT, 16, 16);
        lv_obj_t *lbl = lv_label_create(btn);
        lv_label_set_text(lbl, "B");
        lv_obj_center(lbl);
        view_register_object_default_callback(btn, BTN_WHITE_ID);
    }

    {
        lv_obj_t *btn = lv_btn_create(lv_scr_act());
        lv_obj_set_size(btn, 64, 64);
        lv_obj_align(btn, LV_ALIGN_TOP_RIGHT, -16, 16);
        lv_obj_t *lbl = lv_label_create(btn);
        lv_label_set_text(lbl, LV_SYMBOL_PLUS);
        lv_obj_center(lbl);
        view_register_object_default_callback(btn, BTN_PLUS_ID);
    }

    ESP_LOGI(TAG, "Main open");

    update_page(pdata);
}


static pman_msg_t page_event(pman_handle_t handle, void *state, pman_event_t event) {
    (void)handle;

    pman_msg_t msg = PMAN_MSG_NULL;

    struct page_data *pdata = state;

    msg.user_msg    = &pdata->cmsg;
    pdata->cmsg.tag = VIEW_CONTROLLER_MESSAGE_TAG_NOTHING;

    switch (event.tag) {
        case PMAN_EVENT_TAG_LVGL: {
            lv_obj_t           *target   = lv_event_get_target_obj(event.as.lvgl);
            view_object_data_t *obj_data = lv_obj_get_user_data(target);

            switch (lv_event_get_code(event.as.lvgl)) {
                case LV_EVENT_CLICKED: {
                    switch (obj_data->id) {
                        case BTN_BLACK_ID:
                            pdata->white = 0;
                            update_page(pdata);
                            break;
                        case BTN_WHITE_ID:
                            pdata->white = 1;
                            update_page(pdata);
                            break;
                        case BTN_MINUS_ID:
                            if (pdata->percentage >= 10) {
                                pdata->percentage -= 10;
                            }
                            update_page(pdata);
                            break;
                        case BTN_PLUS_ID:
                            if (pdata->percentage < 100) {
                                pdata->percentage += 10;
                            }
                            update_page(pdata);
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


static void update_page(struct page_data *pdata) {
    if (pdata->white) {
        lv_obj_set_style_bg_color(pdata->screen, lv_color_white(), LV_STATE_DEFAULT);
    } else {
        lv_obj_set_style_bg_color(pdata->screen, lv_color_black(), LV_STATE_DEFAULT);
    }

    bsp_tft_display_brightness_set(pdata->percentage);
    lv_label_set_text_fmt(pdata->lbl_message, "Retro %i%%", pdata->percentage);
}


const pman_page_t page_main = {
    .create        = create_page,
    .destroy       = pman_destroy_all,
    .open          = open_page,
    .close         = pman_close_all,
    .process_event = page_event,
};
