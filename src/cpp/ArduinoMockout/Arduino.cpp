/*
 **********************************************************************
 *  File   : Arduino.cpp
 *  Author : peach
 *  Date   : 19 Dec 2017
 **********************************************************************
 */

#include "Arduino.h"
#include <iostream>

/*
 * Mocked Functions
 */

int analogRead(int pin) {
    printf("Reading from pin %d,\n", pin);
    return 0;
}

void analogWrite(int pin, int value) {
    printf("Analogwriting pin %d to value %d.\n", pin, value);
}

void delay(int input) {
    printf("Im gonna wait for %d milliseconds\n", input);
}

void delayMicroseconds(int input) {
    printf("Im gonna wait for %d mikerowseconds\n", input);
}

void digitalWrite(int pin, int value) {
    printf("Digital Writing pin %d to value %d.\n", pin, value);
}

void pinMode(int pinName, int setting) {
    printf("Setting pin %d to %d.\n", pinName, setting);
}

/*
 * Mocked Variables and Objects
 */

Serial_impl Serial = Serial_impl();
