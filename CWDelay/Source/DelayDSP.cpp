/*
  ==============================================================================

    DelayDSP.cpp
    Created: 28 Dec 2017 12:05:49pm
    Author:  Olly Seber

  ==============================================================================
*/

#include "DelayDSP.h"

//==============================================================================

float VariableDelayLine::getSample (float delaySize, int channel)
{
    /* Get integer value for sample delay and calculate the difference between the float and int values. */
    const int delaySizeI = (int) delaySize;
    const float interpDelta = delaySize - delaySizeI;
    
    /* Get the readPoint by offsetting from writePoint.
     * writepoint points at the sample just written. Allows zero sample delay. */
    const int readPoint = (writePoint[channel] - delaySizeI + delayLength) % delayLength;
    
    /* Interpolation. */
    const float sampleA = delayLine.getSample (channel, readPoint);
    const float sampleB = delayLine.getSample (channel, (readPoint - 1 + delayLength) % delayLength);
    
    return sampleA + interpDelta * (sampleB - sampleA); // Only one multiplication in this arrangement!
}

void VariableDelayLine::writeSample (float value, int channel)
{
    /* writePoint always points at the sample just written. This allows a delay of zero samples. */
    delayLine.setSample (channel, ++writePoint[channel] % delayLength, value);
    writePoint[channel] = writePoint[channel] % delayLength;
}

void VariableDelayLine::prepareDelayLine (int delaySize, int numChannels)
{
    delayLine.setSize (numChannels, delaySize);
    delayLine.clear();
    
    delayLength = delaySize;
    
    writePoint.calloc (numChannels);
}

void VariableDelayLine::freeMemory()
{
    delayLine.setSize (0, 0);
    writePoint.free();
}
