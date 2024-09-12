#include "lvgl.h"
#include "model/model.h"
#include "../view.h"
#include "../theme/style.h"
#include "../common.h"
#include "config/app_config.h"
#include "src/widgets/led/lv_led.h"
#include "../intl/intl.h"


struct page_data {
    lv_obj_t *label_porthole;

    lv_obj_t *led_porthole;
    lv_obj_t *led_bolt_open;
    lv_obj_t *led_bolt_closed;
};


enum {
    BTN_BACK_ID,
    BTN_NEXT_ID,
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

    view_common_create_title(lv_scr_act(), view_intl_get_string(model, STRINGS_SERRATURA), BTN_BACK_ID, -1, BTN_NEXT_ID);

    lv_obj_t *cont = lv_obj_create(lv_scr_act());
    lv_obj_set_size(cont, LV_HOR_RES, LV_VER_RES - 56);
    lv_obj_align(cont, LV_ALIGN_BOTTOM_MID, 0, 0);

    {
        lv_obj_t *lbl = lv_label_create(cont);
        lv_obj_align(lbl, LV_ALIGN_TOP_LEFT, 0, 16);
        pdata->label_porthole = lbl;

        lv_obj_t *led = lv_led_create(cont);
        lv_led_set_color(led, VIEW_STYLE_COLOR_RED);
        lv_obj_set_size(led, 32, 32);
        lv_obj_align(led, LV_ALIGN_TOP_RIGHT, 0, 8);
        pdata->led_porthole = led;
    }

    {
        lv_obj_t *lbl = lv_label_create(cont);
        lv_obj_align(lbl, LV_ALIGN_TOP_MID, 0, 96);
        lv_label_set_text(lbl, view_intl_get_string(model, STRINGS_CHIAVISTELLO));

        {
            lv_obj_t *btn = lv_button_create(cont);
            lv_obj_set_size(btn, 64, 48);
            lv_obj_t *lbl = lv_label_create(btn);
            lv_label_set_text(lbl, view_intl_get_string(model, STRINGS_APRI));
            lv_obj_center(lbl);
            lv_obj_align(btn, LV_ALIGN_BOTTOM_LEFT, 32, -64);

            lv_obj_t *led = lv_led_create(cont);
            lv_led_set_color(led, VIEW_STYLE_COLOR_RED);
            lv_obj_set_size(led, 32, 32);
            lv_obj_align_to(led, btn, LV_ALIGN_OUT_BOTTOM_MID, 0, 8);
            pdata->led_bolt_open = led;
        }

        {
            lv_obj_t *btn = lv_button_create(cont);
            lv_obj_set_size(btn, 64, 48);
            lv_obj_t *lbl = lv_label_create(btn);
            lv_label_set_text(lbl, view_intl_get_string(model, STRINGS_CHIUDI));
            lv_obj_center(lbl);
            lv_obj_align(btn, LV_ALIGN_BOTTOM_RIGHT, -32, -64);

            lv_obj_t *led = lv_led_create(cont);
            lv_led_set_color(led, VIEW_STYLE_COLOR_GREEN);
            lv_obj_set_size(led, 32, 32);
            lv_obj_align_to(led, btn, LV_ALIGN_OUT_BOTTOM_MID, 0, 8);
            pdata->led_bolt_open = led;
        }
    }

    update_page(model, pdata);
}


static pman_msg_t page_event(pman_handle_t handle, void *state, pman_event_t event) {
    struct page_data *pdata = state;

    model_t *model = view_get_model(handle);

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
                            msg.stack_msg = PMAN_STACK_MSG_BACK();
                            break;

                        case BTN_NEXT_ID:
                            msg.stack_msg = PMAN_STACK_MSG_SWAP(&page_test_coins_digital);
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

    lv_label_set_text_fmt(pdata->label_porthole, "%s %s", view_intl_get_string(model, STRINGS_OBLO),
                          view_intl_get_string(model, STRINGS_APERTO));
}


const pman_page_t page_test_led = {
    .create        = create_page,
    .destroy       = pman_destroy_all,
    .open          = open_page,
    .close         = pman_close_all,
    .process_event = page_event,
};
