// ======================================================================
// \title  TPM.hpp
// \author user
// \brief  hpp file for TPM component implementation class
// ======================================================================

#ifndef Components_TPM_HPP
#define Components_TPM_HPP

#include "Components/TPM/TPMComponentAc.hpp"

namespace Components {

  class TPM :
    public TPMComponentBase
  {

    public:

      // ----------------------------------------------------------------------
      // Component construction and destruction
      // ----------------------------------------------------------------------

      //! Construct TPM object
      TPM(
          const char* const compName //!< The component name
      );

      //! Destroy TPM object
      ~TPM();

      /**
      * Configuration of Trusted Platform Module
      */
      void configure();  
    
    PRIVATE:

    static constexpr U32 baseAddressTPM = 0xD40000; // Actually base address is not used during communication with TPM over I2C
    static constexpr U32 accessTPM      = (baseAddressTPM | 0x04);
    static constexpr U32 intfCaps       = (baseAddressTPM | 0x30);
    static constexpr U32 didVid         = (baseAddressTPM | 0x48);
    static constexpr U32 rid            = (baseAddressTPM | 0x4C);

    static constexpr U32 accessTPMValid = 0x80;

    static constexpr U8 SLB9673Addr = 0x2e;
    static constexpr U32 writeTimeout = 0xFFFF'FFFF;
    static constexpr U32 readTimeout = 0xFFFF'FFFF;

    // ----------------------------------------------------------------------
    // Handler implementations for user-defined typed input ports
    // ----------------------------------------------------------------------

    //! Handler implementation for run
    //!
    //! Receiving calls from the rate group
    void run_handler(
        NATIVE_INT_TYPE portNum, //!< The port number
        NATIVE_UINT_TYPE context //!< The call order
    ) override;

    /**
     * Read from TPM
     * @param reg - register to read from
     * @param value - value of register
     * @return true, if read successfully; otherwise - false
     */
    bool readReg(U8 reg, U8 &value);

  };

}

#endif
