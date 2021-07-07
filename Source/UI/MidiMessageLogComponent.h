/*
  ==============================================================================

    MidiMessageLogComponent.h
    Created: 7 Jul 2021 2:58:00pm
    Author:  Sebastian Cyliax

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class MidiMessageLogComponent  : public juce::Component, private juce::MidiKeyboardStateListener, juce::MidiInputCallback
{
public:
    MidiMessageLogComponent(juce::MidiKeyboardState& k, double& t);
    ~MidiMessageLogComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    void setup();

private:
    
    juce::MidiKeyboardState& keyboardState;
    juce::TextEditor midiMessageLog;
    juce::AudioDeviceManager deviceManager;

    double startTime;

    void logMessage(const juce::String& m);
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
        IncomingMessageCallback(MidiMessageLogComponent* o, const juce::MidiMessage& m, const juce::String& s) :
            owner(o), message(m), source(s)
        {}

        void messageCallback() override { owner->addMessageToList(message, source); }

    private:
        juce::Component::SafePointer<MidiMessageLogComponent> owner;
        juce::MidiMessage message;
        juce::String source;
    };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MidiMessageLogComponent)
};
