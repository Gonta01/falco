#include <Os/Mutex.hpp>
#include <Fw/Types/Assert.hpp>
#include <FreeRTOS.h>
#include <semphr.h>

namespace Os {
    
    // Constructor: Initializes the mutex
    Mutex::Mutex(void) {
        SemaphoreHandle_t handle;

        // Create a FreeRTOS mutex
        handle = xSemaphoreCreateMutex();
        
        // Store the handle in m_handle
        this->m_handle = (POINTER_CAST) handle;

        // Assert if the mutex creation failed
        FW_ASSERT(this->m_handle != 0);
    }
    
    // Destructor: Deletes the mutex
    Mutex::~Mutex(void) {
        // Delete the FreeRTOS mutex
        vSemaphoreDelete(this->m_handle);
    }
    
    // lock method: Locks the mutex
    void Mutex::lock(void) {
        // Attempt to take the semaphore/mutex, waiting indefinitely (portMAX_DELAY should be specified in FreeRTOSconfig.h)
        NATIVE_INT_TYPE stat = xSemaphoreTake((SemaphoreHandle_t)this->m_handle, portMAX_DELAY);
        
        // Assert if the operation fails
        FW_ASSERT(stat == pdTRUE, stat);
    }

    // unLock method: Unlocks the mutex
    void Mutex::unLock(void) {
        // Give the semaphore/mutex
        NATIVE_INT_TYPE stat = xSemaphoreGive(this->m_handle);
        
        // Assert if the operation fails
        FW_ASSERT(stat == pdTRUE, stat);
    }
            
}
