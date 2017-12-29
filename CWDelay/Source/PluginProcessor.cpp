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
                       )
#endif
{
    addParameter (inputGain = new AudioParameterFloat ("inputGain",                             // ID
                                                       "Input Gain",                            // name
                                                       NormalisableRange<float> (0.0f, 1.0f),   // set range
                                                       0.5f));                                  // default value
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
    previousInputGain = *inputGain;
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
    
    /* Apply ramp to gain changes to avoid glitches from fast parameter changes. */
    const float currentInputGain = *inputGain;
    
    if (currentInputGain == previousInputGain)
    {
        buffer.applyGain(currentInputGain);
    }
    else
    {
        buffer.applyGainRamp (0, buffer.getNumSamples(), previousInputGain, currentInputGain);
        previousInputGain = currentInputGain;
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
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        float* channelData = buffer.getWritePointer (channel);

        // ..do something to the data...
    }
}

//==============================================================================
bool CwdelayAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* CwdelayAudioProcessor::createEditor()
{
    return new CwdelayAudioProcessorEditor (*this);
}

//==============================================================================
void CwdelayAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    MemoryOutputStream (destData, true).writeFloat (*inputGain);
}

void CwdelayAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    *inputGain = MemoryInputStream (data, static_cast<size_t> (sizeInBytes), false).readFloat();
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new CwdelayAudioProcessor();
}
