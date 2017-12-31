/*
  ==============================================================================

    DelayDSP.h
    Created: 28 Dec 2017 12:05:49pm
    Author:  Olly Seber

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

const int g_maxChannels = 2;

class VariableDelayLine
{
public:
    void prepareDelayLine (int sr, int numChannels);
    
    void processAudio (AudioBuffer<float>& buffer, int channel);
    
    void setDelaySize (float time);
    
    void setFeedback (float fb);
    
    VariableDelayLine() : feedback (0.f) {}
    
protected:
    
private:
    AudioBuffer<float> delayLine;
    int writePoint[g_maxChannels];
    float feedback;
    float delaySize;
    float samplerate;
};
