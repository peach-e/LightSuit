/*
 **********************************************************************
 *  File   : main.cpp
 *  Author : peach
 *  Date   : 19 Dec 2017
 **********************************************************************
 */

/*
 * The main function is used for debugging and checking that
 * the various modules work correctly before integrating them into
 * the arduino.
 *
 * Essentially a sandbox.
 */

#include <iostream>
#include "LevelAnalyzer.h"
#include "Arduino.h"
int main() {
    LevelAnalyzer anz = LevelAnalyzer();
    int rgb[] = { 0, 0, 0 };
    int buckets[] = { 1, 2, 3, 4, 5, 6, 7 };
    anz.getRgbFromBuckets(rgb, buckets);
    Serial.print("Results are ");
    Serial.print(rgb[0]);
    Serial.print(rgb[1]);
    Serial.print(rgb[2]);
}

