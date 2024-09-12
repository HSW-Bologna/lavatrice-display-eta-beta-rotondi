#include "lvgl.h"
#include "model/model.h"
#include "../view.h"
#include "../theme/style.h"
#include "../common.h"
#include "config/app_config.h"
#include "src/widgets/led/lv_led.h"
#include "../intl/intl.h"


struct page_data {
    lv_obj_t *table_mechanical;
};


enum {
    BTN_BACK_ID,
    BTN_NEXT_ID,
    BTN_CLEAR_ID,
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

    view_common_create_title(lv_scr_act(), view_intl_get_string(model, STRINGS_GETTONI), BTN_BACK_ID, -1, BTN_NEXT_ID);

    lv_obj_t *cont = lv_obj_create(lv_scr_act());
    lv_obj_set_size(cont, LV_HOR_RES, LV_VER_RES - 56);
    lv_obj_align(cont, LV_ALIGN_BOTTOM_MID, 0, 0);

    lv_obj_t *table = lv_table_create(cont);
    lv_obj_set_style_pad_all(table, 2, LV_STATE_DEFAULT | LV_PART_ITEMS);

    lv_table_set_col_width(table, 0, 128);
    lv_table_set_col_width(table, 1, 56);
    lv_table_set_col_width(table, 2, 56);
    lv_table_set_col_width(table, 3, 56);

    lv_table_set_cell_value(table, 0, 0, view_intl_get_string(model, STRINGS_MECCANICA));
    lv_table_add_cell_ctrl(table, 0, 0, LV_TABLE_CELL_CTRL_MERGE_RIGHT);
    lv_table_add_cell_ctrl(table, 1, 0, LV_TABLE_CELL_CTRL_MERGE_RIGHT);
    lv_table_add_cell_ctrl(table, 2, 0, LV_TABLE_CELL_CTRL_MERGE_RIGHT);
    lv_table_set_cell_value_fmt(table, 1, 0, "%s 1", view_intl_get_string(model, STRINGS_LINEA));
    lv_table_set_cell_value_fmt(table, 2, 0, "%s 2", view_intl_get_string(model, STRINGS_LINEA));
    lv_table_set_cell_value_fmt(table, 3, 0, "%s", view_intl_get_string(model, STRINGS_CASSA));
    lv_obj_align(table, LV_ALIGN_TOP_MID, 0, 0);

    pdata->table_mechanical = table;

    lv_obj_t *btn = lv_button_create(cont);
    lv_obj_set_size(btn, 96, 48);
    lv_obj_t *lbl = lv_label_create(btn);
    lv_label_set_text(lbl, view_intl_get_string(model, STRINGS_AZZERA));
    lv_obj_center(lbl);
    view_register_object_default_callback(btn, BTN_CLEAR_ID);
    lv_obj_align(btn, LV_ALIGN_BOTTOM_MID, 0, 0);

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
                            msg.stack_msg = PMAN_STACK_MSG_SWAP(&page_test_led);
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

    lv_table_set_cell_value_fmt(pdata->table_mechanical, 1, 1, "%i", 0);
    lv_table_set_cell_value_fmt(pdata->table_mechanical, 2, 1, "%i", 0);
    lv_table_set_cell_value_fmt(pdata->table_mechanical, 3, 1, "%i", 0);
    lv_table_set_cell_value_fmt(pdata->table_mechanical, 1, 2, "%i", 0);
    lv_table_set_cell_value_fmt(pdata->table_mechanical, 2, 2, "%i", 0);
    lv_table_set_cell_value_fmt(pdata->table_mechanical, 3, 2, "%i", 0);
    lv_table_set_cell_value_fmt(pdata->table_mechanical, 1, 3, "%i", 0);
    lv_table_set_cell_value_fmt(pdata->table_mechanical, 2, 3, "%i", 0);
    lv_table_set_cell_value_fmt(pdata->table_mechanical, 3, 3, "%i", 0);
}


const pman_page_t page_test_coins_mechanical = {
    .create        = create_page,
    .destroy       = pman_destroy_all,
    .open          = open_page,
    .close         = pman_close_all,
    .process_event = page_event,
};
