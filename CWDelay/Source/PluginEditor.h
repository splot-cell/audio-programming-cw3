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
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Projucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class CwdelayAudioProcessorEditor  : public AudioProcessorEditor
{
public:
    //==============================================================================
    CwdelayAudioProcessorEditor (CwdelayAudioProcessor& p, AudioProcessorValueTreeState& vts);
    ~CwdelayAudioProcessorEditor();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    typedef AudioProcessorValueTreeState::SliderAttachment SliderAttachment;
    typedef AudioProcessorValueTreeState::ButtonAttachment ButtonAttachment;
    //[/UserMethods]

    void paint (Graphics& g) override;
    void resized() override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    CwdelayAudioProcessor& processor;

    AudioProcessorValueTreeState& valueTreeState;
    int numberOfSliders; // For easy distribution of controls within the plugin window.
    int numberOfRows;

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
    //[/UserVariables]

    //==============================================================================


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CwdelayAudioProcessorEditor)
};

//[EndFile] You can add extra defines here...
//[/EndFile]
