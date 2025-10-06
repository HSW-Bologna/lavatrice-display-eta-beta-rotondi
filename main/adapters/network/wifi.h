#ifndef WIFI_H_INCLUDED
#define WIFI_H_INCLUDED


#include <stdint.h>
#include "freertos/FreeRTOS.h"
#include "model/model.h"


void wifi_init(void);
bool wifi_is_connected(void);
void wifi_connect(const char *ssid, const char *password);
void wifi_scan(void);
void wifi_update(mut_model_t *model);


#endif
