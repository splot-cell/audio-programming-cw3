/*
 ==============================================================================
 
 PluginProcessor.h
 Created from a JUCE template.
 Author:  Olly Seber
 
 ==============================================================================
 */

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "DelayDSP.h"
#include "FilterDSP.h"

//==============================================================================

class CwdelayAudioProcessor  : public AudioProcessor,
                               public AudioProcessorValueTreeState::Listener
{
public:
    //==============================================================================
    /* CONSTRUCTOR AND DESTRUCTOR */
    //==============================================================================
    CwdelayAudioProcessor();
    ~CwdelayAudioProcessor();
    
    //==============================================================================
    /* AUDIO FUNCTIONS */
    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (AudioSampleBuffer&, MidiBuffer&) override;

    //==============================================================================
    /* GUI/'EDITOR' FUNCTIONS */
    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    /* PLUGIN INFO FUNCTIONS */
    //==============================================================================
    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect () const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    /* PROGRAM FUNCTIONS (programs are not currently supported) */
    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    //==============================================================================
    /* STATE STORAGE/RECAL FUNCTIONS */
    /* Store and recalls the current state of the plugin so the setting remain
     * when the GUI is closed/reopened. */
    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    
    //==============================================================================
    /* CALLBACK FUNCTION FROM GUI */
    /* Called when a parameter is changed on the GUI. */
    //==============================================================================
    void parameterChanged(const String& parameterID, float newValue) override;
    
private:
    //==============================================================================
    /* MEMBERS FOR PLUGIN PARAMETERS */
    //==============================================================================
    /* Static functions for parameter text to float and float to text. */
    static String onOffFloatToText (float value);
    static float onOffTextToFloat (const String& text);
    
    /* ValueTreeState object manages plugin parameters. */
    AudioProcessorValueTreeState parameters;
    
    /* Use linear smoothing of parameter changes. */
    LinearSmoothedValue<float> delaySize;
    LinearSmoothedValue<float> wetLevel;
    LinearSmoothedValue<float> feedback;
    
    float samplerate; // Required for real-time calculation of delay size in samples.
    
    /* For smooooothing gain changes. */
    float previousInputGain;
    float previousOutputGain;
    
    //==============================================================================
    /* MEMBERS FOR DSP */
    //==============================================================================
    VariableDelayLine delay;
    LPFilter filter;
    
    HeapBlock<float> out; // For storing feedback samples
    
    //==============================================================================
    /* MEMBERS FOR LFOS */
    //==============================================================================
    /* LFO functions for varying delay line.
     * Must be static as function pointer is used. */
    static float LFOFunc(float angle);
    static float LFO1Func(float angle);
    
    /* LFO parameters.
     * Must also be static in order to be used by above functions. */
    constexpr static const float LFOdepth = 0.001; // in mSec
    constexpr static const float LFO1depth = 0.01; // in mSec
    
    /* LFOs will vary the delay size, so be sure to take this into account when setting delay buffer length. */
    constexpr static const float maxLFOOffset = LFOdepth + LFO1depth;
    
    /* Used at object instantiation, so not required to be static, but must be above the LFO members. */
    const int LFOResolution = 50;
    
    /* Must be instantiated after LFOResolution is initialised. */
    dsp::Oscillator<float> LFO;
    dsp::Oscillator<float> LFO1;
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CwdelayAudioProcessor)
};
