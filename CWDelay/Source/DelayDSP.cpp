/*
  ==============================================================================

    DelayDSP.cpp
    Created: 28 Dec 2017 12:05:49pm
    Author:  Olly Seber

  ==============================================================================
*/

#include "DelayDSP.h"

void VariableDelayLine::processAudio (AudioBuffer<float>& buffer, int channel)
{
    int maxDelay = delayLine.getNumSamples();
    
    for (int i = 0; i < buffer.getNumSamples(); ++i)
    {
        float readPointF = writePoint[channel] - delaySize.getNextValue();
        readPointF = (readPointF >= 0 ? (readPointF < maxDelay ? readPointF : readPointF - maxDelay) : readPointF + maxDelay);
        int readPointI = (int) readPointF;
        float interpDelta = readPointF - readPointI;
        float nextSample = (readPointI < maxDelay - 1 ? delayLine.getSample (channel, readPointI + 1) : delayLine.getSample (channel, 0));
        float outputSample = delayLine.getSample (channel, readPointI) +
                            interpDelta * (nextSample - delayLine.getSample (channel, readPointI));
        delayLine.setSample (channel, writePoint[channel], buffer.getSample (channel, i));
        delayLine.addSample (channel, writePoint[channel], outputSample * feedback);
        
        float wetLevel = dryWet.getNextValue();
        buffer.applyGain (channel, i, 1, 1. - wetLevel);
        buffer.addSample (channel, i, outputSample * wetLevel);
        writePoint[channel] = (writePoint[channel] + 1) % maxDelay;
    }
}

void VariableDelayLine::prepareDelayLine (int sr, int numChannels)
{
    samplerate = sr;
    delayLine.setSize (numChannels, samplerate, true, true);
    delayLine.clear();
    
    /* Assumes correct use: numChannels <= g_maxChannels. */
    for (int i = 0; i < numChannels; ++i)
        writePoint[i] = 0;
    
    delaySize.reset (sr, parameterSmoothingTime * 0.001);
    dryWet.reset (sr, parameterSmoothingTime * 0.001);
}

void VariableDelayLine::setDelaySize (float time)
{
    if (! delaySize.isSmoothing())
        delaySize.setValue (time * samplerate);
}

void VariableDelayLine::setFeedback (float fb)
{
    feedback = fb;
}

void VariableDelayLine::setDryWet (float ratio)
{
    if (! dryWet.isSmoothing())
        dryWet.setValue (ratio);
}
