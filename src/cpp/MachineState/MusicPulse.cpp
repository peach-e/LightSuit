/*
 **********************************************************************
 *  File   : MusicPulse.cpp
 *  Author : peach
 *  Date   : 19 Dec 2017
 **********************************************************************
 */

#include "MusicPulse.h"
#include "Arduino.h"
#include "Constants.h"
#include "LevelAnalyzer.h"

/*
 * Public Functions
 */

MusicPulse::MusicPulse() {
    pinMode(PIN_STROBE, OUTPUT);
    pinMode(PIN_RESET, OUTPUT);
    _levelAnalyzer = LevelAnalyzer();
}

// When entering the state, we reset the output, the equalizer chip
// and the analyzer module.
void MusicPulse::enterState() {
    analogWrite(PIN_R, 0);
    analogWrite(PIN_G, 0);
    analogWrite(PIN_B, 0);

    _levelAnalyzer.reset();

    digitalWrite(PIN_RESET, HIGH);
    delay(20);
    digitalWrite(PIN_RESET, LOW);
}

void MusicPulse::iterateState() {
    int channel;
    int buckets[NUM_FREQUENCY_BINS];

    // Sample the 7 channels provided by the chip.
    for (channel = 0; channel < NUM_FREQUENCY_BINS; channel++) {
        buckets[channel] = analogRead(PIN_ANALOG_IN);

        // Trip the strobe to select the next channel.
        digitalWrite(PIN_STROBE, HIGH);
        delayMicroseconds(16);
        digitalWrite(PIN_STROBE, LOW);
        Serial.print(buckets[channel]);
        Serial.print(",");
    }
    Serial.print("\n");

    int rgb[] = { 0, 0, 0 };

    _levelAnalyzer.getRgbFromBuckets(rgb, buckets);

    analogWrite(PIN_R, rgb[0]);
    analogWrite(PIN_G, rgb[1]);
    analogWrite(PIN_B, rgb[2]);

    //    Serial.println(rgb[0]);
}

/*
 * Private Functions
 */

