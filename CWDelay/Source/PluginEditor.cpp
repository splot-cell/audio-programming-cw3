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

//[Headers] You can add your own extra header files here...
#include "PluginProcessor.h"
//[/Headers]

#include "PluginEditor.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
enum
{
    parameterPercentagePadding = 10,
    parameterSliderPadding = 2,
    parameterLabelSpacing = 8,
    parameterLabelHeight = 24,
    parameterButtonHeight = 50
};
//[/MiscUserDefs]

//==============================================================================
CwdelayAudioProcessorEditor::CwdelayAudioProcessorEditor (CwdelayAudioProcessor& p, AudioProcessorValueTreeState& vts)
    : AudioProcessorEditor (&p), processor (p), valueTreeState (vts)
{
    //[Constructor_pre] You can add your own custom stuff here..
    numberOfSliders = 0;
    numberOfRows = 1; // Initialise to one in order to centralise coordinates.

    inputGainLabel.setText ("Input Gain", dontSendNotification);
    addAndMakeVisible (inputGainLabel);

    inputGainSlider.setSliderStyle (juce::Slider::LinearBarVertical);
    inputGainSlider.setTextValueSuffix("dB");
    addAndMakeVisible (inputGainSlider);
    inputGainAttachment = new SliderAttachment (valueTreeState, "inputGain", inputGainSlider);
    ++numberOfSliders;

    outputGainLabel.setText ("Output Gain", dontSendNotification);
    addAndMakeVisible (outputGainLabel);

    outputGainSlider.setSliderStyle (juce::Slider::LinearBarVertical);
    outputGainSlider.setTextValueSuffix("dB");
    addAndMakeVisible (outputGainSlider);
    outputGainAttachment = new SliderAttachment (valueTreeState, "outputGain", outputGainSlider);
    ++numberOfSliders;

    delayTimeLabel.setText ("Delay Time", dontSendNotification);
    addAndMakeVisible (delayTimeLabel);

    delayTimeSlider.setSliderStyle (juce::Slider::LinearBarVertical);
    delayTimeSlider.setTextValueSuffix ("s");
    addAndMakeVisible (delayTimeSlider);
    delayTimeAttachment = new SliderAttachment (valueTreeState, "delayTime", delayTimeSlider);
    ++numberOfSliders;

    feedbackLabel.setText ("Feedback", dontSendNotification);
    addAndMakeVisible (feedbackLabel);

    feedbackSlider.setSliderStyle (juce::Slider::LinearBarVertical);
    addAndMakeVisible (feedbackSlider);
    feedbackAttachment = new SliderAttachment (valueTreeState, "feedback", feedbackSlider);
    ++numberOfSliders;

    dryWetLabel.setText ("Dry Wet Mix", dontSendNotification);
    addAndMakeVisible (dryWetLabel);

    dryWetSlider.setSliderStyle (juce::Slider::LinearBarVertical);
    dryWetSlider.setTextValueSuffix ("%");
    addAndMakeVisible (dryWetSlider);
    dryWetAttachment = new SliderAttachment (valueTreeState, "wetLevel", dryWetSlider);
    ++numberOfSliders;

    tapeModeButton.setButtonText ("Tape Mode");
    addAndMakeVisible (tapeModeButton);
    tapeModeAttachment = new ButtonAttachment (valueTreeState, "tapeMode", tapeModeButton);
    ++numberOfSliders;
    ++numberOfRows;

    crossModeButton.setButtonText ("Cross-over Mode\n(stereo only)");
    addAndMakeVisible (crossModeButton);
    crossModeAttachment = new ButtonAttachment (valueTreeState, "crossMode", crossModeButton);
    ++numberOfRows;
    //[/Constructor_pre]


    //[UserPreSize]
    //[/UserPreSize]

    setSize (600, 400);


    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

CwdelayAudioProcessorEditor::~CwdelayAudioProcessorEditor()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]



    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void CwdelayAudioProcessorEditor::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colour (0xff323e44));

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void CwdelayAudioProcessorEditor::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    Rectangle<int> r = getLocalBounds();
    {
        Rectangle<int> sliderArea = r.reduced
            (r.getWidth() * 0.01 * parameterPercentagePadding, r.getHeight() * 0.01 * parameterPercentagePadding);

        Rectangle<int> labelArea = sliderArea.removeFromBottom (parameterLabelHeight);
        sliderArea.removeFromBottom (parameterLabelSpacing); // Add paddding between sliders and labels

        const float horizontalSpacing = sliderArea.getWidth() / numberOfSliders;
        const float verticalSpacing = sliderArea.getHeight() / numberOfRows;

        inputGainLabel.setBounds (labelArea.removeFromLeft (horizontalSpacing));
        inputGainSlider.setBounds (sliderArea.removeFromLeft (horizontalSpacing).reduced (parameterSliderPadding));

        delayTimeLabel.setBounds (labelArea.removeFromLeft (horizontalSpacing));
        delayTimeSlider.setBounds (sliderArea.removeFromLeft (horizontalSpacing).reduced (parameterSliderPadding));

        feedbackLabel.setBounds (labelArea.removeFromLeft (horizontalSpacing));
        feedbackSlider.setBounds (sliderArea.removeFromLeft (horizontalSpacing).reduced (parameterSliderPadding));

        dryWetLabel.setBounds (labelArea.removeFromLeft (horizontalSpacing));
        dryWetSlider.setBounds (sliderArea.removeFromLeft (horizontalSpacing).reduced (parameterSliderPadding));

        outputGainLabel.setBounds (labelArea.removeFromLeft (horizontalSpacing));
        outputGainSlider.setBounds (sliderArea.removeFromLeft (horizontalSpacing).reduced (parameterSliderPadding));

        const float remainingWidth = r.getRight() - sliderArea.getX();
        sliderArea.removeFromTop (verticalSpacing);
        tapeModeButton.setBounds (sliderArea.getX(), sliderArea.getY(), remainingWidth, parameterButtonHeight);
        sliderArea.removeFromTop (verticalSpacing);
        crossModeButton.setBounds (sliderArea.getX(), sliderArea.getY(), remainingWidth, parameterButtonHeight);
    }
    //[/UserPreResize]

    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="CwdelayAudioProcessorEditor"
                 componentName="" parentClasses="public AudioProcessorEditor"
                 constructorParams="CwdelayAudioProcessor&amp; p, AudioProcessorValueTreeState&amp; vts"
                 variableInitialisers="AudioProcessorEditor (&amp;p), processor (p), valueTreeState (vts)"
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="0" initialWidth="600" initialHeight="400">
  <BACKGROUND backgroundColour="ff323e44"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
