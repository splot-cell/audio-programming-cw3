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
    int numSamples = buffer.getNumSamples();
    
    for (int i = 0; i < numSamples; ++i)
    {
//        float readPointF = readPoint[channel];
//        int readPointI = (int) readPointF;
//        float interpDelta = readPointF - readPointI;
//        float nextSample = delayLine.getSample (channel, readPointI % delaySize);
//        
//        float outputSample = delayLine.getSample (channel, readPointI) +
//                              interpDelta * (nextSample - delayLine.getSample (channel, readPointI));
//        
//        readPoint[channel] = readPoint[channel] + 1;
//        
//        delayLine.setSample (channel, writePoint[channel], buffer.getSample (channel, i) +
//                             (feedback * outputSample));
        
        float outputSample = delayLine.getSample (channel, readPoint[channel]);
        delayLine.setSample (channel, writePoint[channel], buffer.getSample (channel, i));
        delayLine.addSample (channel, writePoint[channel], outputSample * feedback);
        buffer.addSample (channel, i, outputSample);
        readPoint[channel] = (readPoint[channel] + 1) % delaySize;
        writePoint[channel] = (writePoint[channel] + 1) % delaySize;
    }
}

void VariableDelayLine::prepareDelayLine (int sr, int numChannels)
{
    samplerate = sr;
    delayLine.setSize (numChannels, samplerate, true, true);
    delayLine.clear();
    
    /* Assumes correct use: numChannels <= g_maxChannels. */
    for (int i = 0; i < numChannels; ++i)
    {
        writePoint[i] = 0;
        readPoint[i] = 0;
    }
    
    setDelaySize (0.5);
}

void VariableDelayLine::setDelaySize (float time)
{
    delaySize = (int) (time * samplerate);
}
