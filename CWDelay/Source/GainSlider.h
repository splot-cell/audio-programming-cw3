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
    void setMinDecibelValue (double value) {minDecibelValue = value;} // Simple set
    void setDecimalPlaces (int value) {decimalPlaces = value;} // Simple set
private:
    float minDecibelValue = -90; // Value to display -inf at
    int decimalPlaces = 2; // Decimal places to display gain values at
};
