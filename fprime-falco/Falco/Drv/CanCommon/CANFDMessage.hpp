#ifndef CANFDMessage_HPP
#define CANFDMessage_HPP

#include "etl/span.h"
#include "Ports/CanPorts/FrameFormatEnumAc.hpp"
#include "Ports/CanPorts/FrameFDEnumAc.hpp"
#include "Ports/CanPorts/BitRateSwitchEnumAc.hpp"
#include "Ports/CanPorts/DataLengthCodeEnumAc.hpp"

namespace Drv {
    class CANFDMessage {
        private:
            uint32_t id = 0;
            uint8_t xtd = 0;
            uint8_t dlc = 0;
            uint8_t brs = 0;
            uint8_t fdf = 0;
    
            static constexpr uint32_t lenBuffer = 64;
            static constexpr uint8_t numSequence8byte = 8;
            static constexpr uint8_t numSequence12byte = 12;
            static constexpr uint8_t numSequence16byte = 16;
            static constexpr uint8_t numSequence20byte = 20;
            static constexpr uint8_t numSequence24byte = 24;
            static constexpr uint8_t numSequence32byte = 32;
            static constexpr uint8_t numSequence48byte = 48;
            static constexpr uint8_t numSequence64byte = 64;
    
    
            uint8_t getDataLengthCode(uint8_t length) {
                auto dataLen = static_cast<uint8_t>(Drv::CAN::DataLengthCode::len64bytes);
    
                if (length <= numSequence8byte) {
                    dataLen = length;
                } else if (length <= numSequence12byte) {
                    dataLen = static_cast<uint8_t>(Drv::CAN::DataLengthCode::len12bytes);
                } else if (length <= numSequence16byte) {
                    dataLen = static_cast<uint8_t>(Drv::CAN::DataLengthCode::len16bytes);
                } else if (length <= numSequence20byte) {
                    dataLen = static_cast<uint8_t>(Drv::CAN::DataLengthCode::len20bytes);
                } else if (length <= numSequence24byte) {
                    dataLen = static_cast<uint8_t>(Drv::CAN::DataLengthCode::len24bytes);
                } else if (length <= numSequence32byte) {
                    dataLen = static_cast<uint8_t>(Drv::CAN::DataLengthCode::len32bytes);
                } else if (length <= numSequence48byte) {
                    dataLen = static_cast<uint8_t>(Drv::CAN::DataLengthCode::len48bytes);
                }
    
                return dataLen;
            }
    
        public:
            CANFDMessage() = default;
            CANFDMessage(const etl::span<uint8_t> &dataBuff, uint32_t identifier, Drv::CAN::FrameFormat frameFormat, Drv::CAN::FrameFD frameFD = Drv::CAN::FrameFD::Enable, Drv::CAN::BitRateSwitch bitRateSwitch = Drv::CAN::BitRateSwitch::Disable) {
                id = identifier;
                xtd = static_cast<uint8_t>(frameFormat);
                fdf = static_cast<uint8_t>(frameFD);
                if (frameFD == Drv::CAN::FrameFD::Enable) {
                    brs = static_cast<uint8_t>(bitRateSwitch);
                }
                else {
                    brs =  static_cast<uint8_t>(Drv::CAN::BitRateSwitch::Disable);
                }
                dlc = getDataLengthCode(dataBuff.size_bytes());
                etl::copy_s(dataBuff.begin(), dataBuff.end(), data.begin(), data.end());
            }
    
            uint32_t getID() {
                return id;
            };
    
            uint8_t getExtendedFrameFlag() {
                return xtd;
            };
    
            uint8_t getDataLengthCode() {
                return dlc;
            }
    
            uint8_t getBitRateSwitchFlag() {
                return brs;
            }
    
            uint8_t getFDFrameFlag() {
                return fdf;
            }
    
            void getData(etl::span<uint8_t> buff) {
                etl::copy_s(data.begin(), data.end(), buff.begin(), buff.end());
            }
            etl::array<uint8_t, lenBuffer> data = {};
    
    };
} // namespace Drv
#endif