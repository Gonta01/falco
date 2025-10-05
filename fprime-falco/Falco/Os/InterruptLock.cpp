#include <Os/InterruptLock.hpp>
#include <FreeRTOS.h>
#include <task.h>

namespace Os {
    // Constructor: Initializes the key to 0
    InterruptLock::InterruptLock(void): m_key(0) {}

    // Destructor
    InterruptLock::~InterruptLock(void) {}

    // lock method: Enters a critical section
    void InterruptLock::lock(void) {
        // Enter critical section
        taskENTER_CRITICAL();
        this->m_key = 1; // Setting key to indicate lock is held
    }

    // unLock method: Exits a critical section
    void InterruptLock::unLock(void) {
        // Exit critical section
        taskEXIT_CRITICAL();
        this->m_key = 0; // Resetting key to indicate lock is released
    }

    // getKey method: Returns the current key value
    POINTER_CAST InterruptLock::getKey(void) {
        return this->m_key;
    }
}
