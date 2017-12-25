/*
 **********************************************************************
 *  File   : LevelAnalyzer.cpp
 *  Author : peach
 *  Date   : 19 Dec 2017
 **********************************************************************
 */

#include "LevelAnalyzer.h"
#include "Constants.h"

/*
 * To do smoothing, we're using a single-pole falloff to smooth out a signal.
 * E.g, the unit impulse response for any signal we "smooth" is an exponential decay
 * with a setting time of either 5s, 2s or 0.3s.
 *
 * The formula is this:
 *
 *     0.02 = R ** NS
 *
 * where R is the multiplier and NS is the number of samples that corresponds
 * to the settling time.
 *
 * Values are calculated by hand so I don't need to use the math library at runtime.
 * The time between successive iterations is nominally 1.506 milliseconds.
 *
 * NS = <settling time> / <iteration period>
 * R  = 0.02 ** (1/NS)
 */

// Shortcut for channel holders.
#define CHANNEL_R 0
#define CHANNEL_G 1
#define CHANNEL_B 2

/*
 * Public Functions
 */
LevelAnalyzer::LevelAnalyzer() {
    reset();
}

void LevelAnalyzer::getRgbFromBuckets(int rgb[], int buckets[]) {

    // The values sampled from the various buckets.
    double Rt = 1;
    double Gt = 1;
    double Bt = 1;

    // The log of the sampled bucket is proportional to perceived loudness.
    double logLevel = 0;

    // Sample each of the buckets. For each bucket (channel), allocate some points to
    // each of R, G and B.
    for (int channel = 0; channel < NUM_FREQUENCY_BINS; channel++) {

        // Sample the bucket and take the log. In the event of an invalid value, juse use zero.
        logLevel = _fastLog(buckets[channel]);

        // Allocate the value to the RGB channels.
        switch (channel) {
        case 0:
            Rt += 1.5 * logLevel;
            break;
        case 1:
            Rt += logLevel;
            break;
        case 2:
            Rt += 0.5 * logLevel;
            break;
        case 3:
        case 4:
            Gt += logLevel;
            break;
        case 5:
        case 6:
        default:
            Bt += logLevel;
        }
    }

    rgb[CHANNEL_R] = _processChannel(CHANNEL_R, Rt);
    rgb[CHANNEL_G] = _processChannel(CHANNEL_G, Gt);
    rgb[CHANNEL_B] = _processChannel(CHANNEL_B, Bt);

    return;
}

void LevelAnalyzer::reset() {

    int channels[] = { CHANNEL_R, CHANNEL_G, CHANNEL_B };
    for (int i : channels) {
        _xAverage[i] = 0;
        _xS1[i] = 0;
        _xS2[i] = 0;
        _xMax[i] = 0;
        _xMin[i] = 0;
    }
}

/*
 * Private Functions
 */
int LevelAnalyzer::_processChannel(const int channel, double Xt) {
    int result = 0;

    // Subtract the null offset
#define NULL_OFFSET_R 6.75
#define NULL_OFFSET_G 4.5
#define NULL_OFFSET_B 3.8

    Xt -= channel == CHANNEL_R ? NULL_OFFSET_R :
          channel == CHANNEL_G ? NULL_OFFSET_G : NULL_OFFSET_B;

    // Smooth
#define R_S1 0.85
    double & xSmooth1 = _xS1[channel];
    _smooth(Xt, xSmooth1, R_S1);

#define MULT_R 5
#define MULT_G 2
#define MULT_B 1

    double multiplier = channel == CHANNEL_R ? MULT_R :
                        channel == CHANNEL_G ? MULT_G : MULT_B;
    Xt = xSmooth1 * multiplier;
    // Subtract the noise offset.
#define NOISE_OFFSET 1
    Xt -= NOISE_OFFSET;

    Xt = Xt < 0 ? 0 : Xt;

    // Get the maximum.
#define R_MAX_MIN 0.999
    double & xMax = _xMax[channel];
    double & xMin = _xMin[channel];
    xMax = xSmooth1 > xMax ? xSmooth1 : xMax * R_MAX_MIN;
    xMin = 0;    //xSmooth1 < xMin ? xSmooth1 : xMin * R_MAX_MIN;

    // If there's a max, scale to meet it.
    if (xMax > 0) {
        Xt = Xt / xMax;
    }

    // Smooth again.
    double & xSmooth2 = _xS2[channel];
#define R_S2 0.997
    _smooth(Xt, xSmooth2, R_S2);

    // Scale up to 255.
    Xt *= 255.0;

    // Clamp the value between 0 and 255 as an integer.
    result = _clamp(Xt, 1, 255);
    return result;
}

int LevelAnalyzer::_clamp(const double inputValue, const int lower,
        const int higher) {
    int result = inputValue > higher ? higher :
                 inputValue < lower ? lower : inputValue;

    return result;
}

// Fast log function uses three quadratic polynomials in different parts of the domain
// to kind-of fit the log curve on 0 - 1024.
double LevelAnalyzer::_fastLog(const int & x) {
    double result = 0;
    double a;
    double b;
    double c;

    if (x < 2) {
        return result;
    } else if (x < 10) {
        a = -6.11e-3;
        b = 0.152;
        c = 0.301;
    } else if (x < 100) {
        a = -1.03e-4;
        b = 0.0208;
        c = 0.916;
    } else {
        a = -1.06e-6;
        b = 2.11e-3;
        c = 1.91;
    }

    result = a * x * x + b * x + c;
    return result;
}

void LevelAnalyzer::_smooth(const double & newValue, double & smoothedValue,
        const double decayValue) {
    // Update the "previous value" to be the smoothed result of the current value and the previous value.
    smoothedValue = (1 - decayValue) * newValue + decayValue * smoothedValue;
}

