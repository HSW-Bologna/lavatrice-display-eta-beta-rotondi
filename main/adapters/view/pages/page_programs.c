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


#define PROGRAM_WINDOW_SIZE 3


struct page_data {
    lv_obj_t *button_programs[PROGRAM_WINDOW_SIZE];
    lv_obj_t *button_up;
    lv_obj_t *button_down;
    lv_obj_t *button_move_up;
    lv_obj_t *button_move_down;
    lv_obj_t *button_delete;
    lv_obj_t *button_modify;
    lv_obj_t *button_copy;
    lv_obj_t *button_new;

    lv_obj_t *label_programs[PROGRAM_WINDOW_SIZE];

    pman_timer_t *timer;

    uint16_t program_window_index;
    int16_t  selected_program;
};


enum {
    BTN_UP_ID,
    BTN_DOWN_ID,
    BTN_MOVE_UP_ID,
    BTN_MOVE_DOWN_ID,
    BTN_COPY_ID,
    BTN_DELETE_ID,
    BTN_NEW_ID,
    BTN_MODIFY_ID,
    BTN_PROGRAM_ID,
    BTN_BACK_ID,
};


static void update_page(model_t *model, struct page_data *pdata);


static void *create_page(pman_handle_t handle, void *extra) {
    (void)extra;

    struct page_data *pdata = lv_malloc(sizeof(struct page_data));
    assert(pdata != NULL);

    pdata->program_window_index = 0;
    pdata->selected_program     = -1;
    pdata->timer                = PMAN_REGISTER_TIMER_ID(handle, APP_CONFIG_PAGE_TIMEOUT, 0);

    return pdata;
}


