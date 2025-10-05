// ======================================================================
// \title  FalcoPrimeTopology.cpp
// \brief cpp file containing the topology instantiation code
//
// ======================================================================
// Provides access to autocoded functions
#include <FalcoPrime/Top/FalcoPrimeTopologyAc.hpp>
#include <FalcoPrime/Top/FalcoPrimePacketsAc.hpp>
#include <config/FppConstantsAc.hpp>

// Necessary project-specified types
#include <Fw/Types/MallocAllocator.hpp>
#include <Os/Log.hpp>
#include <Svc/FramingProtocol/FprimeProtocol.hpp>

// Used for 1Hz synthetic cycling
#include <Os/Mutex.hpp>

#include "Falco/Os/FileSystemAddon.hpp"
#include "Falco/SystemLogger/SystemLogger.hpp"

#include "etl/string.h"
#include "etl/error_handler.h"
#include "etl/to_string.h"

// Allows easy reference to objects in FPP/autocoder required namespaces
using namespace FalcoPrime;

Falco::SystemLogger systemLogger;

// The reference topology uses the F´ packet protocol when communicating with the ground and therefore uses the F´
// framing and deframing implementations.
Svc::FprimeFraming framing;
Svc::FprimeDeframing deframing;

// The reference topology divides the incoming clock signal (1kHz) into sub-signals: 10Hz, 2Hz, and 100Hz
Svc::RateGroupDriver::DividerSet rateGroupDivisors = {{ {100, 0}, {500, 0}, {10, 0} }};

// Rate groups may supply a context token to each of the attached children whose purpose is set by the project. The
// reference topology sets each token to zero as these contexts are unused in this project.
NATIVE_INT_TYPE rateGroup1Context[FppConstant_ActiveRateGroupOutputPorts::ActiveRateGroupOutputPorts] = {};
NATIVE_INT_TYPE rateGroup2Context[FppConstant_ActiveRateGroupOutputPorts::ActiveRateGroupOutputPorts] = {};
NATIVE_INT_TYPE rateGroup3Context[FppConstant_ActiveRateGroupOutputPorts::ActiveRateGroupOutputPorts] = {};

const Components::IOExpander::ConfigTable digitalInputConfigTable{
    {{Drv::IOExpander::Channel::Channel_1, Drv::IOExpander::IOType::DigitalIn},
     {Drv::IOExpander::Channel::Channel_2, Drv::IOExpander::IOType::DigitalIn},
     {Drv::IOExpander::Channel::Channel_3, Drv::IOExpander::IOType::DigitalIn},
     {Drv::IOExpander::Channel::Channel_4, Drv::IOExpander::IOType::DigitalIn},
     {Drv::IOExpander::Channel::Channel_5, Drv::IOExpander::IOType::DigitalIn},
     {Drv::IOExpander::Channel::Channel_6, Drv::IOExpander::IOType::DigitalIn},
     {Drv::IOExpander::Channel::Channel_7, Drv::IOExpander::IOType::DigitalIn},
     {Drv::IOExpander::Channel::Channel_8, Drv::IOExpander::IOType::DigitalIn}}
    };

const Components::IOExpander::ConfigTable digitalOutputConfigTable{
    {{Drv::IOExpander::Channel::Channel_1, Drv::IOExpander::IOType::DigitalOutPP},
     {Drv::IOExpander::Channel::Channel_2, Drv::IOExpander::IOType::DigitalOutPP},
     {Drv::IOExpander::Channel::Channel_3, Drv::IOExpander::IOType::DigitalOutPP},
     {Drv::IOExpander::Channel::Channel_4, Drv::IOExpander::IOType::DigitalOutPP},
     {Drv::IOExpander::Channel::Channel_5, Drv::IOExpander::IOType::DigitalOutPP},
     {Drv::IOExpander::Channel::Channel_6, Drv::IOExpander::IOType::DigitalOutPP},
     {Drv::IOExpander::Channel::Channel_7, Drv::IOExpander::IOType::DigitalOutPP},
     {Drv::IOExpander::Channel::Channel_8, Drv::IOExpander::IOType::DigitalOutPP}}
    };

const Components::IOExpander::ConfigTable analogInputConfigTable{
    {{Drv::IOExpander::Channel::Channel_1, Drv::IOExpander::IOType::AnalogIn},
     {Drv::IOExpander::Channel::Channel_2, Drv::IOExpander::IOType::AnalogIn},
     {Drv::IOExpander::Channel::Channel_3, Drv::IOExpander::IOType::AnalogIn},
     {Drv::IOExpander::Channel::Channel_4, Drv::IOExpander::IOType::AnalogIn},
     {Drv::IOExpander::Channel::Channel_5, Drv::IOExpander::IOType::AnalogIn},
     {Drv::IOExpander::Channel::Channel_6, Drv::IOExpander::IOType::AnalogIn},
     {Drv::IOExpander::Channel::Channel_7, Drv::IOExpander::IOType::AnalogIn},
     {Drv::IOExpander::Channel::Channel_8, Drv::IOExpander::IOType::AnalogIn}}
};

