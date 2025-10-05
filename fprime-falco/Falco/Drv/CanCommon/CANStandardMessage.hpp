#ifndef CANStandardMessage_HPP
#define CANStandardMessage_HPP

#include "etl/span.h"
#include "Ports/CanPorts/FrameFormatEnumAc.hpp"
#include "Ports/CanPorts/RemoteFrameEnumAc.hpp"

namespace Drv {
    class CANStandardMessage {
        private:
            uint32_t id = 0;
            uint8_t xtd = 0;
            uint8_t dlc = 0;
            uint8_t rtr = 0;
    
            static constexpr uint32_t lenBuffer = 8;
    
            uint8_t getDataLengthCode(uint8_t length) {
                uint8_t dataLen = length;
    
                if (length > lenBuffer) {
                    dataLen = lenBuffer;
                }
    
                return dataLen;
            }
        public:
            CANStandardMessage() = default;
            CANStandardMessage(const etl::span<U8> &dataBuff, uint32_t identifier, Drv::CAN::FrameFormat frameFormat, Drv::CAN::RemoteFrame remoteFrame = Drv::CAN::RemoteFrame::Disable) {
                id = identifier;
                xtd = static_cast<uint8_t>(frameFormat);
                rtr = static_cast<uint8_t>(remoteFrame);
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
    
            uint8_t getRemoteFrameFlag() {
                return rtr;
            }
    
            void getData(etl::span<uint8_t> buff) {
                etl::copy_s(data.begin(), data.end(), buff.begin(), buff.end());
            }
    
            etl::array<uint8_t, lenBuffer> data = {};
    };
}

#endif