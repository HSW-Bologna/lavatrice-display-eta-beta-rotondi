#include "lvgl.h"
#include "model/model.h"
#include "../view.h"
#include "../theme/style.h"
#include "../common.h"
#include "config/app_config.h"
#include "src/widgets/led/lv_led.h"
#include "../intl/intl.h"


#define BUTTON_WIDTH 150


struct page_data {
    lv_obj_t *dropdown;
};


enum {
    BTN_BACK_ID,
    BTN_TEST_ID,
    BTN_STATS_ID,
    BTN_EVENTS_ID,
    BTN_DEBUG_ID,
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

    view_common_create_title(lv_scr_act(), view_intl_get_string(model, STRINGS_DIAGNOSI), BTN_BACK_ID, -1);

    lv_obj_t *cont = lv_obj_create(lv_scr_act());
    lv_obj_set_style_pad_column(cont, 3, LV_STATE_DEFAULT);
    lv_obj_set_style_pad_row(cont, 4, LV_STATE_DEFAULT);
    lv_obj_set_style_pad_hor(cont, 0, LV_STATE_DEFAULT);
    lv_obj_set_size(cont, LV_HOR_RES, LV_VER_RES - 56);
    lv_obj_set_layout(cont, LV_LAYOUT_FLEX);
    lv_obj_set_flex_flow(cont, LV_FLEX_FLOW_ROW_WRAP);
    lv_obj_set_flex_align(cont, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_align(cont, LV_ALIGN_BOTTOM_MID, 0, 0);

    {
        lv_obj_t *btn = lv_button_create(cont);
        lv_obj_set_width(btn, BUTTON_WIDTH);
        lv_obj_t *lbl = lv_label_create(btn);
        lv_label_set_text(lbl, view_intl_get_string(model, STRINGS_MACCHINA));
        lv_obj_set_style_text_font(lbl, STYLE_FONT_SMALL, LV_STATE_DEFAULT);
        lv_obj_center(lbl);
        view_register_object_default_callback(btn, BTN_TEST_ID);
    }

    {
        lv_obj_t *btn = lv_button_create(cont);
        lv_obj_set_width(btn, BUTTON_WIDTH);
        lv_obj_t *lbl = lv_label_create(btn);
        lv_label_set_text(lbl, view_intl_get_string(model, STRINGS_STATISTICHE));
        lv_obj_set_style_text_font(lbl, STYLE_FONT_SMALL, LV_STATE_DEFAULT);
        lv_obj_center(lbl);
        view_register_object_default_callback(btn, BTN_STATS_ID);
    }

    {
        lv_obj_t *btn = lv_button_create(cont);
        lv_obj_set_width(btn, BUTTON_WIDTH);
        lv_obj_t *lbl = lv_label_create(btn);
        lv_label_set_text(lbl, view_intl_get_string(model, STRINGS_EVENTI));
        lv_obj_set_style_text_font(lbl, STYLE_FONT_SMALL, LV_STATE_DEFAULT);
        lv_obj_center(lbl);
        view_register_object_default_callback(btn, BTN_EVENTS_ID);
    }

    {
        lv_obj_t *cont_debug = lv_obj_create(cont);
        lv_obj_add_style(cont_debug, &style_transparent_cont, LV_STATE_DEFAULT);
        lv_obj_set_size(cont_debug, BUTTON_WIDTH * 2, 56);

        lv_obj_t *btn = lv_button_create(cont_debug);
        lv_obj_set_width(btn, BUTTON_WIDTH);
        lv_obj_align(btn, LV_ALIGN_LEFT_MID, 0, 0);
        view_register_object_default_callback(btn, BTN_DEBUG_ID);

        lv_obj_t *dropdown = lv_dropdown_create(cont_debug);
        lv_obj_set_style_text_font(lv_dropdown_get_list(dropdown), STYLE_FONT_SMALL, LV_STATE_DEFAULT);
        lv_obj_set_style_text_font(dropdown, STYLE_FONT_SMALL, LV_PART_MAIN);
        lv_obj_set_width(dropdown, BUTTON_WIDTH * 2 / 3);
        view_common_dropdown_set_number(dropdown, 100);
        lv_obj_align(dropdown, LV_ALIGN_RIGHT_MID, 0, 0);
        pdata->dropdown = dropdown;

        lv_obj_t *lbl = lv_label_create(btn);
        lv_label_set_text(lbl, "Debug");
        lv_obj_set_style_text_font(lbl, STYLE_FONT_SMALL, LV_STATE_DEFAULT);
        lv_obj_center(lbl);
    }

    update_page(model, pdata);
}


static pman_msg_t page_event(pman_handle_t handle, void *state, pman_event_t event) {
    struct page_data *pdata = state;

    mut_model_t *model = view_get_model(handle);

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

                        case BTN_TEST_ID:
                            msg.stack_msg = PMAN_STACK_MSG_PUSH_PAGE(&page_test_inputs);
                            break;

                        case BTN_STATS_ID:
                            msg.stack_msg = PMAN_STACK_MSG_PUSH_PAGE(&page_statistics);
                            break;

                        case BTN_EVENTS_ID:
                            msg.stack_msg = PMAN_STACK_MSG_PUSH_PAGE(&page_events);
                            break;

                        case BTN_DEBUG_ID:
                            view_get_protocol(handle)->send_debug_code(handle,
                                                                       lv_dropdown_get_selected(pdata->dropdown));
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
}


const pman_page_t page_diagnosis = {
    .create        = create_page,
    .destroy       = pman_destroy_all,
    .open          = open_page,
    .close         = pman_close_all,
    .process_event = page_event,
};
