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
 * with a given setting time.
 *
 * The formula looks like this:
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

// Smoothing amount to calculate running average.
#define R_AVG 0.995

// Initial Smoothing.
#define R_S1 0.85

// Smoothing amount for finding maximum signal level.
#define R_MAX_MIN 0.999

// High and low cutoff for throttling back the envelope on quiet notes.
// Note that these are multiplied by the total allocation for each color before cutting.
#define CUTOFF_A 0.22
#define CUTOFF_B 0.05

// Maximum gain after normalization.
#define MAX_GAIN 270

// Smoothing amount after normalization.
#define R_S2 0.9

// Shortcut for channel holders.
#define CHANNEL_R 0
#define CHANNEL_G 1
#define CHANNEL_B 2


/*
 * Constants
 */

// Bin to Color allocation.
double RLevels[] = { 1, 1, 0.8, 0, 0, 0, 0 };
double GLevels[] = { 0, 0, 0.5, 1, 1, 0, 0 };
double BLevels[] = { 0, 0, 0, 0, 0, 1, 1 };

/*
 * Public Functions
 */
LevelAnalyzer::LevelAnalyzer() {

    _totalAllocation[CHANNEL_R] = 0;
    _totalAllocation[CHANNEL_G] = 0;
    _totalAllocation[CHANNEL_B] = 0;
    for (int channel = 0; channel < NUM_FREQUENCY_BINS; channel++) {
        _totalAllocation[CHANNEL_R] += RLevels[channel];
        _totalAllocation[CHANNEL_G] += GLevels[channel];
        _totalAllocation[CHANNEL_B] += BLevels[channel];
    }

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
        Rt += RLevels[channel] * logLevel;
        Gt += GLevels[channel] * logLevel;
        Bt += BLevels[channel] * logLevel;
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
    }
}

/*
 * Private Functions
 */
int LevelAnalyzer::_processChannel(const int channel, double Xt) {
    int result = 0;

    // Get the average by smoothing long-time.
    double & xAverage = _xAverage[channel];
    _smooth(Xt, xAverage, R_AVG);

    // Subtract the average from the value and smooth to get the zero-centered,
    // smoothed value.
    double & xSmooth1 = _xS1[channel];
    _smooth(Xt - xAverage, xSmooth1, R_S1);

    // Get the maximum value of the smoothed signal
    double & xMax = _xMax[channel];
    xMax = xSmooth1 > xMax ? xSmooth1 : xMax * R_MAX_MIN;

    // Normalize in three steps:
    // 1. Discard all values in the signal that are negative. The positive loudness seems to be AC coupled.
    // 2. Create a multiplication envelope to stamp out quiet sounds and correctly scale loud ones.
    // 3. Scale from 0 to 255.

    double channelCutoffA = CUTOFF_A * _totalAllocation[channel];
    double channelCutoffB = CUTOFF_B * _totalAllocation[channel];

    double slope = (1 / channelCutoffA) / (channelCutoffA - channelCutoffB);
    double b = -slope * channelCutoffB;

    // Keep the positive xSmooth.
    double xNormalized = xSmooth1 > 0 ? xSmooth1 : 0;

    // Establish the envelope.
    double xEnvelope = 0;
    if (xMax < channelCutoffB) {
        // Zero out result
        xEnvelope = 0;
    } else if (xMax < channelCutoffA) {
        // xMax is on thin ice, so we reduce its amplitude.
        xEnvelope = slope * xMax + b;
    } else {
        xEnvelope = 1 / xMax;
    }

    // Multiply 'em and set the value of 'max'.
    Xt = xNormalized * xEnvelope * MAX_GAIN;

    // Smooth the resulting value.
    double & xSmooth2 = _xS2[channel];
    _smooth(Xt, xSmooth2, R_S2);

    // Clamp the value between 0 and 255 as an integer.
    result = _clamp(xSmooth2, 1, 255);
    return result;
}

int LevelAnalyzer::_clamp(const double inputValue, const int lower,
        const int higher) {
    int result = inputValue > higher ? higher :
                 inputValue < lower ? lower : inputValue;

    return result;
}

/*
 * The fast log function uses three quadratic polynomials in different parts of the domain
 * the to kind-of fit the log curve on 0 - 1024.
 *
 * This is definitely NOT a true log(X) function, but it gives us a good ballpark for what
 * we're using it for (e.g. brightness of light).
 */
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

