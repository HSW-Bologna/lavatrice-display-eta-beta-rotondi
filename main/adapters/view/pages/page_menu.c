#include "lvgl.h"
#include "model/model.h"
#include "../view.h"
#include "../theme/style.h"
#include "../common.h"
#include "config/app_config.h"
#include "src/widgets/led/lv_led.h"
#include "../intl/intl.h"


#define COMPUTE_BUILD_YEAR                                                                                             \
    ((__DATE__[7] - '0') * 1000 + (__DATE__[8] - '0') * 100 + (__DATE__[9] - '0') * 10 + (__DATE__[10] - '0'))


#define COMPUTE_BUILD_DAY (((__DATE__[4] >= '0') ? (__DATE__[4] - '0') * 10 : 0) + (__DATE__[5] - '0'))


#define BUILD_MONTH_IS_JAN (__DATE__[0] == 'J' && __DATE__[1] == 'a' && __DATE__[2] == 'n')
#define BUILD_MONTH_IS_FEB (__DATE__[0] == 'F')
#define BUILD_MONTH_IS_MAR (__DATE__[0] == 'M' && __DATE__[1] == 'a' && __DATE__[2] == 'r')
#define BUILD_MONTH_IS_APR (__DATE__[0] == 'A' && __DATE__[1] == 'p')
#define BUILD_MONTH_IS_MAY (__DATE__[0] == 'M' && __DATE__[1] == 'a' && __DATE__[2] == 'y')
#define BUILD_MONTH_IS_JUN (__DATE__[0] == 'J' && __DATE__[1] == 'u' && __DATE__[2] == 'n')
#define BUILD_MONTH_IS_JUL (__DATE__[0] == 'J' && __DATE__[1] == 'u' && __DATE__[2] == 'l')
#define BUILD_MONTH_IS_AUG (__DATE__[0] == 'A' && __DATE__[1] == 'u')
#define BUILD_MONTH_IS_SEP (__DATE__[0] == 'S')
#define BUILD_MONTH_IS_OCT (__DATE__[0] == 'O')
#define BUILD_MONTH_IS_NOV (__DATE__[0] == 'N')
#define BUILD_MONTH_IS_DEC (__DATE__[0] == 'D')


#define COMPUTE_BUILD_MONTH                                                                                            \
    ((BUILD_MONTH_IS_JAN)   ? 1                                                                                        \
     : (BUILD_MONTH_IS_FEB) ? 2                                                                                        \
     : (BUILD_MONTH_IS_MAR) ? 3                                                                                        \
     : (BUILD_MONTH_IS_APR) ? 4                                                                                        \
     : (BUILD_MONTH_IS_MAY) ? 5                                                                                        \
     : (BUILD_MONTH_IS_JUN) ? 6                                                                                        \
     : (BUILD_MONTH_IS_JUL) ? 7                                                                                        \
     : (BUILD_MONTH_IS_AUG) ? 8                                                                                        \
     : (BUILD_MONTH_IS_SEP) ? 9                                                                                        \
     : (BUILD_MONTH_IS_OCT) ? 10                                                                                       \
     : (BUILD_MONTH_IS_NOV) ? 11                                                                                       \
     : (BUILD_MONTH_IS_DEC) ? 12                                                                                       \
                            : /* error default */ 99)

#define BUTTON_WIDTH 150


struct page_data {
    keyboard_page_options_t keyboard_options;

    lv_obj_t *btn_drive;
};


enum {
    BTN_BACK_ID,
    BTN_TEST_ID,
    BTN_DRIVE_ID,
    BTN_PARMAC_ID,
    BTN_PROGRAMS_ID,
    BTN_ADVANCED_ID,
    BTN_DATETIME_ID,
    BTN_STATS_ID,
    BTN_EVENTS_ID,
    BTN_PROGRAMMED_WASH_ID,
    BTN_PASSWORD_ID,
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

