# Components::NORManager

Component NOR management

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
|testRequestEraseNORManager|Tests successful erase operation requested from NOR manager|Passed|
|testRequestReadNORManager|Tests successful read operation requested from NOR manager|Passed|
|testRequestWriteNORManager|Tests successful write operation requested from NOR manager|Passed|
|testRequestEraseNORManagerFailed|Tests failure scenario for erase operation requested from NOR manager|Passed|
|testRequestReadNORManagerFailed|Tests failure scenario for read operation requested from NOR manager|Passed|
|testRequestWriteNORManagerFailed|Tests failure scenario for write operation requested from NOR manager|Passed|
|testGetMemoryAndSectorSizeNORManager|Tests retrieval of memory and sector size parameters from NOR manager|Passed|

## Requirements
3.1.3.5 NORManager Driver Requirements
| Name | Description | Validation |
|---|---|---|
|F_NORMngr1.|The NORManager driver must support the IS25LP01GG W25Q01JV-DTR NOR flash memory chip via the QSPI interface, which has two system usage scenarios. Scenario 1 – mounted as an extension of the program memory address space in XIP (Execute In Place) mode. Scenario 2 – mounted as an external page-oriented persistent memory.|testConfigureNORDriver|
|F_NORMngr2.|All requirements below are relevant to “Scenario 2”;|---|
|F_NORMngr3.|The driver must support the following operations: reading data from NOR, writing data to NOR, putting the chip into sleep mode and waking the chip;|testRequestReadNORManager,testRequestWriteNORManager, powerDown and releasePowerDown inside NORDriver|
|F_NORMngr4.|The driver must provide NOR address space control functions and, in case of violation of the available address space, provide appropriate diagnostics;|testFailureAccessOutOfRangeNORDriver, testFailureUnalignedAccessNORDriver|
|F_NORMngr5.|The driver must operate in asynchronous mode (active component) and provide the status of the last performed operation through the corresponding function;|All NORManager tests validate status reporting after operations.|
|F_NORMngr6.|The driver must be in "sleep" mode when waiting for a command from the user and when waiting for the end of the current operation by lower-level drivers.|Satisfied due to NORManager, NORDriver components architecture|
|F_NORMngr7.|The driver must support atomic data exchange between RAM and NOR binary arrays up to 8 KB inclusive.|testRequestReadNORManager, testRequestWriteNORManager|
|F_NORMngr8.|The driver must support only exclusive ownership of other F'Prime components;|Satisfied due to NORManager, NORDriver components architecture|
|F_NORMngr9.|The driver must not be clocked by F'Prime scheduler system events.|Satisfied due to NORManager, NORDriver components architecture|
|F_NORMngr10|The driver must not control data integrity.|No CRC logic exists in driver|
## Change Log
| Date | Description |
|---|---|
|---| Initial Draft |