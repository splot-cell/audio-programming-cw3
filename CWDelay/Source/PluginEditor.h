/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 5.2.0

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library - "Jules' Utility Class Extensions"
  Copyright (c) 2015 - ROLI Ltd.

  ==============================================================================
*/

#pragma once

//[Headers]     -- You can add your own extra header files here --
#include "../JuceLibraryCode/JuceHeader.h"
#include "GainSlider.h"
#include "HelpOverlay.h"
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    AUDIO PROCESSOR EDITOR
 
    PARAMETERS
    All UI controls are linked to the ValueTreeState object in the processor using "attachments".
    This allows easy updating of paramter changes.
 
    HELP OVERLAY
    The HelpOverlay component, which displays the help text, is implemented using a timer callback
    hence the timer base class. This is because the text shows when the use hovers the mouse over a
    certain part of the window. There is no JUCE callback for this. Instead, the timer callback
    triggers the component to check if the mouse if hovering and update itself.
 
    GUI DESIGN
    The GUI layout is based on a grid. The numberOfColumns and numberOfRows members keep track of
    how many GUI elements need to fit in the window. In the resize() method, these members are used
    to calculate the grid spacing, into which the sliders, labels, and buttons are placed.
                                                                    //[/Comments]
*/
class CwdelayAudioProcessorEditor  : public AudioProcessorEditor,
                                     public Timer
{
public:
    //==============================================================================
    CwdelayAudioProcessorEditor (CwdelayAudioProcessor& p, AudioProcessorValueTreeState& vts);
    ~CwdelayAudioProcessorEditor();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    /* typedefs for ease. Placed here so inside the class, but have to be public so ScopedPointer
     * can access them. */
    typedef AudioProcessorValueTreeState::SliderAttachment SliderAttachment;
    typedef AudioProcessorValueTreeState::ButtonAttachment ButtonAttachment;

    void timerCallback() override; // Used to update help text component.
    //[/UserMethods]

    void paint (Graphics& g) override;
    void resized() override;


    
private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    CwdelayAudioProcessor& processor;

    AudioProcessorValueTreeState& valueTreeState; // Reference to the valueTreeState in the processor.
    int numberOfColumns; // For easy distribution of controls within the plugin window.
    int numberOfRows; // For easy distribution of controls.
    
    /* GUI components. */
    Label inputGainLabel;
    GainSlider inputGainSlider;
    ScopedPointer<SliderAttachment> inputGainAttachment;

    Label outputGainLabel;
    GainSlider outputGainSlider;
    ScopedPointer<SliderAttachment> outputGainAttachment;

    Label delayTimeLabel;
    Slider delayTimeSlider;
    ScopedPointer<SliderAttachment> delayTimeAttachment;

    Label feedbackLabel;
    Slider feedbackSlider;
    ScopedPointer<SliderAttachment> feedbackAttachment;

    Label dryWetLabel;
    Slider dryWetSlider;
    ScopedPointer<SliderAttachment> dryWetAttachment;

    ToggleButton tapeModeButton;
    ScopedPointer<ButtonAttachment> tapeModeAttachment;

    ToggleButton crossModeButton;
    ScopedPointer<ButtonAttachment> crossModeAttachment;

    ToggleButton filterButton;
    ScopedPointer<ButtonAttachment> filterAttachment;

    HelpOverlay helpComponent;
    //[/UserVariables]

    //==============================================================================


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CwdelayAudioProcessorEditor)
};

//[EndFile] You can add extra defines here...
//[/EndFile]
