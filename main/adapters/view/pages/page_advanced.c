#include "lvgl.h"
#include "model/model.h"
#include "../view.h"
#include "../theme/style.h"
#include "../common.h"
#include "config/app_config.h"
#include "src/widgets/led/lv_led.h"
#include "../intl/intl.h"
#include "model/descriptions/AUTOGEN_FILE_pars.h"


#define BUTTON_WIDTH 150


struct page_data {
    lv_obj_t *label_access;

    lv_obj_t *led_communication;

    lv_obj_t *obj_default;
    lv_obj_t *obj_factory_reset;

    lv_obj_t *dropdown;

    uint8_t modified;
    uint8_t factory_reset;
};


enum {
    BTN_BACK_ID,
    BTN_COMMUNICATION_ID,
    BTN_FACTORY_RESET_ID,
    BTN_ACCESS_ID,
    BTN_NAME_ID,
    BTN_CONFIRM_FACTORY_RESET_ID,
    BTN_CANCEL_FACTORY_RESET_ID,
    BTN_DEBUG_ID,
};


static void update_page(model_t *model, struct page_data *pdata);


static void *create_page(pman_handle_t handle, void *extra) {
    (void)handle;
    (void)extra;

    struct page_data *pdata = lv_malloc(sizeof(struct page_data));
    assert(pdata != NULL);
    pdata->modified      = 0;
    pdata->factory_reset = 0;

    return pdata;
}


