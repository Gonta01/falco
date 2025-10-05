#ifndef ICAN_HPP
#define ICAN_HPP

#include "etl/algorithm.h"
#include "etl/span.h"
#include <cstddef>
#include <cstdint>
#include <etl/array.h>

/**
 * @brief Namespase Interfaces
 */
namespace Interfaces {
    /**
     * @brief class Interface of CAN
     */
    class ICAN {
    public:
        class CANMessage {
            static constexpr uint32_t lenBuffer = 64;
            static constexpr uint8_t numSequence8byte = 8;
            static constexpr uint8_t numSequence12byte = 12;
            static constexpr uint8_t numSequence16byte = 16;
            static constexpr uint8_t numSequence20byte = 20;
            static constexpr uint8_t numSequence24byte = 24;
            static constexpr uint8_t numSequence32byte = 32;
            static constexpr uint8_t numSequence48byte = 48;
            static constexpr uint8_t numSequence64byte = 64;

            enum class DataLengthCode : uint8_t {
                len8byte = 0x08U,
                len12bytes = 0x09U,
                len16bytes = 0x0AU,
                len20bytes = 0x0BU,
                len24bytes = 0x0CU,
                len32bytes = 0x0DU,
                len48bytes = 0x0EU,
                len64bytes = 0x0FU,
            };

        public:
            CANMessage() = default;

            uint32_t id = 0;
            uint8_t xtd = 0;
            uint8_t rtr = 0;
            uint8_t dlc = 0;
            uint8_t brs = 0;
            uint8_t fdf = 0;
            etl::array<uint8_t, lenBuffer> data = {0};

            static constexpr uint8_t offsetID = 18; /*!< For CAN FD & common CAN ID ->  should be shift on 18 bit
                                                 *!< For extended CAN FD no shift needed
                                                 */

            uint8_t getDataLengthCodeInBytes() const {
                if (dlc <= numSequence8byte) {
                    return dlc;
                }
                uint8_t sizeInBytes = 0;
                switch (dlc) {
                    case static_cast<uint8_t>(DataLengthCode::len12bytes):
                        sizeInBytes = numSequence12byte;
                        break;
                    case static_cast<uint8_t>(DataLengthCode::len16bytes):
                        sizeInBytes = numSequence16byte;
                        break;
                    case static_cast<uint8_t>(DataLengthCode::len20bytes):
                        sizeInBytes = numSequence20byte;
                        break;
                    case static_cast<uint8_t>(DataLengthCode::len24bytes):
                        sizeInBytes = numSequence24byte;
                        break;
                    case static_cast<uint8_t>(DataLengthCode::len32bytes):
                        sizeInBytes = numSequence32byte;
                        break;
                    case static_cast<uint8_t>(DataLengthCode::len48bytes):
                        sizeInBytes = numSequence48byte;
                        break;
                    default:
                        sizeInBytes = numSequence64byte;
                        break;
                }
                return sizeInBytes;
            }

        private:
            uint8_t getDataLengthCode(uint8_t length) {
                auto dataLen = static_cast<uint8_t>(DataLengthCode::len64bytes);

                if (length <= numSequence8byte) {
                    dataLen = length;
                } else if (length <= numSequence12byte) {
                    dataLen = static_cast<uint8_t>(DataLengthCode::len12bytes);
                } else if (length <= numSequence16byte) {
                    dataLen = static_cast<uint8_t>(DataLengthCode::len16bytes);
                } else if (length <= numSequence20byte) {
                    dataLen = static_cast<uint8_t>(DataLengthCode::len20bytes);
                } else if (length <= numSequence24byte) {
                    dataLen = static_cast<uint8_t>(DataLengthCode::len24bytes);
                } else if (length <= numSequence32byte) {
                    dataLen = static_cast<uint8_t>(DataLengthCode::len32bytes);
                } else if (length <= numSequence48byte) {
                    dataLen = static_cast<uint8_t>(DataLengthCode::len48bytes);
                }

                return dataLen;
            }
        };

        /**
          * @brief Class destructor
          */
        virtual ~ICAN() = default;

        /**
         * @brief Send data through CAN
         * @param buff Buffer to be send
         * @return error code (NoError in case of success)
         */
        virtual Drv::CAN::Status write(Interfaces::ICAN::CANMessage &message, U32 timeoutMs) = 0;

        /**
         * @brief Receive data through CAN
         * @param buff Buffer to be using for received data
         * @return error code (NoError in case of success)
         */
        virtual Drv::CAN::Status read(Interfaces::ICAN::CANMessage &message, U32 timeoutMs) = 0;
    };
} // namespace Interfaces

#endif // ICAN_HPP
