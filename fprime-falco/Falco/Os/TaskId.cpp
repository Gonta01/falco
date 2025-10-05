#include <PlatformTypes.h>
#include <Os/TaskId.hpp>
#include <FreeRTOS.h>
#include <task.h>

namespace Os
{
    // Constructor: Initializes the TaskId object
    TaskId::TaskId(void)
    {
        TaskStatus_t taskStatus;

        // Retrieve information about the currently running task
        vTaskGetInfo(NULL, &taskStatus, pdFALSE, eRunning);

        // Store the task number as the task ID
        id = taskStatus.xTaskNumber;

    }

    // Destructor: No resources to clean up
    TaskId::~TaskId(void)
    {
    }

    // Equality operator: Checks if two TaskId objects are equal
    bool TaskId::operator==(const TaskId& T) const
    {
        return (id == T.id);
    }

    // Inequality operator: Checks if two TaskId objects are not equal
    bool TaskId::operator!=(const TaskId& T) const
    {
        return (id != T.id); 
    }

    // getRepr method: Returns the task ID representation
    TaskIdRepr TaskId::getRepr(void) const
    {
        return this->id;
    }
}
