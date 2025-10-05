# SAMV71Q21B::Uart0Driver

UART0 driver. 

## Typical Usage

It was designed to be used in half-duplex communication.

## Usage Examples

RS-485

## Implementation descriptions

### Data transmission

Buffer that is intended to be transmitted is copied into non-cached buffer (UART0Buffer) and then this buffer is provided to DMA which transfers data from buffer to transmit register. During data transmission receiver is disabled.

Buffer size that can be transmitted is limited by size of non-cached buffer.

Note: non cached buffer is used because of [Cache Coherency Issue](https://ww1.microchip.com/downloads/aemDocuments/documents/MCU32/ProductDocuments/TechnicalReferenceManual/Handling-Cache-Coherency-Issues-at-Runtime-Using-Cache-Maintenance-Operations-on-PIC32MZ-MCUs-DS90003308.pdf)


### Data reception

Data reception implies polling of circular buffer (it is implemented inside Microchip's HAL).


### Error reports

Transmission and receptions errors are counted and can be retrieved through report port.

## Change Log
| Date | Description | Redmine task |
|---|---|---|
| 18.09.24 | Initial implementation | #61734 |