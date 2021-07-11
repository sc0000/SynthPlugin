/*
  ==============================================================================

    JUCE plugin processor

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>


//==============================================================================
/**
*/
class BasicSynth2AudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    BasicSynth2AudioProcessor();
    ~BasicSynth2AudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    void logMessage(const juce::String& m);
    juce::TextEditor midiLog;

    //==============================================================================
    juce::MidiKeyboardState keyboardState;
    juce::MidiMessageCollector midiMessageCollector;
    juce::AudioProcessorValueTreeState apvts;

    const juce::StringArray adsrIdentifiers         { "Attack", "Decay", "Sustain", "Release" };
    const juce::StringArray filterAdsrIdentifiers   { "Filter Attack", "Filter Decay", "Filter Sustain", "Filter Release" };
    const juce::StringArray fmIdentifiers           {"FM Freq", "FM Depth"};
    const juce::StringArray filterIdentifiers       { "Filter Cutoff", "Filter Reso" };
    const juce::StringArray choiceIdentifiers       {"Waveform", "Filter Type"};
    // juce::Array<juce::StringArray> identifiers{adsrIdentifiers, filterAdsrIdentifiers, fmIdentifiers, filterIdentifiers};

private:
    //==============================================================================
    
    juce::Synthesiser synth;
    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BasicSynth2AudioProcessor)
};
