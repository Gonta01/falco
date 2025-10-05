# Components::NORDriver

Component for interaction with NOR flash

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
|testConfigureNORDriver|Tests if the NOR driver configures the memory interface correctly with valid parameters.|Passed|
|testReadNORDriver|Verifies the correct data is read from NOR flash memory at a valid address.|Passed|
|testWriteNORDriver|Ensures data is correctly written to NOR flash memory|Passed|
|testEraseNORDriver|Validates that specified memory sectors are properly erased|Passed|
|testFailureWrongMemoryIdNORDriver|Simulates an incorrect memory ID returned by the device and ensures proper error handling.|Passed|
|testFailureInputBufferEmptyNORDriver|Checks that the driver returns an error when attempting to write from an empty input buffer.|Passed|
|testFailureAccessOutOfRangeNORDriver|Tests access beyond the valid memory range to verify proper bounds checking and error reporting.|Passed|
|testFailureUnalignedAccessNORDriver|Verifies that unaligned operations are correctly rejected|Passed|
|testFailureIOInterfaceErrorNORDriver|Simulates a low-level I/O interface failure and ensures the NOR driver reports it properly.|Passed|
|testFailureFlashMemoryBusyNORDriver|Ensures that operations issued while flash memory is busy are properly rejected.|Passed|
|testGetMemoryAndSectorSizeNORDriver|Verifies that the NOR driver correctly reports the total memory size and the size of individual sectors.|Passed|
## Requirements
Add requirements in the chart below
| Name | Description | Validation |
|---|---|---|
|---|---|---|

## Change Log
| Date | Description |
|---|---|
|---| Initial Draft |