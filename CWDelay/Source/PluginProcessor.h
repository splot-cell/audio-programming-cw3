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
    AudioProcessorValueTreeState parameters;
    
    LinearSmoothedValue<float> delaySize; // Smooth parameter changes.
    LinearSmoothedValue<float> wetLevel;
    LinearSmoothedValue<float> feedback;
    
    float samplerate; // Required for real-time calculation of delay size in samples.
    
    float previousInputGain; // For smoothing gain changes over course of one buffer.
    float previousOutputGain; // As above.
    
    VariableDelayLine delay;
    LPFilter filter;
    
    dsp::Oscillator<float> LFO;
    
//    static float LFOFunc(float angle);
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CwdelayAudioProcessor)
};
