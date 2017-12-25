/*
 **********************************************************************
    File   : LightSuit.ino
    Author : peach
    Date   : 18 December 2017
 **********************************************************************
*/

#include <MachineState.h>
#include <FadeHSV.h>
#include <MusicPulse.h>
#include <Constants.h>

/*
 ************************************
   Definitions
 ************************************
*/

// We can only have modes 0 and 1 right now.
#define MAX_MODE 1

/*
 *************************************
   Globals
 * ************************************
*/

// Array of MachineState instances, which are populated during setup.
MachineState** states = new MachineState*[2];

// Whether the change mode button is pushed or not.
int sensePinState = 0;

// Mode that we're in currently in.
int mode = 0;

/*
 *************************************
   Function Definitions
 * ************************************
*/

// Causes a delay until the PIN_SENSE has been low for a while.
// Helps to debounce the sense pin.
void waitForLowSensePin ();

/*
 *************************************
   Main Functions
 * ************************************
*/
void setup () {
  //Serial.begin(230400); // << Keep commented until needed.

  // Set up pins common to all modes.
  pinMode(PIN_SENSE, INPUT);
  pinMode(PIN_R, OUTPUT);
  pinMode(PIN_G, OUTPUT);
  pinMode(PIN_B, OUTPUT);

  // Set up Modes
  states[0] = new MusicPulse(); // Pulse with the music
  states[1] = new FadeHSV();    // Fade with colors.

  // TODO: add a police car lights option.
}

void loop () {
  sensePinState = digitalRead(PIN_SENSE);

  // Check if sense pin went high. If so, wait for it to go low again
  // and change the mode.
  if (sensePinState) {
    waitForLowSensePin();
    mode = (mode == MAX_MODE) ? 0 : mode + 1;
    states[mode]->enterState();
  }

  // Now in main loop, we run the iteration for whichever mode is active.
  states[mode]->iterateState();
}

/*
*************************************
  Support Functions
* ************************************
*/

void waitForLowSensePin() {

  int sensePinValue = 1;

  // Wait for sense pin to go low, and make sure it's low when it does.
  int sensePinIsHigh = 1;
  while (sensePinIsHigh) {

    // If the pin goes low, wait for 50ms and make sure it's still low after.
    sensePinValue = digitalRead(PIN_SENSE);
    if (!sensePinValue) {
      delay(10);
      sensePinValue = digitalRead(PIN_SENSE);

      // If the pin is still low, exit the loop.
      sensePinIsHigh = sensePinValue ? 1 : 0;
    }
  }
  return;
}

