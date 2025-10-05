# Components::FRAMManager

Component for FRAM management

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
|testRequestReadFRAMManager|Verifies that a valid read request to the FRAM manager successfully returns the expected data.|Passed|
|testRequestWriteFRAMManager|Verifies that a valid write request to the FRAM manager successfully write the expected data.|Passed|
|testRequestWakeupFRAMManager|Tests that the FRAM manager can wake from sleep mode when requested.|Passed|
|testRequestSleepFRAMManager|Verifies that the FRAM manager puts the device into sleep mode correctly.|Passed|
|testFailureRequestReadFRAMManager|Ensures the read request fails gracefully when given invalid parameters.|Passed|
|testFailureRequestWriteFRAMManager|Confirms that a write request returns fails safely when an invalid write is attempted.|Passed|
|testFailureRequestWakeupFRAMManager|Tests the response when the FRAM manager fails to wakeup.|Passed|
|testFailureRequestSleepFRAMManager|Validates that the system handles errors correctly if putting the FRAM to sleep fails.|Passed|
## Requirements
3.1.3.6 Requirements for the FRAMManager driver
| Name | Description | Validation |
|---|---|---|
|F_FRAMMngr1.| The FRAMManager driver must support the operation of the ferroelectric random access memory (FRAM) chip – MB85RS4MT via the SPI interface of the main microcontroller.|testConfigureFRAMDriver, FRAMDriver using SPI interface|
|F_FRAMMngr2.| The driver must provide access to the FRAM at 100% of the maximum allowable communication speed between the main microcontroller and the FRAM chip – 40 MHz.|---|
|F_FRAMMngr3.| The driver must support the following operations: reading data from FRAM, writing data to FRAM, putting the chip into sleep mode and waking the chip.|testRequestReadFRAMManager, testRequestWriteFRAMManager, testRequestWakeupFRAMManager, testRequestSleepFRAMManager|
|F_FRAMMngr4.| The driver must provide functions for monitoring the FRAM address space and, in case of violation of the available address space, provide appropriate diagnostics.|testFailureAddressOutOfRangeFRAMDriver|
|F_FRAMMngr5.| The driver must operate in asynchronous mode (active component) and provide the status of the last performed operation through the corresponding function.|All FRAMManager tests validate status reporting after operations.|
|F_FRAMMngr6.| The driver must be in "sleep" mode when waiting for a command from the user and when waiting for the end of the current operation by lower-level drivers.|testRequestSleepFRAMManager, testRequestWakeupFRAMManager, testFailureRequestSleepFRAMManager, testFailureRequestWakeupFRAMManager|
|F_FRAMMngr7.| The driver must support atomic data exchange between RAM and FRAM binary arrays up to 8 KB inclusive.|testRequestReadFRAMManager, testRequestWriteFRAMManager|
|F_FRAMMngr8.| The driver must generate F'Prime system events of the "error" type in case of diagnosing a corresponding error and provide the corresponding data for storage and processing by F'Prime tools.|errorReport in FRAMManager|
|F_FRAMMngr9.| The driver must support only exclusive ownership to other F'Prime components.|---|
|F_FRAMMngr10.| The driver must not be clocked by F'Prime scheduler system events.|---|
|F_FRAMMngr11.| The driver must not control data integrity.|No CRC logic exists in driver|
## Change Log
| Date | Description |
|---|---|
|---| Initial Draft |