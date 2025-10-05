// ======================================================================
// \title  ExtBatManagerTester.cpp
// \author user
// \brief  cpp file for ExtBatManager component test harness implementation class
// ======================================================================

#include "ExtBatManagerTester.hpp"
#include "STest/Pick/Pick.hpp"
#include "STest/Random/Random.hpp"

namespace Components {

  // ----------------------------------------------------------------------
  // Construction and destruction
  // ----------------------------------------------------------------------

  ExtBatManagerTester ::
    ExtBatManagerTester() :
      ExtBatManagerGTestBase("ExtBatManagerTester", ExtBatManagerTester::MAX_HISTORY_SIZE),
      component("ExtBatManager")
  {
    this->initComponents();
    this->connectPorts();
  }

  ExtBatManagerTester ::
    ~ExtBatManagerTester()
  {

  }

  // ----------------------------------------------------------------------
  // Tests
  // ----------------------------------------------------------------------

  void ExtBatManagerTester::ExtBatManagerUT1() {
    // It is checked that component is able to successfully handle data retrieved from mock ADC driver.
    constexpr F32 adcVoltageData = 0.7;
    setMockAdcVoltage(adcVoltageData);
    const U32 context = STest::Pick::any();
    this->invoke_to_run(0, context);
    this->component.doDispatch();
    
    Drv::ExtBat::VoltageData voltageData;
    this->invoke_to_getVoltage(0, voltageData);
    
    constexpr F32 expectedResult = 5.0;
    constexpr F32 absError = 0.1;

    // Checked the ability to retrieve properly handled data and it can be added to the summary diagnostics by external component (which is out of scope of unit testing).
    ASSERT_NEAR(voltageData.getInstantaneousVoltage(), expectedResult, absError);
    
    // Checked that measured value is added to telemetry channels.
    ASSERT_TLM_InstantaneousVoltage_SIZE(1);
    ASSERT_TLM_AverageVoltage_SIZE(1);
  }

  void ExtBatManagerTester::ExtBatManagerUT2() {
    // Requirement states that the component should measure the external battery voltage at least every 200 ms.
    // This requirement depends on the Rate Group component configuration that is connected to run port of ExtBatManager,
    // that is out of range of unit testing and seems to be more related to integration testing.
    
    // Formally this test is created, but time difference between timePoint1 and timePoint2 is always 0. 
    
    Fw::Time timePoint1;

    const U32 context = STest::Pick::any();
    this->invoke_to_run(0, context);
    this->component.doDispatch();

    Fw::Time timePoint2;

    Fw::Time timeDiff; 
    timeDiff.sub(timePoint2, timePoint1);
    
    constexpr U32 expectedMaxTimeToPass = 200'000;

    ASSERT_TRUE((timePoint2.getUSeconds() <= expectedMaxTimeToPass));
  }

  void ExtBatManagerTester::ExtBatManagerUT3() {
    // It checked that the component has the ability to configure the maximum and minimum allowed measurement voltage.
    Components::ExtBatManager::Thresholds thresholds;
    constexpr U32 maxThresholdMiliVolts = 15'200;
    constexpr U32 minThresholdMiliVolts = 3'300;
    thresholds.Max = maxThresholdMiliVolts;
    thresholds.Min = minThresholdMiliVolts;
    
    component.configure(thresholds);

    ASSERT_EQ(component.extBatThresholds.Max, maxThresholdMiliVolts);
    ASSERT_EQ(component.extBatThresholds.Min, minThresholdMiliVolts);
  }

  void ExtBatManagerTester::ExtBatManagerUT4_1() {
    // Threshods configuration
    Components::ExtBatManager::Thresholds thresholds;
    constexpr U32 maxThresholdMiliVolts = 10'000;
    constexpr U32 minThresholdMiliVolts = 5'000;
    thresholds.Max = maxThresholdMiliVolts;
    thresholds.Min = minThresholdMiliVolts;
    
    component.configure(thresholds);

    // Undervoltage check
    constexpr F32 adcVoltageData = 0.6;
    setMockAdcVoltage(adcVoltageData);
    const U32 context = STest::Pick::any();
    this->invoke_to_run(0, context);
    this->component.doDispatch();
    
    Drv::ExtBat::VoltageData voltageData;
    this->invoke_to_getVoltage(0, voltageData);
    
    constexpr F32 expectedVoltage = 4.3;
    constexpr F32 absError = 0.1;
    ASSERT_NEAR(voltageData.getInstantaneousVoltage(), expectedVoltage, absError);
    
    ASSERT_TRUE(component.undervoltageAlarm); // The undervoltage detection is checked
    // Checked that undervoltageAlarm is added to telemetry channels.
    ASSERT_TLM_UndervoltageAlarm_SIZE(1);
    ASSERT_TLM_UndervoltageAlarm(0, true);

    // Checked that undervoltage event occured
    ASSERT_EVENTS_UndervoltageAlarm_SIZE(1);
  }

