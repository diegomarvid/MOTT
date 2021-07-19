# MOTT
 General library for Arduino IR transmision and sampling
 
 The idea is to transfer strings over IR.
 
 Bit velocity can be set at will but it can't be too low due to carrier frequency or CPU clock.
 
 The transmiter has to be connected to a 555 timer circuit with a 38KHz wave. The signal pin connects to Reset pin of 555 IC.
 
 The IR reciever sensor signal pin can go directly to a microcontroller.
