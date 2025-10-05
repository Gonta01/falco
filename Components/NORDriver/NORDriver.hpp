// ======================================================================
// \title  NORDriver.hpp
// \author user
// \brief  hpp file for NORDriver component implementation class
// ======================================================================

#ifndef Components_NORDriver_HPP
#define Components_NORDriver_HPP

#include "Components/NORDriver/NORDriverComponentAc.hpp"
#include "Os/Mutex.hpp"
#include "etl/expected.h"
#include "etl/span.h"
#include <cstddef>
#include <cstdint>

namespace Components {
  /**
   * WARNING!!!
   * W25Q01JV is comprised of two dies
   * Each die has their individual Status Register, that provides the current state of the die
   * Before changing code, W25Q01JV datasheet and Two Die Stack Operations Application Note should be carefully
   * considered
   */

  /**
   * Driver of W25Q01JV memory. The memory has capacity of 1GBit (128 MByte)
   * and uses QSPI bus as communication interface
   */
    
  class NORDriver :
    public NORDriverComponentBase
  {

    public:
      enum PowerDown : U8 {
        Disable,
        Enable
      };

      // ----------------------------------------------------------------------
      // Component construction and destruction
      // ----------------------------------------------------------------------

      //! Construct NORDriver object
      NORDriver(
          const char* const compName //!< The component name
      );

      //! Destroy NORDriver object
      ~NORDriver();

      /**
       * External flash memory configuration
       * @return NoError in case of success, otherwise - error code
       */
      void configure(PowerDown powerDownMode);

    PRIVATE:
      
      enum WaitProcess : U8 {
        Default,
        WriteStatusRegister,
        PageProgram,
        SectorErase4KB,
        BlockErase32KB,
        BlockErase64KB,
        ChipErase
      };

      // ----------------------------------------------------------------------
      // Handler implementations for user-defined typed input ports
      // ----------------------------------------------------------------------

      //! Handler implementation for erase
      Drv::NOR::ErrorCode erase_handler(
        NATIVE_INT_TYPE portNum, //!< The port number
        U32 startAddress,
        U32 length
      ) override;

      //! Handler implementation for read
      Drv::NOR::ErrorCode read_handler(
          NATIVE_INT_TYPE portNum, //!< The port number
          U32 startAddress,
          Fw::Buffer& dataInBuff
      ) override;

      //! Handler implementation for write
      Drv::NOR::ErrorCode write_handler(
          NATIVE_INT_TYPE portNum, //!< The port number
          U32 startAddress,
          Fw::Buffer& dataOutBuff
      ) override;

      //! Handler implementation for getSectorSize
      U32 getSectorSize_handler(
        NATIVE_INT_TYPE portNum //!< The port number
      ) override;

      //! Handler implementation for getMemorySize
      U32 getMemorySize_handler(
        NATIVE_INT_TYPE portNum //!< The port number
      ) override;

      /**
       * Read data from external flash memory
       * @param startAddress - start address of read operation
       * @param dataIn - buffer where read data is stored
       * @return data buffer size in case of success, otherwise - error code
       */
      etl::expected<uint32_t, Drv::NOR::ErrorCode> read(uint32_t startAddress, etl::span<uint32_t> dataIn);

      /**
       * Write data to external flash memory
       * @param startAddress - start address of write operation
       * @param dataOut - buffer that will be written to external flash
       * @return data buffer size in case of success, otherwise - error code
       */
      etl::expected<uint32_t, Drv::NOR::ErrorCode> write(uint32_t startAddress, etl::span<uint32_t> dataOut);

      /**
       * Perform full erase of external memory
       * @return NoError in case of success, otherwise - error code
       */
      Drv::NOR::ErrorCode fullErase();

      /**
       * Erase external flash memory at the given range
       * @param startAddress - start address of erase operation
       * @param length - in bytes
       * @return
       */
      Drv::NOR::ErrorCode erase(uint32_t startAddress, size_t length);

      /**
       * Function to reset the device.
       * @return error code
       */
     Drv::NOR::ErrorCode reset();

      static constexpr uint32_t memorySizeBytes = 128U * 1024U * 1024U;  /* FLASH size 128 MB*/
      static constexpr uint32_t W25Q01JV_ID = 0x2170EFUL;             /* FLASH identification*/

      static constexpr uint32_t clear = ~0U;
      static constexpr uint32_t sizeOfPage = 256;
      static constexpr uint32_t sizeOfSectorBytes = 4096;
      static constexpr uint32_t size_4KB = 4096;
      static constexpr uint32_t size_32KB = 32768;
      static constexpr uint32_t size_64KB = 65536;
      static constexpr uint32_t mask_4KB = (size_4KB - 1);
      static constexpr uint32_t mask_32KB = (size_32KB - 1);
      static constexpr uint32_t mask_64KB = (size_64KB - 1);

      static constexpr uint16_t writeStatusWaitMs = 10;
      static constexpr uint16_t writeStatusLoopCnt = 3;
      static constexpr uint16_t pageProgramWaitMs = 2;
      static constexpr uint16_t pageProgramLoopCnt = 7;
      static constexpr uint16_t sectorErase4KBWaitMs = 50;
      static constexpr uint16_t sectorErase4KBLoopCnt = 10;
      static constexpr uint16_t blockErase32KBWaitMs = 120;
      static constexpr uint16_t blockErase32KBLoopCnt = 16;
      static constexpr uint16_t blockErase64KBWaitMs = 150;
      static constexpr uint16_t blockErase64KBLoopCnt = 16;
      static constexpr uint32_t chipEraseWaitMs = 200'000;
      static constexpr uint16_t chipEraseLoopCnt = 6;
      static constexpr uint16_t defaultWaitMs = 2;
      static constexpr uint16_t defaultLoopCnt = 5;

      enum class commandCode : uint8_t {
          /* Command to reset the flash. */
          resetDevice = 0x99, // QPI mode compatible
          resetEnable = 0x66, // QPI mode compatible

          /* Commands to enter/exit 4-byte address mode */
          enter4ByteAddressMode = 0xB7, // QPI mode compatible
          exit4ByteAddressMode = 0xE9,

          /*Power down*/
          powerDown = 0xB9,
          releasePowerDown = 0xAB,

          /* Command to read JEDEC-ID of the flash device. */
          readJedecId = 0x9F, // QPI mode compatible

          /* Command to perform Fast Read in QUAD IO Mode */
          fastReadQuadIO = 0xEB,
          fastReadQuadIO4byte = 0xEC, // X Not compatible with QPI mode

          /* Command read data*/
          readData = 0x03,
          readData4byte = 0x13,

          /* Command to perform Fast Read */
          readFastData = 0x0B, // QPI mode compatible
          readFastData4byte = 0x0C,
          readFastDataQuadOutput4byte = 0x6C,

          /* Page Program command. */
          pageProgram = 0x02,          // QPI mode compatible
          pageProgram4byte = 0x12,
          pageProgramQuad = 0x32,
          pageProgramQuad4byte = 0x34, // X Not compatible with QPI mode

          /* Write enable command. */
          volatileWriteEnable = 0x50,  // QPI mode compatible
          writeEnable = 0x06,          // QPI mode compatible
          writeDisable = 0x04,

          /* Command to write the Flash status register. */
          writeStatusRG1 = 0x01,      // QPI mode compatible
          writeStatusRG2 = 0x31,      // QPI mode compatible
          writeStatusRG3 = 0x11,      // QPI mode compatible

          /* Command to write the Flash status register. */
          readStatusRG1 = 0x05,        // QPI mode compatible 
          readStatusRG2 = 0x35,        // QPI mode compatible 
          readStatusRG3 = 0x15,        // QPI mode compatible 

          /* Command to perform sector erase */
          sectorErase = 0x20,          // QPI mode compatible
          sectorErase4byte = 0x21,
          blockErase32KB = 0x52,       // QPI mode compatible
          blockErase64KB = 0xD8,       // QPI mode compatible
          blockErase64KB4byte = 0xDC,

          /* Command to perform enter into QPI mode*/
          enterQPIMode = 0x38,        // Single SPI mode

          /* Command to perform exit from QPI mode*/
          exitQPIMode = 0xFF,         // QPI mode only
          
          /* Command to set read parameters */
          setReadParameters = 0xC0,   // QPI mode compatible

          /* Command to perform Chip erase */
          softwareDieSelect = 0xC2,   // QPI mode compatible

          /* Command to perform Chip erase */
          chipErase = 0xC7            // QPI mode compatible
      };

      enum class StatusRegister : uint8_t {
          RG1 = 0,
          RG2,
          RG3
      };

      enum class Die : uint8_t {
          Die0 = 0,
          Die1 = 1
      };

      enum class NumOfDummyClocks : uint8_t {
        Clocks2 = 0b0000'0000,
        Clocks4 = 0b0001'0000,
        Clocks6 = 0b0010'0000,
        Clocks8 = 0b0011'0000,
      };

      Os::Mutex mutex;
      uint8_t qpiModeReadClocks = 0;

      /**
        * Write enable should be called before send "erase" or "write" command
        * This function unlock the memory and make it possible to write data in memory
        * @return NoError in case of success
        */
      Drv::NOR::ErrorCode writeEnable();

      /**
       * This function allows to write the volatile values into the Status Register bits
       * @return NoError in case of success
       */
      Drv::NOR::ErrorCode writeEnableVolatile(const Drv::QSPI::QSPILaneWidth &QSPILaneWidth);

      /**
       * Read status register value
       * @param statusRegister
       * @return status register value in case of success, otherwise - error code
       */
      etl::expected<uint8_t, Drv::NOR::ErrorCode> getStatusRegister(StatusRegister statusRegister, const Drv::QSPI::QSPILaneWidth &QSPILaneWidth);

      /**
       * Write the value to status register
       * @param statusRegister
       * @param value
       * @return NoError in case of success
       */
      Drv::NOR::ErrorCode writeRegister(NORDriver::StatusRegister statusRegister, uint8_t value, const Drv::QSPI::QSPILaneWidth &QSPILaneWidth);

      /**
       * Read device JEDEC ID
       * @return device JEDEC ID in case of success, otherwise - false
       */
      etl::expected<uint32_t, Drv::NOR::ErrorCode> readID();

      /**
       * Erase 64KB Block
       * @param startAddress
       * @return NoError in case of success
       */
      Drv::NOR::ErrorCode eraseBlock64kb(uint32_t startAddress);

      /**
       * Erase 32KB Block
       * @param startAddress
       * @return NoError in case of success
       */
      Drv::NOR::ErrorCode eraseBlock32kb(uint32_t startAddress);

      /**
       * Erase 4KB Block
       * @param startAddress
       * @return NoError in case of success
       */
      Drv::NOR::ErrorCode eraseBlock4kb(uint32_t startAddress);

      /**
       * Wait until BUSY bit of Status Register-1 will be 0
       * @param process - process after which wait function is initiated
       * Different process are needed because time for which BUSY bit may stay 1 is significantly different
       * Timing for each process are described in datasheet for W25Q01JV_DTR
       * @return NoError in case of success
       */
      Drv::NOR::ErrorCode waitUntilFlashReady(WaitProcess process);

      /**
       * Wait until WEL bit of Status Register-1 will be 0
       * @return NoError in case of success
       */
      Drv::NOR::ErrorCode waitForWriteEnableLatch();

      /**
       * Activate 4 Byte address mode of flash memory
       * @return NoError in case of success
       */
      Drv::NOR::ErrorCode enter4ByteAddressMode();

      /**
       * Activate QE bit of Status Register-2
       * This is needed to use Quad mode commands
       * @return NoError in case of success
       */
      Drv::NOR::ErrorCode quadEnable(const Drv::QSPI::QSPILaneWidth &QSPILaneWidth);

      /**
       * Enter QPI mode
       * @return NoError in case of success
       */
      Drv::NOR::ErrorCode enterQPIMode();

      /**
       * Exit QPI mode
       * @return NoError in case of success
       */
      Drv::NOR::ErrorCode exitQPIMode();

      /**
       * Choose which die will be active
       * @param dieID
       * @return NoError in case of success
       */
      Drv::NOR::ErrorCode selectDie(Die dieID);
      
      /**
       * Function to set read parameters
       * @param numOfDummyClocks
       * @return NoError in case of success
       */
      Drv::NOR::ErrorCode setReadParameters(NumOfDummyClocks numOfDummyClocks);

      /**
       * Function to execute power down instruction
       * The power down is characterised by lower power consumption 
       * @return NoError in case of success
       */
      Drv::NOR::ErrorCode powerDown();

      /**
       * Function to execute release power down instruction
       * @return NoError in case of success
       */
      Drv::NOR::ErrorCode releasePowerDown();

      /**
       * Function to perform error reporting
       */
      void errorReport(Drv::NOR::ErrorCode errorCode);
  };

}

#endif
