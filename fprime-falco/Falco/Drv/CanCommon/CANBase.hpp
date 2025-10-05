#ifndef CANBASE_HPP
#define CANBASE_HPP
#include "ICAN.hpp"
#include "Fw/Types/Assert.hpp"
#include "mcan/plib_mcan0.h"
#include "mcan/plib_mcan1.h"
#include "mcan/plib_mcan_common.h"
#include "Ports/CanPorts/NominalBitRateEnumAc.hpp"
#include "Ports/CanPorts/DataBitRateEnumAc.hpp"
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <etl/array.h>
#include <etl/memory.h>
#include <etl/span.h>
#include <etl/algorithm.h>
#include "FreeRTOS.h"
#include "queue.h"
#include "event_groups.h"
#include "semphr.h"

/**
 * @brief Namespace Base
 */
namespace Base {

    /**
     *
     * @tparam FIFOElementSize - size of one CAN message for receive
     * @tparam FIFOSize - total size of receive FIFO buffer
     */
    template<uint32_t FIFOElementSize, uint32_t FIFOSize>
    class CANBufferRX {
    public:
        CANBufferRX(){};

        MCAN_RX_BUFFER &operator[](int32_t index) {
            if (index > numOfMaxMessages) {
                index %= numOfMaxMessages;
            }
            return *reinterpret_cast<MCAN_RX_BUFFER *>(rxBufferBegin + FIFOElementSize * index);
        }

        void clear() {
            std::memset(rxBuffer.data(), 0, FIFOSize * sizeof(rxBuffer.front()));
        }

        MCAN_RX_BUFFER *begin() {
            return firstMessage;
        }

    private:
        etl::array<uint8_t, FIFOSize> rxBuffer{0};
        uint8_t *rxBufferBegin = rxBuffer.data();
        int32_t numOfMaxMessages = FIFOSize / FIFOElementSize;
        MCAN_RX_BUFFER *firstMessage = reinterpret_cast<MCAN_RX_BUFFER *>(rxBuffer.data());
    };

    class CANBase : virtual public Interfaces::ICAN {
    public:
        using CANMessage = Interfaces::ICAN::CANMessage;

        CANBase(){
            receiveQueueHandle = xQueueCreate(numberOfMessagesRx, sizeof(CANMessage));
            eventGroupTransmitHandle = xEventGroupCreate();
            mutexWriteHandle = xSemaphoreCreateMutex();
            mutexReadHandle  = xSemaphoreCreateMutex();
        };

        /**
         * @brief Send data through CAN
         * @param buff Buffer to be send
         * @return error code (NoError in case of success)
         */
        Drv::CAN::Status write(CANMessage &message, U32 timeoutMs) override {
            BaseType_t mutexStatus = xSemaphoreTake(mutexWriteHandle, portMAX_DELAY);
            FW_ASSERT(mutexStatus == pdTRUE, mutexStatus);

            Drv::CAN::Status error = Drv::CAN::Status::WriteFailed;
            if (startTransmission(message)) {
                if (waitTransmissionEnd(timeoutMs)) {
                    error = Drv::CAN::Status::NoError;
                }
            }

            mutexStatus = xSemaphoreGive(mutexWriteHandle);
            FW_ASSERT(mutexStatus == pdTRUE, mutexStatus);
            return error;
        }

        /**
         * @brief Receive data through CAN
         * @param data Buffer to be using for received data
         * @return error code (NoError in case of success)
         */
        Drv::CAN::Status read(Interfaces::ICAN::CANMessage &message, U32 timeoutMs) override {
            BaseType_t mutexStatus = xSemaphoreTake(mutexReadHandle, portMAX_DELAY);
            FW_ASSERT(mutexStatus == pdTRUE, mutexStatus);

            Drv::CAN::Status error = Drv::CAN::Status::NoDataToRead;
            BaseType_t status = xQueueReceive(receiveQueueHandle, &message, timeoutMs);
            if (status == pdTRUE) {
                error = Drv::CAN::Status::NoError;
            }
            
            mutexStatus = xSemaphoreGive(mutexReadHandle);
            FW_ASSERT(mutexStatus == pdTRUE, mutexStatus);
            return error;
        }

