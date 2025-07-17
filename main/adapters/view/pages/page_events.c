#include <stdio.h>
#include "lvgl.h"
#include "model/model.h"
#include "../view.h"
#include "../theme/style.h"
#include "../common.h"
#include "config/app_config.h"
#include "src/widgets/led/lv_led.h"
#include "../intl/intl.h"


struct page_data {
    lv_obj_t *label_event_count;

    lv_obj_t *button_prev;
    lv_obj_t *button_next;

    lv_obj_t *table;

    uint16_t page_index;
};


enum {
    BTN_BACK_ID,
    BTN_PREV_ID,
    BTN_NEXT_ID,
};


static void update_page(model_t *model, struct page_data *pdata);


static void *create_page(pman_handle_t handle, void *extra) {
    (void)handle;
    (void)extra;

    struct page_data *pdata = lv_malloc(sizeof(struct page_data));
    assert(pdata != NULL);

    pdata->page_index = 0;

    return pdata;
}


static void open_page(pman_handle_t handle, void *state) {
    struct page_data *pdata = state;

    model_t *model = view_get_model(handle);

    {
        LV_IMG_DECLARE(img_door);

        lv_obj_t *btn, *lbl, *cont;

        cont = lv_obj_create(lv_screen_active());
        lv_obj_add_style(cont, (lv_style_t *)&style_padless_cont, LV_STATE_DEFAULT);
        lv_obj_set_size(cont, LV_HOR_RES, 56);

        btn = lv_button_create(cont);
        lv_obj_add_style(btn, (lv_style_t *)&style_config_btn, LV_STATE_DEFAULT);
        lv_obj_add_style(btn, (lv_style_t *)&style_icon_button, LV_STATE_DEFAULT);
        lv_obj_set_size(btn, 48, 48);
        lv_obj_t *img = lv_image_create(btn);
        lv_image_set_src(img, &img_door);
        lv_image_set_scale(img, 220);
        lv_obj_add_style(img, &style_white_icon, LV_STATE_DEFAULT);
        lv_obj_center(img);
        lv_obj_align(btn, LV_ALIGN_LEFT_MID, 4, 0);
        view_register_object_default_callback(btn, BTN_BACK_ID);

        btn = lv_button_create(cont);
        lv_obj_add_style(btn, (lv_style_t *)&style_config_btn, LV_STATE_DEFAULT);
        lv_obj_add_style(btn, (lv_style_t *)&style_icon_button, LV_STATE_DEFAULT);
        lv_obj_set_size(btn, 48, 48);
        lbl = lv_label_create(btn);
        lv_label_set_text(lbl, LV_SYMBOL_RIGHT);
        lv_obj_center(lbl);
        lv_obj_align(btn, LV_ALIGN_RIGHT_MID, -4, 0);
        view_register_object_default_callback(btn, BTN_NEXT_ID);
        pdata->button_next = btn;

        btn = lv_button_create(cont);
        lv_obj_add_style(btn, (lv_style_t *)&style_config_btn, LV_STATE_DEFAULT);
        lv_obj_add_style(btn, (lv_style_t *)&style_icon_button, LV_STATE_DEFAULT);
        lv_obj_set_size(btn, 48, 48);
        lbl = lv_label_create(btn);
        lv_label_set_text(lbl, LV_SYMBOL_LEFT);
        lv_obj_center(lbl);
        lv_obj_align(btn, LV_ALIGN_RIGHT_MID, -56, 0);
        view_register_object_default_callback(btn, BTN_PREV_ID);
        pdata->button_prev = btn;

        lbl = lv_label_create(cont);
        lv_label_set_long_mode(lbl, LV_LABEL_LONG_SCROLL_CIRCULAR);
        lv_obj_set_width(lbl, 180);
        lv_obj_set_style_text_align(lbl, LV_TEXT_ALIGN_CENTER, LV_STATE_DEFAULT);
        lv_obj_set_style_text_font(lbl, STYLE_FONT_MEDIUM, LV_STATE_DEFAULT);
        pdata->label_event_count = lbl;
    }

    lv_obj_t *cont = lv_obj_create(lv_scr_act());
    lv_obj_set_style_pad_column(cont, 3, LV_STATE_DEFAULT);
    lv_obj_set_style_pad_row(cont, 4, LV_STATE_DEFAULT);
    lv_obj_set_style_pad_hor(cont, 0, LV_STATE_DEFAULT);
    lv_obj_set_size(cont, LV_HOR_RES, LV_VER_RES - 56);
    lv_obj_set_layout(cont, LV_LAYOUT_FLEX);
    lv_obj_set_flex_flow(cont, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(cont, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_align(cont, LV_ALIGN_BOTTOM_MID, 0, 0);

    lv_obj_t *table = lv_table_create(cont);
    lv_table_set_column_count(table, 1);
    lv_table_set_column_width(table, 0, 310);
    lv_obj_set_style_text_font(table, STYLE_FONT_SMALL, LV_PART_ITEMS);
    pdata->table = table;

    VIEW_ADD_WATCHED_ARRAY(&model->run.event_log_chunk, EVENT_LOG_CHUNK, 0);

    update_page(model, pdata);
}


static pman_msg_t page_event(pman_handle_t handle, void *state, pman_event_t event) {
    struct page_data *pdata = state;

    mut_model_t *model = view_get_model(handle);

    pman_msg_t msg = PMAN_MSG_NULL;

    switch (event.tag) {
        case PMAN_EVENT_TAG_OPEN:
            view_get_protocol(handle)->read_events(handle, EVENT_LOG_CHUNK * pdata->page_index);
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
                            pdata->page_index = (pdata->page_index + 1) % model_get_event_chunks(model);
                            view_get_protocol(handle)->read_events(handle, EVENT_LOG_CHUNK * pdata->page_index);
                            update_page(model, pdata);
                            break;

                        case BTN_PREV_ID:
                            if (pdata->page_index > 0) {
                                pdata->page_index--;
                            } else {
                                pdata->page_index = model_get_event_chunks(model) - 1;
                            }
                            view_get_protocol(handle)->read_events(handle, EVENT_LOG_CHUNK * pdata->page_index);
                            update_page(model, pdata);
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
    lv_label_set_text_fmt(pdata->label_event_count, "%i/%i (%i)", pdata->page_index + 1,
                          (int)model_get_event_chunks(model), (int)model->run.event_log_number);

    lv_table_set_row_cnt(pdata->table, model->run.event_log_number + 1);

    for (size_t i = 0; i < model->run.event_log_number; i++) {
        event_t *event       = &model->run.event_log_chunk[i];
        char     string[128] = {0};

        char       datetime[64] = {0};
        time_t     t            = event->istante;
        struct tm *lt           = localtime(&t);
        strftime(datetime, sizeof(datetime), "%H:%M:%S %d/%m/%Y", lt);

        char  alarm_string[16] = {0};
        char *alarm_ptr        = (char *)get_alarm_description(event->codice_evento, model->prog.parmac.lingua);
        if (alarm_ptr == NULL) {
            snprintf(alarm_string, sizeof(alarm_string), "%i", event->codice_evento);
            alarm_ptr = alarm_string;
        }

        snprintf(string, sizeof(string), "[%s] %02i-%02i-%02i (%s): %s", datetime, event->lavaggio + 1,
                 event->numero_step + 1, event->codice_evento, view_common_step2str(model, event->codice_step),
                 alarm_ptr);

        lv_table_set_cell_value(pdata->table, i, 0, string);
    }
}


const pman_page_t page_events = {
    .create        = create_page,
    .destroy       = pman_destroy_all,
    .open          = open_page,
    .close         = pman_close_all,
    .process_event = page_event,
};
