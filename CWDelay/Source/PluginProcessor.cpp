/*
 ==============================================================================
 
 PluginProcessor.cpp
 Created from a JUCE template.
 Author:  Olly Seber
 
 ==============================================================================
 */

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================

CwdelayAudioProcessor::CwdelayAudioProcessor() :
#ifndef JucePlugin_PreferredChannelConfigurations
       AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       ),
#endif
    parameters (*this, nullptr),    // Initialise the ValueTreeState for handling parameters.
    filter (126),                   // Set filter order.
    LFO (LFOFunc, LFOResolution),   // Set up each LFO.
    LFO1 (LFO1Func, LFOResolution)
{
    /* Set up all the parameters. */
    parameters.createAndAddParameter ("inputGain",                                      // ID
                                      "Input Gain",                                     // name
                                      "dB",                                             // suffix
                                      NormalisableRange<float> (-90.f, 0.f, 0.f, 5.f),  // set range
                                      -6.f,                                             // default value
                                      nullptr,                                          // Value to text function
                                      nullptr);                                         // Text to value function
    
    parameters.createAndAddParameter ("outputGain",                                     // ID
                                      "Output Gain",                                    // name
                                      "dB",                                             // suffix
                                      NormalisableRange<float> (-90.f, 0.f, 0.f, 5.f),  // set range
                                      -6.f,                                             // default value
                                      nullptr,                                          // Value to text function
                                      nullptr);                                         // Text to value function
    
    parameters.createAndAddParameter ("delayTime",
                                      "Delay Time",
                                      "s",
                                      NormalisableRange<float> (0.f, 1.f, 0.001f, 0.7f),
                                      0.5f,
                                      nullptr,
                                      nullptr);
    parameters.addParameterListener ("delayTime", this);
    
    parameters.createAndAddParameter ("feedback",
                                      "Feedback",
                                      "%",
                                      NormalisableRange<float> (0.f, 100.f, 1.f, 1.f),
                                      70.f,
                                      nullptr,
                                      nullptr);
    parameters.addParameterListener ("feedback", this);
    
    parameters.createAndAddParameter ("wetLevel",
                                      "Dry Wet Mix",
                                      "%",
                                      NormalisableRange<float> (0.f, 100.f, 1.f, 1.f),
                                      50.f,
                                      nullptr,
                                      nullptr);
    parameters.addParameterListener ("wetLevel", this);
    
    parameters.createAndAddParameter ("tapeMode",
                                      "Tape Mode",
                                      String(),
                                      NormalisableRange<float> (0.f, 1.f, 1.f),
                                      0.f,
                                      onOffFloatToText,     // Value to text function
                                      onOffTextToFloat);    // Text to value function
    
    parameters.createAndAddParameter ("crossMode",
                                      "Cross-over Mode",
                                      String(),
                                      NormalisableRange<float> (0.f, 1.f, 1.f),
                                      0.f,
                                      onOffFloatToText,
                                      onOffTextToFloat);
    
    parameters.createAndAddParameter ("filterOn",
                                      "Filter",
                                      String(),
                                      NormalisableRange<float> (0.f, 1.f, 1.f),
                                      1.f,
                                      onOffFloatToText,
                                      onOffTextToFloat);
    
    parameters.state = ValueTree (Identifier ("OllySAPCW3")); // Finish ValueTreeState initialisation.
    LFO.setFrequency (7); // Flutter
    LFO1.setFrequency (0.46); // WOW
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
                // so this should be at least 1, even if you're not really implementing programs
                // (which I'm not).
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
    /* Required to be stored for calculating delay time in samples. */
    samplerate = (float) sampleRate;
    
    /* ProcessSpec contains the samplerate, max block size, and number of channels.
     * Each sample will be called individually in this implementation, and each LFO is shared
     * between both channels. */
    const int LFOblockSize = 1;
    const int numLFOchannels = 1;
    dsp::ProcessSpec spec = {samplerate, LFOblockSize, numLFOchannels};
    LFO.prepare (spec);
    LFO1.prepare (spec);
    
    /* Initalise parameters before playback begins. */
    previousInputGain = Decibels::decibelsToGain (*parameters.getRawParameterValue ("inputGain"), -90.f);
    previousOutputGain = Decibels::decibelsToGain (*parameters.getRawParameterValue ("outputGain"), -90.f);
    
    /* Set up ramping parameter values */
    const float rampTime = 0.2; // Seconds
    delaySize.setValue (*parameters.getRawParameterValue ("delayTime") * sampleRate);
    delaySize.reset (sampleRate, rampTime);
    
    wetLevel.setValue (*parameters.getRawParameterValue ("wetLevel") / 100.f); // Parameter is a percentage
    wetLevel.reset (sampleRate, rampTime);
    
    feedback.setValue (*parameters.getRawParameterValue ("feedback") / 100.f); // Parameter is a percentage
    feedback.reset (sampleRate, rampTime);
    
    /* Prepare delayline for 1 sec plus the max LFO offset worth of samples.
     * As delaySize must be integer, add 1 to total to avoid errors through truncation in
     * the case where sampleRate * (1 + maxLFOOffset) is a float. */
    delay.prepareDelayLine (1 + sampleRate * (1 + maxLFOOffset), getTotalNumInputChannels());
    
    /* Allocate memory for feedback samples. */
    out.calloc (getTotalNumInputChannels());
    
    /* Set cutoff frequency and number of channels in filter. */
    const int filterFreq = 2000; // Hz
    filter.prepareForAudio (filterFreq, sampleRate, getTotalNumInputChannels());
}

