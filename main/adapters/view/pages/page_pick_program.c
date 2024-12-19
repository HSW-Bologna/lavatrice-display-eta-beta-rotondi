#include <stdio.h>
#include <inttypes.h>
#include "lvgl.h"
#include "model/model.h"
#include "../view.h"
#include "../theme/style.h"
#include "../common.h"
#include "config/app_config.h"
#include "src/widgets/led/lv_led.h"
#include "../intl/intl.h"
#include "model/parmac.h"
#include <esp_log.h>


#define PROGRAM_WINDOW_SIZE 3


struct page_data {
    lv_obj_t *button_programs[PROGRAM_WINDOW_SIZE];
    lv_obj_t *button_up;
    lv_obj_t *button_down;
    lv_obj_t *button_confirm;

    lv_obj_t *label_names[PROGRAM_WINDOW_SIZE];
    lv_obj_t *label_numbers[PROGRAM_WINDOW_SIZE];

    uint16_t program_window_index;
    int16_t  selected_program;
};


enum {
    BTN_UP_ID,
    BTN_DOWN_ID,
    BTN_CONFIRM_ID,
    BTN_PROGRAM_ID,
    BTN_BACK_ID,
};


static void     update_page(model_t *model, struct page_data *pdata);
static void     move_window_down(model_t *model, struct page_data *pdata);
static void     move_window_up(model_t *model, struct page_data *pdata);
static uint16_t absolute_program_index(struct page_data *pdata);


static const char *TAG = "PagePrograms";


static void *create_page(pman_handle_t handle, void *extra) {
    (void)handle;
    (void)extra;
    (void)TAG;

    struct page_data *pdata = lv_malloc(sizeof(struct page_data));
    assert(pdata != NULL);

    pdata->program_window_index = 0;
    pdata->selected_program     = -1;

    return pdata;
}


