/*
  ==============================================================================

    GainSlider.cpp
    Created: 13 Jan 2018 4:30:21pm
    Author:  Olly Seber

  ==============================================================================
*/

#include "GainSlider.h"


String GainSlider::getTextFromValue (double value)
{
    if (value <= minDecibelValue)
        return String ("-inf ") + getTextValueSuffix();
    
    if (getNumDecimalPlacesToDisplay() > 0)
        return String (value, getNumDecimalPlacesToDisplay()) + getTextValueSuffix();
    
    return String (roundToInt (value)) + getTextValueSuffix();
}
