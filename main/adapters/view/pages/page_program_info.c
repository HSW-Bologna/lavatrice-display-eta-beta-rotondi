#include "lvgl.h"
#include "model/model.h"
#include "../view.h"
#include "../theme/style.h"
#include "../common.h"
#include "config/app_config.h"
#include "src/widgets/led/lv_led.h"
#include "../intl/intl.h"


LV_IMG_DECLARE(img_lavaggio);
LV_IMG_DECLARE(img_colorati);
LV_IMG_DECLARE(img_lavaggio_molto_sporchi);
LV_IMG_DECLARE(img_lavaggio_sporchi);
LV_IMG_DECLARE(img_normali);
LV_IMG_DECLARE(img_molto_sporchi);
LV_IMG_DECLARE(img_sintetici);
LV_IMG_DECLARE(img_piumoni);
LV_IMG_DECLARE(img_delicati);
LV_IMG_DECLARE(img_freddo);
LV_IMG_DECLARE(img_lana);
LV_IMG_DECLARE(img_fibre_naturali);
LV_IMG_DECLARE(img_solo_centrifuga);
LV_IMG_DECLARE(img_centrifuga_delicati);
LV_IMG_DECLARE(img_igienizza_cesto);
LV_IMG_DECLARE(img_ammollo);
LV_IMG_DECLARE(img_prelavaggio_centrifuga);
LV_IMG_DECLARE(img_risciacquo_centrifuga);


struct page_data {
    lv_obj_t *image_type;

    lv_obj_t *dropdown;

    uint8_t *changed;
};


enum {
    BTN_BACK_ID,
    BTN_NAME_ID,
    BTN_PRICE_ID,
    DROPDOWN_TYPE_ID,
};


static const lv_img_dsc_t *icons[] = {
    &img_lavaggio_molto_sporchi,
    &img_lavaggio_sporchi,
    &img_molto_sporchi,
    &img_normali,
    &img_colorati,
    &img_sintetici,
    &img_piumoni,
    &img_freddo,
    &img_lana,
    &img_fibre_naturali,
    &img_solo_centrifuga,
    &img_centrifuga_delicati,
    &img_igienizza_cesto,
    &img_ammollo,
    &img_prelavaggio_centrifuga,
    &img_risciacquo_centrifuga,
};


static void update_page(model_t *model, struct page_data *pdata);


static void *create_page(pman_handle_t handle, void *extra) {
    (void)handle;
    (void)extra;

    struct page_data *pdata = lv_malloc(sizeof(struct page_data));
    assert(pdata != NULL);
    pdata->changed = (uint8_t *)extra;

    return pdata;
}


