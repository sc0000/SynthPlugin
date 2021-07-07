/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "UI/ADSRComponent.h"
#include "UI/ChoiceBox.h"
#include "UI/GainComponent.h"
#include "UI/FMandFilterComponent.h"

//==============================================================================
/**
*/
class BasicSynth2AudioProcessorEditor  : public juce::AudioProcessorEditor, private juce::Timer, private juce::MidiKeyboardStateListener, juce::MidiInputCallback
{
public:
    BasicSynth2AudioProcessorEditor (BasicSynth2AudioProcessor&);
    ~BasicSynth2AudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

    void logMessage(const juce::String& m);

private:
    // Reference to the plugin processor
    BasicSynth2AudioProcessor& audioProcessor;

    juce::MidiKeyboardComponent keyboardComponent;

    ChoiceBox waveformChoice;
    ChoiceBox filterChoice;
    GainComponent gainComponent;
    ADSRComponent adsrComponent;
    ADSRComponent filterAdsrComponent;
    FMandFilterComponent fmComponent;
    FMandFilterComponent filterComponent;

    juce::TextEditor midiMessageLog;
    
    std::vector<juce::Component*> getComponents() 
    { 
        return { &keyboardComponent, &waveformChoice, &filterChoice, &gainComponent, 
            &adsrComponent, &filterAdsrComponent, &fmComponent, &filterComponent, &midiMessageLog }; 
    }


private:
    //==============================================================================
    // MIDI logging
    
    juce::AudioDeviceManager deviceManager;

    void timerCallback() override;
    double startTime;
    void handleIncomingMidiMessage(juce::MidiInput* source, const juce::MidiMessage& message) override;
    bool isAddingFromMidiInput = false;

    void handleNoteOn(juce::MidiKeyboardState*, int midiChannel, int midiNoteNumber, float velocity) override;
    void handleNoteOff(juce::MidiKeyboardState*, int midiChannel, int midiNoteNumber, float /*velocity*/) override;
    

    void postMessageToList(const juce::MidiMessage& message, const juce::String& source);
    void addMessageToList(const juce::MidiMessage& message, const juce::String& source);
    juce::String getMidiMessageDescription(const juce::MidiMessage& m);
    


    class IncomingMessageCallback : public juce::CallbackMessage
    {
    public:
        IncomingMessageCallback(BasicSynth2AudioProcessorEditor* o, const juce::MidiMessage& m, const juce::String& s) :
            owner(o), message(m), source(s)
        {}

        void messageCallback() override { owner->addMessageToList(message, source); }

    private:
        juce::Component::SafePointer<BasicSynth2AudioProcessorEditor> owner;
        juce::MidiMessage message;
        juce::String source;
    };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BasicSynth2AudioProcessorEditor)
};
