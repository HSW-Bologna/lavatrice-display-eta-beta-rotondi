#include "theme.h"
#include "style.h"


static void theme_apply_cb(lv_theme_t *th, lv_obj_t *obj);


void theme_init(lv_display_t *disp) {
    // lv_theme_t *th = lv_theme_basic_init(disp);
    lv_theme_t *th =
        lv_theme_default_init(disp, STYLE_PRIMARY_DARKER, lv_color_make(20, 20, 60), 1, &lv_font_montserrat_32);

    /*Initialize the new theme from the current theme*/
    static lv_theme_t th_new;
    th_new = *th;

    /*Set the parent theme and the style apply callback for the new theme*/
    lv_theme_set_parent(&th_new, th);
    lv_theme_set_apply_cb(&th_new, theme_apply_cb);

    /*Assign the new theme the the current display*/
    lv_disp_set_theme(disp, &th_new);
}


static void theme_apply_cb(lv_theme_t *th, lv_obj_t *obj) {
    (void)th;

    if (lv_obj_get_class(obj) == &lv_button_class) {
        lv_obj_set_style_radius(obj, 48, LV_STATE_DEFAULT);
    } else if (lv_obj_get_class(obj) == &lv_obj_class) {
        lv_obj_set_style_bg_color(obj, lv_color_make(0x43, 0x49, 0x4C), LV_STATE_DEFAULT);
    }
}
