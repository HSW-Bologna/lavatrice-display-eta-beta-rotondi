
#if defined(LV_LVGL_H_INCLUDE_SIMPLE)
#include "lvgl.h"
#elif defined(LV_BUILD_TEST)
#include "../lvgl.h"
#else
#include "lvgl/lvgl.h"
#endif


#ifndef LV_ATTRIBUTE_MEM_ALIGN
#define LV_ATTRIBUTE_MEM_ALIGN
#endif

#ifndef LV_ATTRIBUTE_IMG_DUST
#define LV_ATTRIBUTE_IMG_DUST
#endif

static const
LV_ATTRIBUTE_MEM_ALIGN LV_ATTRIBUTE_LARGE_CONST LV_ATTRIBUTE_IMG_DUST
uint8_t img_lana_map[] = {

    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xe3,0x18,0x86,0x31,0x86,0x31,0x86,0x31,0xa6,0x31,0x86,0x31,0x86,0x31,0x85,0x31,0xa6,0x31,0xe7,0x39,0x28,0x42,0x49,0x4a,0x07,0x42,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xa6,0x31,0xa6,0x31,0xc6,0x39,0xa6,0x31,0xa6,0x31,0x86,0x31,0x07,0x42,0x49,0x4a,0x28,0x42,0x08,0x42,0xe7,0x41,0xe7,0x39,0x28,0x42,0x8a,0x52,0xcb,0x5a,0x2c,0x63,0x2c,0x6b,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xe7,0x39,0xe7,0x39,0xe7,0x39,0xe7,0x39,0x6d,0x6b,0x71,0x8c,0xd3,0x9c,0xd2,0x9c,0x92,0x94,0x30,0x84,0xcf,0x7b,0x10,0x84,0x30,0x84,0x10,0x84,0xae,0x73,0x0c,0x63,0x6d,0x6b,0xae,0x7b,0xef,0x7b,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xc6,0x39,0x08,0x42,0x08,0x42,0x08,0x42,0x50,0x8c,0xd7,0xbd,0x18,0xc6,0xb6,0xb5,0x55,0xad,0xf3,0x9c,0x92,0x94,0x30,0x84,0xcf,0x7b,0x10,0x84,0x92,0x94,0xf3,0x9c,0x55,0xad,0x75,0xad,0xb2,0x94,0xef,0x7b,0x51,0x8c,0xb2,0x94,0x71,0x8c,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x28,0x42,0x49,0x4a,0x28,0x42,0x4c,0x6b,0x18,0xc6,0xba,0xd6,0x59,0xce,0x18,0xc6,0xb6,0xb5,0x55,0xad,0xf3,0x9c,0xd2,0x9c,0x71,0x8c,0x51,0x8c,0x92,0x94,0xd3,0x9c,0x14,0xa5,0x55,0xad,0xd7,0xbd,0x38,0xc6,0x18,0xc6,0xf3,0x9c,0xf3,0x9c,0x34,0xa5,0xf3,0x9c,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xe7,0x39,0x69,0x4a,0x69,0x4a,0x51,0x8c,0x1c,0xe7,0x1c,0xe7,0xba,0xd6,0x59,0xce,0x18,0xc6,0x18,0xc6,0x79,0xce,0xba,0xd6,0xba,0xd6,0x59,0xce,0x18,0xc6,0xd7,0xbd,0xf7,0xbd,0xd7,0xbd,0xd7,0xbd,0xd7,0xbd,0x18,0xc6,0x9a,0xd6,0xdb,0xde,0xd7,0xbd,0x75,0xad,0xf7,0xbd,0x18,0xc6,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x69,0x4a,0x8a,0x52,0xb2,0x94,0x9e,0xf7,0x7d,0xef,0x1c,0xe7,0xba,0xd6,0x9a,0xd6,0xfb,0xde,0xba,0xd6,0xb6,0xb5,0xd3,0x9c,0x71,0x8c,0x10,0x84,0xcf,0x7b,0x10,0x84,0x92,0x94,0xf3,0x9c,0x55,0xad,0xb6,0xbd,0x18,0xc6,0x9a,0xd6,0xfb,0xde,0x3c,0xe7,0x79,0xce,0x38,0xc6,0x79,0xce,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x69,0x4a,0x8a,0x52,0xef,0x7b,0xbe,0xf7,0xbe,0xf7,0x7d,0xef,0x1c,0xe7,0x1c,0xe7,0xdb,0xde,0x55,0xad,0xd3,0x9c,0x14,0xa5,0xf3,0x9c,0x92,0x94,0x30,0x84,0xcf,0x7b,0x10,0x84,0x92,0x94,0xf3,0x9c,0x55,0xad,0xd7,0xbd,0x38,0xc6,0x9a,0xd6,0xfb,0xde,0x5d,0xef,0x9e,0xf7,0xba,0xd6,0xdb,0xde,0xba,0xd6,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x42,0x8a,0x52,0xeb,0x5a,0xfb,0xde,0xff,0xff,0xbe,0xf7,0x7d,0xef,0x3c,0xe7,0x18,0xc6,0x14,0xa5,0xd7,0xbd,0xb6,0xb5,0x55,0xad,0xf3,0x9c,0x92,0x94,0x30,0x84,0xcf,0x7b,0x10,0x84,0x92,0x94,0xf3,0x9c,0x55,0xad,0xd7,0xbd,0x38,0xc6,0x9a,0xd6,0xfb,0xde,0x5d,0xef,0x9e,0xf7,0xbe,0xf7,0x1c,0xe7,0xdb,0xde,0x79,0xce,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x69,0x4a,0x8a,0x52,0x75,0xad,0x9e,0xf7,0xff,0xff,0xbe,0xf7,0x3c,0xe7,0xb6,0xb5,0xd7,0xbd,0x59,0xce,0x18,0xc6,0xb6,0xb5,0x55,0xad,0xf3,0x9c,0x92,0x94,0x30,0x84,0xcf,0x7b,0x10,0x84,0x92,0x94,0xf3,0x9c,0x55,0xad,0xd7,0xbd,0x38,0xc6,0x9a,0xd6,0xfb,0xde,0x5d,0xef,0x9e,0xf7,0xff,0xff,0x7d,0xef,0xdb,0xde,0x9a,0xd6,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x28,0x42,0x69,0x4a,0x8e,0x73,0x1c,0xe7,0x9e,0xf7,0xdf,0xff,0x3c,0xe7,0xd7,0xbd,0x9a,0xd6,0xba,0xd6,0x59,0xce,0x18,0xc6,0xb6,0xb5,0x55,0xad,0xf3,0x9c,0x92,0x94,0x30,0x84,0xcf,0x7b,0x10,0x84,0x92,0x94,0xf3,0x9c,0x55,0xad,0xd7,0xbd,0x38,0xc6,0x9a,0xd6,0xfb,0xde,0x5d,0xef,0x9e,0xf7,0xff,0xff,0xbe,0xf7,0xfb,0xde,0x9a,0xd6,0x38,0xc6,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x07,0x42,0x49,0x4a,0x69,0x4a,0xf7,0xbd,0x3c,0xe7,0x9e,0xf7,0x7d,0xef,0x18,0xc6,0xfb,0xde,0x1c,0xe7,0xba,0xd6,0x59,0xce,0x18,0xc6,0xb6,0xb5,0x55,0xad,0xf3,0x9c,0x92,0x94,0x30,0x84,0xcf,0x7b,0x10,0x84,0x92,0x94,0xf3,0x9c,0x55,0xad,0xd7,0xbd,0x38,0xc6,0x9a,0xd6,0xfb,0xde,0x5d,0xef,0x9e,0xf7,0xff,0xff,0xbe,0xf7,0x3c,0xe7,0x9a,0xd6,0x38,0xc6,0xf7,0xbd,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x28,0x42,0x49,0x4a,0x51,0x8c,0xdb,0xde,0x3c,0xe7,0x7d,0xef,0x79,0xce,0x1c,0xe7,0x7d,0xef,0x1c,0xe7,0xba,0xd6,0x59,0xce,0x18,0xc6,0xb6,0xb5,0x55,0xad,0xf3,0x9c,0x92,0x94,0x30,0x84,0xcf,0x7b,0x51,0x8c,0x75,0xad,0x38,0xc6,0x79,0xce,0x79,0xce,0x79,0xce,0x79,0xce,0x9a,0xd6,0xfb,0xde,0x7d,0xef,0xdf,0xff,0xbe,0xf7,0x5d,0xef,0xba,0xd6,0x38,0xc6,0xf7,0xbd,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xe7,0x39,0x28,0x42,0xcb,0x5a,0x18,0xc6,0xdb,0xde,0x3c,0xe7,0x1c,0xe7,0xfb,0xde,0xbe,0xf7,0x7d,0xef,0x1c,0xe7,0xba,0xd6,0x59,0xce,0x18,0xc6,0xb6,0xb5,0x55,0xad,0xf3,0x9c,0x92,0x94,0xb2,0x94,0x18,0xc6,0x9a,0xd6,0xf7,0xbd,0x55,0xad,0x34,0xa5,0x96,0xb5,0xf7,0xbd,0x79,0xce,0xdb,0xde,0x1c,0xe7,0x5d,0xef,0x9e,0xf7,0xbe,0xf7,0x5d,0xef,0xdb,0xde,0x59,0xce,0xf7,0xbd,0x96,0xb5,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x42,0x28,0x42,0xf3,0x9c,0x79,0xce,0xdb,0xde,0x3c,0xe7,0x9a,0xd6,0xdf,0xff,0xbe,0xf7,0x7d,0xef,0x1c,0xe7,0xba,0xd6,0x59,0xce,0x18,0xc6,0xb6,0xb5,0x55,0xad,0xf3,0x9c,0xf7,0xbd,0xba,0xd6,0x75,0xad,0x51,0x8c,0x91,0x94,0xf3,0x9c,0x55,0xad,0xd7,0xbd,0x38,0xc6,0x9a,0xd6,0xfb,0xde,0x5d,0xef,0x9e,0xf7,0xff,0xff,0xbe,0xf7,0x5d,0xef,0xfb,0xde,0x79,0xce,0xf7,0xbd,0x96,0xb5,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xc7,0x39,0x08,0x42,0x4d,0x6b,0xf7,0xc5,0x79,0xce,0xdb,0xde,0xdb,0xde,0x3c,0xe7,0xff,0xff,0xbe,0xf7,0x7d,0xef,0x1c,0xe7,0xba,0xd6,0x59,0xce,0x18,0xc6,0xb6,0xb5,0x75,0xad,0x9a,0xd6,0x18,0xc6,0x71,0x8c,0xcf,0x7b,0x10,0x84,0x92,0x94,0xf3,0x9c,0x55,0xad,0xd7,0xbd,0x38,0xc6,0x9a,0xd6,0xfb,0xde,0x5d,0xef,0x9e,0xf7,0xff,0xff,0xbe,0xf7,0x5d,0xef,0xfb,0xde,0x79,0xd6,0xf7,0xbd,0x96,0xb5,0x55,0xad,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xa6,0x31,0xe7,0x39,0x28,0x42,0x34,0xa5,0x18,0xc6,0x79,0xce,0xdb,0xde,0xba,0xd6,0x9e,0xf7,0xff,0xff,0xbe,0xf7,0x7d,0xef,0x1c,0xe7,0xba,0xd6,0x59,0xce,0x18,0xc6,0xd7,0xbd,0xba,0xd6,0xb6,0xb5,0x71,0x8c,0x30,0x84,0xcf,0x7b,0x10,0x84,0x92,0x94,0xf3,0x9c,0x55,0xad,0xd7,0xbd,0x38,0xc6,0x9a,0xd6,0xfb,0xde,0x5d,0xef,0x9e,0xf7,0xff,0xff,0xbe,0xf7,0x5d,0xef,0xfb,0xde,0x9a,0xd6,0x18,0xc6,0x96,0xb5,0x34,0xad,0xf3,0x9c,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xc7,0x39,0xe7,0x39,0xcf,0x7b,0x96,0xb5,0x18,0xc6,0x79,0xce,0xdb,0xde,0x1c,0xe7,0x9e,0xf7,0xff,0xff,0xbe,0xf7,0x7d,0xef,0x1c,0xe7,0xba,0xd6,0x59,0xce,0x18,0xc6,0xba,0xd6,0x96,0xb5,0xd3,0x9c,0x71,0x8c,0x4d,0x6b,0xcf,0x7b,0x10,0x84,0x92,0x94,0xf3,0x9c,0x55,0xad,0xd7,0xbd,0x38,0xc6,0x9a,0xd6,0xfb,0xde,0x5d,0xef,0x9e,0xf7,0xff,0xff,0x9e,0xf7,0x3c,0xe7,0xfb,0xde,0x9a,0xd6,0x18,0xc6,0xb6,0xb5,0x34,0xad,0xf3,0x9c,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x86,0x31,0xc7,0x39,0x89,0x52,0x14,0xa5,0x96,0xb5,0x18,0xc6,0x59,0xce,0xdb,0xde,0x3c,0xe7,0x9e,0xf7,0xff,0xff,0xbe,0xf7,0x7d,0xef,0x1c,0xe7,0xba,0xd6,0x59,0xce,0x9a,0xd6,0xf7,0xbd,0x34,0xa5,0xd3,0x9c,0xeb,0x5a,0x0c,0x63,0xae,0x73,0x30,0x84,0x92,0x94,0xf3,0x9c,0x55,0xad,0xd7,0xbd,0x38,0xc6,0x9a,0xd6,0xfb,0xde,0x5d,0xef,0x9e,0xf7,0xff,0xff,0xbe,0xf7,0x5d,0xef,0xfb,0xde,0x9a,0xd6,0x18,0xc6,0xb6,0xb5,0x34,0xad,0xf3,0x9c,0xb2,0x94,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x65,0x29,0x86,0x31,0xc7,0x39,0x30,0x84,0x34,0xa5,0x96,0xb5,0x18,0xc6,0x59,0xce,0xdb,0xde,0x3c,0xe7,0x9e,0xf7,0xff,0xff,0xbe,0xf7,0x7d,0xef,0x1c,0xe7,0xba,0xd6,0x79,0xce,0x79,0xce,0x75,0xad,0x55,0xad,0xae,0x73,0x34,0xa5,0xf7,0xbd,0xb2,0x94,0xd3,0x9c,0x92,0x94,0x14,0xa5,0x55,0xad,0xd7,0xbd,0x38,0xc6,0x9a,0xd6,0xfb,0xde,0x5d,0xef,0x9e,0xf7,0xff,0xff,0xbe,0xf7,0x5d,0xef,0xfb,0xde,0xdb,0xde,0x18,0xc6,0xb6,0xb5,0x55,0xad,0xf3,0x9c,0x92,0x94,0x0f,0x84,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x65,0x29,0xa6,0x31,0xeb,0x5a,0xd3,0x9c,0x34,0xa5,0x96,0xb5,0x18,0xc6,0x59,0xce,0xdb,0xde,0x3c,0xe7,0x9e,0xf7,0xff,0xff,0xbe,0xf7,0x7d,0xef,0x1c,0xe7,0xba,0xd6,0x9a,0xd6,0xb6,0xbd,0xb6,0xb5,0x92,0x94,0x34,0xa5,0x1b,0xe7,0xd7,0xbd,0x14,0xa5,0x18,0xc6,0x75,0xb5,0x14,0xa5,0x96,0xb5,0xd7,0xbd,0x38,0xc6,0x9a,0xd6,0xfb,0xde,0x5d,0xef,0x9e,0xf7,0xff,0xff,0xbe,0xf7,0x5d,0xef,0xfb,0xde,0xba,0xd6,0xba,0xd6,0xb6,0xb5,0x55,0xad,0xf3,0x9c,0x91,0x94,0x30,0x84,0x00,0x00,0x00,0x00,
    0x00,0x00,0x45,0x29,0x65,0x29,0xc7,0x39,0x30,0x84,0xd3,0x9c,0x34,0xa5,0x96,0xb5,0x18,0xc6,0x59,0xce,0xdb,0xde,0x3c,0xe7,0x9e,0xf7,0xff,0xff,0xbe,0xf7,0x7d,0xef,0x1c,0xe7,0xba,0xd6,0x38,0xc6,0xf7,0xc5,0x96,0xb5,0x30,0x84,0x7d,0xef,0xfb,0xde,0x00,0x00,0x14,0xa5,0x38,0xc6,0xfb,0xde,0x34,0xa5,0xb6,0xb5,0xd7,0xbd,0x38,0xc6,0x9a,0xd6,0xfb,0xde,0x5d,0xef,0x9e,0xf7,0xff,0xff,0xbe,0xf7,0x5d,0xef,0xfb,0xde,0x9a,0xd6,0xdb,0xde,0x18,0xc6,0x55,0xad,0xd3,0x9c,0x92,0x94,0x30,0x84,0xef,0x7b,0x00,0x00,
    0x00,0x00,0x86,0x31,0xa6,0x31,0x0c,0x63,0x51,0x8c,0xd3,0x9c,0x34,0xa5,0x96,0xb5,0x18,0xc6,0x9a,0xd6,0x79,0xce,0x3c,0xe7,0x9e,0xf7,0xff,0xff,0xbe,0xf7,0x7d,0xef,0x1c,0xe7,0xba,0xd6,0x18,0xc6,0x18,0xc6,0x71,0x8c,0x59,0xce,0x9e,0xf7,0x00,0x00,0x00,0x00,0x00,0x00,0xf7,0xbd,0x3c,0xe7,0xf7,0xbd,0x55,0xad,0x38,0xc6,0x38,0xc6,0x9a,0xd6,0xfb,0xde,0x5d,0xef,0x9e,0xf7,0xff,0xff,0xbe,0xf7,0x5d,0xef,0xfb,0xde,0x9a,0xd6,0x59,0xce,0xba,0xd6,0x55,0xad,0xd3,0x9c,0x91,0x94,0x71,0x8c,0x51,0x8c,0x00,0x00,
    0x65,0x29,0x86,0x31,0xe7,0x39,0xcf,0x7b,0x51,0x8c,0xd3,0x9c,0x34,0xa5,0x96,0xb5,0x18,0xc6,0x9a,0xd6,0x38,0xc6,0x3c,0xe7,0x9e,0xf7,0xff,0xff,0xbe,0xf7,0x7d,0xef,0x1c,0xe7,0x79,0xce,0x59,0xce,0xf7,0xbd,0xae,0x7b,0x5d,0xef,0x3c,0xe7,0x00,0x00,0x00,0x00,0x00,0x00,0xb6,0xb5,0x9a,0xd6,0x59,0xce,0x75,0xad,0xf7,0xbd,0x59,0xce,0x9a,0xd6,0xfb,0xde,0x5d,0xef,0x9e,0xf7,0xff,0xff,0xbe,0xf7,0x5d,0xef,0xfb,0xde,0x9a,0xd6,0x18,0xc6,0xba,0xde,0xd7,0xbd,0xd3,0x9c,0x71,0x8c,0x92,0x94,0x51,0x8c,0x30,0x84,
    0x85,0x31,0xa6,0x31,0xaa,0x52,0xef,0x7b,0x51,0x8c,0xd3,0x9c,0x34,0xa5,0x96,0xb5,0x18,0xc6,0x79,0xce,0xba,0xd6,0x79,0xce,0x9e,0xf7,0xff,0xff,0xbe,0xf7,0x7d,0xef,0x1c,0xe7,0x79,0xce,0x59,0xce,0x75,0xad,0xd3,0x9c,0x5d,0xef,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xf7,0xbd,0x18,0xc6,0x75,0xad,0xb6,0xb5,0x9a,0xd6,0x9a,0xd6,0xfb,0xde,0x5d,0xef,0x9e,0xf7,0xff,0xff,0xbe,0xf7,0x5d,0xef,0xfb,0xde,0x9a,0xd6,0x18,0xc6,0x79,0xce,0x38,0xc6,0xd3,0x9c,0x71,0x8c,0x71,0x8c,0x71,0x8c,0x51,0x8c,
    0x86,0x31,0xa6,0x39,0x2c,0x63,0xef,0x7b,0x51,0x8c,0xd3,0x9c,0x34,0xa5,0x96,0xb5,0x18,0xc6,0x79,0xce,0xfb,0xde,0x59,0xce,0x3c,0xe7,0xff,0xff,0xbe,0xf7,0x7d,0xef,0x1c,0xe7,0x9a,0xd6,0x59,0xce,0xb2,0x94,0xb6,0xb5,0x1c,0xe7,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x34,0xa5,0x96,0xb5,0x75,0xad,0xb6,0xb5,0x59,0xce,0x9a,0xd6,0xfb,0xde,0x5d,0xef,0x9e,0xf7,0xff,0xff,0xbe,0xf7,0x5d,0xef,0xfb,0xde,0x9a,0xd6,0x18,0xc6,0x38,0xc6,0x79,0xce,0xf3,0x9c,0x71,0x8c,0x30,0x84,0xb2,0x94,0x71,0x8c,
    0xa6,0x39,0xe7,0x39,0x4d,0x6b,0x30,0x84,0x51,0x8c,0xd3,0x9c,0x34,0xa5,0x96,0xb5,0x18,0xc6,0x79,0xce,0xdb,0xde,0x1c,0xe7,0x18,0xc6,0x9e,0xf7,0xbe,0xf7,0x7d,0xef,0x1c,0xe7,0xba,0xd6,0x59,0xce,0xcf,0x7b,0xb6,0xb5,0xd3,0x9c,0xeb,0x5a,0x00,0x00,0x00,0x00,0x00,0x00,0x49,0x4a,0x0c,0x63,0xb2,0x94,0x75,0xad,0xb6,0xbd,0x38,0xc6,0xba,0xd6,0xfb,0xde,0x5d,0xef,0x9e,0xf7,0xff,0xff,0xbe,0xf7,0x5d,0xef,0xfb,0xde,0x9a,0xd6,0x18,0xc6,0xf7,0xbd,0xba,0xd6,0xf3,0x9c,0x71,0x8c,0x10,0x84,0xb2,0x9c,0x92,0x94,
    0xc7,0x39,0x28,0x42,0x6d,0x6b,0xd3,0x9c,0x51,0x8c,0xd3,0x9c,0x34,0xa5,0x96,0xb5,0x18,0xc6,0x79,0xce,0xdb,0xde,0x3c,0xe7,0x3c,0xe7,0x18,0xc6,0x1c,0xe7,0x7d,0xef,0x1c,0xe7,0xba,0xd6,0x59,0xce,0x30,0x84,0xb6,0xb5,0x55,0xad,0x71,0x8c,0xeb,0x62,0x08,0x42,0x49,0x4a,0xae,0x73,0x14,0xa5,0xf7,0xbd,0xd7,0xbd,0xd7,0xbd,0xd7,0xbd,0xdb,0xde,0xfb,0xde,0x5d,0xef,0x9e,0xf7,0xff,0xff,0xbe,0xf7,0x5d,0xef,0xfb,0xde,0x9a,0xd6,0x18,0xc6,0xd7,0xbd,0x9a,0xd6,0xf3,0x9c,0x71,0x8c,0xef,0x7b,0xd3,0x9c,0xd3,0x9c,
    0xe7,0x39,0x69,0x4a,0x6d,0x6b,0x34,0xa5,0x51,0x8c,0xd3,0x9c,0x34,0xa5,0x96,0xb5,0x18,0xc6,0x79,0xce,0xdb,0xde,0x3c,0xe7,0x9e,0xf7,0x7d,0xef,0x59,0xce,0x59,0xce,0xfb,0xde,0xba,0xd6,0x59,0xce,0xf7,0xbd,0x38,0xc6,0xdb,0xde,0x1c,0xe7,0x79,0xce,0xd7,0xbd,0x18,0xc6,0xdb,0xde,0xdb,0xde,0x38,0xc6,0xb6,0xb5,0xd7,0xbd,0xb6,0xb5,0xfb,0xde,0xfb,0xde,0x5d,0xef,0x9e,0xf7,0xff,0xff,0xbe,0xf7,0x5d,0xef,0xfb,0xde,0x9a,0xd6,0x18,0xc6,0xd7,0xbd,0x9a,0xd6,0xf3,0x9c,0x71,0x8c,0xef,0x7b,0xf3,0x9c,0xd3,0x9c,
    0x08,0x42,0x69,0x4a,0x6d,0x6b,0x14,0xa5,0xb2,0x94,0xd3,0x9c,0x34,0xa5,0x96,0xb5,0x18,0xc6,0x79,0xce,0xdb,0xde,0x3c,0xe7,0x9e,0xf7,0xff,0xff,0x9e,0xf7,0xfb,0xde,0x18,0xc6,0x18,0xc6,0x38,0xce,0x18,0xc6,0xb6,0xb5,0x55,0xad,0xf3,0x9c,0x91,0x94,0x10,0x84,0xcf,0x7b,0x10,0x84,0x92,0x94,0xf3,0x9c,0x55,0xad,0xd7,0xbd,0xb6,0xb5,0xfb,0xde,0xfb,0xde,0x5d,0xef,0x9e,0xf7,0xff,0xff,0xbe,0xf7,0x5d,0xef,0xfb,0xde,0x9a,0xd6,0x18,0xc6,0xd7,0xbd,0x9a,0xd6,0xb2,0x94,0x71,0x8c,0xef,0x7b,0x34,0xa5,0x14,0xa5,
    0x28,0x42,0x8a,0x52,0x6d,0x6b,0x92,0x94,0x75,0xad,0xd3,0x9c,0x34,0xa5,0x96,0xb5,0x18,0xc6,0x79,0xce,0xdb,0xde,0x3c,0xe7,0x9e,0xf7,0xff,0xff,0xbe,0xf7,0x7d,0xef,0x1c,0xe7,0xdb,0xde,0x59,0xce,0xf7,0xbd,0x96,0xb5,0x55,0xad,0xf3,0x9c,0x91,0x94,0x10,0x84,0xef,0x7b,0x51,0x8c,0x92,0x94,0xf3,0x9c,0x55,0xad,0xd7,0xbd,0x96,0xb5,0xfb,0xde,0xfb,0xde,0x5d,0xef,0x9e,0xf7,0xff,0xff,0xbe,0xf7,0x5d,0xef,0xfb,0xde,0x9a,0xd6,0x18,0xc6,0x18,0xc6,0x59,0xce,0xb2,0x94,0x71,0x8c,0xef,0x7b,0x34,0xad,0x34,0xad,
    0x49,0x4a,0x8a,0x52,0x6d,0x6b,0xef,0x7b,0xf7,0xbd,0xd3,0x9c,0x34,0xa5,0x96,0xb5,0x18,0xc6,0x79,0xce,0xdb,0xde,0x3c,0xe7,0x9e,0xf7,0xff,0xff,0xbe,0xf7,0x7d,0xef,0x1c,0xe7,0xba,0xd6,0x79,0xce,0x79,0xce,0x59,0xce,0x18,0xc6,0xd7,0xbd,0x75,0xad,0xf3,0x9c,0x30,0x8c,0x30,0x84,0x92,0x94,0xf3,0x9c,0x55,0xad,0xd6,0xbd,0xb6,0xb5,0xfb,0xde,0xfb,0xde,0x5d,0xef,0x9e,0xf7,0xff,0xff,0xbe,0xf7,0x5d,0xef,0xfb,0xde,0x9a,0xd6,0x18,0xc6,0x59,0xce,0xb6,0xb5,0xd3,0x9c,0x71,0x8c,0x10,0x84,0x96,0xb5,0x75,0xad,
    0x69,0x4a,0xcb,0x5a,0x4d,0x6b,0xef,0x7b,0x75,0xad,0x96,0xb5,0x34,0xa5,0x96,0xb5,0x18,0xc6,0x79,0xce,0xdb,0xde,0x3c,0xe7,0x9e,0xf7,0xff,0xff,0xbe,0xf7,0x7d,0xef,0x1c,0xe7,0xba,0xd6,0x59,0xce,0x18,0xc6,0xb6,0xb5,0x55,0xad,0xf3,0x9c,0x92,0x94,0x30,0x84,0xcf,0x7b,0x10,0x84,0x92,0x94,0xf3,0x9c,0x55,0xad,0xb6,0xb5,0xf7,0xbd,0xdb,0xde,0xfb,0xde,0x5d,0xef,0x9e,0xf7,0xff,0xff,0xbe,0xf7,0x5d,0xef,0xfb,0xde,0x9a,0xd6,0x18,0xc6,0x9a,0xd6,0x14,0xa5,0xd3,0x9c,0x71,0x8c,0x92,0x94,0xd7,0xbd,0xb6,0xb5,
    0xaa,0x52,0xeb,0x5a,0x6d,0x6b,0xcf,0x7b,0x71,0x8c,0x59,0xce,0x55,0xad,0x96,0xb5,0x18,0xc6,0x79,0xce,0xdb,0xde,0x3c,0xe7,0x9e,0xf7,0xff,0xff,0xbe,0xf7,0x7d,0xef,0x1c,0xe7,0xba,0xd6,0x59,0xce,0x18,0xc6,0xb6,0xb5,0x55,0xad,0xf3,0x9c,0x92,0x94,0x30,0x84,0xcf,0x7b,0x10,0x84,0x92,0x94,0xf3,0x9c,0x55,0xad,0x75,0xad,0x9a,0xd6,0xba,0xd6,0xfb,0xde,0x5d,0xef,0x9e,0xf7,0xff,0xff,0xbe,0xf7,0x5d,0xef,0xfb,0xde,0x9a,0xd6,0x38,0xc6,0x59,0xce,0xf3,0x9c,0xd3,0x9c,0x71,0x8c,0x75,0xad,0xf7,0xbd,0xf7,0xbd,
    0xaa,0x52,0x0c,0x63,0x8e,0x73,0xef,0x7b,0x51,0x8c,0x34,0xa5,0x59,0xce,0xb6,0xb5,0x18,0xc6,0x79,0xce,0xdb,0xde,0x3c,0xe7,0x9e,0xf7,0xff,0xff,0xbe,0xf7,0x7d,0xef,0x1c,0xe7,0xba,0xd6,0x59,0xce,0x18,0xc6,0xb6,0xb5,0x55,0xad,0xf3,0x9c,0x92,0x94,0x30,0x84,0xcf,0x7b,0x10,0x84,0x92,0x94,0xf3,0x9c,0x55,0xad,0x75,0xad,0xdb,0xde,0x9a,0xd6,0xfb,0xde,0x5d,0xef,0x9e,0xf7,0xff,0xff,0xbe,0xf7,0x5d,0xef,0xfb,0xde,0x9a,0xd6,0x9a,0xd6,0x75,0xad,0x54,0xad,0xd3,0x9c,0x92,0x94,0x38,0xc6,0x18,0xc6,0x38,0xc6,
    0x00,0x00,0x6d,0x6b,0xae,0x7b,0x10,0x84,0x51,0x8c,0xb2,0x94,0xd7,0xbd,0x79,0xce,0x18,0xc6,0x79,0xce,0xdb,0xde,0x3c,0xe7,0x9e,0xf7,0xff,0xff,0xbe,0xf7,0x7d,0xef,0x1c,0xe7,0xba,0xd6,0x59,0xce,0x18,0xc6,0xb6,0xb5,0x55,0xad,0xf3,0x9c,0x92,0x94,0x30,0x84,0xcf,0x7b,0x10,0x84,0x92,0x94,0xf3,0x9c,0x54,0xad,0x38,0xc6,0x9a,0xd6,0x9a,0xd6,0xfb,0xde,0x5d,0xef,0x9e,0xf7,0xff,0xff,0xbe,0xf7,0x5d,0xef,0xfb,0xde,0x9a,0xd6,0x38,0xc6,0x75,0xad,0x55,0xad,0xd3,0x9c,0xb6,0xb5,0x79,0xce,0x59,0xce,0x00,0x00,
    0x00,0x00,0x6d,0x73,0xef,0x7b,0x71,0x8c,0x71,0x8c,0xd3,0x9c,0xf3,0x9c,0x18,0xc6,0xba,0xd6,0x79,0xce,0xdb,0xde,0x3c,0xe7,0x9e,0xf7,0xff,0xff,0xbe,0xf7,0x7d,0xef,0x1c,0xe7,0xba,0xd6,0x59,0xce,0x18,0xc6,0xb6,0xb5,0x55,0xad,0xf3,0x9c,0x92,0x94,0x30,0x84,0xcf,0x7b,0x10,0x84,0x92,0x94,0xf3,0x9c,0x55,0xad,0x9a,0xd6,0x38,0xc6,0x9a,0xd6,0xfb,0xde,0x5d,0xef,0x9e,0xf7,0xff,0xff,0xbe,0xf7,0x5d,0xef,0xfb,0xde,0x9a,0xd6,0xb6,0xb5,0xb6,0xb5,0x55,0xad,0x55,0xad,0x9a,0xd6,0x79,0xd6,0x9a,0xd6,0x00,0x00,
    0x00,0x00,0x00,0x00,0x30,0x84,0x92,0x94,0xf3,0x9c,0xd3,0x9c,0x34,0xa5,0x34,0xad,0xf7,0xbd,0xdb,0xde,0xfb,0xde,0x3c,0xe7,0x9e,0xf7,0xff,0xff,0xbe,0xf7,0x7d,0xef,0x1c,0xe7,0xba,0xd6,0x59,0xce,0x18,0xc6,0xb6,0xb5,0x55,0xad,0xf3,0x9c,0x92,0x94,0x30,0x84,0xcf,0x7b,0x10,0x84,0x92,0x94,0xf3,0x9c,0x59,0xce,0xf7,0xbd,0x38,0xc6,0x9a,0xd6,0xfb,0xde,0x5d,0xef,0x9e,0xf7,0xff,0xff,0xbe,0xf7,0x5d,0xef,0xdb,0xde,0x18,0xc6,0x18,0xc6,0xb6,0xb5,0x75,0xad,0xba,0xd6,0xba,0xd6,0xba,0xd6,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0xb2,0x94,0x14,0xa5,0x75,0xad,0x55,0xad,0x96,0xb5,0xb6,0xbd,0xb6,0xbd,0x9a,0xd6,0x1c,0xe7,0x7d,0xef,0xff,0xff,0xbe,0xf7,0x7d,0xef,0x1c,0xe7,0xba,0xd6,0x59,0xce,0x18,0xc6,0xb6,0xb5,0x55,0xad,0x54,0xad,0xb2,0x94,0x30,0x84,0xcf,0x7b,0x10,0x84,0xb2,0x94,0xf7,0xbd,0xb6,0xb5,0xd7,0xbd,0x38,0xc6,0x9a,0xd6,0xfb,0xde,0x5d,0xef,0x9e,0xf7,0xff,0xff,0xbe,0xf7,0xfb,0xde,0x9a,0xd6,0x79,0xce,0x18,0xc6,0xd7,0xbd,0xdb,0xde,0xfb,0xde,0xdb,0xde,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x34,0xa5,0x55,0xad,0xd6,0xbd,0xf7,0xc5,0xd7,0xbd,0x18,0xc6,0x79,0xce,0x38,0xc6,0x59,0xce,0xba,0xd6,0xfb,0xde,0x3c,0xe7,0x3c,0xe7,0x1c,0xe7,0xdb,0xde,0x9a,0xd6,0x79,0xce,0x79,0xce,0x18,0xc6,0x14,0xa5,0x92,0x94,0x30,0x84,0xcf,0x7b,0x51,0x8c,0x96,0xb5,0x34,0xa5,0x55,0xad,0xd7,0xbd,0x38,0xc6,0x9a,0xd6,0xfb,0xde,0x5d,0xef,0x9e,0xf7,0xdf,0xff,0x5d,0xef,0x1c,0xe7,0xfb,0xde,0x9a,0xd6,0x79,0xce,0x1c,0xe7,0x3c,0xe7,0x1c,0xe7,0x3c,0xe7,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xb6,0xb5,0xd7,0xbd,0x38,0xc6,0x9a,0xd6,0x9a,0xd6,0x9a,0xd6,0xdb,0xde,0x3c,0xe7,0x5d,0xef,0x5d,0xef,0x1c,0xe7,0x1c,0xe7,0xfb,0xde,0xdb,0xde,0xba,0xd6,0x59,0xce,0xb6,0xb5,0x55,0xad,0xf3,0x9c,0x92,0x94,0x30,0x84,0x10,0x84,0xb2,0x94,0x92,0x94,0xf3,0x9c,0x55,0xad,0xd7,0xbd,0x38,0xc6,0x9a,0xd6,0xfb,0xde,0x5d,0xef,0x9e,0xf7,0xdf,0xff,0xbe,0xf7,0x5d,0xef,0xfb,0xde,0x1c,0xe7,0x7d,0xef,0x5d,0xef,0x3c,0xe7,0x5d,0xef,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x59,0xce,0x9a,0xd6,0xfb,0xde,0x3c,0xe7,0x5d,0xef,0x5d,0xef,0x9e,0xf7,0xff,0xff,0xbe,0xf7,0x7d,0xef,0x1c,0xe7,0xba,0xd6,0x59,0xce,0x18,0xc6,0xb6,0xb5,0x55,0xad,0xf3,0x9c,0x92,0x94,0x30,0x84,0xcf,0x7b,0x10,0x84,0x92,0x94,0xf3,0x9c,0x55,0xad,0xd7,0xbd,0x38,0xc6,0x9a,0xd6,0xfb,0xde,0x5d,0xef,0x9e,0xf7,0xff,0xff,0xbe,0xf7,0xbe,0xf7,0xbe,0xf7,0x9e,0xf7,0x7d,0xef,0x7d,0xef,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xdb,0xde,0x1c,0xe7,0x5d,0xef,0x9e,0xf7,0xbe,0xf7,0xff,0xff,0xdf,0xff,0xbe,0xf7,0x9e,0xf7,0x7d,0xef,0x3c,0xe7,0x1c,0xe7,0xfb,0xde,0xba,0xd6,0x9a,0xd6,0x59,0xce,0x38,0xc6,0xf7,0xbd,0xd7,0xbd,0xf7,0xbd,0x38,0xc6,0x59,0xce,0x9a,0xd6,0xba,0xde,0xfb,0xde,0x1c,0xe7,0x5d,0xef,0x7d,0xef,0xbe,0xf7,0xdf,0xff,0xff,0xff,0xdf,0xff,0xbe,0xf7,0xbe,0xf7,0x9e,0xf7,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x7d,0xef,0x9e,0xf7,0xbe,0xf7,0xff,0xff,0xdf,0xff,0xbe,0xf7,0x9e,0xf7,0x7d,0xef,0x5d,0xef,0x3c,0xe7,0x1c,0xe7,0xfb,0xde,0xba,0xd6,0x9a,0xd6,0x79,0xce,0x38,0xc6,0x18,0xc6,0x38,0xc6,0x59,0xce,0x9a,0xd6,0xba,0xd6,0xfb,0xde,0x1c,0xe7,0x3c,0xe7,0x7d,0xef,0x9e,0xf7,0xbe,0xf7,0xdf,0xff,0xff,0xff,0xff,0xff,0xdf,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x18,0x5e,0x95,0xbc,0xd2,0xd5,0xd2,0xbd,0x97,0x61,0x1b,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x49,0xb4,0xf8,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xfa,0xb9,0x4e,0x04,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x32,0xc1,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xc7,0x39,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x02,0x76,0xf7,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xf8,0x80,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x07,0x99,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xa1,0x09,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x04,0xa0,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xa9,0x07,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x81,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x8d,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x40,0xfa,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xfb,0x4a,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x09,0xd3,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xd8,0x0d,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x7d,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x86,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x28,0xf4,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xf6,0x2e,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0xba,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xc2,0x04,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x5b,0xfe,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x64,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x13,0xe6,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xe9,0x17,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x9a,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xa3,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x3f,0xfa,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xfb,0x45,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x09,0xd0,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xd6,0x0b,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x75,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x7e,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x22,0xf3,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xf4,0x27,0x00,0x00,0x00,0x00,0x00,0x01,0xb5,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xec,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xbb,0x01,0x00,0x00,
    0x00,0x00,0x57,0xfd,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xce,0x1a,0xc4,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xfd,0x5f,0x00,0x00,0x00,0x11,0xe2,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xf4,0x2a,0x00,0x1c,0xee,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xe7,0x15,0x00,
    0x00,0x78,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x84,0x00,0x00,0x00,0x74,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x83,0x00,0x07,0xda,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xf1,0x14,0x00,0x00,0x00,0x0c,0xe5,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xe0,0x09,
    0x3a,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xa0,0x00,0x00,0x00,0x00,0x00,0x90,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x44,0x7e,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x51,0x00,0x00,0x00,0x00,0x00,0x41,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x8b,
    0xb4,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x50,0x08,0x00,0x00,0x00,0x06,0x49,0xfa,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xc0,0xd2,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xfe,0xef,0xe8,0xe3,0xe7,0xee,0xfe,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xdc,
    0xd9,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xeb,0xd8,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xeb,
    0xce,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xdb,0xad,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xbb,
    0x74,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x84,0x2d,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x3e,
    0x04,0xcc,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xdc,0x08,0x00,0x5d,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x6f,0x00,
    0x00,0x08,0xcd,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xdd,0x0d,0x00,0x00,0x00,0x37,0xf6,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xfb,0x48,0x00,0x00,
    0x00,0x00,0x00,0x6d,0xfe,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x7f,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x78,0xfe,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xfe,0x8e,0x03,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x01,0x6e,0xf6,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xfa,0x7d,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x39,0xcd,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xd8,0x43,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x5e,0xcc,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xd3,0x66,0x0b,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x04,0x2c,0x73,0xac,0xcc,0xd7,0xd7,0xd7,0xd7,0xd7,0xd8,0xd8,0xd8,0xd8,0xd8,0xd8,0xd8,0xd9,0xd9,0xd9,0xd9,0xd9,0xd9,0xda,0xda,0xd9,0xcf,0xaf,0x77,0x33,0x05,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

};

const lv_image_dsc_t img_lana = {
  .header.magic = LV_IMAGE_HEADER_MAGIC,
  .header.cf = LV_COLOR_FORMAT_RGB565A8,
  .header.flags = 0,
  .header.w = 49,
  .header.h = 44,
  .header.stride = 98,
  .data_size = sizeof(img_lana_map),
  .data = img_lana_map,
};

