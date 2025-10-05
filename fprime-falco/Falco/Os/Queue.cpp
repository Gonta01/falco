#include <Os/Queue.hpp>
#include <Fw/Types/Assert.hpp>
#include "samv71q21b.h"
#include <FreeRTOS.h>
#include <queue.h>
#include <etl/map.h>

#define Q_MAX_MSG_SIZE              1024
#define Q_ACTUAL_SIZE_RESERVED      (sizeof(U16))
#define Q_MAX_USER_MSG_SIZE         (Q_MAX_MSG_SIZE - Q_ACTUAL_SIZE_RESERVED)

constexpr size_t maxAmountOfQueues = 50;
static etl::map<POINTER_CAST, NATIVE_INT_TYPE, maxAmountOfQueues> queueMsgSizes;

namespace Os {

    // Constructor: Initialize queue handle and message size to default values
    Queue::Queue() :
        m_handle(0) { // Initialize m_msgSize to 0 which is needed to support msgSize function
    }

    // CreateInternal method: Creates a FreeRTOS queue
    // - name: The name of the queue
    // - depth: The maximum number of messages the queue can hold
    // - msgSize: The size of each message
    // Returns the status of queue creation
    Queue::QueueStatus Queue::createInternal(const Fw::StringBase &name, NATIVE_INT_TYPE depth, NATIVE_INT_TYPE msgSize) {
        if (Queue::s_numQueues >= maxAmountOfQueues)  {
            return QUEUE_UNKNOWN_ERROR;
        }

        this->m_name = "/QP_"; // Prefix queue name with "/QP_"
        this->m_name += name; // Append the given name

        if (msgSize > static_cast<NATIVE_INT_TYPE>(Q_MAX_USER_MSG_SIZE)) {
            return QUEUE_SIZE_MISMATCH;
        }

        FW_ASSERT(msgSize + Q_ACTUAL_SIZE_RESERVED <= Q_MAX_USER_MSG_SIZE, msgSize);

        UBaseType_t uxQueueLength = depth; // Set queue length
        UBaseType_t uxItemSize = msgSize + Q_ACTUAL_SIZE_RESERVED; // Set item size

        // Create FreeRTOS queue
        QueueHandle_t xQueue = xQueueCreate(uxQueueLength, uxItemSize);  // Create FreeRTOS queue
        
        if (NULL == xQueue) {
            return QUEUE_UNINITIALIZED; // Return if queue creation failed
        }

        this->m_handle = reinterpret_cast<POINTER_CAST>(xQueue); // Store the queue handle
        queueMsgSizes[this->m_handle] = static_cast<NATIVE_INT_TYPE>(uxItemSize);

        Queue::s_numQueues++; // Increment the number of queues

        return QUEUE_OK; // Return success
    }

    // Destructor: Deletes the FreeRTOS queue
    Queue::~Queue() {
        queueMsgSizes.erase(this->m_handle);
        vQueueDelete(reinterpret_cast<QueueHandle_t>(this->m_handle));
    }

    // Send method: Sends a message to the queue
    // - buffer: Pointer to the message buffer
    // - size: Size of the message
    // - priority: Priority of the message (not used in FreeRTOS)
    // - block: Whether the send should block if the queue is full
    // Returns the status of the send operation
    Queue::QueueStatus Queue::send(const U8* buffer, NATIVE_INT_TYPE size, NATIVE_INT_TYPE priority, QueueBlocking block) {
        QueueHandle_t queueHandle = reinterpret_cast<QueueHandle_t>(this->m_handle);

        if (NULL == queueHandle) {
            return QUEUE_UNINITIALIZED;
        }
        
        if (NULL == buffer) {
            return QUEUE_EMPTY_BUFFER;
        }

        if (size < 0 || size > this->getMsgSize()) {
            return QUEUE_SIZE_MISMATCH;
        }

        U8 tmp_buf[Q_MAX_MSG_SIZE];
        *reinterpret_cast<U16 *>(tmp_buf) = static_cast<U16>(size);
        memcpy(&tmp_buf[Q_ACTUAL_SIZE_RESERVED], buffer, size);
        // Attempt to send the message to the queue, using portMAX_DELAY if blocking, otherwise a timeout of 0
        BaseType_t stat;
        if (__get_IPSR() == 0) {
            stat = xQueueSend(queueHandle, tmp_buf, block == QUEUE_BLOCKING ? portMAX_DELAY : 0);
        }
        else {
            stat = xQueueSendFromISR(queueHandle, tmp_buf, NULL);
        }

        if (stat != pdPASS) {
            return QUEUE_FULL; // Return if the queue is full
        }
       
        return QUEUE_OK; // Return success
    }

