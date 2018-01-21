/*
  ==============================================================================

    HelpOverlay.h
    Created: 21 Jan 2018 2:19:52pm
    Author:  Olly Seber

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/*
*/
class HelpOverlay    : public Component
{
public:
    HelpOverlay();
    ~HelpOverlay();

    void paint (Graphics&) override;
    void resized() override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (HelpOverlay)
};
