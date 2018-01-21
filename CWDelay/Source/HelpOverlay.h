/*
  ==============================================================================

    HelpOverlay.h
    Created: 21 Jan 2018 2:19:52pm
    Author:  Olly Seber

  ==============================================================================
*/

/*
 GUI component for providing a help text overlay within the plugin window.
 
 The GUI updates every time the repaint() method is called from the parent component to either
 show the help text or not.
 */

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================

class HelpOverlay    : public Component
{
public:
    HelpOverlay();
    ~HelpOverlay();

    void paint (Graphics&) override;
    void resized() override;

private:
    bool onTop; // Used to track awareness of being on top or behind other components.
    Label helpButton; // Not actually a button but used as a "hover sensitive" button hence the name.
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (HelpOverlay)
};