    protected:
        void configureNominalBitTimings(MCAN_BIT_TIMING_SETUP &mcanBitTimingSetup, Drv::CAN::NominalBitRate nominalBitRate) {
            if (mcanBitTimingSetup.nominalBitTimingSet) {
                switch (nominalBitRate) {
                    case Drv::CAN::NominalBitRate::BitRate250kbps:
                        mcanBitTimingSetup.nominalBitRate = Bitrate250kbps;
                        mcanBitTimingSetup.nominalSamplePoint = Bitrate250kbpsSamplePoint;
                        mcanBitTimingSetup.nominalPrescaler = 0;
                        break;
                    case Drv::CAN::NominalBitRate::BitRate500kbps:
                        mcanBitTimingSetup.nominalBitRate = Bitrate500kbps;
                        mcanBitTimingSetup.nominalSamplePoint = Bitrate500kbpsSamplePoint;
                        mcanBitTimingSetup.nominalPrescaler = 0;
                        break;
                    case Drv::CAN::NominalBitRate::BitRate1000kbps:
                        mcanBitTimingSetup.nominalBitRate = Bitrate1000kbps;
                        mcanBitTimingSetup.nominalSamplePoint = Bitrate1000kbpsSamplePoint;
                        mcanBitTimingSetup.nominalPrescaler = 0;
                        break;
                    default:
                        break;
                }
            }
        }

        void configureDataBitTimings(MCAN_BIT_TIMING_SETUP &mcanBitTimingSetup, Drv::CAN::DataBitRate dataBitRate) {
            if (mcanBitTimingSetup.dataBitTimingSet) {
                switch (dataBitRate) {
                    case Drv::CAN::DataBitRate::BitRate2Mbps:
                        mcanBitTimingSetup.dataBitRate = Bitrate2000kbps;
                        mcanBitTimingSetup.dataSamplePoint = Bitrate2000kbpsSamplePoint;
                        mcanBitTimingSetup.dataPrescaler = 0;
                        break;
                    case Drv::CAN::DataBitRate::BitRate4Mbps:
                        mcanBitTimingSetup.dataBitRate = Bitrate4000kbps;
                        mcanBitTimingSetup.dataSamplePoint = Bitrate4000kbpsSamplePoint;
                        mcanBitTimingSetup.dataPrescaler = 0;
                        break;
                    default:
                        break;
                }
            }
        }
        
        void ErrorCallbackHandler(uint32_t interruptStatus) {
            auto error = getError();
        }

        void TxFifoCallbackHandler() {
            uint8_t event = evtTransmitComplete;
            BaseType_t xHigherPriorityTaskWoken = pdFALSE;
            xEventGroupSetBitsFromISR(eventGroupTransmitHandle, event, &xHigherPriorityTaskWoken);
        }

        void RxFifoCallbackHandler(uint8_t numberOfMessage) {
            canBufferRx.clear();
            receiveMessage(MCAN_RX_FIFO_0, numberOfMessage, canBufferRx.begin());
            for (uint8_t i = 0; i < numberOfMessage; i++) {
                CANMessages.at(i).xtd = canBufferRx[i].xtd;
                CANMessages.at(i).id =
                        (CANMessages.at(i).xtd == static_cast<uint8_t>(Drv::CAN::FrameFormat::StandardFrame))
                                ? (canBufferRx[i].id >> CANMessage::offsetID)
                                : (canBufferRx[i].id);
                CANMessages.at(i).dlc = canBufferRx[i].dlc;
                CANMessages.at(i).fdf = canBufferRx[i].fdf;
                CANMessages.at(i).brs = canBufferRx[i].brs;
                CANMessages.at(i).rtr = canBufferRx[i].rtr;

                const uint8_t sizeOfMessageInBytes = CANMessages.at(i).getDataLengthCodeInBytes();
                etl::copy_n(canBufferRx[i].data, sizeOfMessageInBytes, CANMessages.at(i).data.data());

                UBaseType_t cnt = uxQueueSpacesAvailable(receiveQueueHandle);
                bool isMessageQueueFull = (cnt == 0);
                if (isMessageQueueFull) {
                    break;
                }
                BaseType_t xHigherPriorityTaskWoken = pdFALSE;
                xQueueSendToBackFromISR(receiveQueueHandle, &CANMessages.at(i), &xHigherPriorityTaskWoken);
            }
        }

