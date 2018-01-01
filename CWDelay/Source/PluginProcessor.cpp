/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
CwdelayAudioProcessor::CwdelayAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       ),
#endif
    parameters (*this, nullptr),
    delay(getTotalNumInputChannels())
{
    parameters.createAndAddParameter ("inputGain",                              // ID
                                      "Input Gain",                             // name
                                      String(),                                 // suffix
                                      NormalisableRange<float> (0.0f, 1.0f),    // set range
                                      0.5f,                                     // default value
                                      nullptr,
                                      nullptr);
    
    parameters.createAndAddParameter ("outputGain",                             // ID
                                      "Output Gain",                            // name
                                      String(),                                 // suffix
                                      NormalisableRange<float> (0.0f, 1.0f),    // set range
                                      0.5f,                                     // default value
                                      nullptr,
                                      nullptr);
    
    parameters.createAndAddParameter ("delayTime",                              // ID
                                      "Delay Time",                             // name
                                      "s",                                      // suffix
                                      NormalisableRange<float> (0.0f, 1.0f),    // set range
                                      0.5f,                                     // default value
                                      nullptr,
                                      nullptr);
    parameters.addParameterListener ("delayTime", this);
    
    parameters.createAndAddParameter ("feedback",                               // ID
                                      "Feedback",                               // name
                                      String(),                                 // suffix
                                      NormalisableRange<float> (0.0f, 1.0f),    // set range
                                      0.0f,                                     // default value
                                      nullptr,
                                      nullptr);
    parameters.addParameterListener ("feedback", this);
    
    parameters.createAndAddParameter ("wetLevel",                               // ID
                                      "Dry Wet Mix",                            // name
                                      String(),                                 // suffix
                                      NormalisableRange<float> (0.0f, 1.0f),    // set range
                                      0.5f,                                     // default value
                                      nullptr,
                                      nullptr);
    parameters.addParameterListener ("wetLevel", this);
    
    parameters.state = ValueTree (Identifier ("OllySAPCW3"));
}

CwdelayAudioProcessor::~CwdelayAudioProcessor()
{
}

//==============================================================================
const String CwdelayAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool CwdelayAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool CwdelayAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool CwdelayAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double CwdelayAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int CwdelayAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int CwdelayAudioProcessor::getCurrentProgram()
{
    return 0;
}

void CwdelayAudioProcessor::setCurrentProgram (int index)
{
}

const String CwdelayAudioProcessor::getProgramName (int index)
{
    return {};
}

void CwdelayAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void CwdelayAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    samplerate = (float) sampleRate;
    
    previousInputGain = *parameters.getRawParameterValue("inputGain");
    previousOutputGain = *parameters.getRawParameterValue("outputGain");
    
    /* Initalise parameters before playback begins. */
    delaySize.setValue (*parameters.getRawParameterValue ("delayTime") * sampleRate);
    delaySize.reset (sampleRate, 0.2);
    
    wetLevel.setValue (* parameters.getRawParameterValue ("wetLevel"));
    delaySize.reset (sampleRate, 0.2);
    
//    delay.setDelaySize (*parameters.getRawParameterValue ("delayTime"));
//    delay.setFeedback (*parameters.getRawParameterValue ("feedback"));
    
    delay.prepareDelayLine (sampleRate, getTotalNumInputChannels());
}

void CwdelayAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool CwdelayAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void CwdelayAudioProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    const int totalNumInputChannels  = getTotalNumInputChannels();
    const int totalNumOutputChannels = getTotalNumOutputChannels();
    
    //delay.setDelaySize (*parameters.getRawParameterValue ("delayTime"));
    //delay.setFeedback (*parameters.getRawParameterValue ("feedback"));
    
    /* Apply ramp to gain changes to avoid glitches from fast parameter changes. */
    {
        const float currentInputGain = *parameters.getRawParameterValue("inputGain");
    
        if (currentInputGain == previousInputGain)
        {
            buffer.applyGain(currentInputGain);
        }
        else
        {
            buffer.applyGainRamp (0, buffer.getNumSamples(), previousInputGain, currentInputGain);
            previousInputGain = currentInputGain;
        }
    }

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (int i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    
    float out[totalNumInputChannels];
    for ( int i = 0; i < totalNumInputChannels; ++i)
        out[i] = 0;
    
    for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
    {
        const float delayValue = delaySize.getNextValue();
        
        for (int channel = 0; channel < totalNumInputChannels; ++channel)
        {
            float in = buffer.getSample (channel, sample) + (out[channel] * *parameters.getRawParameterValue ("feedback"));
            delay.writeSample (in, channel);
            out[channel] = delay.getSample (delayValue, channel);
            
            /* Dry / wet logic */
            buffer.addSample (channel, sample, out[channel]);
        }
    }
    
    
    /* Apply ramp to gain changes to avoid glitches from fast parameter changes. */
    {
        const float currentOutputGain = *parameters.getRawParameterValue("outputGain");;
    
        if (currentOutputGain == previousOutputGain)
        {
            buffer.applyGain(currentOutputGain);
        }
        else
        {
            buffer.applyGainRamp (0, buffer.getNumSamples(), previousOutputGain, currentOutputGain);
            previousOutputGain = currentOutputGain;
        }
    }
}

//==============================================================================
void CwdelayAudioProcessor::parameterChanged(const String& parameterID, float newValue)
{
    if (parameterID == "delayTime")
    {
        if (! delaySize.isSmoothing())
            delaySize.setValue (newValue * samplerate);
    }
    else if (parameterID == "wetLevel")
    {
        if (! wetLevel.isSmoothing())
            wetLevel.setValue (newValue);
    }
}

//==============================================================================
bool CwdelayAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* CwdelayAudioProcessor::createEditor()
{
    return new CwdelayAudioProcessorEditor (*this, parameters);
}

//==============================================================================
void CwdelayAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    ScopedPointer<XmlElement> xml (parameters.state.createXml());
    copyXmlToBinary (*xml, destData);
}

void CwdelayAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    ScopedPointer<XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));
    if (xmlState != nullptr)
        if (xmlState->hasTagName (parameters.state.getType()))
            parameters.state = ValueTree::fromXml (*xmlState);
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new CwdelayAudioProcessor();
}
