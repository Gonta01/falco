// ======================================================================
// \title  QspiDriver.cpp
// \author user
// \brief  cpp file for QspiDriver component implementation class
// ======================================================================

#include "Falco/Drv/QspiDriver/QspiDriver.hpp"
#include "FpConfig.hpp"


namespace Falco {
  using QSPIError = Drv::QSPI::QSPIError;

  // ----------------------------------------------------------------------
  // Component construction and destruction
  // ----------------------------------------------------------------------

  QspiDriver ::
    QspiDriver(const char* const compName) :
      QspiDriverComponentBase(compName)
  {

  }

  QspiDriver ::
    ~QspiDriver()
  {

  }

  // ----------------------------------------------------------------------
  // Handler implementations for user-defined typed input ports
  // ----------------------------------------------------------------------

  QSPIError QspiDriver ::
    commandWrite_handler(
        NATIVE_INT_TYPE portNum,
        Drv::QSPI::QPSICommandConfig& config,
        U32 address
    )
  {
    qspi_command_xfer_t nativeConfig = transformCommandConfig(config);

    QSPIError error = QSPIError::IOError;
    if (QSPI_CommandWrite(&nativeConfig, address)) {
        error = QSPIError::NoError;
    }

    return error;
  }

  QSPIError QspiDriver ::
    memoryRead_handler(
        NATIVE_INT_TYPE portNum,
        Drv::QSPI::QSPIMemoryConfig& config,
        U32 address,
        Fw::Buffer& buff
    )
  {
    qspi_memory_xfer_t nativeConfig = transformMemoryConfig(config);

    QSPIError error = QSPIError::IOError;
    if (QSPI_MemoryRead(&nativeConfig, reinterpret_cast<U32*>(buff.getData()), buff.getSize(), address)) {
      error = QSPIError::NoError;
    }

    return error;
  }

  QSPIError QspiDriver ::
    memoryWrite_handler(
        NATIVE_INT_TYPE portNum,
        Drv::QSPI::QSPIMemoryConfig& config,
        U32 address,
        Fw::Buffer& buff
    )
  {
    qspi_memory_xfer_t nativeConfig = transformMemoryConfig(config);

    QSPIError error = QSPIError::IOError;
    if (QSPI_MemoryWrite(&nativeConfig, reinterpret_cast<U32*>(buff.getData()), buff.getSize(), address)) {
      error = QSPIError::NoError;
    }

    return error;
  }

  QSPIError QspiDriver ::
    registerRead_handler(
        NATIVE_INT_TYPE portNum,
        Drv::QSPI::QSPIRegisterConfig& config,
        Fw::Buffer& buff
    )
  {
    qspi_register_xfer_t nativeConfig = transformRegisterConfig(config);

    QSPIError error = QSPIError::IOError;
    // NOLINTBEGIN(cppcoreguidelines-pro-type-reinterpret-cast)
    if (QSPI_RegisterRead(&nativeConfig, reinterpret_cast<U32 *>(buff.getData()), buff.getSize())) {

        error = QSPIError::NoError;
    }
    // NOLINTEND(cppcoreguidelines-pro-type-reinterpret-cast)

    return error;
  }

  QSPIError QspiDriver ::
    registerWrite_handler(
        NATIVE_INT_TYPE portNum,
        Drv::QSPI::QSPIRegisterConfig& config,
        Fw::Buffer& buff
    )
  {
    qspi_register_xfer_t nativeConfig = transformRegisterConfig(config);
  
    QSPIError error = QSPIError::IOError;
    // NOLINTBEGIN(cppcoreguidelines-pro-type-reinterpret-cast)
    if (QSPI_RegisterWrite(&nativeConfig, reinterpret_cast<uint32_t *>(buff.getData()), buff.getSize())) {
        error = QSPIError::NoError;
    }
    // NOLINTEND(cppcoreguidelines-pro-type-reinterpret-cast)
  
    return error;
  }

  qspi_command_xfer_t QspiDriver::transformCommandConfig(QPSICommandConfig &config) {
    qspi_command_xfer_t nativeConfig{};
    nativeConfig.addr_en = config.getAddr_en();
    nativeConfig.instruction = config.getInstruction();
    nativeConfig.addr_len = static_cast<QSPI_ADDRESS_LENGTH>(config.getAddr_len());
    nativeConfig.width = static_cast<QSPI_LANE_WIDTH>(config.getWidth());
    return nativeConfig;
  }

  qspi_register_xfer_t QspiDriver::transformRegisterConfig(QSPIRegisterConfig &config) {
    qspi_register_xfer_t nativeConfig{};
    nativeConfig.instruction = config.getInstruction();
    nativeConfig.width = static_cast<QSPI_LANE_WIDTH>(config.getWidth());
    nativeConfig.dummy_cycles = config.getDummy_cycles();
    return nativeConfig;
  }

  qspi_memory_xfer_t QspiDriver::transformMemoryConfig(QSPIMemoryConfig &config) {
    qspi_memory_xfer_t nativeConfig{};
    nativeConfig.instruction = config.getInstruction();
    nativeConfig.option = config.getOption();
    nativeConfig.width = static_cast<QSPI_LANE_WIDTH>(config.getWidth());
    nativeConfig.addr_len = static_cast<QSPI_ADDRESS_LENGTH>(config.getAddr_len());
    nativeConfig.option_en = config.getOption_en();
    nativeConfig.option_len = static_cast<QSPI_OPTION_LENGTH>(config.getOption_len());
    nativeConfig.continuous_read_en = config.getContinuous_read_en();
    nativeConfig.dummy_cycles = config.getDummy_cycles();
    return nativeConfig;
  }
}