static void open_page(pman_handle_t handle, void *state) {
    struct page_data *pdata = state;

    model_t *model = view_get_model(handle);

    view_common_create_title(lv_scr_act(), view_intl_get_string(model, STRINGS_PROGRAMMI), BTN_BACK_ID, -1);

    lv_obj_t *cont = lv_obj_create(lv_scr_act());
    lv_obj_set_style_pad_ver(cont, 2, LV_STATE_DEFAULT);
    lv_obj_set_style_pad_hor(cont, 2, LV_STATE_DEFAULT);
    lv_obj_set_size(cont, LV_HOR_RES, LV_VER_RES - 56);
    lv_obj_remove_flag(cont, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_align(cont, LV_ALIGN_BOTTOM_MID, 0, 0);

    const int32_t button_height = 48;

    {
        lv_obj_t *obj_button_bar = lv_obj_create(cont);
        lv_obj_set_size(obj_button_bar, LV_PCT(100), button_height + 4);
        lv_obj_remove_flag(obj_button_bar, LV_OBJ_FLAG_SCROLLABLE);
        lv_obj_add_style(obj_button_bar, &style_transparent_cont, LV_STATE_DEFAULT);
        lv_obj_set_style_pad_column(obj_button_bar, 12, LV_STATE_DEFAULT);
        lv_obj_set_style_pad_row(obj_button_bar, 12, LV_STATE_DEFAULT);
        lv_obj_set_layout(obj_button_bar, LV_LAYOUT_FLEX);
        lv_obj_set_flex_flow(obj_button_bar, LV_FLEX_FLOW_ROW);
        lv_obj_set_flex_align(obj_button_bar, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
        lv_obj_align(obj_button_bar, LV_ALIGN_TOP_MID, 0, 0);

        pdata->button_up = view_common_icon_button_create(obj_button_bar, LV_SYMBOL_UP, BTN_UP_ID);
    }

    {
        lv_obj_t *obj_button_bar = lv_obj_create(cont);
        lv_obj_set_size(obj_button_bar, LV_PCT(100), button_height + 4);
        lv_obj_remove_flag(obj_button_bar, LV_OBJ_FLAG_SCROLLABLE);
        lv_obj_add_style(obj_button_bar, &style_transparent_cont, LV_STATE_DEFAULT);
        lv_obj_set_style_pad_column(obj_button_bar, 12, LV_STATE_DEFAULT);
        lv_obj_set_style_pad_row(obj_button_bar, 12, LV_STATE_DEFAULT);
        lv_obj_set_layout(obj_button_bar, LV_LAYOUT_FLEX);
        lv_obj_set_flex_flow(obj_button_bar, LV_FLEX_FLOW_ROW);
        lv_obj_set_flex_align(obj_button_bar, LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
        lv_obj_align(obj_button_bar, LV_ALIGN_BOTTOM_MID, 0, 0);

        pdata->button_down    = view_common_icon_button_create(obj_button_bar, LV_SYMBOL_DOWN, BTN_DOWN_ID);
        pdata->button_confirm = view_common_icon_button_create(obj_button_bar, LV_SYMBOL_OK, BTN_CONFIRM_ID);
    }

    {
        lv_obj_t *obj_program_list = lv_obj_create(cont);
        lv_obj_set_size(obj_program_list, LV_PCT(100), LV_HOR_RES - 56 - button_height * 2 - 30);
        lv_obj_remove_flag(obj_program_list, LV_OBJ_FLAG_SCROLLABLE);
        lv_obj_add_style(obj_program_list, &style_transparent_cont, LV_STATE_DEFAULT);
        lv_obj_set_style_pad_column(obj_program_list, 4, LV_STATE_DEFAULT);
        lv_obj_set_style_pad_ver(obj_program_list, 2, LV_STATE_DEFAULT);
        lv_obj_set_layout(obj_program_list, LV_LAYOUT_FLEX);
        lv_obj_set_flex_flow(obj_program_list, LV_FLEX_FLOW_COLUMN);
        lv_obj_set_flex_align(obj_program_list, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
        lv_obj_align(obj_program_list, LV_ALIGN_CENTER, 0, -2);

        for (size_t i = 0; i < PROGRAM_WINDOW_SIZE; i++) {
            lv_obj_t *button = lv_button_create(obj_program_list);
            lv_obj_add_flag(button, LV_OBJ_FLAG_CHECKABLE);
            lv_obj_set_size(button, 300, 40);
            pdata->button_programs[i] = button;

            lv_obj_t *label = lv_label_create(button);
            lv_obj_set_style_text_font(label, STYLE_FONT_SMALL, LV_STATE_DEFAULT);
            lv_label_set_long_mode(label, LV_LABEL_LONG_SCROLL_CIRCULAR);
            lv_obj_set_width(label, 250);
            lv_obj_align(label, LV_ALIGN_RIGHT_MID, 0, 0);
            pdata->label_names[i] = label;

            lv_obj_t *label_number = lv_label_create(button);
            lv_obj_set_style_text_font(label_number, STYLE_FONT_SMALL, LV_STATE_DEFAULT);
            lv_label_set_long_mode(label_number, LV_LABEL_LONG_CLIP);
            lv_obj_align(label_number, LV_ALIGN_LEFT_MID, 0, 0);
            pdata->label_numbers[i] = label_number;

            view_register_object_default_callback_with_number(button, BTN_PROGRAM_ID, i);
        }
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

        case PMAN_EVENT_TAG_TIMER: {
            msg.stack_msg = PMAN_STACK_MSG_REBASE(view_common_main_page(model));
            break;
        }

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

                        case BTN_PROGRAM_ID: {
                            if (pdata->selected_program == obj_data->number) {
                                pdata->selected_program = -1;
                            } else {
                                pdata->selected_program = obj_data->number;
                            }
                            update_page(model, pdata);
                            break;
                        }

                        case BTN_UP_ID: {
                            pdata->selected_program = -1;
                            move_window_up(model, pdata);
                            update_page(model, pdata);
                            break;
                        }

                        case BTN_DOWN_ID: {
                            pdata->selected_program = -1;
                            move_window_down(model, pdata);
                            update_page(model, pdata);
                            break;
                        }

                        case BTN_CONFIRM_ID:
                            if (pdata->selected_program >= 0) {
                                model->prog.programmed_wash.program_index = absolute_program_index(pdata);
                                msg.stack_msg = PMAN_STACK_MSG_PUSH_PAGE_EXTRA(&page_datetime, (void *)(uintptr_t)1);
                            }
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
    for (uint16_t i = 0; i < PROGRAM_WINDOW_SIZE; i++) {
        uint16_t absolute_index = pdata->program_window_index * PROGRAM_WINDOW_SIZE + i;

        if (absolute_index < model->prog.num_programmi) {
            const programma_preview_t *preview = model_get_preview(model, absolute_index);

            if (pdata->selected_program == i) {
                lv_obj_add_state(pdata->button_programs[i], LV_STATE_CHECKED);
            } else {
                lv_obj_clear_state(pdata->button_programs[i], LV_STATE_CHECKED);
            }

            if (strcmp(lv_label_get_text(pdata->label_names[i]), preview->name)) {
                lv_label_set_text(pdata->label_names[i], preview->name);
            }
            lv_label_set_text_fmt(pdata->label_numbers[i], "%i.", absolute_index + 1);

            view_common_set_hidden(pdata->button_programs[i], 0);
        } else {
            view_common_set_hidden(pdata->button_programs[i], 1);
        }
    }

    if (pdata->selected_program >= 0) {
        lv_obj_clear_state(pdata->button_up, LV_STATE_DISABLED);
        lv_obj_clear_state(pdata->button_down, LV_STATE_DISABLED);
        lv_obj_clear_state(pdata->button_confirm, LV_STATE_DISABLED);

        if (model->prog.num_programmi < PROGRAM_WINDOW_SIZE) {
            lv_obj_add_state(pdata->button_up, LV_STATE_DISABLED);
            lv_obj_add_state(pdata->button_down, LV_STATE_DISABLED);
        } else {
            lv_obj_clear_state(pdata->button_up, LV_STATE_DISABLED);
            lv_obj_clear_state(pdata->button_down, LV_STATE_DISABLED);
        }
    } else {
        lv_obj_add_state(pdata->button_confirm, LV_STATE_DISABLED);
        lv_obj_clear_state(pdata->button_up, LV_STATE_DISABLED);
        lv_obj_clear_state(pdata->button_down, LV_STATE_DISABLED);

        if (model->prog.num_programmi < PROGRAM_WINDOW_SIZE) {
            lv_obj_add_state(pdata->button_up, LV_STATE_DISABLED);
            lv_obj_add_state(pdata->button_down, LV_STATE_DISABLED);
        } else {
            lv_obj_clear_state(pdata->button_up, LV_STATE_DISABLED);
            lv_obj_clear_state(pdata->button_down, LV_STATE_DISABLED);
        }
    }
}


static void destroy_page(void *state, void *extra) {
    struct page_data *pdata = state;
    (void)extra;
    lv_free(pdata);
}


static void close_page(void *state) {
    struct page_data *pdata = state;
    (void)pdata;
    lv_obj_clean(lv_scr_act());
}


static void move_window_down(model_t *model, struct page_data *pdata) {
    pdata->program_window_index++;
    if (pdata->program_window_index * PROGRAM_WINDOW_SIZE >= model->prog.num_programmi) {
        pdata->program_window_index = 0;
    }
}


static void move_window_up(model_t *model, struct page_data *pdata) {
    if (pdata->program_window_index > 0) {
        pdata->program_window_index--;
    } else {
        if (model->prog.num_programmi <= PROGRAM_WINDOW_SIZE) {
            pdata->program_window_index = 0;
        } else {
            int16_t extra_index         = (model->prog.num_programmi % PROGRAM_WINDOW_SIZE) != 0 ? 1 : 0;
            pdata->program_window_index = (model->prog.num_programmi / PROGRAM_WINDOW_SIZE) + extra_index - 1;
        }
    }
}


static uint16_t absolute_program_index(struct page_data *pdata) {
    if (pdata->selected_program >= 0) {
        return pdata->program_window_index * PROGRAM_WINDOW_SIZE + pdata->selected_program;
    } else {
        return pdata->program_window_index * PROGRAM_WINDOW_SIZE;
    }
}


const pman_page_t page_pick_program = {
    .create        = create_page,
    .destroy       = destroy_page,
    .open          = open_page,
    .close         = close_page,
    .process_event = page_event,
};