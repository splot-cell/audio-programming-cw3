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
//[/MiscUserDefs]

//==============================================================================
CwdelayAudioProcessorEditor::CwdelayAudioProcessorEditor (CwdelayAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    addAndMakeVisible (inputGainSlider = new Slider ("Input Gain"));
    inputGainSlider->setTooltip (TRANS("Change the input level"));
    inputGainSlider->setRange (0, 1, 0);
    inputGainSlider->setSliderStyle (Slider::LinearBarVertical);
    inputGainSlider->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    inputGainSlider->addListener (this);

    addAndMakeVisible (inputGainLabel = new Label ("Input Gain Label",
                                                   TRANS("Input Gain")));
    inputGainLabel->setFont (Font (16.00f, Font::plain).withTypefaceStyle ("Regular"));
    inputGainLabel->setJustificationType (Justification::centred);
    inputGainLabel->setEditable (false, false, false);
    inputGainLabel->setColour (TextEditor::textColourId, Colours::black);
    inputGainLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));


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

    inputGainSlider = nullptr;
    inputGainLabel = nullptr;


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
    //[/UserPreResize]

    inputGainSlider->setBounds (40, 104, 40, 200);
    inputGainLabel->setBounds (16, 312, 80, 24);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void CwdelayAudioProcessorEditor::sliderValueChanged (Slider* sliderThatWasMoved)
{
    //[UsersliderValueChanged_Pre]
    //[/UsersliderValueChanged_Pre]

    if (sliderThatWasMoved == inputGainSlider)
    {
        //[UserSliderCode_inputGainSlider] -- add your slider handling code here..
        processor->inputGain = (float) inputGainSlider->getValue();
        //[/UserSliderCode_inputGainSlider]
    }

    //[UsersliderValueChanged_Post]
    //[/UsersliderValueChanged_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
AudioParameterFloat* getParameterForSlider (Slider* slider)
{
    const OwnedArray<AudioProcessorParameter>& params = getAudioProcessor()->getParameters();
    return dynamic_cast<AudioParameterFloat*> (params[paramSliders.indexOf (slider)]);
}

void Slider::Listener::sliderDragStarted (Slider* slider) override
{
    if (AudioParameterFloat* param = getParameterForSlider (slider))
        param->beginChangeGesture();
}

void Slider::Listener::sliderDragEnded (Slider* slider) override
{
    if (AudioParameterFloat* param = getParameterForSlider (slider))
        param->endChangeGesture();
}
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="CwdelayAudioProcessorEditor"
                 componentName="" parentClasses="public AudioProcessorEditor"
                 constructorParams="CwdelayAudioProcessor&amp; p" variableInitialisers="AudioProcessorEditor (&amp;p), processor (p)"
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="0" initialWidth="600" initialHeight="400">
  <BACKGROUND backgroundColour="ff323e44"/>
  <SLIDER name="Input Gain" id="ffbb1bc7392db120" memberName="inputGainSlider"
          virtualName="" explicitFocusOrder="0" pos="40 104 40 200" tooltip="Change the input level"
          min="0" max="1" int="0" style="LinearBarVertical" textBoxPos="NoTextBox"
          textBoxEditable="0" textBoxWidth="80" textBoxHeight="20" skewFactor="1"
          needsCallback="1"/>
  <LABEL name="Input Gain Label" id="d1a5405df03b1826" memberName="inputGainLabel"
         virtualName="" explicitFocusOrder="0" pos="16 312 80 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Input Gain" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="16"
         kerning="0" bold="0" italic="0" justification="36"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
