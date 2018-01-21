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
/* ENUM for storing some GUI parameters, for easy editing. */
enum
{
    parameterPercentagePadding = 10, // Margin around the GUI controls.
    parameterSliderPadding = 2, // Padding between sliders.
    parameterLabelSpacing = 8, // Spacing between labels and sliders.
    parameterLabelHeight = 24,
    parameterButtonHeight = 50
};
//[/MiscUserDefs]

//==============================================================================
CwdelayAudioProcessorEditor::CwdelayAudioProcessorEditor (CwdelayAudioProcessor& p, AudioProcessorValueTreeState& vts)
    : AudioProcessorEditor (&p), processor (p), valueTreeState (vts)
{
    //[Constructor_pre] You can add your own custom stuff here..
    numberOfColumns = 0; // Keep track of number of columns in grid layout.
    numberOfRows = 1; // Start counting at one in order to centre rows vertically.

    /* Add all the labels and sliders.
     * The attachment objects connect these controls to the parameters stored in the ValueTreeState
     * object in the processor. */
    inputGainLabel.setText ("Input Gain", dontSendNotification);
    addAndMakeVisible (inputGainLabel);

    inputGainSlider.setSliderStyle (juce::Slider::LinearBarVertical);
    inputGainSlider.setTextValueSuffix("dB");
    addAndMakeVisible (inputGainSlider);
    inputGainAttachment = new SliderAttachment (valueTreeState, "inputGain", inputGainSlider);
    ++numberOfColumns; // Add another column for each slider.

    outputGainLabel.setText ("Output Gain", dontSendNotification);
    addAndMakeVisible (outputGainLabel);

    outputGainSlider.setSliderStyle (juce::Slider::LinearBarVertical);
    outputGainSlider.setTextValueSuffix("dB");
    addAndMakeVisible (outputGainSlider);
    outputGainAttachment = new SliderAttachment (valueTreeState, "outputGain", outputGainSlider);
    ++numberOfColumns;

    delayTimeLabel.setText ("Delay Time", dontSendNotification);
    addAndMakeVisible (delayTimeLabel);

    delayTimeSlider.setSliderStyle (juce::Slider::LinearBarVertical);
    delayTimeSlider.setTextValueSuffix ("s");
    addAndMakeVisible (delayTimeSlider);
    delayTimeAttachment = new SliderAttachment (valueTreeState, "delayTime", delayTimeSlider);
    ++numberOfColumns;

    feedbackLabel.setText ("Feedback", dontSendNotification);
    addAndMakeVisible (feedbackLabel);

    feedbackSlider.setSliderStyle (juce::Slider::LinearBarVertical);
    feedbackSlider.setTextValueSuffix ("%");
    addAndMakeVisible (feedbackSlider);
    feedbackAttachment = new SliderAttachment (valueTreeState, "feedback", feedbackSlider);
    ++numberOfColumns;

    dryWetLabel.setText ("Dry Wet Mix", dontSendNotification);
    addAndMakeVisible (dryWetLabel);

    dryWetSlider.setSliderStyle (juce::Slider::LinearBarVertical);
    dryWetSlider.setTextValueSuffix ("%");
    addAndMakeVisible (dryWetSlider);
    dryWetAttachment = new SliderAttachment (valueTreeState, "wetLevel", dryWetSlider);
    ++numberOfColumns;

    /* Add all the buttons. */
    tapeModeButton.setButtonText ("Tape Mode");
    addAndMakeVisible (tapeModeButton);
    tapeModeAttachment = new ButtonAttachment (valueTreeState, "tapeMode", tapeModeButton);
    ++numberOfColumns; // Add a column for the buttons.
    ++numberOfRows; // Add a row for each new button.

    crossModeButton.setButtonText ("Cross-over Mode\n(stereo only)");
    addAndMakeVisible (crossModeButton);
    crossModeAttachment = new ButtonAttachment (valueTreeState, "crossMode", crossModeButton);
    ++numberOfRows;

    filterButton.setButtonText ("Filter");
    addAndMakeVisible (filterButton);
    filterAttachment = new ButtonAttachment (valueTreeState, "filterOn", filterButton);
    ++numberOfRows;

    /* Add the help text component. */
    addAndMakeVisible (helpComponent, 0); // Zero flag to add component behind its siblings.
    //[/Constructor_pre]


    //[UserPreSize]
    //[/UserPreSize]

    setSize (600, 400);


    //[Constructor] You can add your own custom stuff here..
    startTimer (10); // Timer callback required to update for the help component.
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
    /* Get the size of the window. */
    Rectangle<int> r = getLocalBounds();
    {
        /* Get an area for sliders that is reduced by the padding percentage set in the enum above.
         * Multiply by 0.01 is because this is a percentage. */
        Rectangle<int> sliderArea = r.reduced
            (r.getWidth() * 0.01 * parameterPercentagePadding, r.getHeight() * 0.01 * parameterPercentagePadding);

        /* Get an area for labels. */
        Rectangle<int> labelArea = sliderArea.removeFromBottom (parameterLabelHeight);
        sliderArea.removeFromBottom (parameterLabelSpacing); // Add paddding between sliders and labels
        
        /* Calculate spacing of rows and columns in the grid from number of sliders and buttons
         * added in constructor. */
        const float horizontalSpacing = sliderArea.getWidth() / numberOfColumns;
        const float verticalSpacing = sliderArea.getHeight() / numberOfRows;
        
        /* Set the size of each label and slider based on the spacing calculated. Add padding to sliders. */
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
        
        /* Let the text run into the margin, as it looks better than wrapping for no real reason.
         * First calculate remaining space in window. */
        const float remainingWidth = r.getRight() - sliderArea.getX();
        /* Set sizing and position of each button. */
        sliderArea.removeFromTop (verticalSpacing);
        tapeModeButton.setBounds (sliderArea.getX(), sliderArea.getY(), remainingWidth, parameterButtonHeight);
        sliderArea.removeFromTop (verticalSpacing);
        crossModeButton.setBounds (sliderArea.getX(), sliderArea.getY(), remainingWidth, parameterButtonHeight);
        sliderArea.removeFromTop (verticalSpacing);
        filterButton.setBounds (sliderArea.getX(), sliderArea.getY(), remainingWidth, parameterButtonHeight);
        
        /* Help component should cover whole window. */
        helpComponent.setBounds (getLocalBounds());
    }
    //[/UserPreResize]

    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
/* No callback function for a mouse hover, so have to use a timer to update the help text component. */
void CwdelayAudioProcessorEditor::timerCallback()
{
    helpComponent.repaint(); // Calls the paint() method.
}
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="CwdelayAudioProcessorEditor"
                 componentName="" parentClasses="public AudioProcessorEditor, public Timer"
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
