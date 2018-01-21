/*
  ==============================================================================

    DelayDSP.h
    Created: 28 Dec 2017 12:05:49pm
    Author:  Olly Seber

  ==============================================================================
*/

/*
 Simple delay line class implementing linear interpolation between samples.
 */

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================

class VariableDelayLine
{
public:
    void prepareDelayLine (int delaySize, int numChannels);
    
    void freeMemory (void);
    
    void writeSample (float value, int channel);
    
    float getSample (float delaySize, int channel);
    
    VariableDelayLine () {}
    
    ~VariableDelayLine() {}
    
protected:
    
private:
    AudioBuffer<float> delayLine;
    HeapBlock<int, true> writePoint;
    int delayLength;
};
