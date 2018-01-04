/*
  ==============================================================================

    FilterDSP.cpp
    Created: 28 Dec 2017 12:05:58pm
    Author:  Olly Seber

  ==============================================================================
*/

#include "FilterDSP.h"
#include <cmath>
                                
void LPFilter::setCoef (float frequency, float samplerate)
{
    double ft = frequency / samplerate;
    float M_2 = (numCoefs - 1) / 2.0;
    
    for (int i = 0; i < numCoefs; ++i)
    {
        if ( i == M_2)
            coefficients[i] = 2 * ft;
        else
            coefficients[i] = sin (2. * double_Pi * ft * (i - M_2)) / (double_Pi * (i - M_2));
        
        /* Hamming window. */
        coefficients[i] *= 0.54 - (0.46 * cos (2.0 * double_Pi * i / (numCoefs - 1)));
    }
    
}

void LPFilter::prepareForAudio (float frequency, float samplerate, int numChannels)
{
    setCoef (frequency, samplerate);
    delayLine.setSize (numChannels, numCoefs);
    delayLine.clear();
    writePoint.calloc (numChannels); // Frees, allocats, and clears.
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