static void open_page(pman_handle_t handle, void *state) {
    struct page_data *pdata = state;

    model_t *model = view_get_model(handle);

    view_common_create_title(lv_scr_act(), view_intl_get_string(model, STRINGS_AVANZATE), BTN_BACK_ID, -1);

    lv_obj_t *cont = lv_obj_create(lv_scr_act());
    lv_obj_remove_flag(cont, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_pad_column(cont, 8, LV_STATE_DEFAULT);
    lv_obj_set_size(cont, LV_HOR_RES, LV_VER_RES - 56);
    lv_obj_set_layout(cont, LV_LAYOUT_FLEX);
    lv_obj_set_flex_flow(cont, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(cont, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_align(cont, LV_ALIGN_BOTTOM_MID, 0, 0);

    {
        lv_obj_t *btn = lv_btn_create(cont);
        lv_obj_set_width(btn, 280);
        lv_obj_t *lbl = lv_label_create(btn);
        lv_label_set_text(lbl, view_intl_get_string(model, STRINGS_COMUNICAZIONE));
        lv_obj_set_style_text_font(lbl, STYLE_FONT_SMALL, LV_STATE_DEFAULT);
        lv_obj_align(lbl, LV_ALIGN_LEFT_MID, 16, 0);

        lv_obj_t *led = lv_led_create(btn);
        lv_obj_set_size(led, 28, 28);
        lv_led_set_color(led, VIEW_STYLE_COLOR_RED);
        lv_obj_align(lbl, LV_ALIGN_RIGHT_MID, -16, 0);
        pdata->led_communication = led;

        view_register_object_default_callback(btn, BTN_COMMUNICATION_ID);
    }

    {
        lv_obj_t *btn = lv_btn_create(cont);
        lv_obj_set_width(btn, 280);
        lv_obj_t *lbl = lv_label_create(btn);
        lv_obj_set_style_text_align(lbl, LV_TEXT_ALIGN_CENTER, LV_STATE_DEFAULT);
        lv_label_set_long_mode(lbl, LV_LABEL_LONG_WRAP);
        lv_obj_set_width(lbl, 270);
        lv_label_set_text(lbl, view_intl_get_string(model, STRINGS_RIPRISTINO_A_IMPOSTAZIONI_DI_FABBRICA));
        lv_obj_set_style_text_font(lbl, STYLE_FONT_SMALL, LV_STATE_DEFAULT);
        lv_obj_align(lbl, LV_ALIGN_CENTER, 0, 0);

        view_register_object_default_callback(btn, BTN_FACTORY_RESET_ID);
    }

    {
        lv_obj_t *btn = lv_btn_create(cont);
        lv_obj_set_width(btn, 280);
        lv_obj_t *lbl = lv_label_create(btn);
        lv_obj_set_style_text_align(lbl, LV_TEXT_ALIGN_CENTER, LV_STATE_DEFAULT);
        lv_label_set_long_mode(lbl, LV_LABEL_LONG_WRAP);
        lv_obj_set_width(lbl, 270);
        lv_label_set_text(lbl, view_intl_get_string(model, STRINGS_RINOMINA_MACCHINA));
        lv_obj_set_style_text_font(lbl, STYLE_FONT_SMALL, LV_STATE_DEFAULT);
        lv_obj_align(lbl, LV_ALIGN_CENTER, 0, 0);

        view_register_object_default_callback(btn, BTN_NAME_ID);
    }

    {
        lv_obj_t *btn = lv_btn_create(cont);
        lv_obj_set_width(btn, 280);
        lv_obj_t *lbl = lv_label_create(btn);
        lv_obj_set_style_text_align(lbl, LV_TEXT_ALIGN_CENTER, LV_STATE_DEFAULT);
        lv_label_set_long_mode(lbl, LV_LABEL_LONG_WRAP);
        lv_obj_set_width(lbl, 270);
        lv_label_set_text(lbl, view_intl_get_string(model, STRINGS_ACCESSO));
        lv_obj_set_style_text_font(lbl, STYLE_FONT_SMALL, LV_STATE_DEFAULT);
        lv_obj_align(lbl, LV_ALIGN_CENTER, 0, 0);
        pdata->label_access = lbl;

        view_register_object_default_callback(btn, BTN_ACCESS_ID);
    }

    pdata->obj_default = cont;

    lv_obj_t *cont_factory_reset = lv_obj_create(lv_screen_active());
    lv_obj_set_size(cont_factory_reset, LV_HOR_RES, LV_VER_RES - 56);
    lv_obj_align(cont_factory_reset, LV_ALIGN_BOTTOM_MID, 0, 0);

    lv_obj_t *label = lv_label_create(cont_factory_reset);
    lv_label_set_text(label, view_intl_get_string(model, STRINGS_AZZERARE_ALLE_IMPOSTAZIONI_DI_FABBRICA));
    lv_obj_set_style_text_font(label, STYLE_FONT_MEDIUM, LV_STATE_DEFAULT);
    lv_label_set_long_mode(label, LV_LABEL_LONG_WRAP);
    lv_obj_set_style_text_align(label, LV_TEXT_ALIGN_CENTER, LV_STATE_DEFAULT);
    lv_obj_set_width(label, LV_PCT(100));
    lv_obj_align(label, LV_ALIGN_TOP_MID, 0, 16);

    {
        lv_obj_t *button =
            view_common_icon_button_create(cont_factory_reset, LV_SYMBOL_CLOSE, BTN_CANCEL_FACTORY_RESET_ID);
        lv_obj_align(button, LV_ALIGN_BOTTOM_LEFT, 0, 0);
    }

    {
        lv_obj_t *button =
            view_common_icon_button_create(cont_factory_reset, LV_SYMBOL_OK, BTN_CONFIRM_FACTORY_RESET_ID);
        lv_obj_align(button, LV_ALIGN_BOTTOM_RIGHT, 0, 0);
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

    pdata->obj_factory_reset = cont_factory_reset;

    VIEW_ADD_WATCHED_VARIABLE(&model->system.comunicazione_abilitata, 0);

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
                            if (pdata->modified) {
                                view_get_protocol(handle)->save_parmac(handle);
                            }
                            msg.stack_msg = PMAN_STACK_MSG_BACK();
                            break;

                        case BTN_COMMUNICATION_ID:
                            model->system.comunicazione_abilitata = !model->system.comunicazione_abilitata;
                            update_page(model, pdata);
                            break;

                        case BTN_ACCESS_ID:
                            model->prog.parmac.livello_accesso = (model->prog.parmac.livello_accesso + 1) % 2;
                            pdata->modified                    = 1;
                            update_page(model, pdata);
                            break;

                        case BTN_FACTORY_RESET_ID:
                            pdata->factory_reset = 1;
                            update_page(model, pdata);
                            break;

                        case BTN_NAME_ID:
                            msg.stack_msg   = PMAN_STACK_MSG_PUSH_PAGE_EXTRA(&page_keyboard, model->prog.parmac.nome);
                            pdata->modified = 1;
                            break;

                        case BTN_CANCEL_FACTORY_RESET_ID:
                            pdata->factory_reset = 0;
                            update_page(model, pdata);
                            break;

                        case BTN_CONFIRM_FACTORY_RESET_ID:
                            pdata->factory_reset = 0;
                            view_get_protocol(handle)->factory_reset(handle);
                            update_page(model, pdata);
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
    lv_label_set_text_fmt(pdata->label_access, "%s: %s", view_intl_get_string(model, STRINGS_ACCESSO),
                          pars_livello_accesso[model->prog.parmac.livello_accesso][model->prog.parmac.lingua]);

    lv_led_set_brightness(pdata->led_communication,
                          model->system.comunicazione_abilitata ? LV_LED_BRIGHT_MAX : LV_LED_BRIGHT_MIN);

    if (pdata->factory_reset) {
        view_common_set_hidden(pdata->obj_default, 1);
        view_common_set_hidden(pdata->obj_factory_reset, 0);
    } else {
        view_common_set_hidden(pdata->obj_default, 0);
        view_common_set_hidden(pdata->obj_factory_reset, 1);
    }
}


const pman_page_t page_advanced = {
    .create        = create_page,
    .destroy       = pman_destroy_all,
    .open          = open_page,
    .close         = pman_close_all,
    .process_event = page_event,
};
