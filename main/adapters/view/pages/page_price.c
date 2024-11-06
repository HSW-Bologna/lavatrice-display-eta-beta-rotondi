#include "lvgl.h"
#include "model/model.h"
#include "../view.h"
#include "../theme/style.h"
#include "../common.h"
#include "config/app_config.h"
#include "src/widgets/led/lv_led.h"
#include "../intl/intl.h"


struct page_data {
    char     *string;
    lv_obj_t *textarea;
};


enum {
    KEYBOARD_ID,
    TEXTAREA_ID,
};


static void update_page(model_t *model, struct page_data *pdata);
static void price_kb_event_cb(lv_event_t *e);


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

    lv_obj_t *cont = lv_obj_create(lv_screen_active());
    lv_obj_add_style(cont, &style_padless_cont, LV_STATE_DEFAULT);
    lv_obj_set_size(cont, LV_PCT(100), LV_PCT(100));
    lv_obj_center(cont);

    lv_obj_t *textarea = lv_textarea_create(cont);
    lv_obj_set_size(textarea, 200, 40);
    lv_textarea_set_one_line(textarea, 1);
    lv_textarea_set_max_length(textarea,
                               model->prog.parmac.cifre_prezzo + (model->prog.parmac.cifre_decimali_prezzo > 0));
    lv_obj_align(textarea, LV_ALIGN_TOP_MID, 0, 24);
    lv_textarea_set_cursor_pos(textarea, LV_TEXTAREA_CURSOR_LAST);
    view_register_object_default_callback(textarea, TEXTAREA_ID);

    char string[32] = {0};
    model_formatta_prezzo(string, model, model_get_program(model)->prezzo);
    lv_textarea_set_text(textarea, string);

    pdata->textarea = textarea;

    lv_obj_t *keyboard = lv_keyboard_create(cont);

    static const char *kbmap[] = {
        "1",
        "2",
        "3",
        LV_SYMBOL_BACKSPACE,
        "\n",
        "4",
        "5",
        "6",
        LV_SYMBOL_CLOSE,
        "\n",
        "7",
        "8",
        "9",
        LV_SYMBOL_OK,
        "\n",
        LV_SYMBOL_LEFT,
        "0",
        LV_SYMBOL_RIGHT,
        "",
    };

    static const lv_buttonmatrix_ctrl_t ctrl_map[] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};

    lv_keyboard_set_map(keyboard, LV_KEYBOARD_MODE_NUMBER, kbmap, ctrl_map);
    lv_keyboard_set_mode(keyboard, LV_KEYBOARD_MODE_NUMBER);
    lv_keyboard_set_textarea(keyboard, textarea);
    lv_obj_add_style(keyboard, (lv_style_t *)&style_config_btn, LV_PART_ITEMS);
    lv_obj_set_style_text_font(keyboard, STYLE_FONT_BIG, LV_PART_ITEMS);
    lv_obj_set_style_bg_opa(keyboard, LV_OPA_TRANSP, LV_STATE_DEFAULT);
    lv_obj_set_size(keyboard, 240, 200);
    lv_obj_align(keyboard, LV_ALIGN_BOTTOM_MID, 0, 0);
    lv_obj_remove_event_cb(keyboard, lv_keyboard_def_event_cb);
    lv_obj_add_event_cb(keyboard, price_kb_event_cb, LV_EVENT_VALUE_CHANGED, NULL);
    view_register_object_default_callback(keyboard, KEYBOARD_ID);

    update_page(model, pdata);
}


