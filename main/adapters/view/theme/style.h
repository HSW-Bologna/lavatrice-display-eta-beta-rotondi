#ifndef STYLE_H_INCLUDED
#define STYLE_H_INCLUDED

#include "lvgl.h"


#define STYLE_FONT_SMALL  (&lv_font_montserrat_16)
#define STYLE_FONT_MEDIUM (&lv_font_montserrat_20)
#define STYLE_FONT_BIG    (&lv_font_montserrat_32)
#define STYLE_FONT_HUGE   (&lv_font_montserrat_48)

#define VIEW_STYLE_PRIMARY          (lv_color_darken(((lv_color_t)lv_color_make(0x9e, 0xc1, 0xe7)), LV_OPA_30))
#define VIEW_STYLE_SECONDARY        ((lv_color_t)LV_COLOR_MAKE(0x0, 0x0, 0x0))
#define VIEW_STYLE_COLOR_RED        ((lv_color_t)LV_COLOR_MAKE(0xFF, 0x0, 0x0))
#define VIEW_STYLE_COLOR_BLACK      ((lv_color_t)LV_COLOR_MAKE(0x0, 0x0, 0x0))
#define VIEW_STYLE_COLOR_WHITE      ((lv_color_t)LV_COLOR_MAKE(0xFF, 0xFF, 0xFF))
#define VIEW_STYLE_COLOR_GREEN      ((lv_color_t)LV_COLOR_MAKE(0xA3, 0xA7, 0x39))
#define VIEW_STYLE_COLOR_START      ((lv_color_t)LV_COLOR_MAKE(0x2F, 0xBA, 0x74))
#define VIEW_STYLE_COLOR_STOP       VIEW_STYLE_COLOR_RED
#define VIEW_STYLE_COLOR_PAUSE      ((lv_color_t)LV_COLOR_MAKE(0xFF, 0xB3, 0x66))
#define VIEW_STYLE_COLOR_BACKGROUND ((lv_color_t)LV_COLOR_MAKE(0x6B, 0x6B, 0x6B))
#define STYLE_PRIMARY_DARKER        ((lv_color_t)LV_COLOR_MAKE(0x38, 0x6A, 0x80))


extern const lv_style_t style_transparent_cont;
extern const lv_style_t style_padless_cont;
extern const lv_style_t style_borderless_cont;
extern const lv_style_t style_black_border;
extern const lv_style_t style_config_btn;
extern const lv_style_t style_white_icon;
extern const lv_style_t style_black_icon;
extern const lv_style_t style_icon_button;
extern lv_style_t       style_work_button;
extern lv_style_t       style_tall_button;


void style_init(void);


#endif
