/*
  ==============================================================================

    DelayDSP.cpp
    Created: 28 Dec 2017 12:05:49pm
    Author:  Olly Seber

  ==============================================================================
*/

#include "DelayDSP.h"

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
    delayLine.setSize (numChannels, nextPowerOfTwo (delaySize));
    delayLine.clear();
    
    writePoint.calloc (numChannels);
    
    bitMask = nextPowerOfTwo (delaySize) - 1;
    
    for (int i = 0; i < numChannels; ++i)
        writePoint[i] = 0;
}
