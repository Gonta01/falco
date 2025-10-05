# Components::ComModule

Component that is intermediate between frame/deframer and UART driver

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
|testComModuleRoutine|Tests the communication module's routine operation, verifying expected data flow and correct handling of input output signals.|Passed|
## Requirements
3.1.3.24 Requirements for the driver for communication with the ground station GSComAdapter
| Name | Description | Validation |
|---|---|---|
|F_GsComAdp_1| The driver must provide the functions of connecting the telemetry, logging and command subsystem F’ and the components of the SDR BO “Boryviter”.|---|
|F_GsComAdp_2| The driver must be built according to the ByteStreamDriverModel model, which is included in the standard F’ design paradigms.|---|
|F_GsComAdp_3| The driver must provide an abstraction layer between the standard Framer/Deframer components and lower-level drivers.|---|
|F_GsComAdp_4| The driver must support all available lower-level drivers that provide access to serial communication interfaces.|---|
|F_GsComAdp_5| The driver must be built according to the “adapter” principle and, accordingly, be a “passive” component.|---|

## Change Log
| Date | Description |
|---|---|
| 18.09.24 | Initial implementation | #61734 |