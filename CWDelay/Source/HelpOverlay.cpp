/*
  ==============================================================================

    HelpOverlay.cpp
    Created: 21 Jan 2018 2:19:52pm
    Author:  Olly Seber

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "HelpOverlay.h"

//==============================================================================
HelpOverlay::HelpOverlay() : onTop (false)
{
    helpButton.setText ("Hover for help", dontSendNotification);
    addAndMakeVisible (helpButton);
}

HelpOverlay::~HelpOverlay()
{
}

void HelpOverlay::paint (Graphics& g)
{
    if (helpButton.isMouseOver())
    {
        /* If this component is not in front of its siblings bring it in front of them. This is
         * required so the help text is on top of the other GUI controls when it is showing. */
        if (onTop == false)
        {
            toFront (false);
            onTop = true;
        }
        
        /* Give a semi-transparent background to hide the other GUI elements. */
        g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId).withAlpha (0.9f).withMultipliedBrightness (0.5f));
        
        g.setOpacity (1.0);
        g.setColour (Colours::grey);
        g.drawRect (getLocalBounds(), 2); // Border.
        
        g.setColour (Colours::white);
        g.setFont (14.0f);
        
        String message("Welcome to the third and final installment of...\n>>> Olly's Audio Programming Coursework! <<<\nYou didn't think I wouldn't include a message, did you?!\n\n---\n\nMost of this GUI should be fairly self explanitory, but here are some hints:\n\n Tape Mode adds two LFOs to the delay time to give some (tasteless) WOW and flutter.\n\nCross Mode adds a ping-pong effect to each delay through the feedback loop.\nTry feeding audio to only one channel to hear the effects!\nNote, this will only be effective when the plugin is loaded in a stereo configuration.\n\nThe Filter switch simply turns the 2kHz LPF on or off!\n\n---\n\nAlso, I'm aware the the JUCE logo covers the hover label for this help text upon loading the plugin.\n I wanted it in the bottom right so I did it anyway, patience is a virtue, no?\n\nThank you for reading, friends; this module is now over and so is our journey.");
        
        const int maxNumLines = 40; // Arbitrary line limit big enough to display all the abvove text
        
        g.drawFittedText (message, getLocalBounds(), Justification::centred, maxNumLines);
    }
    else
    {
        /* If this component is in front of its siblings, bring it behind them. This is required so
         * the user can interact with the other GUI components when no help text is showing. */
        if (onTop == true)
        {
            toBack();
            onTop = false;
        }
    }
}

void HelpOverlay::resized()
{
    Rectangle<int> r = getLocalBounds();

    const int buttonWidth = 100;
    const int buttonHeight = 32;
    
    helpButton.setBounds (r.getRight() - buttonWidth, r.getBottom() - buttonHeight, buttonWidth, buttonHeight);
}
