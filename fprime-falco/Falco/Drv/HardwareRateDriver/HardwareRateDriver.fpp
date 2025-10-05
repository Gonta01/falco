module Falco {

  @ An interrupt based driver for rate groups
  active component HardwareRateDriver {
    @ The rate group scheduler input
    async input port CycleIn: Svc.Cycle

    @ The cycle outputs. Meant to be connected to rate group driver
    output port CycleOut: Svc.Cycle

    @ The cycle outputs. Meant to be connected to the Sched input
    output port CycleBackward: Svc.Cycle

  }

}
