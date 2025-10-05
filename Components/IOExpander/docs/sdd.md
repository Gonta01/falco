# Components::IOExpander

IO Expander ADS7138IRTER

## Usage Examples
Add usage examples here

### Diagrams
Add diagrams here

### Typical Usage
And the typical usage of the component here

## Class Diagram
Add a class diagram here

## Port Descriptions
| Name | Description |
|---|---|
|---|---|

## Component States
Add component states in the chart below
| Name | Description |
|---|---|
|---|---|

## Sequence Diagrams
Add sequence diagrams here

## Parameters
| Name | Description |
|---|---|
|---|---|

## Commands
| Name | Description |
|---|---|
|---|---|

## Events
| Name | Description |
|---|---|
|---|---|

## Telemetry
| Name | Description |
|---|---|
|---|---|

## Unit Tests
Add unit test descriptions in the chart below
| Name | Description | Output | Coverage |
|---|---|---|---|
|testConfigureIOExpander|Tests whether the IO expander is properly initialized and configured with valid parameters.|Passed|
|testReadAnalogInputIOExpander|Tests reading analog input values from the IO expander and verifies the values are within expected ranges.|Passed|
|testReadDigitalMultipleInputsIOExpander|Verifies the correct reading of multiple digital input pins.|Passed|
|testWriteDigitalMultipleOutputsIOExpander|Tests setting values on multiple digital output pins.|Passed|
|testReadDigitalSingleInputIOExpander|Ensures accurate reading of a single digital input pin.|Passed|
|testWriteDigitalSingleOutputIOExpander|Validates writing a value to a single digital output pin.|Passed|
|testFailureNotInitedIOExpander|Tests system response when an IO operation is attempted before initialization|Passed|
|testFailureWrongChannelConfigurationIOExpander|Tests behavior when attempting to access a pin or channel with an invalid configuration|Passed|
|testFailureCommunicationErrorIOExpander|Simulates a communication error (I2C failure) and checks error handling|Passed|
|testFailureCRCErrorIOExpander|Simulates a CRC mismatch in data transmission|Passed|
|testFailureCalibrationUnfinishedIOExpander|Tests calibration unfinished|Passed|
## Requirements
Requirements for the IOExpanderManager (Input Output Expander Manager) driver
| Name | Description | Validation |
|---|---|---|
|IOExpMngr1.| The driver must support two ADS7138IRTER IO port expanders connected to the main microcontroller via the I2C0 interface.|Satisfied due to IOExpander component architecture|
|IOExpMngr2.| The driver must perform digital communication at the maximum allowable I2C0 speed, taking into account other devices connected to the I2C0 interface.|---|
|IOExpMngr3.| The driver must provide control of the correctness of the CRC-8 CCIT checksum for each exchange session with the port expander. In case of a checksum mismatch, provide appropriate diagnostics, ignore data received with an incorrect checksum and perform a re-query. The CRC polynomial is (x^8 + x^2 + x + 1).|Each test has a CRC check.testFailureCRCErrorIOExpander| 
|IOExpMngr4.| The driver must provide the ability to change each individual port configured in DI/DO mode, both separately and in one clock cycle with other ports within the same IO expander.|---|
|IOExpMngr5.| The driver must use the following provided functional names for each of the expanders - digital channels: DIx/DOx. Analog channels AIy. Where x is from 1 to 8 and y is from 1 to 8.|Satisfied due to IOExpander component architecture|
|IOExpMngr6.| The driver must allow each of the ports to be configured to one of the following operating modes: digital input, digital output, analog input. A channel configured to any of the three modes must operate exclusively in this mode.|testConfigureIOExpander|
|IOExpMngr7.| For analog input channels, the driver must meet the following functional requirements:|---|
|IOExpMngr8.a| Measure the voltage on each of the configured channels in the full dynamic range (0 - 3.0 V);|testReadAnalogInputIOExpander|
|IOExpMngr9.a| Provide the user with both “raw” data (16 bits) in the form of an ADC code and in physical quantities – V;|testReadAnalogInputIOExpander|
|IOExpMngr10.a| Support ADC conversion both in autonomous automatic mode and in single measurement mode;|testConfigureIOExpander|
|IOExpMngr11.a| Perform averaging using the internal digital automaton of the ADC module;|testConfigureIOExpander|
|IOExpMngr12.a| The driver must provide the ability to configure each individual analog channel in analog comparator mode and read the number of operations of this comparator.|---|
|IOExpMngr13.a| The driver must support the ability to record the channel-wise “offset” of the measured voltage using the corresponding calibration table;|calibrateADCOffsets inside IOExpander, was tested in testReadAnalogInputIOExpander|
|IOExpMngr14.a| The driver must provide an indication of the “quality” of the measured voltage for the corresponding expander channel. Quality – “1” - “good” when measurements were within acceptable limits, quality – “0” - “bad” when measurements were outside the limits, or a checksum error or exchange timeout with the port expander occurred.|testReadAnalogInputIOExpander|

## Change Log
| Date | Description |
|---|---|
|---| Initial Draft |