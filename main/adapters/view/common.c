#include "view.h"
#include "common.h"
#include "theme/style.h"


void view_common_set_hidden(lv_obj_t *obj, uint8_t hidden) {
    if (hidden) {
        lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
    } else {
        lv_obj_clear_flag(obj, LV_OBJ_FLAG_HIDDEN);
    }
}


password_page_options_t *view_common_default_password_page_options(pman_stack_msg_t msg, const char *password) {
    password_page_options_t *fence = (password_page_options_t *)lv_malloc(sizeof(password_page_options_t));
    assert(fence != NULL);
    fence->password = password;
    fence->msg      = msg;
    return fence;
}


lv_obj_t *view_common_create_title(lv_obj_t *root, const char *text, int back_id, int prev_id, int next_id) {
    LV_IMG_DECLARE(img_door);

    lv_obj_t *btn, *lbl, *cont;

    cont = lv_obj_create(root);
    lv_obj_add_style(cont, (lv_style_t *)&style_padless_cont, LV_STATE_DEFAULT);
    lv_obj_set_size(cont, LV_HOR_RES, 56);

    btn = lv_btn_create(cont);
    lv_obj_add_style(btn, (lv_style_t *)&style_config_btn, LV_STATE_DEFAULT);
    lv_obj_set_size(btn, 48, 48);
    lv_obj_t *img = lv_image_create(btn);
    lv_image_set_src(img, &img_door);
    lv_obj_center(img);
    lv_obj_align(btn, LV_ALIGN_LEFT_MID, 4, 0);
    view_register_object_default_callback(btn, back_id);

    if (prev_id >= 0) {
        btn = lv_btn_create(cont);
        lv_obj_add_style(btn, (lv_style_t *)&style_config_btn, LV_STATE_DEFAULT);
        lv_obj_set_size(btn, 52, 52);
        lbl = lv_label_create(btn);
        lv_obj_set_style_text_font(lbl, STYLE_FONT_BIG, LV_STATE_DEFAULT);
        lv_label_set_text(lbl, LV_SYMBOL_LEFT);
        lv_obj_center(lbl);
        lv_obj_align(btn, LV_ALIGN_RIGHT_MID, -64, 0);
        view_register_object_default_callback(btn, prev_id);
    }

    if (next_id >= 0) {
        btn = lv_btn_create(cont);
        lv_obj_add_style(btn, (lv_style_t *)&style_config_btn, LV_STATE_DEFAULT);
        lv_obj_set_size(btn, 52, 52);
        lbl = lv_label_create(btn);
        lv_obj_set_style_text_font(lbl, STYLE_FONT_BIG, LV_STATE_DEFAULT);
        lv_label_set_text(lbl, LV_SYMBOL_RIGHT);
        lv_obj_center(lbl);
        lv_obj_align(btn, LV_ALIGN_RIGHT_MID, -4, 0);
        view_register_object_default_callback(btn, next_id);
    }

    lbl = lv_label_create(cont);
    lv_label_set_long_mode(lbl, LV_LABEL_LONG_WRAP);
    lv_obj_set_style_text_align(lbl, LV_TEXT_ALIGN_CENTER, LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(lbl, STYLE_FONT_MEDIUM, LV_STATE_DEFAULT);
    lv_obj_set_width(lbl, LV_HOR_RES - 128);
    lv_label_set_text(lbl, text);
    lv_obj_align(lbl, LV_ALIGN_CENTER, 0, 0);

    return cont;
}


void view_common_image_set_src(lv_obj_t *img, const lv_image_dsc_t *img_dsc) {
    if (lv_image_get_src(img) != img_dsc) {
        lv_image_set_src(img, img_dsc);
    }
}
