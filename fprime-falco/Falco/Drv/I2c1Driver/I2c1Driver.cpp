#include <Falco/Drv/I2c1Driver/I2c1Driver.hpp>
#include <FpConfig.hpp>
#include "Fw/Types/Assert.hpp"
#include "twihs/master/plib_twihs1_master.h"
#include "FreeRTOS.h"
#include "event_groups.h"

namespace Falco {

  // ----------------------------------------------------------------------
  // Construction, initialization, and destruction
  // ----------------------------------------------------------------------

  I2c1Driver::I2c1Driver(const char *const compName) : I2c1DriverComponentBase(compName) {
    eventGroupHandleI2c1 = xEventGroupCreate();
    TWIHS1_CallbackRegister(I2c1Callback, reinterpret_cast<uintptr_t>(this));
  }

  I2c1Driver::~I2c1Driver() {

  }

  void I2c1Driver::TWIHS1_Deinitialize() {
    // Reset the i2c Module
    TWIHS1_REGS->TWIHS_CR = TWIHS_CR_SWRST_Msk;

    // Disable the I2C Master/Slave Mode
    TWIHS1_REGS->TWIHS_CR = TWIHS_CR_MSDIS_Msk | TWIHS_CR_SVDIS_Msk;

    // Disable TXRDY, TXCOMP and RXRDY interrupts
    TWIHS1_REGS->TWIHS_IDR = TWIHS_IDR_TXCOMP_Msk | TWIHS_IDR_TXRDY_Msk | TWIHS_IDR_RXRDY_Msk;
  }

  void I2c1Driver::i2cBusRecovery() {
    // 1. Disable TWIHS
    TWIHS1_Deinitialize();

    // 2. Set SDA and SCL as GPIO output, drive high
    configureGPIOPins();
    constexpr uint32_t timeOfDelayUs     = 100;
    constexpr uint32_t numOfPinsToggling = 10;

    // 3. Drive SCL high and pulse it manually 9 times;
    for (int i = 0; i < numOfPinsToggling; i++) {
        PIO_PinWrite(PIO_PIN_PA4, false);
 
        SYS_TIME_DelayUS (timeOfDelayUs, &timeHandle);
        while (SYS_TIME_DelayIsComplete(timeHandle) == false);
                
        PIO_PinWrite(PIO_PIN_PA4, true);
        SYS_TIME_DelayUS (timeOfDelayUs, &timeHandle);
        while (SYS_TIME_DelayIsComplete(timeHandle) == false);
    }

    // 4. Generate stop condition manually;
    PIO_PinWrite(PIO_PIN_PA3, false);
    SYS_TIME_DelayUS (timeOfDelayUs, &timeHandle);
    while (SYS_TIME_DelayIsComplete(timeHandle) == false);
    
    PIO_PinWrite(PIO_PIN_PA4, true);
    SYS_TIME_DelayUS (timeOfDelayUs, &timeHandle);
    while (SYS_TIME_DelayIsComplete(timeHandle) == false);

    PIO_PinWrite(PIO_PIN_PA3, true);
    SYS_TIME_DelayUS (timeOfDelayUs, &timeHandle);
    while (SYS_TIME_DelayIsComplete(timeHandle) == false);

    // 5. Reconfigure pins GPIOs on SDA and SCL;
    configureI2CPins();

    TWIHS1_Initialize();
  }

  void I2c1Driver::configureGPIOPins() {
    ((pio_registers_t*)PIO_PORT_B)->PIO_PDR = 0xc;
    ((pio_registers_t*)PIO_PORT_B)->PIO_PER = 0xfffffff3;
    ((pio_registers_t*)PIO_PORT_B)->PIO_MDER = 0x30;
    ((pio_registers_t*)PIO_PORT_B)->PIO_MDDR = 0xffffffcf;
    ((pio_registers_t*)PIO_PORT_B)->PIO_OER = 0x30;
    ((pio_registers_t*)PIO_PORT_B)->PIO_ODR = 0xffffffcf;
  }

  void I2c1Driver::configureI2CPins() {
    ((pio_registers_t*)PIO_PORT_B)->PIO_PDR = 0x3c;
    ((pio_registers_t*)PIO_PORT_B)->PIO_PER = 0xffffffc3;
    ((pio_registers_t*)PIO_PORT_B)->PIO_MDER = 0x0;
    ((pio_registers_t*)PIO_PORT_B)->PIO_MDDR = 0xffffffff;
    ((pio_registers_t*)PIO_PORT_B)->PIO_OER = 0x30;
    ((pio_registers_t*)PIO_PORT_B)->PIO_ODR = 0xffffffcf;
  }

  void I2c1Driver::I2c1Callback(uintptr_t thisObjects) {
    reinterpret_cast<I2c1Driver *>(thisObjects)->callback();
  }

  void I2c1Driver::callback() {
    EventBits_t status = static_cast<EventBits_t>(TWIHS1_ErrorGet());
    BaseType_t xHigherPriorityTaskWoken{0};
    if (status == TWIHS_ERROR::TWIHS_ERROR_NONE) {
      status = transactionComplete;
    }
    xEventGroupSetBitsFromISR(eventGroupHandleI2c1, status, &xHigherPriorityTaskWoken);
  }

