// ======================================================================
// \title  QspiDriver.hpp
// \author user
// \brief  hpp file for QspiDriver component implementation class
// ======================================================================

#ifndef Falco_QspiDriver_HPP
#define Falco_QspiDriver_HPP

#include "Falco/Drv/QspiDriver/QspiDriverComponentAc.hpp"

#include "qspi/plib_qspi.h"
#include "qspi/plib_qspi_common.h"
#include "xdmac/plib_xdmac.h"
#include "xdmac/plib_xdmac_common.h"

namespace Falco {
  using QPSICommandConfig = Drv::QSPI::QPSICommandConfig;
  using QSPIRegisterConfig = Drv::QSPI::QSPIRegisterConfig;
  using QSPIMemoryConfig = Drv::QSPI::QSPIMemoryConfig;

  class QspiDriver :
    public QspiDriverComponentBase
  {

    public:

      // ----------------------------------------------------------------------
      // Component construction and destruction
      // ----------------------------------------------------------------------

      //! Construct QspiDriver object
      QspiDriver(
          const char* const compName //!< The component name
      );

      //! Destroy QspiDriver object
      ~QspiDriver();

      //! Configure driver
      //!
      void configure();

      PRIVATE:
      // ----------------------------------------------------------------------
      // Handler implementations for user-defined typed input ports
      // ----------------------------------------------------------------------

      //! Handler implementation for commandWrite
      //!
      //! QSPI command write
      Drv::QSPI::QSPIError commandWrite_handler(
        NATIVE_INT_TYPE portNum, //!< The port number
        Drv::QSPI::QPSICommandConfig& config,
        U32 address
    ) override;

    //! Handler implementation for memoryRead
    //!
    //! QSPI memory read
    Drv::QSPI::QSPIError memoryRead_handler(
        NATIVE_INT_TYPE portNum, //!< The port number
        Drv::QSPI::QSPIMemoryConfig& config,
        U32 address,
        Fw::Buffer& buff
    ) override;

    //! Handler implementation for memoryWrite
    //!
    //! QSPI memory write
    Drv::QSPI::QSPIError memoryWrite_handler(
        NATIVE_INT_TYPE portNum, //!< The port number
        Drv::QSPI::QSPIMemoryConfig& config,
        U32 address,
        Fw::Buffer& buff
    ) override;

    //! Handler implementation for registerRead
    //!
    //! QSPI register read
    Drv::QSPI::QSPIError registerRead_handler(
        NATIVE_INT_TYPE portNum, //!< The port number
        Drv::QSPI::QSPIRegisterConfig& config,
        Fw::Buffer& buff
    ) override;

    //! Handler implementation for registerWrite
    //!
    //! QSPI register write
    Drv::QSPI::QSPIError registerWrite_handler(
        NATIVE_INT_TYPE portNum, //!< The port number
        Drv::QSPI::QSPIRegisterConfig& config,
        Fw::Buffer& buff
    ) override;

    qspi_command_xfer_t transformCommandConfig(QPSICommandConfig &config);
    qspi_register_xfer_t transformRegisterConfig(QSPIRegisterConfig &config);
    qspi_memory_xfer_t transformMemoryConfig(QSPIMemoryConfig &config);

  };

}

#endif
