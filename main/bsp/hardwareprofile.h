#ifndef HARDWAREPROFILE_H_INCLUDED
#define HARDWAREPROFILE_H_INCLUDED


#include <driver/gpio.h>


/*
 * Definizioni dei pin da utilizzare
 */

#define BSP_HAP_SDA                      GPIO_NUM_1
#define BSP_HAP_SCL                      GPIO_NUM_2
#define BSP_HAP_IRQ                      GPIO_NUM_3
#define BSP_HAP_RESET_T                  GPIO_NUM_4
#define BSP_HAP_CS_D                     GPIO_NUM_5
#define BSP_HAP_D                        GPIO_NUM_6
#define BSP_HAP_CLK_D                    GPIO_NUM_7
#define BSP_HAP_DOUT_D                   GPIO_NUM_8
#define BSP_HAP_RESET_D                  GPIO_NUM_9
#define BSP_HAP_RETRO                    GPIO_NUM_10
#define BSP_HAP_BUZZER                   GPIO_NUM_40
#define BSP_HAP_RUN                      GPIO_NUM_17
#define BSP_HAP_CHIAVETTA_ON             GPIO_NUM_18
#define BSP_HAP_PRESENZA_USB_MASTER      GPIO_NUM_33
#define BSP_HAP_ALIMENTAZIONE_CHIAVE_USB GPIO_NUM_34
#define BSP_HAP_TX                       GPIO_NUM_38
#define BSP_HAP_RX                       GPIO_NUM_39
#define BSP_HAP_SDA_CLK                  GPIO_NUM_42
#define BSP_HAP_SCL_CLK                  GPIO_NUM_41

#endif
