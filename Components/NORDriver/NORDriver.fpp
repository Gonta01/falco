module Components {
    @ Component for interaction with NOR flash
    passive component NORDriver {

        ##############################################################################
        #### Uncomment the following examples to start customizing your component ####
        ##############################################################################

        output port commandWrite : Drv.QSPI.CommandWrite
        output port registerRead : Drv.QSPI.RegisterRead
        output port registerWrite: Drv.QSPI.RegisterWrite
        output port memoryRead   : Drv.QSPI.MemoryRead
        output port memoryWrite  : Drv.QSPI.MemoryWrite

        guarded input port read  : Drv.NOR.Read
        guarded input port write : Drv.NOR.Write
        guarded input port erase : Drv.NOR.Erase

        guarded input port getSectorSize : Drv.NOR.GetSectorSize
        guarded input port getMemorySize : Drv.NOR.GetMemorySize

        @ Report wrong memory id error
        event WrongMemoryId \
            severity warning high \
            format "Wrong memory ID."

        @ Report input buffer empty error
        event InputBufferEmpty \
            severity warning high \
            format "Input buffer empty."

        @ Report access out of range error
        event AccessOutOfRange \
            severity warning high \
            format "Access out of range."

        @ Report unaligned access error
        event UnalignedAccess \
            severity warning high \
            format "Unaligned access."

        @ Report incomplete erase error
        event IncompleteErase \
            severity warning high \
            format "Incomplete erase."

        @ Report IO interface error
        event IOInterfaceError \
            severity warning high \
            format "IO interface error."
        
        @ Report flash memory busy error
        event FlashMemoryBusy \
            severity warning high \
            format "Flash memory busy."

        @ Report unknown error
        event UnknownError \
            severity warning high \
            format "Unknown error."    

        ###############################################################################
        # Standard AC Ports: Required for Channels, Events, Commands, and Parameters  #
        ###############################################################################
        @ Port for requesting the current time
        time get port timeCaller

        @ Port for sending textual representation of events
        text event port logTextOut

        @ Port for sending events to downlink
        event port logOut

        @ Port for sending telemetry channels to downlink
        telemetry port tlmOut

    }
}