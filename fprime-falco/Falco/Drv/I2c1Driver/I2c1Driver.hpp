#ifndef I2c1Driver_HPP
#define I2c1Driver_HPP

#include "Falco/Drv/I2c1Driver/I2c1DriverComponentAc.hpp"
#include "FreeRTOS.h"
#include "event_groups.h"
#include "twihs/master/plib_twihs1_master.h"
#include "system/time/sys_time.h"
#include "pio/plib_pio.h"

namespace Falco {

  class I2c1Driver :
    public I2c1DriverComponentBase
  {

    public:

      // ----------------------------------------------------------------------
      // Construction, initialization, and destruction
      // ----------------------------------------------------------------------

      //! Construct object I2c1Driver
      //!
      I2c1Driver(
          const char *const compName /*!< The component name*/
      );

      //! Destroy object I2c1Driver
      //!
      ~I2c1Driver();

    PRIVATE:
      EventGroupHandle_t eventGroupHandleI2c1;
      static constexpr EventBits_t nakError            = TWIHS_ERROR::TWIHS_ERROR_NACK;
      static constexpr EventBits_t busError            = TWIHS_ERROR::TWIHS_BUS_ERROR;
      static constexpr EventBits_t transactionComplete = 4;

      static void I2c1Callback(uintptr_t thisObjects);

      void callback();

      bool waitTransactionEnd(U32 timeoutMs);
      
      //! Read the actual data
      Drv::I2C::Status readData(U32 addr, Fw::Buffer& fwBuffer, U32 timeout);

      //! Write the actual data
      Drv::I2C::Status writeData(U32 addr, Fw::Buffer& fwBuffer, U32 timeout);

      // ----------------------------------------------------------------------
      // Handler implementations for user-defined typed input ports
      // ----------------------------------------------------------------------

      // ----------------------------------------------------------------------
      // Handler implementations for user-defined typed input ports
      // ----------------------------------------------------------------------

      //! Handler implementation for read
      Drv::I2C::Status read_handler(
          NATIVE_INT_TYPE portNum, //!< The port number
          U32 addr, //!< I2C slave device address
          Fw::Buffer& serBuffer, //!< Buffer with data to write from
          U32 timeout //!< Max time of waititing for operation ending
      ) override;

      //! Handler implementation for write
      Drv::I2C::Status write_handler(
          NATIVE_INT_TYPE portNum, //!< The port number
          U32 addr, //!< I2C slave device address
          Fw::Buffer& serBuffer, //!< Buffer with data to read to
          U32 timeout //!< Max time of waititing for operation ending
      ) override;

      //! Handler implementation for writeRead
      Drv::I2C::Status writeRead_handler(
        NATIVE_INT_TYPE portNum, //!< The port number
        U32 addr, //!< I2C slave device address
        Fw::Buffer& writeBuffer, //!< Buffer to write data to the i2c device
        Fw::Buffer& readBuffer, //!< Buffer to read back data from the i2c device, must set size when passing in read buffer
        U32 timeout //!< Max time of waititing for operation ending
      ) override;

      void i2cBusRecovery();
      void configureGPIOPins();
      void configureI2CPins();
      void TWIHS1_Deinitialize();
      SYS_TIME_HANDLE timeHandle;

    };

} // end namespace Falco

#endif
