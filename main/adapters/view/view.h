#ifndef VIEW_H_INCLUDED
#define VIEW_H_INCLUDED


#include "model/model.h"
#include "page_manager.h"


#define VIEW_LVGL_BUFFER_SIZE                    (BUILD_CONFIG_DISPLAY_HORIZONTAL_RESOLUTION * 10)
#define VIEW_ADD_WATCHED_VARIABLE(ptr, code)     view_add_watched_variable((void *)ptr, sizeof(*ptr), code)
#define VIEW_ADD_WATCHED_ARRAY(ptr, items, code) view_add_watched_variable((void *)ptr, sizeof(*ptr) * items, code)


typedef enum {
    VIEW_CONTROLLER_MESSAGE_TAG_NOTHING = 0,
} view_controller_msg_tag_t;

typedef struct {
    view_controller_msg_tag_t tag;
    union {
    } as;
} view_controller_msg_t;


typedef struct {
    uint16_t id;
    uint16_t number;
} view_object_data_t;


typedef enum {
    VIEW_EVENT_TAG_PAGE_WATCHER,
} view_event_tag_t;


typedef struct {
    view_event_tag_t tag;
    union {
        struct {
            int code;
        } page_watcher;
    } as;
} view_event_t;


typedef struct {
    void (*retry_communication)(pman_handle_t handle);
    void (*set_output)(pman_handle_t handle, uint16_t output, uint8_t level);
    void (*clear_outputs)(pman_handle_t handle);
    void (*set_test_mode)(pman_handle_t handle, uint8_t test_mode);
    void (*pressure_calibration)(pman_handle_t handle);
    void (*import_configuration)(pman_handle_t handle, const char *name);
    int (*load_program)(pman_handle_t handle, size_t number);
    void (*save_parmac)(pman_handle_t handle);
    void (*start_program)(pman_handle_t handle);
    void (*stop_program)(pman_handle_t handle);
    void (*pause_program)(pman_handle_t handle);
} view_protocol_t;


void view_init(model_t *p_model, pman_user_msg_cb_t controller_cb, lv_display_flush_cb_t flush_cb,
               lv_indev_read_cb_t read_cb, view_protocol_t protocol);

void             view_change_page(const pman_page_t *page);
void             view_display_flush_ready(void);
void             view_register_object_default_callback(lv_obj_t *obj, int id);
void             view_register_object_default_callback_with_number(lv_obj_t *obj, int id, int number);
mut_model_t     *view_get_model(pman_handle_t handle);
void             view_add_watched_variable(void *ptr, size_t size, int code);
void             view_manage(model_t *model);
void             view_event(view_event_t event);
view_protocol_t *view_get_protocol(pman_handle_t handle);


extern const pman_page_t page_main, page_password, page_menu, page_test_inputs, page_test_outputs, page_test_drum,
    page_test_level, page_test_temperature, page_test_coins_digital, page_test_coins_mechanical, page_test_led,
    page_test_lock, page_archiving, page_drive, page_import_configuration, page_choice, page_parmac, page_washing;


#endif