static void open_page(pman_handle_t handle, void *state) {
    struct page_data *pdata = state;

    model_t               *model   = view_get_model(handle);
    programma_lavatrice_t *program = model_get_program(model);

    view_common_create_title(lv_scr_act(), view_intl_get_string(model, STRINGS_INFORMAZIONI), BTN_BACK_ID, -1);

    lv_obj_t *cont = lv_obj_create(lv_scr_act());
    lv_obj_set_style_pad_column(cont, 8, LV_STATE_DEFAULT);
    lv_obj_set_size(cont, LV_HOR_RES, LV_VER_RES - 56);
    lv_obj_set_layout(cont, LV_LAYOUT_FLEX);
    lv_obj_set_flex_flow(cont, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(cont, LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_align(cont, LV_ALIGN_BOTTOM_MID, 0, 0);

    {
        lv_obj_t *obj = lv_obj_create(cont);
        lv_obj_set_size(obj, LV_PCT(100), 64);
        lv_obj_add_style(obj, &style_transparent_cont, LV_STATE_DEFAULT);

        lv_obj_t *label_name = lv_label_create(obj);
        lv_obj_set_style_text_font(label_name, STYLE_FONT_SMALL, LV_STATE_DEFAULT);
        lv_label_set_long_mode(label_name, LV_LABEL_LONG_SCROLL_CIRCULAR);
        lv_obj_set_width(label_name, 230);
        lv_label_set_text(label_name, program->nomi[model->prog.parmac.lingua]);
        lv_obj_align(label_name, LV_ALIGN_LEFT_MID, 0, 0);

        lv_obj_t *button = view_common_icon_button_create(obj, LV_SYMBOL_EDIT, BTN_NAME_ID);
        lv_obj_align(button, LV_ALIGN_RIGHT_MID, 0, 0);
    }

    {
        lv_obj_t *obj = lv_obj_create(cont);
        lv_obj_set_size(obj, LV_PCT(100), 64);
        lv_obj_add_style(obj, &style_transparent_cont, LV_STATE_DEFAULT);

        lv_obj_t *label_price = lv_label_create(obj);
        lv_obj_set_style_text_font(label_price, STYLE_FONT_MEDIUM, LV_STATE_DEFAULT);
        lv_label_set_long_mode(label_price, LV_LABEL_LONG_SCROLL_CIRCULAR);
        lv_obj_set_width(label_price, 230);

        char string[32] = {0};
        model_formatta_prezzo(string, model, model_get_program(model)->prezzo);
        lv_label_set_text_fmt(label_price, "%s: %s", view_intl_get_string(model, STRINGS_PREZZO), string);
        lv_obj_align(label_price, LV_ALIGN_LEFT_MID, 0, 0);

        lv_obj_t *button = view_common_icon_button_create(obj, LV_SYMBOL_EDIT, BTN_PRICE_ID);
        lv_obj_align(button, LV_ALIGN_RIGHT_MID, 0, 0);

        view_register_object_default_callback(button, BTN_PRICE_ID);
    }

    {
        const size_t max_length = 1024;
        char        *string     = malloc(max_length);
        assert(string);

        const strings_t wash_types[] = {
            STRINGS_MOLTO_SPORCHI_CON_PRELAVAGGIO,
            STRINGS_MEDIO_SPORCHI_CON_PRELAVAGGIO,
            STRINGS_MOLTO_SPORCHI,
            STRINGS_MEDIO_SPORCHI,
            STRINGS_COLORATI,
            STRINGS_SINTETICI,
            STRINGS_PIUMONI,
            STRINGS_DELICATI_A_FREDDO,
            STRINGS_LANA,
            STRINGS_LINO_E_TENDAGGI,
            STRINGS_SOLO_CENTRIFUGA_1000_GIRI,
            STRINGS_SOLO_CENTRIFUGA_600_GIRI,
            STRINGS_SANIFICAZIONE,
            STRINGS_AMMOLLO,
            STRINGS_PRELAVAGGIO_CON_CENTRIFUGA,
            STRINGS_RISCIACQUO_CON_CENTRIFUGA,
        };
        memset(string, 0, max_length);
        for (int i = 0; i < NUM_TIPI_PROGRAMMA; i++) {
            const char *string_wash_type = view_intl_get_string(model, wash_types[i]);
            assert(strlen(string) + strlen(string_wash_type) < max_length);
            strcat(string, string_wash_type);
            string[strlen(string)] = '\n';
        }
        string[strlen(string) - 1] = '\0';

        lv_obj_t *dropdown = lv_dropdown_create(cont);
        lv_dropdown_set_options(dropdown, string);
        lv_obj_t *list = lv_dropdown_get_list(dropdown);
        lv_obj_set_style_text_font(dropdown, STYLE_FONT_SMALL, LV_PART_MAIN);
        lv_obj_set_style_text_font(list, STYLE_FONT_SMALL, LV_PART_MAIN);
        lv_obj_set_style_max_height(list, 100, LV_PART_MAIN);
        lv_dropdown_set_dir(dropdown, LV_DIR_TOP);
        lv_obj_set_width(dropdown, LV_PCT(100));
        view_register_object_default_callback(dropdown, DROPDOWN_TYPE_ID);
        pdata->dropdown = dropdown;

        lv_obj_t *image_type = lv_image_create(cont);
        lv_obj_align(image_type, LV_ALIGN_BOTTOM_MID, 0, 0);
        pdata->image_type = image_type;
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

                        case BTN_NAME_ID:
                            *pdata->changed = 1;
                            msg.stack_msg   = PMAN_STACK_MSG_PUSH_PAGE_EXTRA(
                                &page_keyboard, model_get_program(model)->nomi[model->prog.parmac.lingua]);
                            break;

                        case BTN_PRICE_ID:
                            *pdata->changed = 1;
                            msg.stack_msg   = PMAN_STACK_MSG_PUSH_PAGE(&page_price);
                            break;
                    }
                    break;
                }

                case LV_EVENT_VALUE_CHANGED: {
                    switch (obj_data->id) {
                        case DROPDOWN_TYPE_ID:
                            *pdata->changed                = 1;
                            model_get_program(model)->tipo = lv_dropdown_get_selected(pdata->dropdown);
                            update_page(model, pdata);
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
    programma_lavatrice_t *program = model_get_program(model);
    lv_dropdown_set_selected(pdata->dropdown, program->tipo);
    lv_image_set_src(pdata->image_type, icons[program->tipo]);
}


const pman_page_t page_program_info = {
    .create        = create_page,
    .destroy       = pman_destroy_all,
    .open          = open_page,
    .close         = pman_close_all,
    .process_event = page_event,
};
