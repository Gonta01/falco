#include <Os/Task.hpp>
#include <Fw/Types/Assert.hpp>
#include <stdlib.h>
#include <string.h>
#include <FreeRTOS.h>
#include <task.h>

namespace Os {

    // Constructor: Initializes task attributes to default values
    Task::Task() : m_handle(0), m_identifier(0), m_affinity(-1), m_started(false), m_suspendedOnPurpose(false), m_routineWrapper() {
    }

    // Start method: Creates and starts a FreeRTOS task
    Task::TaskStatus Task::start(const Fw::StringBase &name, taskRoutine routine, void* arg, NATIVE_UINT_TYPE priority, NATIVE_UINT_TYPE stackSize,  NATIVE_UINT_TYPE cpuAffinity, NATIVE_UINT_TYPE identifier) {
        TaskHandle_t xHandle; // FreeRTOS task handle
        this->m_name = "TP_"; // Prefix task name with "TP_"
        this->m_name += name; // Append the given name
        this->m_identifier = identifier; // Set the task identifier
        Task::TaskStatus tStat = TASK_OK; // Initialize task status to OK
        BaseType_t basetype; // Correct type for FreeRTOS task creation return value

        // Create a FreeRTOS task
        basetype = xTaskCreate(routine, (char*)this->m_name.toChar(), stackSize, arg, priority, &xHandle);
        if (basetype != pdPASS) { // Correct comparison for task creation
            tStat = TASK_INVALID_PARAMS; // Set status to invalid if creation failed
        } else {
            this->m_handle = (POINTER_CAST)xHandle; // Store the task handle
        }
        return tStat; // Return the task status
    }

    // Delay method: Delays the task for a given number of milliseconds
    Task::TaskStatus Task::delay(NATIVE_UINT_TYPE milliseconds) {
        vTaskDelay(pdMS_TO_TICKS(milliseconds)); // Convert milliseconds to ticks and delay
        return TASK_OK; // Return OK status
    }

    // Destructor: Removes the task from the registry if it exists
    Task::~Task() {
        if (Task::s_taskRegistry) {
            Task::s_taskRegistry->removeTask(this);
        }
    }

    // Suspend method: Not implemented, asserts failure if called
    void Task::suspend(bool onPurpose) {
        FW_ASSERT(0); // Assert failure
    }

    // Resume method: Not implemented, asserts failure if called
    void Task::resume(void) {
        FW_ASSERT(0); // Assert failure
    }

    // isSuspended method: Not implemented, asserts failure if called
    bool Task::isSuspended(void) {
        FW_ASSERT(0); // Assert failure
        return false;
    }

    // getOsIdentifier method: Returns a default TaskId
    TaskId Task::getOsIdentifier(void) {
        TaskId T;
        return T; // Return default TaskId
    }

    // Join method: Not implemented, asserts failure if called
    Task::TaskStatus Task::join(void **value_ptr) {
        FW_ASSERT(0); // Assert failure
        return TASK_OK; // Return OK status for coverage
    }
}
