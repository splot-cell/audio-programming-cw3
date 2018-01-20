/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "DelayDSP.h"
#include "FilterDSP.h"


//==============================================================================
/**
*/
class CwdelayAudioProcessor  : public AudioProcessor,
                               public AudioProcessorValueTreeState::Listener
{
public:
    
    //==============================================================================
    CwdelayAudioProcessor();
    ~CwdelayAudioProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (AudioSampleBuffer&, MidiBuffer&) override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect () const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    
    //==============================================================================
    void parameterChanged(const String& parameterID, float newValue) override;
    
private:
    //==============================================================================
    /* LFO functions for varying delay line. */
    static float LFOFunc(float angle);
    static float LFO1Func(float angle);
    //==============================================================================
    /* LFO parameters. */
    constexpr static const float LFOdepth = 0.001; // in mSec
    constexpr static const float LFO1depth = 0.01; // in mSec
    
    constexpr static const float maxLFOOffset = LFOdepth + LFO1depth; // LFOs vary delay size, this is the max positive offset in mSec.
    
    const int LFOResolution = 50;
    //==============================================================================
    /* Static functions for parameter text to float and float to text. */
    static String onOffFloatToText (float value);
    static float onOffTextToFloat (const String& text);
    
    AudioProcessorValueTreeState parameters;
    
    LinearSmoothedValue<float> delaySize; // Smooth parameter changes.
    LinearSmoothedValue<float> wetLevel;
    LinearSmoothedValue<float> feedback;
    
    float samplerate; // Required for real-time calculation of delay size in samples.
    
    float previousInputGain; // For smoothing gain changes over course of one buffer.
    float previousOutputGain; // As above.
    
    VariableDelayLine delay;
    LPFilter filter;
    
    HeapBlock<float> out; // For storing feedback samples
    
    dsp::Oscillator<float> LFO;
    dsp::Oscillator<float> LFO1;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CwdelayAudioProcessor)
};
