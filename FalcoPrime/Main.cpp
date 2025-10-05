// ======================================================================
// \title  Main.cpp
// \brief main program for the F' application. Intended for CLI-based systems (Linux, macOS)
//
// ======================================================================
// Used to access topology functions
#include <FalcoPrime/Top/FalcoPrimeTopology.hpp>
#include <FalcoPrime/Top/FalcoPrimeTopologyAc.hpp>
#include "FreeRTOS.h"
#include "task.h"

/* Task to be created. */
void initTask( void * pvParameters )
{
    FalcoPrime::TopologyState inputs;
    FalcoPrime::setupTopology(inputs);
    vTaskDelete(NULL);
}

extern "C" {
void app_init() {
    BaseType_t xReturned;
    TaskHandle_t xHandle = NULL;

    /* Create the task, storing the handle. */
    xReturned = xTaskCreate(
                    initTask,                   /* Function that implements the task. */
                    "INIT_TASK",                /* Text name for the task. */
                    2048,                       /* Stack size in words, not bytes. */
                    NULL,                       /* Parameter passed into the task. */
                    31,                          /* Priority at which the task is created. */
                    &xHandle );                 /* Used to pass out the created task's handle. */
    
}
}