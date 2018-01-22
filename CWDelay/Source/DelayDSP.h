/*
  ==============================================================================

    DelayDSP.h
    Created: 28 Dec 2017 12:05:49pm
    Author:  Olly Seber

  ==============================================================================
*/

/*
 Simple delay line class implementing linear interpolation between samples.
 
 Uses an AudioBuffer<float> for storing audio samples, and HeapBlock<int> for storing
 sample indexes for each channel.
 */

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================

class VariableDelayLine
{
public:
    /* Allocated required memory. */
    void prepareDelayLine (int delaySize, int numChannels);
    
    /* Free allocated memroy. */
    void freeMemory ();
    
    /* Write a sample to a channel of the delay. */
    void writeSample (float value, int channel);
    
    /* Get the next delayed sample for a given channel. */
    float getSample (float delaySize, int channel);
    
    VariableDelayLine () {}
    
    ~VariableDelayLine() {}
    
protected:
    
private:
    AudioBuffer<float> delayLine; // Store audio.
    HeapBlock<int, true> writePoint; // Keep track of write point for each channel.
    int delayLength; // Keep track of delay line length.
};
