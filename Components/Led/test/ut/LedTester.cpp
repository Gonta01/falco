// ======================================================================
// \title  LedTester.cpp
// \author aos
// \brief  cpp file for Led component test harness implementation class
// ======================================================================

#include "LedTester.hpp"

namespace Components
{

  // ----------------------------------------------------------------------
  // Construction and destruction
  // ----------------------------------------------------------------------

  LedTester ::
      LedTester() : LedGTestBase("LedTester", LedTester::MAX_HISTORY_SIZE),
                    component("Led")
  {
    this->initComponents();
    this->connectPorts();
  }

  LedTester ::
      ~LedTester()
  {
  }

  // ----------------------------------------------------------------------
  // Tests
  // ----------------------------------------------------------------------

  void LedTester ::
      testCmdBlinkingOnOffLed()
  {
    this->sendCmd_BLINKING_ON_OFF(0, 0, Fw::On::ON);

    ASSERT_CMD_RESPONSE_SIZE(1);
    ASSERT_CMD_RESPONSE(0, 0, 0, Fw::CmdResponse::OK);
    ASSERT_EVENTS_SetBlinkingState_SIZE(1);
    ASSERT_TLM_SIZE(1);
    ASSERT_TLM_BlinkingState_SIZE(1);
    ASSERT_TLM_BlinkingState(0, Fw::On::ON);
  }

  void LedTester ::
      testRunHandlerLed()
  {
    constexpr U32 sizeOfOperations = 10;
    for (size_t i = 0; i < sizeOfOperations; i++)
    {
      this->invoke_to_run(0, 0);
    }
    ASSERT_TLM_LedTransitions_SIZE(sizeOfOperations);
    etl::vector<Fw::Logic, sizeOfOperations> actualStates = this->getCapturedStates();
    for (size_t i = 0; i < sizeOfOperations; i++)
    {
      if(i % 2 == 0)
      {
        ASSERT_EQ(actualStates[i], Fw::Logic::HIGH);
      }
      else
      {
        ASSERT_EQ(actualStates[i], Fw::Logic::LOW);
      }
      ASSERT_TLM_LedTransitions(i, i + 1);
    }
  }

  // ----------------------------------------------------------------------
  // Handlers for typed from ports
  // ----------------------------------------------------------------------

  void LedTester ::
      from_gpioSet_handler(
          NATIVE_INT_TYPE portNum,
          const Fw::Logic &state)
  {
    capturedStates.push_back(state);
  }

  etl::vector<Fw::Logic, 10> &LedTester ::
      getCapturedStates()
  {
    return this->capturedStates;
  }
}
