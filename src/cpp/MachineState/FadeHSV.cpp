/*
 **********************************************************************
 *  File   : FadeHSV.cpp
 *  Author : peach
 *  Date   : 19 Dec 2017
 **********************************************************************
 */

#include "Arduino.h"
#include "FadeHSV.h"
#include "Constants.h"

/*
 * Public Functions
 */
FadeHSV::FadeHSV() {
    _hue = 0;
}

void FadeHSV::enterState() {
    _hue = 0;
    _setHue();
}
void FadeHSV::iterateState() {

    // Increase Hue and set to 0 if > 360
    _hue = _hue == 360 ? 0 : _hue + 1;
    _setHue();
    delay(20);
}

/*
 * Private Functions
 */

void FadeHSV::_setHue() {

    int brightness = 1;
    unsigned int hue_times_100 = _hue * 100;
    int hp_times_100 = hue_times_100 / 60;
    double remainder = (hp_times_100 % 200) / 100.0;
    double x = brightness * (1 - abs(remainder - 1));

    double Rp;
    double Gp;
    double Bp;
    if (_hue < 60) {
        Rp = brightness;
        Gp = x;
        Bp = 0;
    } else if (_hue < 120) {
        Rp = x;
        Gp = brightness;
        Bp = 0;
    } else if (_hue < 180) {
        Rp = 0;
        Gp = brightness;
        Bp = x;
    } else if (_hue < 240) {
        Rp = 0;
        Gp = x;
        Bp = brightness;
    } else if (_hue < 300) {
        Rp = x;
        Gp = 0;
        Bp = brightness;
    } else {
        Rp = brightness;
        Gp = 0;
        Bp = x;
    }

    int R = 255 * Rp;
    int G = 255 * Gp;
    int B = 255 * Bp;

    analogWrite(PIN_R, R);
    analogWrite(PIN_G, G);
    analogWrite(PIN_B, B);

    return;

}
