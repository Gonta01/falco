// ======================================================================
// \title  ICM20948.hpp
// \author user
// \brief  hpp file for ICM20948 component implementation class
// ======================================================================

#ifndef Components_ICM20948Interface_HPP
#define Components_ICM20948Interface_HPP

#include <cstdint>
#include <etl/span.h>

namespace Interfaces {
    class ICM20948 {
    public:
        enum class ErrorCode : uint8_t {
            NoError = 0,
            NotInited,
            FailedWriteToIMURegister,
            FailedReadIMURegister,
            ReadNotEqualTOWrittenRegister
        };

        enum class RegisterBankSel : uint8_t {
            UserBank0 = 0x00,
            UserBank1 = 0x01,
            UserBank2 = 0x02,
            UserBank3 = 0x03,
            Common
        };

        enum class GyroFullScale : uint8_t {
            Scale250dps = 0x00,
            Scale500dps = 0x01,
            Scale1000dps = 0x02,
            Scale2000dps = 0x03
        };

        enum class AccelFullScale : uint8_t {
            Scale2g = 0x00,
            Scale4g = 0x01,
            Scale8g = 0x02,
            Scale16g = 0x03
        };

        /**
         * Unique IDs of used registers
         */
        enum class AddressID : uint8_t {
            WHO_AM_I = 0,
            USER_CTRL,
            LP_CONFIG,
            PWR_MGMT_1,
            PWR_MGMT_2,
            INT_PIN_CFG,
            INT_ENABLE_1,
            INT_ENABLE_2,
            INT_STATUS,
            INT_STATUS_1,
            INT_STATUS_2,
            INT_STATUS_3,
            SINGLE_FIFO_PRIORITY_SEL,
            ACCEL_XOUT_H,
            GYRO_XOUT_H,
            EXT_SLV_SENS_DATA_00,
            FIFO_EN_1,
            FIFO_EN_2,
            FIFO_RST,
            FIFO_MODE,
            FIFO_COUNTH,
            FIFO_R_W,
            HW_FIX_DISABLE,
            MEM_START_ADDR,
            MEM_R_W,       
            MEM_BANK_SEL,
            TIMEBASE_CORRECTION_PLL,
            GYRO_SMPLRT_DIV,
            GYRO_CONFIG_1,
            ACCEL_SMPLRT_DIV_2,
            ACCEL_CONFIG,
            PRGM_START_ADDRH,
            PRGM_START_ADDRL,
            I2C_MST_ODR_CONFIG,
            I2C_MST_CTRL,
            I2C_SLV0_ADDR,
            I2C_SLV0_REG,
            I2C_SLV0_CTRL,
            I2C_SLV0_DO,
            I2C_SLV1_ADDR,
            I2C_SLV1_REG,
            I2C_SLV1_CTRL,
            I2C_SLV1_DO
        };

        struct AccelData {
            float x;
            float y;
            float z;
        };

        struct GyroData {
            float x;
            float y;
            float z;
        };

        struct MagnData {
            float x;
            float y;
            float z;
        };

        struct Quaternion {
            double Q0;
            double Q1;
            double Q2;
            double Q3;
            int16_t Accuracy;
        };

        struct BankWithAddress {
            RegisterBankSel UserBank;
            uint8_t datasheetAddress;
        };

        struct MagnetWrite {
            uint8_t reg;
            uint8_t value;
        };

        struct MagnetRead {
            uint8_t reg;
            uint8_t length;
        };

        /**
         * Check validity of address map
         * @return true, if valid, otherwise - false
         */
        virtual ErrorCode checkMapValidity() = 0;

        /**
        * Read from ICM-20948 register
        * @param reg - register to read from
        * @param value - value of register
        * @return true, if read successfully; otherwise - false
        */
        virtual bool readReg(AddressID reg, uint8_t &value) = 0;

        /**
        * Burst read from ICM-20948 registers.
        * The amount of read registers is equal to the size of passed buffer
        * @param reg - start register
        * @param buff - buffer where read data will be stored
        * @return true, if read successfully; otherwise - false
        */
        virtual bool burstReadRegs(AddressID reg, etl::span<uint8_t> buff) = 0;

        /**
        * Write to ICM-20948 register
        * @param reg - register to write to
        * @param value - value, that is written to register
        * @return true, if write successfully; otherwise - false
        */
        virtual bool writeReg(AddressID reg, uint8_t value) = 0;

        /**
        * Burst write to ICM-20948 registers.
        * The amount of written registers is equal to the size of passed buffer
        * @param reg - start register
        * @param buff - buffer where values to written are stored
        * @return true, if write successfully; otherwise - false
        */
        virtual bool burstWriteRegs(AddressID reg, etl::span<uint8_t> buff) = 0;

        /**
        * Select register bank of ICM-20948
        * @param regBank
        * @return true, if selected successfully; otherwise - false
        */
        virtual bool selectRegisterBank(RegisterBankSel regBank) = 0;

        /**
         * Reset register bank, so it will be set for sure during next selectRegisterBank call
         */
        virtual void resetRegisterBank() = 0;

        /**
        * Function to check that written value to the register is equal to expected value
        * @param addressID - register address
        * @param expectedValue - value that expected to be stored there
        * @return error code
        */
        virtual ErrorCode checkRegister(AddressID addressID, uint8_t expectedValue) = 0;

        /**
        * Magnetometer initialization
        * @return error code
        */
        virtual ErrorCode initMagnetometer() = 0;

        /**
         * Get magnetometer sensitivity
         * @return magnetometer sensitivity
         */
        virtual float getSensitivity() = 0;

        /**
         * Set sensor overflow status
         * @param status
         */
        virtual void setSensorOverflowStatus(uint8_t status) = 0;

        /**
         * Get sensor overflow status
         * @return
         */
        virtual uint8_t getSensorOverflowStatus() const = 0;
    };
}// Interfaces namespace

#endif