  bool I2c1Driver::waitTransactionEnd(U32 timeoutMs) {
    auto ticks = timeoutMs / portTICK_PERIOD_MS; 
    auto transactionResult = xEventGroupWaitBits(eventGroupHandleI2c1,
                        transactionComplete | busError | nakError,
                        pdTRUE,
                        pdFALSE,
                        ticks);
    bool transactionSuccess = (transactionComplete == transactionResult);

    if (transactionResult == TWIHS_ERROR::TWIHS_BUS_ERROR) {
      this->log_WARNING_HI_BusError();
      i2cBusRecovery();
    }
    else if (transactionResult == TWIHS_ERROR::TWIHS_ERROR_NACK) {
      this->log_WARNING_HI_NAKError();
    }
    else {
      if (false == transactionSuccess) {
        this->log_WARNING_HI_TimeoutError();
      }
    }

    return transactionSuccess;
  }

  //! Read data
  Drv::I2C::Status I2c1Driver::readData(U32 addr, Fw::Buffer& fwBuffer, U32 timeout)
  {
    auto i2cStatus = Drv::I2C::Status::SUCCESS;

    bool readStatus = TWIHS1_Read(addr, fwBuffer.getData(), fwBuffer.getSize());

    if (readStatus == true) {
      if(waitTransactionEnd(timeout) == false) {
        this->log_WARNING_HI_ReadError();
        i2cStatus = Drv::I2C::Status::READ_ERR;
      }
    }
    else {
      auto errorReason = TWIHS1_ErrorGet();
      if (errorReason == TWIHS_ERROR::TWIHS_BUS_ERROR) {
        this->log_WARNING_HI_BusError();
        i2cBusRecovery();
      }
      else if (errorReason == TWIHS_ERROR::TWIHS_ERROR_NACK) {
        this->log_WARNING_HI_NAKError();
      }
      else {
        this->log_WARNING_HI_UnknownError();
      }
      this->log_WARNING_HI_ReadError();
      i2cStatus = Drv::I2C::Status::READ_ERR;
    }

    return i2cStatus;
  }

  //! Write data
  Drv::I2C::Status I2c1Driver::writeData(U32 addr, Fw::Buffer& fwBuffer, U32 timeout)
  {
    auto i2cStatus = Drv::I2C::Status::SUCCESS;

    bool writeStatus = TWIHS1_Write(addr, fwBuffer.getData(), fwBuffer.getSize());

    if (writeStatus == true) {
      if(waitTransactionEnd(timeout) == false) {
        this->log_WARNING_HI_WriteError();
        i2cStatus = Drv::I2C::Status::WRITE_ERR;
      }
    } 
    else {
      auto errorReason = TWIHS1_ErrorGet();
      if (errorReason == TWIHS_ERROR::TWIHS_BUS_ERROR) {
        this->log_WARNING_HI_BusError();
        i2cBusRecovery();
      }
      else if (errorReason == TWIHS_ERROR::TWIHS_ERROR_NACK) {
        this->log_WARNING_HI_NAKError();
      }
      else {
        this->log_WARNING_HI_UnknownError();
      }
      this->log_WARNING_HI_WriteError();
      i2cStatus = Drv::I2C::Status::WRITE_ERR;
    }
    
    return i2cStatus;
  }

  // ----------------------------------------------------------------------
  // Handler implementations for user-defined typed input ports
  // ----------------------------------------------------------------------

  Drv::I2C::Status I2c1Driver ::
    read_handler(
        NATIVE_INT_TYPE portNum,
        U32 addr,
        Fw::Buffer& serBuffer,
        U32 timeout
    )
  {
    // Ensure buffer is not a nullptr
    FW_ASSERT(serBuffer.getData() != nullptr);

    return readData(addr, serBuffer, timeout);
  }

  Drv::I2C::Status I2c1Driver ::
    write_handler(
        NATIVE_INT_TYPE portNum,
        U32 addr,
        Fw::Buffer& serBuffer,
        U32 timeout
    )
  {
    // Ensure buffer is not a nullptr
    FW_ASSERT(serBuffer.getData() != nullptr);
    
    return writeData(addr, serBuffer, timeout);
  }

  Drv::I2C::Status I2c1Driver ::
    writeRead_handler(
        NATIVE_INT_TYPE portNum,
        U32 addr,
        Fw::Buffer& writeBuffer,
        Fw::Buffer& readBuffer,
        U32 timeout
    )
  {
    auto i2cStatus = Drv::I2C::Status::SUCCESS;

    bool writeReadStatus = TWIHS1_WriteRead(addr, writeBuffer.getData(), writeBuffer.getSize(), readBuffer.getData(), readBuffer.getSize());

    if (writeReadStatus == true) {
      if(waitTransactionEnd(timeout) == false) {
        this->log_WARNING_HI_WriteReadError();
        i2cStatus = Drv::I2C::Status::WRITE_READ_ERR;
      }
    }
    else {
      auto errorReason = TWIHS1_ErrorGet();
      if (errorReason == TWIHS_ERROR::TWIHS_BUS_ERROR) {
        this->log_WARNING_HI_BusError();
        i2cBusRecovery();
      }
      else if (errorReason == TWIHS_ERROR::TWIHS_ERROR_NACK) {
        this->log_WARNING_HI_NAKError();
      }
      else {
        this->log_WARNING_HI_UnknownError();
      }
      this->log_WARNING_HI_WriteReadError();
      i2cStatus = Drv::I2C::Status::WRITE_READ_ERR;
    }
    
    return i2cStatus;
  }

} // end namespace Falco
