MSP430
======

Lab work with the Texas Instruments MSP430 microcontroller. The instructions were written by INSA Lyon (France) hardware architecture professors.

## Objectives

This project aims to experiment with low level concepts such as:

- The architecture of a microcontroller
- Some aspects of compilation
- Assembly programming
- Hardware interrupts

During this project, we:

- Read the microcontroller, board and compiler documentations
- Implemented a simple driver for the built-in LCD display
- Implemented a (somewhat) random number generator by taking advantage of numbers' representation in memory
- Implemented a stopwatch using hardware interrupts

## Tools

- In order to upload code and debug, we used IAR Workbench, an IDE provided by Texas Instrument. (The various `.dep`, `.ewd`, `.ewp`, etc are project files for this IDE).
- The code was compiled with the built-in compiler, which contains MSP430-specific C extensions.
- `msp430fg4618.h` is a lib which provides many useful constants to work with the MSP430.