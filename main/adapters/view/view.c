#include <stdlib.h>
#include "page_manager.h"
#include "config/app_config.h"
#include "view.h"
#include "services/timestamp.h"
#include "theme/style.h"
#include "theme/theme.h"
#include "esp_log.h"
#include "watcher.h"
#include "common.h"
#include "intl/intl.h"


#define TECHVIEW_HEADER_HEIGHT 36
#define TECHVIEW_WINDOW_SIZE   200


static const char *TAG = "View";

static struct {
    pman_t                      pman;
    lv_display_t               *display;
    watcher_t                   watcher;
    communication_error_popup_t popup_communication_error;
    view_protocol_t             protocol;
    uint16_t                    communication_attempts;
    timestamp_t                 last_communication_attempt;
    lv_obj_t                   *win_tech_view;
} state = {
    .pman                       = {},
    .display                    = NULL,
    .watcher                    = {},
    .popup_communication_error  = {},
    .protocol                   = {},
    .communication_attempts     = 0,
    .last_communication_attempt = 0,
};


static void    clear_watcher(pman_handle_t handle);
static uint8_t event_callback(pman_handle_t handle, pman_event_t event);
static void    watcher_cb(void *old_value, const void *memory, uint16_t size, void *user_ptr, void *arg);
static void    retry_communication_callback(lv_event_t *event);
static void    ignore_communication_error_callback(lv_event_t *event);
static void    drag_event_handler(lv_event_t *e);
static void    window_hide_callback(lv_event_t *e);
static void    window_close_callback(lv_event_t *e);