    // Receive method: Receives a message from the queue
    // - buffer: Pointer to the buffer to store the received message
    // - capacity: Capacity of the buffer
    // - actualSize: Actual size of the received message
    // - priority: Priority of the message (not used in FreeRTOS)
    // - block: Whether the receive should block if the queue is empty
    // Returns the status of the receive operation
    Queue::QueueStatus Queue::receive(U8* buffer, NATIVE_INT_TYPE capacity, NATIVE_INT_TYPE &actualSize, NATIVE_INT_TYPE &priority, QueueBlocking block) {
        QueueHandle_t queueHandle = reinterpret_cast<QueueHandle_t>(this->m_handle);

        if (NULL == queueHandle) {
            return QUEUE_UNINITIALIZED;
        }

        if (buffer == nullptr) {
            return QUEUE_EMPTY_BUFFER;
        }

        if (capacity < this->getMsgSize()) {
            return QUEUE_SIZE_MISMATCH;
        }

        U8 tmp_buf[Q_MAX_MSG_SIZE];
        // Attempt to receive the message
        BaseType_t stat;
        if (__get_IPSR() == 0) {
            stat = xQueueReceive(queueHandle, tmp_buf, block == QUEUE_BLOCKING ? portMAX_DELAY : 0);
        }
        else {
            stat = xQueueReceiveFromISR(queueHandle, tmp_buf, NULL);
        }

        if (stat == errQUEUE_EMPTY) {
            return QUEUE_NO_MORE_MSGS;
        }

        actualSize = *reinterpret_cast<U16 *>(tmp_buf);
        memcpy(buffer, &tmp_buf[Q_ACTUAL_SIZE_RESERVED], actualSize);
        return QUEUE_OK; // Return success
    }

    // Get the number of messages in the queue
    // Returns the number of messages currently in the queue
    NATIVE_INT_TYPE Queue::getNumMsgs(void) const {
        QueueHandle_t queueHandle = reinterpret_cast<QueueHandle_t>(this->m_handle);
        UBaseType_t numMsg = uxQueueMessagesWaiting(queueHandle);

        return static_cast<NATIVE_INT_TYPE>(numMsg);
    }

    // Implementing the maximum number of messages the queue can hold
    // Returns the total capacity of the queue (number of messages it can hold)
    NATIVE_INT_TYPE Queue::getMaxMsgs(void) const {
        QueueHandle_t queueHandle = reinterpret_cast<QueueHandle_t>(this->m_handle);
        // The maximum number of messages is the sum of messages waiting and spaces available
        return (NATIVE_INT_TYPE) (uxQueueSpacesAvailable(queueHandle) + uxQueueMessagesWaiting(queueHandle));
    }

    // Get the queue size (maximum number of messages queue can hold)
    // Returns the current total number of messages and available spaces in the queue
    NATIVE_INT_TYPE Queue::getQueueSize(void) const {
        QueueHandle_t queueHandle = reinterpret_cast<QueueHandle_t>(this->m_handle);
        return (NATIVE_INT_TYPE)(uxQueueMessagesWaiting(queueHandle) + uxQueueSpacesAvailable(queueHandle));
    }

    // Implementing the message size for the queue
    // Returns the size of each message in the queue
    // Not a native FreeRTOS API function
    NATIVE_INT_TYPE Queue::getMsgSize(void) const {
        NATIVE_INT_TYPE msgSize = queueMsgSizes[this->m_handle] - Q_ACTUAL_SIZE_RESERVED;
        return msgSize;
    }

}