constexpr I32 channel1OffsetMiliVolts       = 0;
constexpr I32 channel2OffsetMiliVolts       = 0;
constexpr I32 channel3OffsetMiliVolts       = 0;
constexpr I32 channel4OffsetMiliVolts       = 0;
constexpr I32 channel5OffsetMiliVolts       = 0;
constexpr I32 channel6OffsetMiliVolts       = 0;
constexpr I32 channel7OffsetMiliVolts       = 0;
constexpr I32 channel8OffsetMiliVolts       = 0;

constexpr I32 channel1MinThresholdMiliVolts = 0;
constexpr I32 channel2MinThresholdMiliVolts = 0;
constexpr I32 channel3MinThresholdMiliVolts = 0;
constexpr I32 channel4MinThresholdMiliVolts = 0;
constexpr I32 channel5MinThresholdMiliVolts = 0;
constexpr I32 channel6MinThresholdMiliVolts = 0;
constexpr I32 channel7MinThresholdMiliVolts = 0;
constexpr I32 channel8MinThresholdMiliVolts = 0;

constexpr I32 channel1MaxThresholdMiliVolts = 3300;
constexpr I32 channel2MaxThresholdMiliVolts = 3300;
constexpr I32 channel3MaxThresholdMiliVolts = 3300;
constexpr I32 channel4MaxThresholdMiliVolts = 3300;
constexpr I32 channel5MaxThresholdMiliVolts = 3300;
constexpr I32 channel6MaxThresholdMiliVolts = 3300;
constexpr I32 channel7MaxThresholdMiliVolts = 3300;
constexpr I32 channel8MaxThresholdMiliVolts = 3300;

const Components::IOExpander::AnalogOffsetsMiliVolts analogOffsets{
    channel1OffsetMiliVolts,
    channel2OffsetMiliVolts,
    channel3OffsetMiliVolts,
    channel4OffsetMiliVolts,

    channel5OffsetMiliVolts,
    channel6OffsetMiliVolts,
    channel7OffsetMiliVolts,
    channel8OffsetMiliVolts,
};

const Components::IOExpander::MinThresholdsMiliVolts minThresholdsMiliVolts{
    channel1MinThresholdMiliVolts,
    channel2MinThresholdMiliVolts,
    channel3MinThresholdMiliVolts,
    channel4MinThresholdMiliVolts,

    channel5MinThresholdMiliVolts,
    channel6MinThresholdMiliVolts,
    channel7MinThresholdMiliVolts,
    channel8MinThresholdMiliVolts,
};

const Components::IOExpander::MaxThresholdsMiliVolts maxThresholdsMiliVolts{
    channel1MaxThresholdMiliVolts,
    channel2MaxThresholdMiliVolts,
    channel3MaxThresholdMiliVolts,
    channel4MaxThresholdMiliVolts,
    
    channel5MaxThresholdMiliVolts,
    channel6MaxThresholdMiliVolts,
    channel7MaxThresholdMiliVolts,
    channel8MaxThresholdMiliVolts,
};

Components::IOExpander::AnalogConfig analogConfig{&analogOffsets, &minThresholdsMiliVolts, &maxThresholdsMiliVolts};

constexpr uint8_t EXPANDER1_ADDRESS = 0x17;
constexpr uint8_t EXPANDER2_ADDRESS = 0x14;

constexpr U16 currentLimitDomain1 = 512;
constexpr U16 currentLimitDomain2 = 512;
constexpr U16 currentLimitDomain3 = 512;
constexpr U16 currentLimitDomain4 = 512;

constexpr U32 extBatMinThresholdMiliVolts = 0;
constexpr U32 extBatMaxThresholdMiliVolts = 16'800;

Drv::Calendar::Time initialTime;

// A number of constants are needed for construction of the topology. These are specified here.
enum TopologyConstants {
    HEALTH_WATCHDOG_CODE = 0x123
};

Svc::Health::PingEntry pingEntries[] = {
    {PingEntries::rateGroup1::WARN, PingEntries::rateGroup1::FATAL, "rateGroup1"},
    {PingEntries::rateGroup2::WARN, PingEntries::rateGroup2::FATAL, "rateGroup2"},
    {PingEntries::rateGroup3::WARN, PingEntries::rateGroup3::FATAL, "rateGroup3"},
    {PingEntries::EnvSensManager::WARN, PingEntries::EnvSensManager::FATAL, "EnvSensManager"},
    {PingEntries::logDataCollector::WARN, PingEntries::logDataCollector::FATAL, "logDataCollector"},
    {PingEntries::rtcManager::WARN, PingEntries::rtcManager::FATAL, "rtcManager"},
};

