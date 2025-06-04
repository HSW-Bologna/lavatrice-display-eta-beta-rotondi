#ifndef VIEW_COMMON_H_INCLUDED
#define VIEW_COMMON_H_INCLUDED


#include <stdint.h>
#include "lvgl.h"
#include "view.h"


#define MENU_BUTTON_WIDTH LV_PCT(48);


typedef struct {
    char    *string;
    uint8_t  numeric;
    uint16_t max_length;
} keyboard_page_options_t;

typedef enum {
    VIEW_COMMON_PAGE_TEST_DRUM_TYPE_BACKWARD = 0,
    VIEW_COMMON_PAGE_TEST_DRUM_TYPE_FORWARD,
    VIEW_COMMON_PAGE_TEST_DRUM_TYPE_ACCELEROMETER,
} view_common_page_test_drum_type_t;


typedef struct {
    parametri_step_t *step;
    uint8_t           changed;
} step_modification_t;


typedef struct {
    const char      *password;
    pman_stack_msg_t msg;
} password_page_options_t;


typedef struct {
    lv_obj_t *blanket;

    lv_obj_t *lbl_msg;
    lv_obj_t *lbl_retry;
    lv_obj_t *lbl_disable;
    lv_obj_t *btn_retry;
    lv_obj_t *btn_disable;
} communication_error_popup_t;


typedef struct {
    lv_obj_t *blanket;

    lv_obj_t *lbl_description;

    lv_obj_t *btn_cancel;
    lv_obj_t *btn_ok;
} popup_t;


typedef struct {
    lv_obj_t *obj_title;
    lv_obj_t *label_title;
    lv_obj_t *button_back;
    lv_obj_t *button_next;
} view_title_t;


void                        view_common_set_hidden(lv_obj_t *obj, uint8_t hidden);
password_page_options_t    *view_common_default_password_page_options(pman_stack_msg_t msg, const char *password);
view_title_t                view_common_create_title(lv_obj_t *root, const char *text, int back_id, int next_id);
void                        view_common_image_set_src(lv_obj_t *img, const lv_image_dsc_t *img_dsc);
communication_error_popup_t view_common_communication_error_popup(lv_obj_t *parent);
const char                 *view_common_alarm_description(model_t *pmodel);
const char                 *view_require_payment_string(model_t *pmodel, int language);
void                        view_common_set_disabled(lv_obj_t *obj, uint8_t disabled);
const char                 *view_common_step2str(model_t *model, uint16_t step);
const char                 *view_common_pedantic_string(model_t *pmodel);
popup_t                     view_common_alarm_popup_create(lv_obj_t *parent, int id);
popup_t                     view_common_popup_create(lv_obj_t *parent, const char *text, int ok_id, int cancel_id);
void               view_common_alarm_popup_update(popup_t *alarm_popup, uint16_t language, uint16_t alarm_code);
const pman_page_t *view_common_main_page(model_t *model);
const pman_page_t *view_common_choice_page(model_t *model);
const pman_page_t *view_common_washing_page(model_t *model);
lv_obj_t          *view_common_icon_button_create(lv_obj_t *parent, const char *icon, int id);
lv_obj_t          *view_common_alarm_led_create(lv_obj_t *parent, lv_obj_t **led, const char *text);


#endif
