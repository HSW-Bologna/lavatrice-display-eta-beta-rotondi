#include <stdio.h>
#include "bsp/msc.h"
#include <dirent.h>
#include <errno.h>
#include "controller/configuration/archive_management.h"
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"


#define MOUNTPOINT "/tmp/usb"


static QueueHandle_t response_queue;


void msc_init(void) {
    static uint8_t       response_queue_buffer[sizeof(msc_response_t) * 4] = {0};
    static StaticQueue_t static_response_queue;
    response_queue = xQueueCreateStatic(sizeof(response_queue_buffer) / sizeof(msc_response_t), sizeof(msc_response_t),
                                        response_queue_buffer, &static_response_queue);
}


removable_drive_state_t msc_is_device_mounted(void) {
    DIR *dir = opendir(MOUNTPOINT);

    /* Directory exists. */
    if (dir) {
        closedir(dir);
        return REMOVABLE_DRIVE_STATE_MOUNTED;
    }
    /* Directory does not exist. */
    else if (ENOENT == errno) {
        return REMOVABLE_DRIVE_STATE_MISSING;
    }
    /* opendir() failed for some other reason. */
    else {
        return REMOVABLE_DRIVE_STATE_INVALID;
    }
}


size_t msc_read_archives(model_t *pmodel) {
    if (msc_is_device_mounted() == REMOVABLE_DRIVE_STATE_MOUNTED) {
        char **strings = NULL;
        int    res     = archive_management_list_archives(MOUNTPOINT, &strings);

        if (res < 0) {
            res = 0;
        }

        pmodel->system.num_archivi = archive_management_copy_archive_names(strings, &pmodel->system.archivi, res);
        return pmodel->system.num_archivi;
    } else {
        pmodel->system.num_archivi = 0;
        free(pmodel->system.archivi);
        pmodel->system.archivi = NULL;
        return 0;
    }
}


void msc_extract_archive(const char *archive) {
    char string[128] = {0};
    snprintf(string, sizeof(string), "%s/%s%s", MOUNTPOINT, archive, ARCHIVE_SUFFIX);
    msc_response_t response = {.code = MSC_RESPONSE_CODE_ARCHIVE_EXTRACTION_COMPLETE};
    response.error          = archive_management_extract_configuration(string);
    xQueueSend(response_queue, &response, portMAX_DELAY);
}


int msc_get_response(msc_response_t *response) {
    return xQueueReceive(response_queue, response, 0) == pdTRUE;
}