void view_init(model_t *model, pman_user_msg_cb_t controller_cb, lv_display_flush_cb_t flush_cb,
               lv_indev_read_cb_t read_cb, view_protocol_t protocol) {
    (void)TAG;
    lv_init();

    state.protocol = protocol;

    WATCHER_INIT_STD(&state.watcher, NULL);

#ifdef BUILD_CONFIG_SIMULATED_APP
    (void)flush_cb;
    (void)read_cb;

    state.display =
        lv_sdl_window_create(BUILD_CONFIG_DISPLAY_HORIZONTAL_RESOLUTION, BUILD_CONFIG_DISPLAY_VERTICAL_RESOLUTION);
    lv_indev_t *touch_indev = lv_sdl_mouse_create();

#else
    state.display =
        lv_display_create(BUILD_CONFIG_DISPLAY_HORIZONTAL_RESOLUTION, BUILD_CONFIG_DISPLAY_VERTICAL_RESOLUTION);

    /*Static or global buffer(s). The second buffer is optional*/
    static lv_color_t buf_1[VIEW_LVGL_BUFFER_SIZE] = {0};

    /*Initialize `disp_buf` with the buffer(s). With only one buffer use NULL instead buf_2 */
    lv_display_set_buffers(state.display, buf_1, NULL, VIEW_LVGL_BUFFER_SIZE, LV_DISPLAY_RENDER_MODE_PARTIAL);
    lv_display_set_flush_cb(state.display, flush_cb);

    lv_indev_t *touch_indev = lv_indev_create();
    lv_indev_set_type(touch_indev, LV_INDEV_TYPE_POINTER);
    lv_indev_set_read_cb(touch_indev, read_cb);

#endif

    style_init();
    theme_init(state.display);

    lv_obj_t *win = lv_win_create(lv_layer_top());
    lv_obj_set_style_text_font(win, STYLE_FONT_SMALL, LV_STATE_DEFAULT);
    lv_obj_set_size(win, TECHVIEW_WINDOW_SIZE, TECHVIEW_WINDOW_SIZE);

    lv_obj_t *header = lv_win_get_header(win);
    lv_obj_set_style_pad_gap(header, 8, LV_STATE_DEFAULT);
    lv_obj_set_style_pad_hor(header, 8, LV_STATE_DEFAULT);
    lv_obj_add_event_cb(header, drag_event_handler, LV_EVENT_PRESSING, win);
    lv_obj_set_height(header, TECHVIEW_HEADER_HEIGHT);

    lv_win_add_title(win, "Techview");
    lv_obj_add_event_cb(lv_win_add_button(win, LV_SYMBOL_DOWN, 32), window_hide_callback, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(lv_win_add_button(win, LV_SYMBOL_CLOSE, 32), window_close_callback, LV_EVENT_CLICKED, model);

    lv_obj_t *cont = lv_win_get_content(win); /*Content can be added here*/
    lv_obj_set_style_text_font(cont, STYLE_FONT_SMALL, LV_STATE_DEFAULT);
    lv_label_create(cont);
    lv_obj_set_style_bg_opa(cont, LV_OPA_70, LV_STATE_DEFAULT);

    state.win_tech_view = win;

    state.communication_attempts     = 0;
    state.last_communication_attempt = timestamp_get();
    state.popup_communication_error  = view_common_communication_error_popup(lv_layer_top());

    lv_obj_add_event_cb(state.popup_communication_error.btn_retry, retry_communication_callback, LV_EVENT_CLICKED,
                        &state.pman);
    lv_obj_add_event_cb(state.popup_communication_error.btn_disable, ignore_communication_error_callback,
                        LV_EVENT_CLICKED, &state.pman);
    view_common_set_hidden(state.popup_communication_error.blanket, 1);

    pman_init(&state.pman, (void *)model, touch_indev, controller_cb, clear_watcher, event_callback);
}


void view_manage(model_t *model) {
    // Reset the attempt counter every minute
    if (state.communication_attempts > 0 && timestamp_is_expired(state.last_communication_attempt, 60000UL)) {
        state.communication_attempts = 0;
    }

    if (model->system.errore_comunicazione && model->system.comunicazione_abilitata) {
        // After 5 attempts allow the popup to disappear without retrying
        if (state.communication_attempts >= 5 || model->prog.parmac.livello_accesso == CODICE_LVL_COSTRUTTORE) {
            view_common_set_hidden(state.popup_communication_error.btn_disable, 0);
            lv_obj_align(state.popup_communication_error.btn_disable, LV_ALIGN_BOTTOM_RIGHT, 0, 0);
            lv_obj_align(state.popup_communication_error.btn_retry, LV_ALIGN_BOTTOM_LEFT, 0, 0);
        } else {
            view_common_set_hidden(state.popup_communication_error.btn_disable, 1);
            lv_obj_align(state.popup_communication_error.btn_retry, LV_ALIGN_BOTTOM_MID, 0, 0);
        }

        lv_label_set_text(state.popup_communication_error.lbl_msg,
                          view_intl_get_string(model, STRINGS_ERRORE_DI_COMUNICAZIONE));
        lv_label_set_text(state.popup_communication_error.lbl_retry, view_intl_get_string(model, STRINGS_RIPROVA));
        lv_label_set_text(state.popup_communication_error.lbl_disable, view_intl_get_string(model, STRINGS_DISABILITA));

        view_common_set_hidden(state.popup_communication_error.blanket, 0);
    } else {
        view_common_set_hidden(state.popup_communication_error.blanket, 1);
    }

    view_common_set_hidden(state.win_tech_view, !model->run.tech_view);

    lv_obj_t *label = lv_obj_get_child(lv_win_get_content(state.win_tech_view), 0);

    lv_label_set_text_fmt(label, "temp: %i/%i\npress: %i\ndrum: %i\naquaplus:\n%3i s, %4i ml/s\n%i%%",
                          model->run.macchina.temperatura, model->run.temperature_setpoint, model->run.macchina.livello,
                          model->run.macchina.velocita_rilevata, model->run.macchina.aquaplus.erogation_seconds,
                          model->run.macchina.aquaplus.milliliters_per_second, model->run.macchina.aquaplus.percentage);

    watcher_watch(&state.watcher, timestamp_get());
}


void view_change_page(const pman_page_t *page) {
    pman_change_page(&state.pman, *page);
}


void view_display_flush_ready(void) {
    if (state.display) {
        lv_display_flush_ready(state.display);
    }
}


void view_register_object_default_callback(lv_obj_t *obj, int id) {
    view_register_object_default_callback_with_number(obj, id, 0);
}


mut_model_t *view_get_model(pman_handle_t handle) {
    return (mut_model_t *)pman_get_user_data(handle);
}


view_protocol_t *view_get_protocol(pman_handle_t handle) {
    (void)handle;
    return &state.protocol;
}


void view_event(view_event_t event) {
    pman_event(&state.pman, (pman_event_t){.tag = PMAN_EVENT_TAG_USER, .as = {.user = &event}});
}


void view_register_object_default_callback_with_number(lv_obj_t *obj, int id, int number) {
    view_object_data_t *data = malloc(sizeof(view_object_data_t));
    data->id                 = id;
    data->number             = number;
    lv_obj_set_user_data(obj, data);

    pman_unregister_obj_event(obj);
    pman_register_obj_event(&state.pman, obj, LV_EVENT_CLICKED);
    pman_register_obj_event(&state.pman, obj, LV_EVENT_VALUE_CHANGED);
    pman_register_obj_event(&state.pman, obj, LV_EVENT_RELEASED);
    pman_register_obj_event(&state.pman, obj, LV_EVENT_PRESSED);
    pman_register_obj_event(&state.pman, obj, LV_EVENT_PRESSING);
    pman_register_obj_event(&state.pman, obj, LV_EVENT_LONG_PRESSED);
    pman_register_obj_event(&state.pman, obj, LV_EVENT_LONG_PRESSED_REPEAT);
    pman_register_obj_event(&state.pman, obj, LV_EVENT_CANCEL);
    pman_register_obj_event(&state.pman, obj, LV_EVENT_READY);
    pman_set_obj_self_destruct(obj);
}


void view_add_watched_variable(void *ptr, size_t size, int code) {
    watcher_add_entry(&state.watcher, ptr, size, watcher_cb, (void *)(uintptr_t)code);
}


static uint8_t event_callback(pman_handle_t handle, pman_event_t event) {
    switch (event.tag) {
        case PMAN_EVENT_TAG_LVGL: {
            switch (event.as.lvgl->code) {
                case LV_EVENT_LONG_PRESSED:
                case LV_EVENT_CLICKED:
                    view_get_protocol(handle)->beep();
                    break;
                default:
                    break;
            }

            break;
        }

        default:
            break;
    }

    return 0;
}


static void window_close_callback(lv_event_t *e) {
    mut_model_t *model   = lv_event_get_user_data(e);
    model->run.tech_view = 0;
}


static void window_hide_callback(lv_event_t *e) {
    lv_obj_t *cont = lv_win_get_content(state.win_tech_view);
    if (lv_obj_has_flag(cont, LV_OBJ_FLAG_HIDDEN)) {
        lv_obj_remove_flag(cont, LV_OBJ_FLAG_HIDDEN);
        lv_obj_set_height(state.win_tech_view, TECHVIEW_WINDOW_SIZE);
    } else {
        lv_obj_add_flag(cont, LV_OBJ_FLAG_HIDDEN);
        lv_obj_set_height(state.win_tech_view, TECHVIEW_HEADER_HEIGHT);
    }
}


static void clear_watcher(pman_handle_t handle) {
    (void)handle;
    watcher_destroy(&state.watcher);
    WATCHER_INIT_STD(&state.watcher, NULL);
}


static void watcher_cb(void *old_value, const void *memory, uint16_t size, void *user_ptr, void *arg) {
    (void)old_value;
    (void)memory;
    (void)size;
    (void)user_ptr;
    view_event((view_event_t){.tag = VIEW_EVENT_TAG_PAGE_WATCHER, .as.page_watcher.code = (uintptr_t)arg});
}


static void retry_communication_callback(lv_event_t *event) {
    state.communication_attempts++;
    state.last_communication_attempt = timestamp_get();
    state.protocol.retry_communication(lv_event_get_user_data(event));
    view_common_set_hidden(state.popup_communication_error.blanket, 1);
}


static void ignore_communication_error_callback(lv_event_t *event) {
    mut_model_t *model                    = view_get_model(lv_event_get_user_data(event));
    model->system.comunicazione_abilitata = 0;
    view_common_set_hidden(state.popup_communication_error.blanket, 1);
}


static void drag_event_handler(lv_event_t *e) {
    lv_obj_t *win = lv_event_get_user_data(e);

    lv_indev_t *indev = lv_indev_active();
    if (indev == NULL)
        return;

    lv_point_t vect;
    lv_indev_get_vect(indev, &vect);

    int32_t x = lv_obj_get_x_aligned(win) + vect.x;
    int32_t y = lv_obj_get_y_aligned(win) + vect.y;
    lv_obj_set_pos(win, x, y);
}
