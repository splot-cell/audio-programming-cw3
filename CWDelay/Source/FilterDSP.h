/*
  ==============================================================================

    FilterDSP.h
    Created: 28 Dec 2017 12:05:58pm
    Author:  Olly Seber

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class LPFilter
{
public:
    
    void prepareForAudio (float frequency, float samplerate, int numChannels);
    
    float processSample (float sample, int channel);
    
    LPFilter (int order) : coefficients (order + 1), numCoefs (order + 1) {}
    
    ~LPFilter() {}
    
protected:
    
private:
    AudioBuffer<float> delayLine;
    HeapBlock<int> writePoint;
    HeapBlock<double> coefficients;
    int numCoefs;
    
    void setCoef (float frequency, float samplerate);
};
