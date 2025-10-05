module Components {
    @ Component that specifies communication with external I2C device
    active component ExternalI2cDevice {

        @ Command to write data through I2C port
        async command WRITE(
            regValue:  U8  @< Device register to write in
            writeData: U8  @< Value that should be written to register
        )

        @ Command to read data through I2C port
        async command READ(
            regValue:  U8  @< Device register to read from
        )

        @ Device address
        telemetry DeviceAddress: U32
        
        @ Write data
        telemetry DeviceRegister: U8

        @ Read data
        telemetry ReadData: U8

        @ Event logged that address is invalid or not set
        event InvalidAddress() \
            severity warning high \
            format "Device address is invalid or not set"

        @ Event logged that failure occured during write operation
        event WriteFailure() \
            severity warning high \
            format "Failure during write operation"

        @ Event logged that write operation finished successfully
        event WriteSuccess() \
            severity activity low \
            format "Successfully written to device"

        @ Event logged that failure occured during read operation
        event ReadFailure() \
            severity warning high \
            format "Failure during read operation"
        
        @ Event logged when the device address was changed
        event ReadSuccess() \
            severity activity low \
            format "Successfully read from device"

        @ Event logged when the address was changed
        event DeviceAddressSet(addr: U8) \
            severity activity high \
            format "Device address set to {}"

        @ Event logged when the data is read
        event ReadData(data: U8) \
            severity activity high \
            format "Read data {}"

        @ Device address parameter
        param ADDRESS: U32

        @ Receiving calls from the rate group
        async input port run: Svc.Sched

        @ I2C port to write data
        output port write: Drv.I2C.Write

        @ I2C port to read data
        output port read: Drv.I2C.Read

        ###############################################################################
        # Standard AC Ports: Required for Channels, Events, Commands, and Parameters  #
        ###############################################################################
        @ Port for requesting the current time
        time get port timeCaller

        @ Port for sending command registrations
        command reg port cmdRegOut

        @ Port for receiving commands
        command recv port cmdIn

        @ Port for sending command responses
        command resp port cmdResponseOut

        @ Port for sending textual representation of events
        text event port logTextOut

        @ Port for sending events to downlink
        event port logOut

        @ Port for sending telemetry channels to downlink
        telemetry port tlmOut

        @ Port to return the value of a parameter
        param get port prmGetOut

        @Port to set the value of a parameter
        param set port prmSetOut

    }
}