  void ExtBatManagerTester::ExtBatManagerUT4_2() {
    // Threshods configuration
    Components::ExtBatManager::Thresholds thresholds;
    constexpr U32 maxThresholdMiliVolts = 10'000;
    constexpr U32 minThresholdMiliVolts = 5'000;
    thresholds.Max = maxThresholdMiliVolts;
    thresholds.Min = minThresholdMiliVolts;
    
    component.configure(thresholds);

    // Overvoltage check
    constexpr F32 adcVoltageData = 2.1;
    setMockAdcVoltage(adcVoltageData);
    const U32 context = STest::Pick::any();
    this->invoke_to_run(0, context);
    this->component.doDispatch();

    Drv::ExtBat::VoltageData voltageData;
    this->invoke_to_getVoltage(0, voltageData);
    
    constexpr F32 expectedVoltage = 15.1;
    constexpr F32 absError = 0.1;
    ASSERT_NEAR(voltageData.getInstantaneousVoltage(), expectedVoltage, absError);

    ASSERT_TRUE(component.overvoltageAlarm); // The overvoltage detection is checked
    // Checked that overvoltageAlarm is added to telemetry channels.
    ASSERT_TLM_OvervoltageAlarm_SIZE(1);
    ASSERT_TLM_OvervoltageAlarm(0, true);

    // Checked that overvoltage event occured
    ASSERT_EVENTS_OvervoltageAlarm_SIZE(1);
  }

  void ExtBatManagerTester::ExtBatManagerUT5_1() {
    // Threshods configuration
    Components::ExtBatManager::Thresholds thresholds;
    constexpr U32 maxThresholdMiliVolts = 10'000;
    constexpr U32 minThresholdMiliVolts = 5'000;
    thresholds.Max = maxThresholdMiliVolts;
    thresholds.Min = minThresholdMiliVolts;
    
    component.configure(thresholds);

    // Overvoltage simulation
    constexpr F32 adcVoltageData = 2.1;
    setMockAdcVoltage(adcVoltageData);
    const U32 context = STest::Pick::any();
    this->invoke_to_run(0, context);
    this->component.doDispatch();

    Drv::ExtBat::VoltageData voltageData;
    this->invoke_to_getVoltage(0, voltageData);
    
    constexpr F32 expectedVoltage = 15.1;
    constexpr F32 absError = 0.1;
    ASSERT_NEAR(voltageData.getInstantaneousVoltage(), expectedVoltage, absError);

    ASSERT_FALSE(component.voltageValid); // The bad quality is checked 
    // Checked that isValid is added to telemetry channels.
    ASSERT_TLM_IsValid_SIZE(1);
    ASSERT_TLM_IsValid(0, false);

    // Checked that voltage invalid event occured
    ASSERT_EVENTS_VoltageInvalid_SIZE(1);
  }

  void ExtBatManagerTester::ExtBatManagerUT5_2() {
    // Threshods configuration
    Components::ExtBatManager::Thresholds thresholds;
    constexpr U32 maxThresholdMiliVolts = 10'000;
    constexpr U32 minThresholdMiliVolts = 5'000;
    thresholds.Max = maxThresholdMiliVolts;
    thresholds.Min = minThresholdMiliVolts;
    
    component.configure(thresholds);

    // Undervoltage simulation
    constexpr F32 adcVoltageData = 0.6;
    setMockAdcVoltage(adcVoltageData);
    const U32 context = STest::Pick::any();
    this->invoke_to_run(0, context);
    this->component.doDispatch();
    
    Drv::ExtBat::VoltageData voltageData;
    this->invoke_to_getVoltage(0, voltageData);
    
    constexpr F32 expectedVoltage = 4.3;
    constexpr F32 absError = 0.1;
    ASSERT_NEAR(voltageData.getInstantaneousVoltage(), expectedVoltage, absError);

    ASSERT_FALSE(component.voltageValid); // The bad quality is checked 
    // Checked that isValid is added to telemetry channels.
    ASSERT_TLM_IsValid_SIZE(1);
    ASSERT_TLM_IsValid(0, false);

    // Checked that voltage invalid event occured
    ASSERT_EVENTS_VoltageInvalid_SIZE(1);
  }

