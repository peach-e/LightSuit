/*
 **********************************************************************
 *  File   : Constants.h
 *  Author : peach
 *  Date   : 19 Dec 2017
 **********************************************************************
 */

#ifndef CONSTANTS
#define CONSTANTS

/*
 * PIN DEFINITIONS
 */

// Listens for +5V when changing Modes
#define PIN_SENSE 9

// Output Pins to the lights. (High == on)
#define PIN_R 3
#define PIN_G 5
#define PIN_B 6

// I/O to the equalizer chip.
#define PIN_ANALOG_IN A0
#define PIN_RESET 7
#define PIN_STROBE 4

/*
 * Global Constants
 */

// How many channels are supported by the chip in the chip for frequency detection.
#define NUM_FREQUENCY_BINS 7

#endif

