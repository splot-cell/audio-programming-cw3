/*
  ==============================================================================

    FilterDSP.cpp
    Created: 28 Dec 2017 12:05:58pm
    Author:  Olly Seber

  ==============================================================================
*/

#include "FilterDSP.h"
#include <cmath>

//==============================================================================
                                
void LPFilter::setCoef (float frequency, float samplerate)
{
    /* Adjust frequency for samplerate. */
    double ft = frequency / samplerate;
    
    /* Filter order over 2. */
    float M_2 = (numCoefs - 1) / 2.0;
    
    /* Calculate each coefficient. */
    for (int i = 0; i < numCoefs; ++i)
    {
        if ( i == M_2)
            coefficients[i] = 2 * ft;
        else
            coefficients[i] = sin (2. * double_Pi * ft * (i - M_2)) / (double_Pi * (i - M_2));
        
        /* Hamming window. */
        coefficients[i] *= 0.54 - (0.46 * cos (2.0 * double_Pi * i / (numCoefs - 1)));
        /* I double checked and it's actually definitely doing it this time! */
    }
    
}

void LPFilter::prepareForAudio (float frequency, float samplerate, int numChannels)
{
    setCoef (frequency, samplerate);
    delayLine.setSize (numChannels, numCoefs);
    delayLine.clear();
    writePoint.calloc (numChannels); // Frees, allocates, and clears. Thanks JUCE.
}

void LPFilter::freeMemory()
{
    delayLine.setSize (0, 0);
    writePoint.free();
}

float LPFilter::processSample (float sample, int channel)
{
    float out = 0;
    
    delayLine.setSample (channel, writePoint[channel], sample);
    
    for (int i = 0; i < numCoefs; ++i)
        out += coefficients[i] * delayLine.getSample (channel, (writePoint[channel] + numCoefs - i) % numCoefs);
    
    writePoint[channel] = (writePoint[channel] + 1) % numCoefs;
    
    return out;
}