        bool (*transmiteMessage)(uint8_t numberOfMessage, MCAN_TX_BUFFER *txBuffer){};
        bool (*receiveMessage)(MCAN_RX_FIFO_NUM rxFifoNum, uint8_t numberOfMessage, MCAN_RX_BUFFER *rxBuffer){};
        MCAN_ERROR (*getError)(){};

    private:
        static constexpr uint32_t Bitrate250kbps = 250'000;
        static constexpr uint32_t Bitrate500kbps = 500'000;
        static constexpr uint32_t Bitrate1000kbps = 1'000'000;
        static constexpr uint32_t Bitrate2000kbps = 2'000'000;
        static constexpr uint32_t Bitrate4000kbps = 4'000'000;

        static constexpr float Bitrate250kbpsSamplePoint = 98.75F;
        static constexpr float Bitrate500kbpsSamplePoint = 97.5F;
        static constexpr float Bitrate1000kbpsSamplePoint = 95.0F;
        static constexpr float Bitrate2000kbpsSamplePoint = 90.0F;
        static constexpr float Bitrate4000kbpsSamplePoint = 80.0F;


        static constexpr uint32_t FIFOElementSizeRX = MCAN0_RX_FIFO0_ELEMENT_SIZE;
        static constexpr uint32_t FIFOSizeRX = MCAN0_RX_FIFO0_SIZE;
        static constexpr uint32_t FIFOElementSizeTX = MCAN0_TX_FIFO_BUFFER_ELEMENT_SIZE;
        static constexpr uint32_t FIFOSizeTX = MCAN0_TX_FIFO_BUFFER_SIZE;

        static constexpr uint32_t numberOfMessagesRx = FIFOSizeRX / FIFOElementSizeRX;
        etl::array<CANMessage, numberOfMessagesRx> CANMessages;

        QueueHandle_t receiveQueueHandle;

        CANBufferRX<FIFOElementSizeRX, FIFOSizeRX> canBufferRx;

        static constexpr uint8_t evtUntrackedEvent = 1U << 3U;
        static constexpr uint8_t evtError = 1U << 2U;
        static constexpr uint8_t evtTransmitComplete = 1U << 1U;
        
        EventGroupHandle_t eventGroupTransmitHandle;
        SemaphoreHandle_t mutexWriteHandle;
        SemaphoreHandle_t mutexReadHandle;

        bool waitTransmissionEnd(U32 timeoutMs) {
            auto ticks = timeoutMs / portTICK_PERIOD_MS; 
            auto transmissionResult = xEventGroupWaitBits(eventGroupTransmitHandle,
                                      evtTransmitComplete | evtError,
                                      pdTRUE,
                                      pdFALSE,
                                      ticks);

            return (transmissionResult == evtTransmitComplete);
        }

        bool startTransmission(CANMessage &msg) {
            etl::array<uint8_t, FIFOElementSizeTX> txBuffer{};
            auto *Message = reinterpret_cast<MCAN_TX_BUFFER *>(txBuffer.data());

            Message->xtd = msg.xtd;
            Message->id = (msg.xtd == static_cast<uint8_t>(Drv::CAN::FrameFormat::StandardFrame))
                                ? (msg.id << CANMessage::offsetID)
                                : msg.id;
            Message->dlc = msg.dlc;
            Message->brs = msg.brs;
            Message->fdf = msg.fdf;
            Message->rtr = msg.rtr;

            etl::span<uint8_t> const payLoad(Message->data);
            etl::mem_copy(msg.data.begin(), msg.data.end(), payLoad.begin());
            return transmiteMessage(1, Message);
        }
    };
} // namespace Base
#endif // CANBASE_HPP