void CwdelayAudioProcessor::releaseResources()
{
    /* Free all allocated memory. */
    delay.freeMemory();
    out.free();
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool CwdelayAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    /* Support for Mono and Stereo. */
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;

    /* Check if input layout matches output layout. */
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
    const int totalNumInputChannels  = getTotalNumInputChannels();;
    
    /* Apply ramp to gain changes to avoid glitches from fast parameter changes. */
    {
        const float currentInputGain = Decibels::decibelsToGain (*parameters.getRawParameterValue ("inputGain"), -90.f);
    
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
    
    /* Sample by sample prcoessing. */
    for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
    {
        /* Get parameter values here: per sample, not per channel.
         * This avoids conflicting parameter values across the channels. */
        const float wetLevelValue = wetLevel.getNextValue();
        const float feedbackValue = feedback.getNextValue();
        
        /* If "tapeMode" parameter is below 0.5, set LFOvalue to nothing. Otherwise, get next
         * values from the LFOs. This will be used to modulate the delaytime, so multiply by
         * samplerate to get a time in samples. */
        const float LFOvalue = *parameters.getRawParameterValue ("tapeMode") < 0.5 ? 0.f :
            (LFO.processSample (1.f) + LFO1.processSample (1.f)) * samplerate;
        
        /* Delay time is calculated from delay parameter plus the LFOvalue. */
        const float delayValue = delaySize.getNextValue() + LFOvalue;
        
        /* Cross value is a channel offset used to give "ping pong" style effects when plugin
         * is fed a single channel input. */
        const int crossValue = *parameters.getRawParameterValue ("crossMode") < 0.5 ? 0 : 1;
        
        /* Channel by channel processing. */
        for (int channel = 0; channel < totalNumInputChannels; ++channel)
        {
            /* Write input sample into delayline. */
            float in = buffer.getSample (channel, sample) +
                (out[channel] * feedbackValue);
            delay.writeSample (in, (channel + crossValue) % totalNumInputChannels);
            
            /* Get the next sample from the delay. */
            in = delay.getSample (delayValue, channel);
            
            /* Process the sample in the filter so switching filter on/off is instantanious. */
            out[channel] = filter.processSample (in, channel);
            
            /* If filter is off, disregard filtered sample. */
            if (*parameters.getRawParameterValue ("filterOn") < 0.5)
                out[channel] = in;
            
            /* Dry / wet logic */
            buffer.applyGain (channel, sample, 1, 1. - wetLevelValue);
            buffer.addSample (channel, sample, wetLevelValue * out[channel]);
        }
    }
    
    
    /* Apply ramp to gain changes to avoid glitches from fast parameter changes. */
    {
        const float currentOutputGain = Decibels::decibelsToGain (*parameters.getRawParameterValue ("outputGain"), -90.f);
    
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

/* Callback function. */
void CwdelayAudioProcessor::parameterChanged(const String& parameterID, float newValue)
{
    if (parameterID == "delayTime")
        delaySize.setValue (newValue * samplerate); // delaySize is in samples, newValue in seconds
    else if (parameterID == "wetLevel")
        wetLevel.setValue (newValue / 100.f); // Parameter is a percentage
    else if (parameterID == "feedback")
        feedback.setValue (newValue / 100.f); // Parameter is a percentage
}

//==============================================================================
bool CwdelayAudioProcessor::hasEditor() const
{
    return true;
}

AudioProcessorEditor* CwdelayAudioProcessor::createEditor()
{
    return new CwdelayAudioProcessorEditor (*this, parameters);
}

//==============================================================================
void CwdelayAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    /* Write current state to memory so it can be recalled by the DAW. */
    ScopedPointer<XmlElement> xml (parameters.state.createXml());
    copyXmlToBinary (*xml, destData);
}

void CwdelayAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    /* Recreate previous plugin state provided by DAW. */
    ScopedPointer<XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));
    if (xmlState != nullptr)
        if (xmlState->hasTagName (parameters.state.getType()))
            parameters.state = ValueTree::fromXml (*xmlState);
}

/* LFO functions.
 * Separate function are used for each LFO for two reasons:
 *  1) the depth may be set independantly for each oscillator
 *      * A depth parameter cannot be used as the functions must conform to the format given in the
 *        dsp::Oscilllator class.
 * and
 *  2) the depth is implemented directly.
 *      * This avoids scaling the LFO output during audio processing, which would result
 *        in many additional multiplications, thus reducing efficiency.
 */
float CwdelayAudioProcessor::LFOFunc(float angle)
{
    return LFOdepth + (LFOdepth * sinf (angle));
}

float CwdelayAudioProcessor::LFO1Func(float angle)
{
    return LFO1depth + (LFO1depth * sinf (angle));
}

/* Convert float to text for binary parameters. */
String CwdelayAudioProcessor::onOffFloatToText (float value)
{
    return value < 0.5 ? "Off" : "On";
}

/* Convert text to float for binary parameters. */
float CwdelayAudioProcessor::onOffTextToFloat (const String& text)
{
    if (text == "Off") return 0.f;
    if (text == "On")  return 1.f;
    return 0.f;
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new CwdelayAudioProcessor();
}