static pman_msg_t page_event(pman_handle_t handle, void *state, pman_event_t event) {
    struct page_data *pdata = state;

    mut_model_t *model = view_get_model(handle);

    pman_msg_t msg = PMAN_MSG_NULL;

    switch (event.tag) {
        case PMAN_EVENT_TAG_OPEN:
            lv_obj_send_event(pdata->textarea, LV_EVENT_FOCUSED, NULL);
            lv_textarea_set_cursor_pos(pdata->textarea, LV_TEXTAREA_CURSOR_LAST);
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
                case LV_EVENT_VALUE_CHANGED: {
                    switch (obj_data->id) {
                        case TEXTAREA_ID:
                            break;

                        default:
                            break;
                    }
                    break;
                }

                case LV_EVENT_READY: {
                    switch (obj_data->id) {
                        case KEYBOARD_ID:
                            update_program_price(model_get_program(model), lv_textarea_get_text(pdata->textarea));
                            msg.stack_msg = PMAN_STACK_MSG_BACK();
                            break;

                        default:
                            break;
                    }
                    break;
                }

                case LV_EVENT_CANCEL: {
                    switch (obj_data->id) {
                        case KEYBOARD_ID:
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
    (void)model;
    (void)pdata;
}


static void skip_dot(lv_obj_t *ta, int dir) {
    size_t      cursor = lv_textarea_get_cursor_pos(ta);
    const char *text   = lv_textarea_get_text(ta);

    if (text[cursor] == '.') {
        if (dir) {
            lv_textarea_cursor_right(ta);
        } else {
            lv_textarea_cursor_left(ta);
        }
    }
}


static void price_kb_event_cb(lv_event_t *e) {
    lv_obj_t *obj = lv_event_get_current_target(e);

    LV_ASSERT_OBJ(obj, MY_CLASS);
    lv_keyboard_t *keyboard = (lv_keyboard_t *)obj;
    uint32_t       btn_id   = lv_buttonmatrix_get_selected_button(obj);
    if (btn_id == LV_BUTTONMATRIX_BUTTON_NONE)
        return;

    const char *txt = lv_buttonmatrix_get_button_text(obj, btn_id);
    if (txt == NULL)
        return;

    if (lv_strcmp(txt, LV_SYMBOL_CLOSE) == 0 || lv_strcmp(txt, LV_SYMBOL_KEYBOARD) == 0) {
        lv_result_t res = lv_obj_send_event(obj, LV_EVENT_CANCEL, NULL);
        if (res != LV_RESULT_OK)
            return;

        if (keyboard->ta) {
            res = lv_obj_send_event(keyboard->ta, LV_EVENT_CANCEL, NULL);
            if (res != LV_RESULT_OK)
                return;
        }
        return;
    } else if (lv_strcmp(txt, LV_SYMBOL_OK) == 0) {
        lv_result_t res = lv_obj_send_event(obj, LV_EVENT_READY, NULL);
        if (res != LV_RESULT_OK)
            return;

        if (keyboard->ta) {
            res = lv_obj_send_event(keyboard->ta, LV_EVENT_READY, NULL);
            if (res != LV_RESULT_OK)
                return;
        }
        return;
    }

    /*Add the characters to the text area if set*/
    if (keyboard->ta == NULL)
        return;

    skip_dot(keyboard->ta, 1);

    if (lv_strcmp(txt, "Enter") == 0 || lv_strcmp(txt, LV_SYMBOL_NEW_LINE) == 0) {
        lv_textarea_add_char(keyboard->ta, '\n');
        if (lv_textarea_get_one_line(keyboard->ta)) {
            lv_result_t res = lv_obj_send_event(keyboard->ta, LV_EVENT_READY, NULL);
            if (res != LV_RESULT_OK)
                return;
        }
    } else if (strcmp(txt, LV_SYMBOL_LEFT) == 0 || strcmp(txt, LV_SYMBOL_BACKSPACE) == 0) {
        lv_textarea_cursor_left(keyboard->ta);
        skip_dot(keyboard->ta, 0);
    } else if (strcmp(txt, LV_SYMBOL_RIGHT) == 0) {
        lv_textarea_cursor_right(keyboard->ta);
        skip_dot(keyboard->ta, 1);
    } else {
        lv_textarea_delete_char_forward(keyboard->ta);
        lv_textarea_add_text(keyboard->ta, txt);
        skip_dot(keyboard->ta, 1);
    }
}


const pman_page_t page_price = {
    .create        = create_page,
    .destroy       = pman_destroy_all,
    .open          = open_page,
    .close         = pman_close_all,
    .process_event = page_event,
};
