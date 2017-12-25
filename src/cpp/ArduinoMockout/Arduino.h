/*
 **********************************************************************
 *  File   : Arduino.h
 *  Author : peach
 *  Date   : 19 Dec 2017
 **********************************************************************
 */

/*
 * Provides mocks for the Arduino so that we can run our sketches locally and without
 * flashing them to the m/c all the time.
 *
 * Great for debugging!
 *
 * Functions are added to this library as they are needed.
 */

#ifndef _ARDUINO_H_
#define _ARDUINO_H_

#include <iostream>

// Mock out some of the pins and values.
#define OUTPUT 999
#define HIGH 1
#define LOW 0
#define A0 42

/*
 * Mocked Classes
 */
class Serial_impl {
public:
    template<typename T>
    void println(T message) {
        std::cout << message << std::endl;
    }

    template<typename T>
    void print(T message) {
        std::cout << message;
    }
};


/*
 * Mocked Functions
 */

int analogRead(int pin);
void analogWrite(int pin, int value);
void delay(int input);
void delayMicroseconds(int input);
void digitalWrite(int pin, int value);
void pinMode(int pinName, int setting);

/*
 * Mocked Variables and Objects
 */
extern Serial_impl Serial;

#endif
