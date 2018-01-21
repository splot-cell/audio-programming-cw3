/*
  ==============================================================================

    GainSlider.h
    Created: 13 Jan 2018 4:30:21pm
    Author:  Olly Seber

  ==============================================================================
*/

/*
 Custom slider component.
 
 Displays gain in dB down to -inf dB, which is a user-setable limit.
 Rounds gain value to user-setable number of decimal places.
 */

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================

class GainSlider : public Slider
{
public:
    String getTextFromValue (double value) override;
    void setMinDecibelValue (double value) {minDecibelValue = value;}
    void setDecimalPlaces (int value) {decimalPlaces = value;}
private:
    float minDecibelValue = -90;
    int decimalPlaces = 2;
};
