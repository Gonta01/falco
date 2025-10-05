// ======================================================================
// \title  Adc1Driver.hpp
// \author user
// \brief  hpp file for Adc1Driver component implementation class
// ======================================================================

#ifndef Falco_Adc1Driver_HPP
#define Falco_Adc1Driver_HPP

#include "Falco/Drv/Adc1Driver/Adc1DriverComponentAc.hpp"
#include "Falco/Drv/AdcCommon/ADCBase.hpp"

namespace Falco {

  class Adc1Driver :
    public Adc1DriverComponentBase, public Base::ADC
  {

    public:

      // ----------------------------------------------------------------------
      // Component construction and destruction
      // ----------------------------------------------------------------------

      //! Construct Adc1Driver object
      Adc1Driver(
          const char* const compName //!< The component name
      );

      //! Destroy Adc1Driver object
      ~Adc1Driver();

      //! Configure driver
      //!
      void configure();

    PRIVATE:
      /**
       * @brief Function to start ADC conversion and sample buffer filling
       */
      void startConversion() override;

      /**
       * @brief DMA callback function. Function calls from DMA interrupt routine after DMA buffer full filled
       * @param event - DMA event
       * @param xdmacContext - user data
       */
      static void DMACallback(XDMAC_TRANSFER_EVENT event, uintptr_t xdmacContext);
      
      // ----------------------------------------------------------------------
      // Handler implementations for user-defined typed input ports
      // ----------------------------------------------------------------------
      //! Handler implementation for getADCCode
      Drv::ADC::Status getADCCode_handler(
          NATIVE_INT_TYPE portNum, //!< The port number
          U32& adc
      ) override;

      //! Handler implementation for getADCMax
      U32 getADCMax_handler(
          NATIVE_INT_TYPE portNum //!< The port number
      ) override;

      //! Handler implementation for getVoltageReference
      F32 getVoltageReference_handler(
          NATIVE_INT_TYPE portNum //!< The port number
      ) override;

      //! Handler implementation for getVoltageValue
      Drv::ADC::Status getVoltageValue_handler(
          NATIVE_INT_TYPE portNum, //!< The port number
          F32& voltage
      ) override;

  };

}

#endif