  void ExtBatManagerTester::ExtBatManagerUT5_3() {
    // Simulate ADC voltage measurement error
    setMockAdcStatus(Drv::ADC::Status::FAIL);

    const U32 context = STest::Pick::any();
    this->invoke_to_run(0, context);
    this->component.doDispatch();

    ASSERT_FALSE(component.voltageValid); // The bad quality is checked 
    // Checked that isValid is added to telemetry channels.
    ASSERT_TLM_IsValid_SIZE(1);
    ASSERT_TLM_IsValid(0, false);

    // Checked that voltage invalid event occured
    ASSERT_EVENTS_VoltageInvalid_SIZE(1);
  }

  void ExtBatManagerTester::ExtBatManagerUT6_1() {
    // Simulate 10 measurements
    constexpr U32 numOfElements = 10;
    etl::array<F32, numOfElements> adcVoltages {0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0};
    etl::array<F32, numOfElements> outputVoltages{};
    F32 averageOutputVoltage = 0.0;

    for (U32 i = 0; i < numOfElements; i++) {
      outputVoltages.at(i) = component.calculateBatteryVoltage(adcVoltages.at(i));
      averageOutputVoltage += outputVoltages.at(i);
    }
    averageOutputVoltage /= numOfElements;

    for (U32 i = 0; i < numOfElements; i++) {
      setMockAdcVoltage(adcVoltages.at(i));
      const U32 context = STest::Pick::any();
      this->invoke_to_run(0, context);
      this->component.doDispatch();
    }

    Drv::ExtBat::VoltageData voltageData;
    this->invoke_to_getVoltage(0, voltageData);
    
    // Check that average voltage and instantaneous voltages are equal to expected values
    constexpr F32 absError = 0.1;
    ASSERT_NEAR(voltageData.getAverageVoltage(), averageOutputVoltage, absError);
    ASSERT_NEAR(voltageData.getInstantaneousVoltage(), outputVoltages.at(numOfElements-1), absError);
  }

  void ExtBatManagerTester::ExtBatManagerUT6_2() {
    // Simulate 10 measurements
    STest::Random::seed();

    constexpr U32 numOfElements = 10;
    etl::array<F32, numOfElements> adcVoltages{};
    etl::array<F32, numOfElements> outputVoltages{};
    F32 averageOutputVoltage = 0.0;

    for (U32 i = 0; i < numOfElements; i++) {
      U32 rnd = STest::Random::lowerUpper(0, 2300);
      adcVoltages.at(i) = static_cast<F32>(rnd) / 1000.0;
      outputVoltages.at(i) = component.calculateBatteryVoltage(adcVoltages.at(i));
      averageOutputVoltage += outputVoltages.at(i);
    }
    averageOutputVoltage /= numOfElements;

    for (U32 i = 0; i < numOfElements; i++) {
      setMockAdcVoltage(adcVoltages.at(i));
      const U32 context = STest::Pick::any();
      this->invoke_to_run(0, context);
      this->component.doDispatch();
    }

    Drv::ExtBat::VoltageData voltageData;
    this->invoke_to_getVoltage(0, voltageData);
    
    // Check that average voltage and instantaneous voltages are equal to expected values
    constexpr F32 absError = 0.1;
    ASSERT_NEAR(voltageData.getAverageVoltage(), averageOutputVoltage, absError);
    ASSERT_NEAR(voltageData.getInstantaneousVoltage(), outputVoltages.at(numOfElements-1), absError);
  }

  // ----------------------------------------------------------------------
  // Handlers for typed from ports
  // ----------------------------------------------------------------------

  void ExtBatManagerTester ::
    from_enablePin_handler(
        NATIVE_INT_TYPE portNum,
        const Fw::Logic& state
    )
  {
    // Not used during for Unit tests
  }

  Drv::ADC::Status ExtBatManagerTester ::
    from_getVoltageValue_handler(
        NATIVE_INT_TYPE portNum,
        F32& voltage
    )
  {
    voltage = adcVoltage;
    return adcStatus;
  }

  void ExtBatManagerTester::setMockAdcVoltage(F32 value) {
    adcVoltage = value;
    adcStatus = Drv::ADC::Status::SUCCESS;
  }

  void ExtBatManagerTester::setMockAdcStatus(Drv::ADC::Status status) {
    adcStatus = status;
  }
}
