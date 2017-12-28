/*
 **********************************************************************
 *  File   : LevelAnalyzer.h
 *  Author : peach
 *  Date   : 19 Dec 2017
 **********************************************************************
 */

/*
 * Platform-agnostic library to analyze the music levels in the provided buckets
 * and find a good way of mapping those values to RGB.
 */

#ifndef LEVELANALYZER_LEVELANALYZER_H_
#define LEVELANALYZER_LEVELANALYZER_H_

class LevelAnalyzer {
private:

    // Total allocation of bins to each channel.
    // e.g If each of the 7 bins was multiplied by 0.5 and added to Green,
    // Green would have 3.5.
    double _totalAllocation[3];

    // Average values of input
    double _xAverage[3];

    // First smoothing
    double _xS1[3];

    // Second smoothing
    double _xS2[3];

    // Running Maximum
    double _xMax[3];

    /*
     *
     * Processes a single channel in R,G,B.
     * Channel: 0 => R
     *          1 => G
     *          2 => B
     * Xt is the value of R,G,B right after being assigned the levels from the buckets.
     *
     * Returns the new smoothed channel value, on 0 to 255.
     */
    int _processChannel(const int channel, double Xt);

    // Forces a given double value to be between the lower and upper integers.
    int _clamp(const double inputValue, const int lower, const int higher);

    // Behaves kind of like a logarithmic function but is only good for qualitative things like
    // perceived loudness and brightness, and only when 'x' is < 1024.
    double _fastLog(const int & x);

    // Smooths the provided value by the decay constant and previous value.
    void _smooth(const double & newValue, double & smoothedValue,
            const double decayValue);

public:
    LevelAnalyzer();

    // Given an RGB array (3-elements) and buckets array (7), the values are mapped to RGB
    // from the frequency bins.
    void getRgbFromBuckets(int rgb[], int buckets[]);

    // Clears all integrated values, averages and biases.
    void reset();
};

#endif /* LEVELANALYZER_LEVELANALYZER_H_ */
