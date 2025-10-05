// ======================================================================
// \title  IOExpander.hpp
// \author user
// \brief  hpp file for IOExpander component implementation class
// ======================================================================

#ifndef Components_IOExpander_HPP
#define Components_IOExpander_HPP

#include "Components/IOExpander/IOExpanderComponentAc.hpp"
#include "Ports/IOExpanderPorts/IOTypeEnumAc.hpp"
#include "Os/Mutex.hpp"
#include "etl/expected.h"
#include "etl/array.h"
#include <etl/span.h>

namespace Components {

  class IOExpander :
    public IOExpanderComponentBase
  {

    public:
      /**
       * Independent configuration of a channel
       */
      struct ChannelConfig {
          Drv::IOExpander::Channel channel;
          Drv::IOExpander::IOType ioType;
      };
      
      using AnalogOffsetsMiliVolts = etl::array<I32, Drv::IOExpander::Channel::ChannelAmount>;
      using MinThresholdsMiliVolts = etl::array<I32, Drv::IOExpander::Channel::ChannelAmount>;
      using MaxThresholdsMiliVolts = etl::array<I32, Drv::IOExpander::Channel::ChannelAmount>;

      struct AnalogConfig {
        const AnalogOffsetsMiliVolts *analogOffsetsMiliVolts;
        const MinThresholdsMiliVolts *minThresholdsMiliVolts;
        const MaxThresholdsMiliVolts *maxThresholdsMiliVolts;
      };

      using ConfigTable   = etl::array<ChannelConfig, Drv::IOExpander::Channel::ChannelAmount>;

      enum OversamplingRatio {
        NO_AVERAGING,
        SAMPLES2,
        SAMPLES4,
        SAMPLES8,
        SAMPLES16,
        SAMPLES32,
        SAMPLES64,
        SAMPLES128
      };

      enum AnalogMeasurementsMode {
        ONE_SHOT,
        AUTO
      };

      // ----------------------------------------------------------------------
      // Component construction and destruction
      // ----------------------------------------------------------------------

      //! Construct IOExpander object
      IOExpander(
          const char* const compName //!< The component name
      );

      //! Destroy IOExpander object
      ~IOExpander();

      //! Configure IO Expander
      void configure(uint8_t devAddr, const ConfigTable *config, const AnalogConfig *analogConfig = NULL, AnalogMeasurementsMode mode = AnalogMeasurementsMode::ONE_SHOT, Components::IOExpander::OversamplingRatio oversamplingRatio = Components::IOExpander::OversamplingRatio::NO_AVERAGING);
      
      PRIVATE:
        Os::Mutex mutex;
        AnalogMeasurementsMode analogMeasurementsMode;
        bool calibrationUnfinished = false;
        bool isCRCEnabled = false;
        bool isOversamplingEnabled = false;
        
        static constexpr U32 waitTimeAfterReset = 6;
        static constexpr U32 writeTimeout = 0xFFFF'FFFF;
        static constexpr U32 readTimeout = 0xFFFF'FFFF;
        static constexpr F32 ADC_VREF{3.3};

        static constexpr U16 maxADCCodeWithOversampling{65535};
        static constexpr U16 maxADCCodeDefault{4095};
        static constexpr U8 maxNumOfCalibResultReadAttempts{5};
        U16 maxADCCode{maxADCCodeDefault};

        uint8_t devAddr;
        const etl::array<ChannelConfig, Drv::IOExpander::Channel::ChannelAmount> *configTable{nullptr};
        bool initializationComplete{false};
        bool thresholdsAreSet{false};
        U32 numOfConfiguredAutoAnalogChannels{0};
        static constexpr U32 thresholdMiliVoltMaxValue = 3300;
        static constexpr U32 voltInMiliVoltCoeff = 1000;

        etl::array<U16, Drv::IOExpander::Channel::ChannelAmount> autoAdcCode;
        AnalogOffsetsMiliVolts analogOffsetsMiliVolts{0};
        MinThresholdsMiliVolts minThresholdsMiliVolts{0};
        MaxThresholdsMiliVolts maxThresholdsMiliVolts{0};
        
        bool isOffsetCalibrationInitiated{false};

        U8 digitalOutputsValue{0};
        // Initial values are set for first measurement.
        // For example, if first measurement is 0V and min threshold is 1V,
        // then taking into account that initial previous value for min estimation is set to max possible voltage (3.3V) the min threshold counter will be incremented;
        // For example, if first measurement is 3.3V and max threshold is 3V,
        // then taking into account that initial previous value for max estimation is set to min possible voltage (0V) the max threshold counter will be incremented;
        etl::array<F32, Drv::IOExpander::Channel::ChannelAmount> previousAutoVoltageValueForMaxValueEstimation{0, 0, 0, 0, 0, 0, 0, 0};
        etl::array<F32, Drv::IOExpander::Channel::ChannelAmount> previousAutoVoltageValueForMinValueEstimation{ADC_VREF, ADC_VREF, ADC_VREF, ADC_VREF, ADC_VREF, ADC_VREF, ADC_VREF, ADC_VREF};
        
        etl::array<F32, Drv::IOExpander::Channel::ChannelAmount> autoVoltageValues;

        etl::array<I32, Drv::IOExpander::Channel::ChannelAmount> voltageOffsetsMiliVolt;
        etl::array<U32, Drv::IOExpander::Channel::ChannelAmount> minThresholdsHitCounter;
        etl::array<U32, Drv::IOExpander::Channel::ChannelAmount> maxThresholdsHitCounter;

      /**
       * General-purpose input/output logic level
       */
      enum GPIOState {
          GpioReset = 0,
          GpioSet = 1
      };
     
      /**
       * NoError if operation completed successfully or error code
       */
      enum ErrorCode {
        NoError,
        NotInitialized,
        WrongConfiguration,
        CommError,
        CrcError,
        CalibrationUnfinished
       };
  
      /**
       * ADS7138 opcodes
       */
      enum ADS7138_OPCODE {
          READ = 0x10,
          WRITE = 0x08
      };

      /**
       * ADS7138 registers
       */
      enum ADS7138_REG {
          SYSTEM_STATUS   = 0x0,
          GENERAL_CFG     = 0x1,
          DATA_CFG        = 0x2,
          OSR_CFG         = 0x3,
          PIN_CFG         = 0x5,
          GPIO_CFG        = 0x7,
          GPO_DRIVE_CFG   = 0x9,
          GPO_VALUE       = 0xB,
          GPI_VALUE       = 0xD,
          SEQUENCE_CFG    = 0x10,
          CHANNEL_SEL     = 0x11,
          AUTO_SEQ_CH_SEL = 0x12,
          ALERT_MAP       = 0x16,
          ALERT_PIN_CFG   = 0x17
      };
  
      /**
       * GENERAL_CFG Register Fields
       */
      enum ADS7138_GENERAL_CFG_REG {
        CRC_EN   = 0x40,
        STATS_EN = 0x20,
        CNVST    = 0x8,
        CAL      = 0x2,
        RST      = 0x1
      };
  
      /**
       * DATA_CFG Register Fields
       */
      enum ADS7138_DATA_CFG_REG {
          APPEND_STATUS = 0x10
      };

      /**
       * SEQ_CFG Register Fields
       */
      enum ADS7138_SEQ_CFG_REG {
        SEQ_START = 0x10,
        SEQ_MODE  = 0x3
    };

      /**
       * SYSTEM_STATUS Register Fields
       */
      enum ADS7138_SYSTEM_STATUS_REG {
          DEFAULT      = 0x81,
          OSR_DONE     = 0x8,
          CRC_ERR_FUSE = 0x4,
          CRC_ERR_IN   = 0x2,
          BOR          = 0x1
      };
  
      /**
       * ALERT_MAP Register Fields
       */
      enum ADS7138_ALERT_MAP_REG {
        ALERT_CRCIN = 0x1
      };
  
      /**
       * ALERT_PIN_CFG Register Fields
       */
      enum ADS7138_ALERT_PIN_CFG_REG {
        ALERT_DRIVE = 0x4,
        ALERT_LOGIC = 0x3
      };

      // ----------------------------------------------------------------------
      // Handler implementations for user-defined typed input ports
      // ----------------------------------------------------------------------
      
      //! Handler implementation for getNumOfThresholdHit
      void getNumOfThresholdHit_handler(
        NATIVE_INT_TYPE portNum, //!< The port number
        const Drv::IOExpander::Channel& channel,
        Drv::IOExpander::NumOfThresholdHitData& data
      ) override;

      //! Handler implementation for readAnalogInput
      void readAnalogInput_handler(
          NATIVE_INT_TYPE portNum, //!< The port number
          const Drv::IOExpander::Channel& channel,
          Drv::IOExpander::ReadAnaloglInputData& data
      ) override;

      //! Handler implementation for readDigitalMultipleInputs
      void readDigitalMultipleInputs_handler(
          NATIVE_INT_TYPE portNum, //!< The port number
          const Drv::IOExpander::Ports& ports, //!< Ports to work with
          Drv::IOExpander::DigitalMultipleInputsData& data
      ) override;

      //! Handler implementation for readDigitalSingleInput
      void readDigitalSingleInput_handler(
          NATIVE_INT_TYPE portNum, //!< The port number
          const Drv::IOExpander::Channel& channel,
          Drv::IOExpander::DigitalSingleInputData& data
      ) override;

      //! Handler implementation for run
      //!
      //! Receiving calls from the rate group
      void run_handler(
          NATIVE_INT_TYPE portNum, //!< The port number
          NATIVE_UINT_TYPE context //!< The call order
      ) override;

      //! Handler implementation for writeDigitalMultipleOutputs
      void writeDigitalMultipleOutputs_handler(
        NATIVE_INT_TYPE portNum, //!< The port number
        const Drv::IOExpander::DigitalMultipleOutputsData& data,
        bool& IsValid
    ) override;

    //! Handler implementation for writeDigitalSingleOutput
    void writeDigitalSingleOutput_handler(
        NATIVE_INT_TYPE portNum, //!< The port number
        const Drv::IOExpander::DigitalSingleOutputData& data,
        bool& IsValid
    ) override;

    //! Handler implementation for calibrateADCOffset
    void calibrateADCOffset_handler(
      NATIVE_INT_TYPE portNum //!< The port number
    ) override;

    /**
     * Function to initialize value of digital outputs
     * @return true, if successfully enabled, otherwise - false
     */  
    [[nodiscard]] bool initDigitalOutputs();

      /**
       * Caclulate CRC8 of the byte with the usage of CRC-8-CCITT polynomial (x8 + x2 + x + 1)
       */
      [[nodiscard]] U8 calculateCRC8(U8 byte);

      /**
       * Enable alert notification for CRC error on input data
       * @return true, if successfully enabled, otherwise - false
       */
      [[nodiscard]] bool enableCRCAlert();

      /**
       * Configure ALERT pin to be asserted as active high
       * @return true, if successfully configured, otherwise - false
       */
      [[nodiscard]] bool configAlertPin();

      /**
       * Check whether IOExpander communicates through CRC or not
       * Variable isCRCEnabled will be modified
       * @return true, if succesfully conducted check, otherwise - false
       */
      [[nodiscard]] bool checkCRCEnabled();
      
      /**
       * Check power up configuration
       * @return true, if everything ok, otherwise - false
       */
      [[nodiscard]] bool checkPowerUpConfig();

      /**
       * Perform software reset of IOExpander
       * @return true, if succesfully performed software reset, otherwise - false
       */
      [[nodiscard]] bool softwareReset();

      /**
       * Enable CRC module for checking the integrity of the data bits exchanged over the I2C interface. 
       * @return true, if successfully enabled, otherwise - false
       */
      [[nodiscard]] bool crcEnable();

      /**
       * Check CRC status of ioExpander
       * @return true, if
       */
      [[nodiscard]] bool checkCRCError();

      /**
       * Clear CRC error
       * @return true, if successfully cleared, otherwise - false
       */
      [[nodiscard]] bool clearCrcErrorBit();
     
      /**
       * Set Digital Output Value
       * @param channel - analog input channel
       * @param param state - new channel state
       */  
      [[nodiscard]] IOExpander::ErrorCode setDigitalOutputValue(const Drv::IOExpander::Channel& channel, Fw::Logic state);

      /**
       * Initialization and configuration of channels
       * @param config Channel configuration table
       * @return ErrorCode
       */
      [[nodiscard]] IOExpander::ErrorCode setConfig(const ConfigTable *config);

      /**
       * Set oversampling rate for averagin filter
       * @param oversamplingRatio
       * @return ErrorCode
       */
      [[nodiscard]] IOExpander::ErrorCode setOversamplingRatio(Components::IOExpander::OversamplingRatio oversamplingRatio);

      /**
       * Configure auto sequence mode
       * @return ErrorCode
       */
      [[nodiscard]] IOExpander::ErrorCode configAutoSeqMode();

      /**
       * Configure manual sequence mode
       * @return ErrorCode
       */
      [[nodiscard]] IOExpander::ErrorCode configManualSeqMode();

      /**
       * Append channel ID to output data
       * @return ErrorCode
       */
      [[nodiscard]] IOExpander::ErrorCode appendChannelID();

      /**
       * Start of channel sequence
       * @return ErrorCode
       */
      [[nodiscard]] IOExpander::ErrorCode seqStart();
      
      /**
       * Enable statistics module
       * @return ErrorCode
       */
      [[nodiscard]] IOExpander::ErrorCode enableStats();

      /**
       * Disable statistics module
       * @return ErrorCode
       */
      [[nodiscard]] IOExpander::ErrorCode disableStats();

      /**
       * Start conversion
       * @return ErrorCode
       */
      [[nodiscard]] IOExpander::ErrorCode startConversion();

      /** 
       * Function to poll that averaging result is ready
       */
      [[nodiscard]] etl::expected<bool, ErrorCode> isAveragingResultReady();

      /**
       * Get the analog input value of the configured channel
       * @param channel Channel number
       * @return analog input value or ErrorCode
       */
      [[nodiscard]] etl::expected<uint16_t, ErrorCode> getAnalogValue(Drv::IOExpander::Channel channel);

      /**
       * Poll analog inputs in Auto mode
       * @return ErrorCode
       */
      [[nodiscard]] ErrorCode pollAutoAnalogIn();

      /**
       * Get the digital input state of the configured channel
       * @param channel Channel number
       * @return digital input state or ErrorCode
       */
      [[nodiscard]] etl::expected<GPIOState, ErrorCode> getDigitalInput(Drv::IOExpander::Channel channel);

      /**
       * Set the digital output state of the configured channel
       * @param channel Channel number
       * @param state Output state
       * @return ErrorCode
       */
      [[nodiscard]] ErrorCode setDigitalOutput(Drv::IOExpander::Channel channel, GPIOState state);

      /**
       * Write one register value via i2c (automatically identifies whether CRC used or not)
       * @param reg Register address
       * @param regVal Value to be written
       * @return true if write succeeds, false otherwise
       */
      [[nodiscard]] bool writeReg(uint8_t reg, uint8_t regVal);

      /**
       * Write one register value via i2c (+ CRC byte)
       * @param reg Register address
       * @param regVal Value to be written
       * @return true if write succeeds, false otherwise
       */
      [[nodiscard]] bool writeRegWithCRC(uint8_t reg, uint8_t regVal);

      /**
       * Write one register value via i2c (no CRC byte)
       * @param reg Register address
       * @param regVal Value to be written
       * @return true if write succeeds, false otherwise
       */
      [[nodiscard]] bool writeRegWithoutCRC(uint8_t reg, uint8_t regVal);

     /**
       * Read one register value via i2c (automatically identifies whether CRC used or not)
       * @param reg Register address
       * @param regVal Reference to read value
       * @return true if write succeeds, false otherwise
       */
      [[nodiscard]] bool readReg(uint8_t reg, uint8_t *regVal);

      /**
       * Read one register value via i2c (no CRC byte)
       * @param reg Register address
       * @param regVal Reference to read value
       * @return true if write succeeds, false otherwise
       */
      [[nodiscard]] bool readRegWithoutCRC(uint8_t reg, uint8_t *regVal);

      /**
       * Read one register value via i2c (+ CRC byte)
       * @param reg Register address
       * @param regVal Reference to read value
       * @return true if write succeeds, false otherwise
       */
      [[nodiscard]] bool readRegWithCRC(uint8_t reg, uint8_t *regVal);

      /**
       * Read ADC data
       * @param isCRCEnabled          - parameter that marks whether CRC is enabled or not
       * @param isOversamplingEnabled - parameter that marks whether oversampling (averaging) is enabled or not
       * @param data                  - ADC data
       * @param channel               - channel ID received among with data (could be used for addition checking in case of manual mode) 
       * @return ErrorCode
       */
      [[nodiscard]] IOExpander::ErrorCode readADCData(bool isCRCEnabled, bool isOversamplingEnabled, U16 *data, Drv::IOExpander::Channel& channel);
      
      /**
       * Functions to report error
       */
      void errorReport(ErrorCode errorCode);

      /**
       * Function to transform U8 id in Drv::IOExpander::Channel data type
       * @param id - parameter that must be transformed, (received in data packet from IOExpander)
       * @return channel in case of success or ErrorCode in case of failure
       */
      [[nodiscard]] etl::expected<Drv::IOExpander::Channel, ErrorCode> getChannelBasedOn(U8 id);

      /**
       * Function to perform ADC offset calibration
       * @return ErrorCode
       */
      [[nodiscard]] ErrorCode calibrateADCOffset();

  };

}

#endif
