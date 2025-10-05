module Falco {
    @ A component for getting time
    passive component SystemTime {
        @ Port to retrieve time
        guarded input port timeGetPort: Fw.Time

    }
}