    view_common_create_title(lv_scr_act(), view_intl_get_string(model, STRINGS_IMPOSTAZIONI), BTN_BACK_ID, -1);

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
        lv_obj_t *btn = lv_btn_create(cont);
        lv_obj_set_width(btn, BUTTON_WIDTH);
        lv_obj_t *lbl = lv_label_create(btn);
        lv_label_set_text(lbl, view_intl_get_string(model, STRINGS_DIAGNOSI));
        lv_obj_set_style_text_font(lbl, STYLE_FONT_SMALL, LV_STATE_DEFAULT);
        lv_obj_center(lbl);
        view_register_object_default_callback(btn, BTN_TEST_ID);
    }

    {
        lv_obj_t *btn = lv_btn_create(cont);
        lv_obj_set_width(btn, BUTTON_WIDTH);
        lv_obj_t *lbl = lv_label_create(btn);
        lv_label_set_text(lbl, view_intl_get_string(model, STRINGS_PARAMETRI));
        lv_obj_set_style_text_font(lbl, STYLE_FONT_SMALL, LV_STATE_DEFAULT);
        lv_obj_center(lbl);
        view_register_object_default_callback(btn, BTN_PARMAC_ID);
    }

    {
        lv_obj_t *btn = lv_btn_create(cont);
        lv_obj_set_width(btn, BUTTON_WIDTH);
        lv_obj_t *lbl = lv_label_create(btn);
        lv_label_set_text(lbl, view_intl_get_string(model, STRINGS_PROGRAMMI));
        lv_obj_set_style_text_font(lbl, STYLE_FONT_SMALL, LV_STATE_DEFAULT);
        lv_obj_center(lbl);
        view_register_object_default_callback(btn, BTN_PROGRAMS_ID);
    }

    {
        lv_obj_t *btn = lv_btn_create(cont);
        lv_obj_set_width(btn, BUTTON_WIDTH);
        lv_obj_t *lbl = lv_label_create(btn);
        lv_label_set_text(lbl, view_intl_get_string(model, STRINGS_ARCHIVIAZIONE));
        lv_obj_set_style_text_font(lbl, STYLE_FONT_SMALL, LV_STATE_DEFAULT);
        lv_obj_center(lbl);
        view_register_object_default_callback(btn, BTN_DRIVE_ID);
        pdata->btn_drive = btn;
    }

    {
        lv_obj_t *btn = lv_btn_create(cont);
        lv_obj_set_width(btn, BUTTON_WIDTH);
        lv_obj_t *lbl = lv_label_create(btn);
        lv_label_set_text(lbl, view_intl_get_string(model, STRINGS_AVANZATE));
        lv_obj_set_style_text_font(lbl, STYLE_FONT_SMALL, LV_STATE_DEFAULT);
        lv_obj_center(lbl);
        view_register_object_default_callback(btn, BTN_ADVANCED_ID);
    }

    {
        lv_obj_t *btn = lv_btn_create(cont);
        lv_obj_set_width(btn, BUTTON_WIDTH);
        lv_obj_t *lbl = lv_label_create(btn);
        lv_label_set_text(lbl, view_intl_get_string(model, STRINGS_ORA_DATA));
        lv_obj_set_style_text_font(lbl, STYLE_FONT_SMALL, LV_STATE_DEFAULT);
        lv_obj_center(lbl);
        view_register_object_default_callback(btn, BTN_DATETIME_ID);
    }

    if (model->prog.parmac.abilitazione_lavaggio_programmato) {
        lv_obj_t *btn = lv_btn_create(cont);
        lv_obj_set_width(btn, BUTTON_WIDTH);
        lv_obj_t *lbl = lv_label_create(btn);
        lv_obj_set_width(lbl, LV_PCT(100));
        lv_label_set_long_mode(lbl, LV_LABEL_LONG_SCROLL);
        lv_obj_set_style_text_font(lbl, STYLE_FONT_SMALL, LV_STATE_DEFAULT);
        lv_label_set_text(lbl, view_intl_get_string(model, STRINGS_LAVAGGIO_PROGRAMMATO));
        lv_obj_center(lbl);
        view_register_object_default_callback(btn, BTN_PROGRAMMED_WASH_ID);
    }

    {
        lv_obj_t *btn = lv_btn_create(cont);
        lv_obj_set_width(btn, BUTTON_WIDTH);
        lv_obj_t *lbl = lv_label_create(btn);
        lv_label_set_text(lbl, view_intl_get_string(model, STRINGS_PASSWORD));
        lv_obj_set_style_text_font(lbl, STYLE_FONT_SMALL, LV_STATE_DEFAULT);
        lv_obj_center(lbl);
        view_register_object_default_callback(btn, BTN_PASSWORD_ID);
    }

    {
        lv_obj_t *btn = lv_btn_create(cont);
        lv_obj_set_width(btn, BUTTON_WIDTH);
        lv_obj_t *lbl = lv_label_create(btn);
        lv_label_set_text(lbl, view_intl_get_string(model, STRINGS_STATISTICHE));
        lv_obj_set_style_text_font(lbl, STYLE_FONT_SMALL, LV_STATE_DEFAULT);
        lv_obj_center(lbl);
        view_register_object_default_callback(btn, BTN_STATS_ID);
    }

    {
        lv_obj_t *btn = lv_btn_create(cont);
        lv_obj_set_width(btn, BUTTON_WIDTH);
        lv_obj_t *lbl = lv_label_create(btn);
        lv_label_set_text(lbl, view_intl_get_string(model, STRINGS_EVENTI));
        lv_obj_set_style_text_font(lbl, STYLE_FONT_SMALL, LV_STATE_DEFAULT);
        lv_obj_center(lbl);
        view_register_object_default_callback(btn, BTN_EVENTS_ID);
    }

    lv_obj_t *label_machine_name = lv_label_create(lv_screen_active());
    lv_obj_set_style_text_font(label_machine_name, STYLE_FONT_SMALL, LV_STATE_DEFAULT);
    lv_obj_align(label_machine_name, LV_ALIGN_TOP_MID, 0, 64);
    lv_label_set_text(label_machine_name, model->prog.parmac.nome);

    lv_obj_t *label_machine_version = lv_label_create(lv_screen_active());
    lv_obj_set_style_text_font(label_machine_version, STYLE_FONT_SMALL, LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(label_machine_version, lv_color_lighten(VIEW_STYLE_COLOR_GREEN, LV_OPA_50),
                                LV_STATE_DEFAULT);
    lv_label_set_text_fmt(label_machine_version, "macchina %s %s", model->system.machine_fw_version,
                          model->system.machine_fw_date);
    lv_obj_align(label_machine_version, LV_ALIGN_BOTTOM_RIGHT, 0, 0);

    lv_obj_t *label_display_version = lv_label_create(lv_screen_active());
    lv_obj_set_style_text_font(label_display_version, STYLE_FONT_SMALL, LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(label_display_version, lv_color_lighten(VIEW_STYLE_COLOR_RED, LV_OPA_50),
                                LV_STATE_DEFAULT);
    lv_label_set_text_fmt(label_display_version, "display v%i.%i.%i %02i/%02i/%i", APP_CONFIG_FIRMWARE_VERSION_MAJOR,
                          APP_CONFIG_FIRMWARE_VERSION_MINOR, APP_CONFIG_FIRMWARE_VERSION_PATCH, COMPUTE_BUILD_DAY,
                          COMPUTE_BUILD_MONTH, COMPUTE_BUILD_YEAR);
    lv_obj_align(label_display_version, LV_ALIGN_BOTTOM_LEFT, 0, -18);

    VIEW_ADD_WATCHED_VARIABLE(&model->system.removable_drive_state, 0);

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

                        case BTN_DRIVE_ID:
                            msg.stack_msg = PMAN_STACK_MSG_PUSH_PAGE(&page_drive);
                            break;

                        case BTN_PARMAC_ID:
                            msg.stack_msg = PMAN_STACK_MSG_PUSH_PAGE(&page_parmac);
                            break;

                        case BTN_PROGRAMS_ID:
                            msg.stack_msg = PMAN_STACK_MSG_PUSH_PAGE(&page_programs);
                            break;

                        case BTN_ADVANCED_ID:
                            if (model->prog.parmac.livello_accesso == CODICE_LVL_COSTRUTTORE) {
                                msg.stack_msg = PMAN_STACK_MSG_PUSH_PAGE(&page_advanced);
                            } else {
                                pman_stack_msg_t         pw_msg = PMAN_STACK_MSG_SWAP(&page_advanced);
                                password_page_options_t *opts   = view_common_default_password_page_options(
                                    pw_msg, (const char *)APP_CONFIG_PASSWORD);
                                msg.stack_msg = PMAN_STACK_MSG_PUSH_PAGE_EXTRA(&page_password, opts);
                            }
                            break;

                        case BTN_DATETIME_ID:
                            msg.stack_msg = PMAN_STACK_MSG_PUSH_PAGE_EXTRA(&page_datetime, (void *)(uintptr_t)0);
                            break;

                        case BTN_PROGRAMMED_WASH_ID:
                            msg.stack_msg = PMAN_STACK_MSG_PUSH_PAGE(&page_pick_program);
                            break;

                        case BTN_PASSWORD_ID: {
                            pdata->keyboard_options.max_length = strlen(APP_CONFIG_BACKDOOR_PASSWORD);
                            pdata->keyboard_options.numeric    = 1;
                            pdata->keyboard_options.string     = model->prog.password;

                            msg.stack_msg = PMAN_STACK_MSG_PUSH_PAGE_EXTRA(&page_numpad, &pdata->keyboard_options);
                            break;
                        }

                        case BTN_STATS_ID:
                            msg.stack_msg = PMAN_STACK_MSG_PUSH_PAGE(&page_statistics);
                            break;

                        case BTN_EVENTS_ID:
                            msg.stack_msg = PMAN_STACK_MSG_PUSH_PAGE(&page_events);
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
    if (model->system.removable_drive_state != REMOVABLE_DRIVE_STATE_MOUNTED) {
        lv_obj_add_state(pdata->btn_drive, LV_STATE_DISABLED);
    } else {
        lv_obj_remove_state(pdata->btn_drive, LV_STATE_DISABLED);
    }
}


const pman_page_t page_menu = {
    .create        = create_page,
    .destroy       = pman_destroy_all,
    .open          = open_page,
    .close         = pman_close_all,
    .process_event = page_event,
};
