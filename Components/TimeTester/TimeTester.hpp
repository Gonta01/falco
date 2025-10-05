// ======================================================================
// \title  TimeTester.hpp
// \author user
// \brief  hpp file for TimeTester component implementation class
// ======================================================================

#ifndef Components_TimeTester_HPP
#define Components_TimeTester_HPP

#include "Components/TimeTester/TimeTesterComponentAc.hpp"

namespace Components {

  class TimeTester :
    public TimeTesterComponentBase
  {

    public:

      // ----------------------------------------------------------------------
      // Component construction and destruction
      // ----------------------------------------------------------------------

      //! Construct TimeTester object
      TimeTester(
          const char* const compName //!< The component name
      );

      //! Destroy TimeTester object
      ~TimeTester();

    PRIVATE:

      // ----------------------------------------------------------------------
      // Handler implementations for user-defined typed input ports
      // ----------------------------------------------------------------------

      //! Handler implementation for run
      //!
      //! Port receiving calls from the rate group
      void run_handler(
        NATIVE_INT_TYPE portNum, //!< The port number
        NATIVE_UINT_TYPE context //!< The call order
      ) override;

      Fw::Time startTime;
      bool isStartTimeSet{false};
      static constexpr U32 timeToWaitS = 15;

      bool isTimeSetOnce{false};

  };

}

#endif
