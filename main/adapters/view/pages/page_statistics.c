#include "lvgl.h"
#include "model/model.h"
#include "../view.h"
#include "../theme/style.h"
#include "../common.h"
#include "config/app_config.h"
#include "src/widgets/led/lv_led.h"
#include "../intl/intl.h"
#include <esp_log.h>


#define HMS(t) "%02ih%02im%02is", (int)(t / 3600), (int)((t / 60) % 60), (int)(t % 60)


struct page_data {
    lv_obj_t *table_cycles;
    lv_obj_t *table_time;
    lv_obj_t *table_detergent;
    lv_obj_t *table_porthole;
};


enum {
    BTN_BACK_ID,
};


static void draw_event_cb(lv_event_t *e);
static void update_page(model_t *model, struct page_data *pdata);


static const char *TAG = __FILE_NAME__;


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

    view_common_create_title(lv_screen_active(), view_intl_get_string(model, STRINGS_STATISTICHE), BTN_BACK_ID, -1);

    lv_obj_t *cont = lv_obj_create(lv_screen_active());
    lv_obj_set_style_pad_column(cont, 8, LV_STATE_DEFAULT);
    lv_obj_set_style_pad_row(cont, 8, LV_STATE_DEFAULT);
    lv_obj_set_style_pad_hor(cont, 0, LV_STATE_DEFAULT);
    lv_obj_set_size(cont, LV_HOR_RES, LV_VER_RES - 56);
    lv_obj_set_layout(cont, LV_LAYOUT_FLEX);
    lv_obj_set_flex_flow(cont, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(cont, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_align(cont, LV_ALIGN_BOTTOM_MID, 0, 0);

    {
        lv_obj_t *tab = lv_table_create(cont);
        lv_obj_set_style_text_font(tab, STYLE_FONT_SMALL, LV_STATE_DEFAULT);
        lv_obj_set_style_bg_color(tab, lv_color_make(0x10, 0x60, 0x10), LV_STATE_DEFAULT | LV_PART_ITEMS);
        lv_table_set_column_count(tab, 2);
        lv_table_set_row_count(tab, 4);
        lv_table_set_column_width(tab, 0, 120);
        lv_table_set_column_width(tab, 1, 160);

        lv_table_add_cell_ctrl(tab, 0, 0, LV_TABLE_CELL_CTRL_MERGE_RIGHT);
        lv_table_set_cell_value(tab, 0, 0, view_intl_get_string(model, STRINGS_CICLI));
        lv_table_set_cell_value(tab, 1, 0, view_intl_get_string(model, STRINGS_TOTALI));
        lv_table_set_cell_value(tab, 2, 0, view_intl_get_string(model, STRINGS_INTERROTTI));
        lv_table_set_cell_value(tab, 3, 0, view_intl_get_string(model, STRINGS_CONTINUI));

        lv_obj_add_event_cb(tab, draw_event_cb, LV_EVENT_DRAW_TASK_ADDED, NULL);
        lv_obj_add_flag(tab, LV_OBJ_FLAG_SEND_DRAW_TASK_EVENTS);

        pdata->table_cycles = tab;
    }

    {
        lv_obj_t *tab = lv_table_create(cont);
        lv_obj_set_style_text_font(tab, STYLE_FONT_SMALL, LV_STATE_DEFAULT);
        lv_obj_set_style_bg_color(tab, lv_color_make(0xFF, 0xA5, 0x00), LV_STATE_DEFAULT | LV_PART_ITEMS);
        lv_table_set_column_count(tab, 2);
        lv_table_set_row_count(tab, 9);
        lv_table_set_column_width(tab, 0, 120);
        lv_table_set_column_width(tab, 1, 160);

        lv_table_add_cell_ctrl(tab, 0, 0, LV_TABLE_CELL_CTRL_MERGE_RIGHT);
        lv_table_set_cell_value(tab, 0, 0, view_intl_get_string(model, STRINGS_TEMPI));
        lv_table_set_cell_value(tab, 1, 0, view_intl_get_string(model, STRINGS_ACCESO));
        lv_table_set_cell_value(tab, 2, 0, view_intl_get_string(model, STRINGS_LAVORO));
        lv_table_set_cell_value(tab, 3, 0, view_intl_get_string(model, STRINGS_MOTO));
        lv_table_set_cell_value(tab, 4, 0, view_intl_get_string(model, STRINGS_RISCALDAMENTO));
        lv_table_set_cell_value(tab, 5, 0, view_intl_get_string(model, STRINGS_ACQUA_FREDDA));
        lv_table_set_cell_value(tab, 6, 0, view_intl_get_string(model, STRINGS_ACQUA_CALDA));
        lv_table_set_cell_value(tab, 7, 0, view_intl_get_string(model, STRINGS_ACQUA_RECUPERO));
        lv_table_set_cell_value(tab, 8, 0, view_intl_get_string(model, STRINGS_ACQUA_FLUSSAGGIO));

        lv_obj_add_event_cb(tab, draw_event_cb, LV_EVENT_DRAW_TASK_ADDED, NULL);
        lv_obj_add_flag(tab, LV_OBJ_FLAG_SEND_DRAW_TASK_EVENTS);

        pdata->table_time = tab;
    }

    {
        lv_obj_t *tab = lv_table_create(cont);
        lv_obj_set_style_text_font(tab, STYLE_FONT_SMALL, LV_STATE_DEFAULT);
        lv_obj_set_style_bg_color(tab, lv_color_make(0x00, 0x80, 0x80), LV_STATE_DEFAULT | LV_PART_ITEMS);
        lv_table_set_column_count(tab, 2);
        lv_table_set_row_count(tab, model->prog.parmac.numero_saponi_utilizzabili + 1);
        lv_table_set_column_width(tab, 0, 120);
        lv_table_set_column_width(tab, 1, 160);

        lv_table_add_cell_ctrl(tab, 0, 0, LV_TABLE_CELL_CTRL_MERGE_RIGHT);

        lv_table_set_cell_value(tab, 0, 0, view_intl_get_string(model, STRINGS_SAPONE));
        for (size_t i = 0; i < model->prog.parmac.numero_saponi_utilizzabili; i++) {
            lv_table_set_cell_value_fmt(tab, i + 1, 0, "%zu", i + 1);
        }

        lv_obj_add_event_cb(tab, draw_event_cb, LV_EVENT_DRAW_TASK_ADDED, NULL);
        lv_obj_add_flag(tab, LV_OBJ_FLAG_SEND_DRAW_TASK_EVENTS);

        pdata->table_detergent = tab;
    }

    {
        lv_obj_t *tab = lv_table_create(cont);
        lv_obj_set_style_text_font(tab, STYLE_FONT_SMALL, LV_STATE_DEFAULT);
        lv_obj_set_style_bg_color(tab, lv_color_make(0x00, 0x00, 0x80), LV_STATE_DEFAULT | LV_PART_ITEMS);
        lv_table_set_column_count(tab, 2);
        lv_table_set_row_count(tab, 3);
        lv_table_set_column_width(tab, 0, 120);
        lv_table_set_column_width(tab, 1, 160);

        lv_table_add_cell_ctrl(tab, 0, 0, LV_TABLE_CELL_CTRL_MERGE_RIGHT);
        lv_table_set_cell_value(tab, 0, 0, view_intl_get_string(model, STRINGS_OBLO));
        lv_table_set_cell_value(tab, 1, 0, view_intl_get_string(model, STRINGS_APERTURE));
        lv_table_set_cell_value(tab, 2, 0, view_intl_get_string(model, STRINGS_CHIUSURE));

        lv_obj_add_event_cb(tab, draw_event_cb, LV_EVENT_DRAW_TASK_ADDED, NULL);
        lv_obj_add_flag(tab, LV_OBJ_FLAG_SEND_DRAW_TASK_EVENTS);

        pdata->table_porthole = tab;
    }

    VIEW_ADD_WATCHED_VARIABLE(&model->stats, 0);

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
    lv_table_set_cell_value_fmt(pdata->table_cycles, 1, 1, "%" PRIu32, model->stats.cicli_eseguiti);
    lv_table_set_cell_value_fmt(pdata->table_cycles, 2, 1, "%" PRIu32, model->stats.cicli_interrotti);
    lv_table_set_cell_value_fmt(pdata->table_cycles, 3, 1, "%" PRIu32, model->stats.cicli_loop);

    lv_table_set_cell_value_fmt(pdata->table_time, 1, 1, HMS(model->stats.tempo_accensione));
    lv_table_set_cell_value_fmt(pdata->table_time, 2, 1, HMS(model->stats.tempo_lavoro));
    lv_table_set_cell_value_fmt(pdata->table_time, 3, 1, HMS(model->stats.tempo_moto));
    lv_table_set_cell_value_fmt(pdata->table_time, 4, 1, HMS(model->stats.tempo_riscaldamento));
    lv_table_set_cell_value_fmt(pdata->table_time, 5, 1, HMS(model->stats.tempo_h2o_calda));
    lv_table_set_cell_value_fmt(pdata->table_time, 6, 1, HMS(model->stats.tempo_h2o_fredda));
    lv_table_set_cell_value_fmt(pdata->table_time, 7, 1, HMS(model->stats.tempo_h2o_rec_dep));
    lv_table_set_cell_value_fmt(pdata->table_time, 8, 1, HMS(model->stats.tempo_h2o_flusso));

    for (size_t i = 0; i < model->prog.parmac.numero_saponi_utilizzabili; i++) {
        lv_table_set_cell_value_fmt(pdata->table_detergent, i + 1, 1, HMS(model->stats.tempo_saponi[i]));
    }

    lv_table_set_cell_value_fmt(pdata->table_porthole, 1, 1, "%i", (int)model->stats.aperture_oblo);
    lv_table_set_cell_value_fmt(pdata->table_porthole, 2, 1, "%i", (int)model->stats.chiusure_oblo);
}


static void draw_event_cb(lv_event_t *e) {
    lv_draw_task_t     *draw_task = lv_event_get_draw_task(e);
    lv_draw_dsc_base_t *base_dsc  = draw_task->draw_dsc;
    /*If the cells are drawn...*/
    if (base_dsc->part == LV_PART_ITEMS) {
        uint32_t row = base_dsc->id1;

        /*Make the texts in the first cell center aligned*/
        if (row == 0) {
            lv_draw_label_dsc_t *label_draw_dsc = lv_draw_task_get_label_dsc(draw_task);
            if (label_draw_dsc) {
                label_draw_dsc->align = LV_TEXT_ALIGN_CENTER;
            }
            lv_draw_fill_dsc_t *fill_draw_dsc = lv_draw_task_get_fill_dsc(draw_task);
            if (fill_draw_dsc) {
                fill_draw_dsc->color = lv_color_mix(lv_palette_main(LV_PALETTE_BLUE), fill_draw_dsc->color, LV_OPA_30);
                fill_draw_dsc->opa   = LV_OPA_COVER;
            }
        }

        /*Make every 2nd row grayish*/
        if ((row != 0 && row % 2) == 0) {
            lv_draw_fill_dsc_t *fill_draw_dsc = lv_draw_task_get_fill_dsc(draw_task);
            if (fill_draw_dsc) {
                fill_draw_dsc->color = lv_color_mix(lv_palette_main(LV_PALETTE_GREY), fill_draw_dsc->color, LV_OPA_20);
                fill_draw_dsc->opa   = LV_OPA_COVER;
            }
        }
    }
}


const pman_page_t page_statistics = {
    .create        = create_page,
    .destroy       = pman_destroy_all,
    .open          = open_page,
    .close         = pman_close_all,
    .process_event = page_event,
};
