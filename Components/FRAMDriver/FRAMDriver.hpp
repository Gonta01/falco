// ======================================================================
// \title  FRAMDriver.hpp
// \author user
// \brief  hpp file for FRAMDriver component implementation class
// ======================================================================

#ifndef Components_FRAMDriver_HPP
#define Components_FRAMDriver_HPP

#include "Components/FRAMDriver/FRAMDriverComponentAc.hpp"
#include "etl/expected.h"
#include "etl/span.h"

namespace Components {

  class FRAMDriver :
    public FRAMDriverComponentBase
  {

    public:

      // ----------------------------------------------------------------------
      // Component construction and destruction
      // ----------------------------------------------------------------------

      //! Construct FRAMDriver object
      FRAMDriver(
          const char* const compName //!< The component name
      );

      //! Destroy FRAMDriver object
      ~FRAMDriver();

      /**
       * @brief Configuration function
       */
      void configure();


    PRIVATE:

      // ----------------------------------------------------------------------
      // Handler implementations for user-defined typed input ports
      // ----------------------------------------------------------------------

      //! Handler implementation for read
      Drv::FRAM::InStatus read_handler(
          NATIVE_INT_TYPE portNum, //!< The port number
          U32 addr,
          Fw::Buffer& buffer
      ) override;

      //! Handler implementation for sleep
      Drv::FRAM::InStatus sleep_handler(
          NATIVE_INT_TYPE portNum //!< The port number
      ) override;

      //! Handler implementation for wakeup
      Drv::FRAM::InStatus wakeup_handler(
          NATIVE_INT_TYPE portNum //!< The port number
      ) override;

      //! Handler implementation for write
      Drv::FRAM::InStatus write_handler(
          NATIVE_INT_TYPE portNum, //!< The port number
          U32 addr,
          Fw::Buffer& buffer
      ) override;

      //! Handler implementation for reinit
      void reinit_handler(
          NATIVE_INT_TYPE portNum //!< The port number
      ) override;

      // ----------------------------------------------------------------------
      // Internal implementation
      // ----------------------------------------------------------------------
      /**
      * @brief Enumerate class of error codes
      */
      enum class ErrorCode : uint8_t {
          NoError,
          WrongMemoryId,
          InputBufferEmpty,
          AddressOutOfRange,
          DriverBusy,
          IOInterfaceError
      };
        /**
         * @brief Initialization function
         * @return NoError if driver initialized successfully
         */
        ErrorCode initialize();

       /**
         * @brief Read data at "address"
         * @param addr - start address of data read
         * @param dataIn - buffer where data will be stored
         * @return ErrorCode or number of received data bytes
         */
        etl::expected<uint32_t, ErrorCode> read(uint32_t addr, Fw::Buffer &dataIn);

        /**
         * @brief Write data at "address"
         * @param addr - start address where data should be writen
         * @param dataOut - data buffer to be send
         * @return ErrorCode or number of bytes were writen
         */
        etl::expected<uint32_t, ErrorCode> write(uint32_t addr, Fw::Buffer &dataOut);

        /**
         * @brief Wake up from Sleep mode
         * @return NoError in case of success
         */
        ErrorCode wakeup();

        /**
         * @brief Put to Sleep mode
         * @return NoError in case of success
         */
        ErrorCode sleep();

        /**
         * @brief Read FRAM ID
         * @return ErrorCode or number read data
         */
        [[nodiscard]] etl::expected<uint32_t, ErrorCode> readID();

      /**
      * @brief Enumerate class for command codes
      */
      enum class CommandCode : uint8_t {
          CmdWriteStatus [[maybe_unused]] = 0x01U,           /*!< Write Status Register*/
          CmdWrite = 0x02U,                                  /*!< Write Memory Code*/
          CmdRead = 0x03U,                                   /*!< Read Memory Code*/
          CmdWriteDisable [[maybe_unused]] = 0x04U,          /*!< Reset Write Enable Latch*/
          CmdReadStatus = 0x05U,                             /*!< Read Status Register*/
          CmdWriteEnable = 0x06U,                            /*!< Set Write Enable Latch*/
          CmdReadFast [[maybe_unused]] = 0x0BU,              /*!< Fast Read Memory Code*/
          CmdReadID = 0x9FU,                                 /*!< Read Device ID*/
          CmdSleep = 0xB9U,                                  /*!< Sleep Mode*/
      };

      static constexpr uint32_t writeTimeoutMs = 0xFFFF'FFFF;
      static constexpr uint32_t readTimeoutMs  = 0xFFFF'FFFF;

      static constexpr uint32_t memorySize = 512U * 1024U;   /*!< FRAM size 512 kB*/
      static constexpr uint32_t MB85RS4MT_ID = 0x047F4903LU; /*!< FRAM identification*/

      static constexpr uint32_t fourthByteOffset = 24U;
      static constexpr uint32_t thirdByteOffset = 16U;
      static constexpr uint32_t secondByteOffset = 8U;
      static constexpr uint32_t firstByteOffset = 0U;

      static constexpr uint32_t wakeupDelayTimeMs = 1U; /*!< Wake up time for FRAM*/
      bool isInited;
      /**
       * @brief Function for reading status register
       * @return NoError in case of success
       */
      etl::expected<uint8_t, ErrorCode> readStatusRegister();

      /**
       * @brief Function for writing status register
       * @return NoError in case of success
       */
      etl::expected<void, ErrorCode> writeStatusRegister(uint8_t);

      /**
        * @brief Write enable
        *        This function unlock the memory and make it possible to store data in memory
        * @return NoError in case of success
        */
      etl::expected<void, ErrorCode> writeEnable();

      /**
        * @brief Write disable
        *        This function lock the memory and make it not possible to store data in memory
        * @return NoError in case of success
        */
      etl::expected<void, ErrorCode> writeDisable();

      /**
       * Function to set chip select pin
       */
      void chipSelectSet();

      /**
       * Function to reset chip select pin
       */
      void chipSelectReset();
  };

}

#endif
