/*
  ==============================================================================

    FilterDSP.h
    Created: 28 Dec 2017 12:05:58pm
    Author:  Olly Seber

  ==============================================================================
*/

/*
 Low-pass FIR filter class.
 */

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================

class LPFilter
{
public:
    /* Set up the filter coefficients and memeory. */
    void prepareForAudio (float frequency, float samplerate, int numChannels);
    
    /* Free allocated memory, except for the coefficients. */
    void freeMemory();
    
    /* Filter an individual sample. */
    float processSample (float sample, int channel);
    
    LPFilter (int order) : coefficients (order + 1), numCoefs (order + 1) {}
    
    ~LPFilter() {}
    
protected:
    
private:
    AudioBuffer<float> delayLine; // Store audio samples.
    HeapBlock<int> writePoint; // Keep a track of the write index for each channel.
    HeapBlock<double> coefficients; // Store filter coefficients.
    int numCoefs; // Keep a track of number of coefficients.
    
    void setCoef (float frequency, float samplerate); // Private function for calculating coeffs.
};
