#ifndef CAN_HPP
#define CAN_HPP

#include "CANBase.hpp"
#include "mcan/plib_mcan0.h"
#include "mcan/plib_mcan1.h"
#include "mcan/plib_mcan_common.h"
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <etl/array.h>
#include <etl/span.h>

/**
 * @brief Namespace Base
 */
namespace Base {

    extern etl::array<uint8_t, MCAN0_MESSAGE_RAM_CONFIG_SIZE> MCAN0MessageRAM __attribute__((aligned(32))) __attribute__((section(".ram_nocache")));

    extern etl::array<uint8_t, MCAN1_MESSAGE_RAM_CONFIG_SIZE> MCAN1MessageRAM __attribute__((aligned(32))) __attribute__((section(".ram_nocache")));

    class CAN0Base : public CANBase {
    public:
    protected:
        void configMCAN() {
            MCAN0_MessageRAMConfigSet(reinterpret_cast<uint8_t *>(&MCAN0MessageRAM));
            receiveMessage = MCAN0_MessageReceiveFifo;
            transmiteMessage = MCAN0_MessageTransmitFifo;
            getError = MCAN0_ErrorGet;
            MCAN0_TxFifoCallbackRegister(TxFifoCallback, reinterpret_cast<uintptr_t>(this));
            MCAN0_RxFifoCallbackRegister(MCAN_RX_FIFO_NUM::MCAN_RX_FIFO_0, RxFifoCallback, reinterpret_cast<uintptr_t>(this));
            MCAN0_CallbackRegister(ErrorCallback, reinterpret_cast<uintptr_t>(this));
        }

        static void RxFifoCallback(uint8_t numberOfMessage, uintptr_t thisObjects) {
            reinterpret_cast<CAN0Base *>(thisObjects)->RxFifoCallbackHandler(numberOfMessage);
        }

        static void TxFifoCallback(uintptr_t thisObjects) {
            reinterpret_cast<CAN0Base *>(thisObjects)->TxFifoCallbackHandler();
        }

        static void ErrorCallback(uint32_t interruptStatus, uintptr_t thisObjects) {
            reinterpret_cast<CAN0Base *>(thisObjects)->ErrorCallbackHandler(interruptStatus);
        }
    };

    class CAN1Base : public CANBase {
    public:
    protected:
        void configMCAN() {
            MCAN1_MessageRAMConfigSet(reinterpret_cast<uint8_t *>(&MCAN1MessageRAM));
            receiveMessage = MCAN1_MessageReceiveFifo;
            transmiteMessage = MCAN1_MessageTransmitFifo;
            getError = MCAN1_ErrorGet;
            MCAN1_TxFifoCallbackRegister(TxFifoCallback, reinterpret_cast<uintptr_t>(this));
            MCAN1_RxFifoCallbackRegister(MCAN_RX_FIFO_NUM::MCAN_RX_FIFO_0, RxFifoCallback, reinterpret_cast<uintptr_t>(this));
            MCAN1_CallbackRegister(ErrorCallback, reinterpret_cast<uintptr_t>(this));
        }

        static void RxFifoCallback(uint8_t numberOfMessage, uintptr_t thisObjects) {
            reinterpret_cast<CAN1Base *>(thisObjects)->RxFifoCallbackHandler(numberOfMessage);
        }

        static void TxFifoCallback(uintptr_t thisObjects) {
            reinterpret_cast<CAN1Base *>(thisObjects)->TxFifoCallbackHandler();
        }

        static void ErrorCallback(uint32_t interruptStatus, uintptr_t thisObjects) {
            reinterpret_cast<CAN1Base *>(thisObjects)->ErrorCallbackHandler(interruptStatus);
        }
    };

} // namespace Base

#endif // CAN_HPP
