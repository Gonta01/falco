
#include "FreeRTOS.h"
#include "semphr.h"
#include "ff.h"
#include "ffconf.h"

static SemaphoreHandle_t fs_mutexes[FF_VOLUMES+1];

// Called by FatFs during mount or volume init
int ff_mutex_create(int vol) {
    if (vol > FF_VOLUMES) {
        return 0;
    }
    fs_mutexes[vol] = xSemaphoreCreateMutex();
    return (fs_mutexes[vol] != NULL);
}

// Called by FatFs during volume unmount or cleanup
void ff_mutex_delete(int vol) {
    if (vol <= FF_VOLUMES && fs_mutexes[vol]) {
        vSemaphoreDelete(fs_mutexes[vol]);
        fs_mutexes[vol] = NULL;
    }
}

// Called before file operation
int ff_mutex_take(int vol) {
    if (vol > FF_VOLUMES || fs_mutexes[vol] == NULL) {
        return 0;
    }
    return (xSemaphoreTake(fs_mutexes[vol], FF_FS_TIMEOUT) == pdTRUE);
}

// Called after file operation
void ff_mutex_give(int vol) {
    if (vol <= FF_VOLUMES && fs_mutexes[vol]) {
        xSemaphoreGive(fs_mutexes[vol]);
    }
}
