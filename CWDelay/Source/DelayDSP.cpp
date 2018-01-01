/*
  ==============================================================================

    DelayDSP.cpp
    Created: 28 Dec 2017 12:05:49pm
    Author:  Olly Seber

  ==============================================================================
*/

#include "DelayDSP.h"

//void VariableDelayLine::processAudio (AudioBuffer<float>& buffer, int channel)
//{
//    int maxDelay = delayLine.getNumSamples();
//    
//    for (int i = 0; i < buffer.getNumSamples(); ++i)
//    {
//        float readPointF = writePoint[channel] - delaySize.getNextValue();
//        readPointF = (readPointF >= 0 ? (readPointF < maxDelay ? readPointF : readPointF - maxDelay) : readPointF + maxDelay);
//        int readPointI = (int) readPointF;
//        float interpDelta = readPointF - readPointI;
//        float nextSample = (readPointI < maxDelay - 1 ? delayLine.getSample (channel, readPointI + 1) : delayLine.getSample (channel, 0));
//        float outputSample = delayLine.getSample (channel, readPointI) +
//                            interpDelta * (nextSample - delayLine.getSample (channel, readPointI));
//        delayLine.setSample (channel, writePoint[channel], buffer.getSample (channel, i));
//        delayLine.addSample (channel, writePoint[channel], outputSample * feedback);
//        
//        float wetLevel = dryWet.getNextValue();
//        buffer.applyGain (channel, i, 1, 1. - wetLevel);
//        buffer.addSample (channel, i, outputSample * wetLevel);
//        writePoint[channel] = (writePoint[channel] + 1) % maxDelay;
//    }
//}

float VariableDelayLine::getSample (float delaySize, int channel)
{
    const int delaySizeI = (int) delaySize;
    const float interpDelta = delaySize - delaySizeI;
    
    const int readPoint = (writePoint[channel] - delaySizeI) & bitMask;
    
    const float sampleA = delayLine.getSample (channel, readPoint);
    const float sampleB = delayLine.getSample (channel, (readPoint - 1) & bitMask);
    
    return sampleA + interpDelta * (sampleB - sampleA);
}

void VariableDelayLine::writeSample (float value, int channel)
{
    delayLine.setSample (channel, ++writePoint[channel] & bitMask, value);
}

void VariableDelayLine::prepareDelayLine (int delaySize, int numChannels)
{
    delayLine.setSize (numChannels, nextPowerOfTwo (delaySize), true, true);
    delayLine.clear();
    
    bitMask = nextPowerOfTwo (delaySize) - 1;
    
    for (int i = 0; i < numChannels; ++i)
        writePoint[i] = 0;
}