static void open_page(pman_handle_t handle, void *state) {
    struct page_data *pdata = state;

    model_t *model = view_get_model(handle);

    view_common_create_title(lv_scr_act(), view_intl_get_string(model, STRINGS_PROGRAMMI), BTN_BACK_ID, -1);

    lv_obj_t *cont = lv_obj_create(lv_scr_act());
    lv_obj_set_size(cont, LV_HOR_RES, LV_VER_RES - 56);
    lv_obj_clear_flag(cont, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_align(cont, LV_ALIGN_BOTTOM_MID, 0, 0);

    const int32_t button_height = 48;

    {
        lv_obj_t *obj_button_bar = lv_obj_create(cont);
        lv_obj_set_size(obj_button_bar, LV_PCT(100), button_height);
        lv_obj_clear_flag(obj_button_bar, LV_OBJ_FLAG_SCROLLABLE);
        lv_obj_add_style(obj_button_bar, &style_transparent_cont, LV_STATE_DEFAULT);
        lv_obj_set_style_pad_column(obj_button_bar, 12, LV_STATE_DEFAULT);
        lv_obj_set_style_pad_row(obj_button_bar, 12, LV_STATE_DEFAULT);
        lv_obj_set_layout(obj_button_bar, LV_LAYOUT_FLEX);
        lv_obj_set_flex_flow(obj_button_bar, LV_FLEX_FLOW_ROW);
        lv_obj_set_flex_align(obj_button_bar, LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
        lv_obj_align(obj_button_bar, LV_ALIGN_TOP_MID, 0, -6);

        {
            lv_obj_t *button = lv_button_create(obj_button_bar);
            lv_obj_set_style_radius(button, LV_RADIUS_CIRCLE, LV_STATE_DEFAULT);
            lv_obj_set_size(button, button_height, button_height);

            lv_obj_t *label = lv_label_create(button);
            lv_label_set_text(label, LV_SYMBOL_UP);
            lv_obj_center(label);

            view_register_object_default_callback(button, BTN_UP_ID);
            pdata->button_up = button;
        }

        {
            lv_obj_t *button = lv_button_create(obj_button_bar);
            lv_obj_set_style_radius(button, LV_RADIUS_CIRCLE, LV_STATE_DEFAULT);
            lv_obj_set_size(button, button_height, button_height);

            lv_obj_t *label = lv_label_create(button);
            lv_label_set_text(label, LV_SYMBOL_COPY);
            lv_obj_center(label);

            view_register_object_default_callback(button, BTN_COPY_ID);
            pdata->button_copy = button;
        }

        {
            lv_obj_t *button = lv_button_create(obj_button_bar);
            lv_obj_set_style_radius(button, LV_RADIUS_CIRCLE, LV_STATE_DEFAULT);
            lv_obj_set_size(button, button_height, button_height);

            lv_obj_t *label = lv_label_create(button);
            lv_label_set_text(label, LV_SYMBOL_TRASH);
            lv_obj_center(label);

            view_register_object_default_callback(button, BTN_DELETE_ID);
            pdata->button_delete = button;
        }

        {
            lv_obj_t *button = lv_button_create(obj_button_bar);
            lv_obj_set_style_radius(button, LV_RADIUS_CIRCLE, LV_STATE_DEFAULT);
            lv_obj_set_size(button, button_height, button_height);

            {
                lv_obj_t *label = lv_label_create(button);
                lv_label_set_text(label, LV_SYMBOL_UP);
                lv_obj_align(label, LV_ALIGN_CENTER, 0, -8);
            }

            {
                lv_obj_t *label = lv_label_create(button);
                lv_label_set_text(label, LV_SYMBOL_UP);
                lv_obj_align(label, LV_ALIGN_CENTER, 0, +8);
            }

            view_register_object_default_callback(button, BTN_MOVE_UP_ID);
            pdata->button_move_up = button;
        }
    }

    {
        lv_obj_t *obj_button_bar = lv_obj_create(cont);
        lv_obj_set_size(obj_button_bar, LV_PCT(100), button_height);
        lv_obj_clear_flag(obj_button_bar, LV_OBJ_FLAG_SCROLLABLE);
        lv_obj_add_style(obj_button_bar, &style_transparent_cont, LV_STATE_DEFAULT);
        lv_obj_set_style_pad_column(obj_button_bar, 12, LV_STATE_DEFAULT);
        lv_obj_set_style_pad_row(obj_button_bar, 12, LV_STATE_DEFAULT);
        lv_obj_set_layout(obj_button_bar, LV_LAYOUT_FLEX);
        lv_obj_set_flex_flow(obj_button_bar, LV_FLEX_FLOW_ROW);
        lv_obj_set_flex_align(obj_button_bar, LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
        lv_obj_align(obj_button_bar, LV_ALIGN_BOTTOM_MID, 0, 6);

        {
            lv_obj_t *button = lv_button_create(obj_button_bar);
            lv_obj_set_style_radius(button, LV_RADIUS_CIRCLE, LV_STATE_DEFAULT);
            lv_obj_set_size(button, button_height, button_height);

            lv_obj_t *label = lv_label_create(button);
            lv_label_set_text(label, LV_SYMBOL_DOWN);
            lv_obj_center(label);

            view_register_object_default_callback(button, BTN_DOWN_ID);
            pdata->button_down = button;
        }

        {
            lv_obj_t *button = lv_button_create(obj_button_bar);
            lv_obj_set_style_radius(button, LV_RADIUS_CIRCLE, LV_STATE_DEFAULT);
            lv_obj_set_size(button, button_height, button_height);

            lv_obj_t *label = lv_label_create(button);
            lv_label_set_text(label, LV_SYMBOL_PLUS);
            lv_obj_center(label);

            view_register_object_default_callback(button, BTN_MODIFY_ID);
            pdata->button_new = button;
        }

        {
            lv_obj_t *button = lv_button_create(obj_button_bar);
            lv_obj_set_style_radius(button, LV_RADIUS_CIRCLE, LV_STATE_DEFAULT);
            lv_obj_set_size(button, button_height, button_height);

            lv_obj_t *label = lv_label_create(button);
            lv_label_set_text(label, LV_SYMBOL_EDIT);
            lv_obj_center(label);

            view_register_object_default_callback(button, BTN_MODIFY_ID);
            pdata->button_modify = button;
        }

        {
            lv_obj_t *button = lv_button_create(obj_button_bar);
            lv_obj_set_style_radius(button, LV_RADIUS_CIRCLE, LV_STATE_DEFAULT);
            lv_obj_set_size(button, button_height, button_height);

            {
                lv_obj_t *label = lv_label_create(button);
                lv_label_set_text(label, LV_SYMBOL_DOWN);
                lv_obj_align(label, LV_ALIGN_CENTER, 0, -8);
            }

            {
                lv_obj_t *label = lv_label_create(button);
                lv_label_set_text(label, LV_SYMBOL_DOWN);
                lv_obj_align(label, LV_ALIGN_CENTER, 0, +8);
            }

            view_register_object_default_callback(button, BTN_MOVE_DOWN_ID);
            pdata->button_move_down = button;
        }
    }

    {
        lv_obj_t *obj_program_list = lv_obj_create(cont);
        lv_obj_set_size(obj_program_list, LV_PCT(100), LV_HOR_RES - 56 - button_height * 2 - 40);
        lv_obj_clear_flag(obj_program_list, LV_OBJ_FLAG_SCROLLABLE);
        lv_obj_add_style(obj_program_list, &style_transparent_cont, LV_STATE_DEFAULT);
        lv_obj_set_style_pad_column(obj_program_list, 8, LV_STATE_DEFAULT);
        lv_obj_set_layout(obj_program_list, LV_LAYOUT_FLEX);
        lv_obj_set_flex_flow(obj_program_list, LV_FLEX_FLOW_COLUMN);
        lv_obj_set_flex_align(obj_program_list, LV_FLEX_ALIGN_SPACE_AROUND, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
        lv_obj_align(obj_program_list, LV_ALIGN_CENTER, 0, 0);

        for (size_t i = 0; i < PROGRAM_WINDOW_SIZE; i++) {
            lv_obj_t *button = lv_button_create(obj_program_list);
            lv_obj_add_flag(button, LV_OBJ_FLAG_CHECKABLE);
            lv_obj_set_size(button, LV_PCT(100), 32);
            pdata->button_programs[i] = button;

            lv_obj_t *label = lv_label_create(button);
            lv_obj_set_style_text_font(label, STYLE_FONT_SMALL, LV_STATE_DEFAULT);
            lv_label_set_long_mode(label, LV_LABEL_LONG_SCROLL_CIRCULAR);
            lv_obj_set_width(label, LV_PCT(100));
            lv_obj_center(label);
            pdata->label_programs[i] = label;

            view_register_object_default_callback_with_number(button, BTN_PROGRAM_ID, i);
        }
    }

    pman_timer_reset(pdata->timer);
    pman_timer_resume(pdata->timer);

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
            msg.stack_msg = PMAN_STACK_MSG_REBASE(&page_main);
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
                    pman_timer_reset(pdata->timer);

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
                            if (pdata->program_window_index > 0) {
                                pdata->program_window_index--;
                            } else {
                                if (model->prog.num_programmi <= PROGRAM_WINDOW_SIZE) {
                                    pdata->program_window_index = 0;
                                } else {
                                    int16_t extra_index =
                                        (model->prog.num_programmi % PROGRAM_WINDOW_SIZE) != 0 ? 1 : 0;
                                    pdata->program_window_index =
                                        (model->prog.num_programmi / PROGRAM_WINDOW_SIZE) + extra_index - 1;
                                }
                            }
                            update_page(model, pdata);
                            break;
                        }

                        case BTN_DOWN_ID: {
                            pdata->selected_program = -1;
                            pdata->program_window_index++;
                            if (pdata->program_window_index * PROGRAM_WINDOW_SIZE > model->prog.num_programmi) {
                                pdata->program_window_index = 0;
                            }
                            update_page(model, pdata);
                            break;
                        }

                        case BTN_MOVE_DOWN_ID:
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
        uint16_t                   absolute_index = pdata->program_window_index * PROGRAM_WINDOW_SIZE + i;
        const programma_preview_t *preview        = model_get_preview(model, absolute_index);

        if (pdata->selected_program == i) {
            lv_obj_add_state(pdata->button_programs[i], LV_STATE_CHECKED);
        } else {
            lv_obj_clear_state(pdata->button_programs[i], LV_STATE_CHECKED);
        }

        char string[64] = {0};
        snprintf(string, sizeof(string), "%02i. %s", absolute_index + 1, preview->name);
        if (strcmp(lv_label_get_text(pdata->label_programs[i]), string)) {
            lv_label_set_text(pdata->label_programs[i], string);
        }
    }

    if (pdata->selected_program >= 0) {
        lv_obj_clear_state(pdata->button_move_up, LV_STATE_DISABLED);
        lv_obj_clear_state(pdata->button_move_down, LV_STATE_DISABLED);
        lv_obj_clear_state(pdata->button_modify, LV_STATE_DISABLED);
        lv_obj_clear_state(pdata->button_copy, LV_STATE_DISABLED);
        lv_obj_clear_state(pdata->button_delete, LV_STATE_DISABLED);
    } else {
        lv_obj_add_state(pdata->button_move_up, LV_STATE_DISABLED);
        lv_obj_add_state(pdata->button_move_down, LV_STATE_DISABLED);
        lv_obj_add_state(pdata->button_modify, LV_STATE_DISABLED);
        lv_obj_add_state(pdata->button_copy, LV_STATE_DISABLED);
        lv_obj_add_state(pdata->button_delete, LV_STATE_DISABLED);
    }

    if (model->prog.num_programmi < PROGRAM_WINDOW_SIZE) {
        lv_obj_add_state(pdata->button_up, LV_STATE_DISABLED);
        lv_obj_add_state(pdata->button_down, LV_STATE_DISABLED);
    } else {
        lv_obj_clear_state(pdata->button_up, LV_STATE_DISABLED);
        lv_obj_clear_state(pdata->button_down, LV_STATE_DISABLED);
    }

    if (model->prog.num_programmi < MAX_PROGRAMMI) {
        lv_obj_clear_state(pdata->button_new, LV_STATE_DISABLED);
    } else {
        lv_obj_add_state(pdata->button_new, LV_STATE_DISABLED);
    }
}


static void destroy_page(void *state, void *extra) {
    struct page_data *pdata = state;
    (void)extra;
    pman_timer_delete(pdata->timer);
    lv_free(pdata);
}


static void close_page(void *state) {
    struct page_data *pdata = state;
    pman_timer_pause(pdata->timer);
    lv_obj_clean(lv_scr_act());
}


const pman_page_t page_programs = {
    .create        = create_page,
    .destroy       = destroy_page,
    .open          = open_page,
    .close         = close_page,
    .process_event = page_event,
};