Os::File errorFile;
etl::string<512> errorMessage;
void logErrorInFile(const etl::exception& e) {
    auto openStatus = errorFile.open("error.txt", Os::File::OPEN_CREATE, false);
    if (openStatus != Os::File::Status::OP_OK) {
        return;
    }
    errorMessage.append("The error was ");
    errorMessage.append(e.what());
    errorMessage.append(" in ");
    errorMessage.append(e.file_name());
    errorMessage.append(" at ");
    etl::to_string(e.line_number(), errorMessage, etl::format_spec(), true);
    errorMessage.append("\n");
    NATIVE_INT_TYPE sizeOfLog = errorMessage.size();
    Os::File::Status writeStatus = errorFile.write(errorMessage.data(), sizeOfLog, true);
    if (writeStatus != Os::File::Status::OP_OK) {
        return;
    }

    errorFile.close();
}

void handleError(const etl::exception& e) {
    #ifdef ERROR_LOGGING_IN_FILE
    logErrorInFile(e);
    #endif
    assert(false);
}

/**
 * \brief configure/setup components in project-specific way
 *
 * This is a *helper* function which configures/sets up each component requiring project specific input. This includes
 * allocating resources, passing-in arguments, etc. This function may be inlined into the topology setup function if
 * desired, but is extracted here for clarity.
 */
void configureTopology() {
    // Rate group driver needs a divisor list
    rateGroupDriver.configure(rateGroupDivisors);

    // Rate groups require context arrays.
    rateGroup1.configure(rateGroup1Context, FW_NUM_ARRAY_ELEMENTS(rateGroup1Context));
    rateGroup2.configure(rateGroup2Context, FW_NUM_ARRAY_ELEMENTS(rateGroup2Context));
    rateGroup3.configure(rateGroup3Context, FW_NUM_ARRAY_ELEMENTS(rateGroup3Context));

    health.setPingEntries(pingEntries, FW_NUM_ARRAY_ELEMENTS(pingEntries), HEALTH_WATCHDOG_CODE);

    // Framer and Deframer components need to be passed a protocol handler
    framer.setup(framing);
    deframer.setup(deframing);
    
    etl::error_handler::set_callback<handleError>();
}

// Public functions for use in main program are namespaced with deployment name FalcoPrime
namespace FalcoPrime {
void setupTopology(const TopologyState& state) {
    // Autocoded initialization. Function provided by autocoder.
    initComponents(state);
    // Autocoded id setup. Function provided by autocoder.
    setBaseIds();
    // Autocoded connection wiring. Function provided by autocoder.
    connectComponents();
    // Autocoded command registration. Function provided by autocoder.
    regCommands();
    // Project-specific component configuration. Function provided above. May be inlined, if desired.
    configureTopology();
    // Autocoded parameter loading. Function provided by autocoder.
    // loadParameters();
    // Autocoded task kick-off (active components). Function provided by autocoder.
    startTasks(state);

    greenLed.configure(Status_MMCU_PIN);
    spiCS.configure(SPI0_CS_PIN);
    
    alert1.configure(ALERT_EXP1_PIN);
    alert2.configure(ALERT_EXP2_PIN);
    enableBatteryMeasurement.configure(EnablePin_PIN);
    IMU1IntPin.configure(IMU1_INT_PIN);

    spi0Driver.configure();
    Uart0Driver.configure();
    Uart1Driver.configure();
    Uart4Driver.configure();
    RS485Driver.configure();
    comModule.configure();
    can0Driver.configure(Drv::CAN::NominalBitRate::BitRate1000kbps);
    can1Driver.configure(Drv::CAN::NominalBitRate::BitRate1000kbps);
    
    adc0Driver.configure();
    adc1Driver.configure();

    dps310.configure();
    sts21.configure();
    ntc.configure();

    icm20948.configure(Components::ICM20948::Mode::NO_FUSION);
    bno055.configure(Components::BNO055::Mode::NO_FUSION);
    NORDriver.configure(Components::NORDriver::PowerDown::Disable);
    NORTester.configure();
    framDriver.configure();

    madgwickFusion.configure();

    initialTime.setTm_sec(0);
    initialTime.setTm_min(0);
    initialTime.setTm_hour(0);
    initialTime.setTm_mday(9);
    initialTime.setTm_mon(8);
    initialTime.setTm_year(125);
    initialTime.setTm_wday(2);
    initialTime.setTm_yday(252);
    initialTime.setTm_isdst(0);
    RTCDriver.configure(initialTime);

    auto initResult = Os::FileSystem::initFileSystem();
    if (initResult == Os::FileSystem::Status::OP_OK) {
        textLogger.setLogFile("events", 0xffff'ffff, 100);
    }
    Fw::Logger::registerLogger(&systemLogger);

    rateDriver.startTimer();
}

void teardownTopology(const TopologyState& state) {
    // Autocoded (active component) task clean-up. Functions provided by topology autocoder.
    stopTasks(state);
    freeThreads(state);
}
};  // namespace FalcoPrime
