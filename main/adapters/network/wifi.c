#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "wifi.h"
#include "lwip/err.h"
#include "lwip/sys.h"
#include "services/timestamp.h"


#define CONNECTION_RETRIES  5
#define WIFI_CONNECTED_BIT  BIT0
#define WIFI_CONNECTING_BIT BIT1


static EventGroupHandle_t s_wifi_event_group;
static SemaphoreHandle_t  sem;

static const char      *TAG = __FILE_NAME__;
static wifi_ap_record_t ap_info[MAX_AP_SCAN_LIST_SIZE];
static uint16_t         ap_count  = 0;
static uint32_t         ip_addr   = 0;
static unsigned         attempts  = 0;
static bool             reconnect = true;
static timestamp_t      ts        = 0;


static void event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data) {

    ESP_LOGI(TAG, "Event %i.%i", (int)event_base, (int)event_id);

    if (event_base == WIFI_EVENT) {
        switch (event_id) {
            case WIFI_EVENT_STA_DISCONNECTED: {
                if (attempts++ < CONNECTION_RETRIES) {
                    esp_wifi_connect();
                    xEventGroupClearBits(s_wifi_event_group, WIFI_CONNECTED_BIT);
                    xEventGroupSetBits(s_wifi_event_group, WIFI_CONNECTING_BIT);
                    ESP_LOGI(TAG, "retry to connect to the AP");
                } else {
                    attempts  = 0;
                    reconnect = true;
                    ts        = timestamp_get();
                    ESP_LOGI(TAG, "Attempts exceeded, waiting a bit");
                }
                break;
            }

            case WIFI_EVENT_STA_START: {
                ESP_LOGI(TAG, "WiFi started, attempting connection");
                esp_wifi_connect();
                break;
            }

            case WIFI_EVENT_SCAN_DONE: {
                uint16_t number = MAX_AP_SCAN_LIST_SIZE;
                memset(ap_info, 0, sizeof(ap_info));

                ESP_LOGI(TAG, "Max AP number ap_info can hold = %u", number);
                xSemaphoreTake(sem, portMAX_DELAY);
                ESP_ERROR_CHECK(esp_wifi_scan_get_ap_num(&ap_count));
                ESP_ERROR_CHECK(esp_wifi_scan_get_ap_records(&number, ap_info));
                xSemaphoreGive(sem);
                ESP_LOGI(TAG, "Total APs scanned = %u, actual AP number ap_info holds = %u", ap_count, number);
                for (int i = 0; i < number; i++) {
                    ESP_LOGI(TAG, "SSID \t\t%s", ap_info[i].ssid);
                    ESP_LOGI(TAG, "RSSI \t\t%d", ap_info[i].rssi);
                    ESP_LOGI(TAG, "Channel \t\t%d", ap_info[i].primary);
                }
                break;
            }

            default:
                break;
        }
    } else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
        ip_event_got_ip_t *event = (ip_event_got_ip_t *)event_data;
        ESP_LOGI(TAG, "got ip:" IPSTR, IP2STR(&event->ip_info.ip));
        xSemaphoreTake(sem, portMAX_DELAY);
        ip_addr = event->ip_info.ip.addr;
        xSemaphoreGive(sem);
        xEventGroupSetBits(s_wifi_event_group, WIFI_CONNECTED_BIT);
        xEventGroupClearBits(s_wifi_event_group, WIFI_CONNECTING_BIT);
    }
}


void wifi_connect(const char *ssid, const char *password) {
    wifi_config_t wifi_config = {
        .sta =
            {
                /* Authmode threshold resets to WPA2 as default if password matches WPA2 standards (password len =>
                 * 8). If you want to connect the device to deprecated WEP/WPA networks, Please set the threshold
                 * value to WIFI_AUTH_WEP/WIFI_AUTH_WPA_PSK and set the password with length and format matching to
                 * WIFI_AUTH_WEP/WIFI_AUTH_WPA_PSK standards.
                 */
                .threshold.authmode = WIFI_AUTH_WPA2_PSK,
                .sae_pwe_h2e        = WPA3_SAE_PWE_UNSPECIFIED,
                .sae_h2e_identifier = "",
            },
    };
    snprintf((char *)wifi_config.sta.ssid, sizeof(wifi_config.sta.ssid), "%s", ssid);
    snprintf((char *)wifi_config.sta.password, sizeof(wifi_config.sta.password), "%s", password);

    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    if (wifi_is_connected()) {
        ESP_LOGI(TAG, "Already connected, disconnecting");
        ESP_ERROR_CHECK(esp_wifi_disconnect());
    } else {
        ESP_LOGI(TAG, "Starting WiFi");
        ESP_ERROR_CHECK(esp_wifi_start());
    }

    ESP_LOGI(TAG, "Attempting connection to %s", ssid);
}


bool wifi_is_connected(void) {
    EventBits_t bits = xEventGroupGetBits(s_wifi_event_group);
    return (bits & WIFI_CONNECTED_BIT) > 0;
}


bool wifi_is_connecting(void) {
    EventBits_t bits = xEventGroupGetBits(s_wifi_event_group);
    return (bits & WIFI_CONNECTING_BIT) > 0;
}


void wifi_update(mut_model_t *model) {
    xSemaphoreTake(sem, portMAX_DELAY);
    if (wifi_is_connected()) {
        model->run.wifi_state = WIFI_STATE_CONNECTED;

        wifi_config_t wifi_config = {};
        esp_wifi_get_config(WIFI_IF_STA, &wifi_config);
        snprintf(model->run.connected_ssid, sizeof(model->run.connected_ssid), "%s",
                 (const char *)wifi_config.sta.ssid);
        model->run.ip_addr = ip_addr;
    } else if (wifi_is_connecting()) {
        model->run.wifi_state = WIFI_STATE_CONNECTING;
    } else {
        model->run.wifi_state = WIFI_STATE_DISCONNECTED;
        model->run.ssid_num   = 0;
    }

    model->run.ssid_num = ap_count;
    for (size_t i = 0; i < ap_count; i++) {
        snprintf(model->run.ssids[i], sizeof(model->run.ssids[i]), "%s", (const char *)ap_info[i].ssid);
    }
    xSemaphoreGive(sem);

    if (reconnect && timestamp_is_expired(ts, 60000)) {
        reconnect = false;
        esp_wifi_connect();
    }
}


void wifi_init(void) {
    ESP_LOGI(TAG, "ESP_WIFI_MODE_STA");

    s_wifi_event_group = xEventGroupCreate();
    sem                = xSemaphoreCreateMutex();

    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_create_default_wifi_sta();

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    esp_event_handler_instance_t instance_any_id;
    esp_event_handler_instance_t instance_got_ip;
    ESP_ERROR_CHECK(
        esp_event_handler_instance_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &event_handler, NULL, &instance_any_id));
    ESP_ERROR_CHECK(
        esp_event_handler_instance_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &event_handler, NULL, &instance_got_ip));

    wifi_config_t wifi_config = {};
    esp_wifi_get_config(WIFI_IF_STA, &wifi_config);
    ESP_LOGI(TAG, "Attempting connection to %s %s", wifi_config.sta.ssid, wifi_config.sta.password);

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_start());
}


/* Initialize Wi-Fi as sta and set scan method */
void wifi_scan(void) {
    ESP_LOGI(TAG, "Scanning");
    esp_wifi_scan_start(NULL, false);
}
