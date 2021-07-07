/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
BasicSynth2AudioProcessorEditor::BasicSynth2AudioProcessorEditor(BasicSynth2AudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p),
    keyboardComponent(audioProcessor.keyboardState, juce::MidiKeyboardComponent::horizontalKeyboard),
    waveformChoice(audioProcessor.apvts, audioProcessor.choiceIdentifiers[0], keyboardComponent),
    filterChoice(audioProcessor.apvts, audioProcessor.choiceIdentifiers[1], keyboardComponent),
    gainComponent(audioProcessor.apvts),
    adsrComponent(audioProcessor.apvts, audioProcessor.adsrIdentifiers),
    filterAdsrComponent(audioProcessor.apvts, audioProcessor.filterAdsrIdentifiers),
    fmComponent(audioProcessor.apvts, audioProcessor.fmIdentifiers),
    filterComponent(audioProcessor.apvts, audioProcessor.filterIdentifiers),
    startTime(juce::Time::getMillisecondCounterHiRes() * 0.001)
{
    for (auto* component : getComponents())
    {
        addAndMakeVisible(component);
    }
    
    // visual setup midi message log
    midiMessageLog.setTextToShowWhenEmpty("MIDI message log", juce::Colours::white);
    midiMessageLog.setMultiLine(true);
    midiMessageLog.setReturnKeyStartsNewLine(true);
    midiMessageLog.setReadOnly(true);
    midiMessageLog.setScrollbarsShown(true);
    midiMessageLog.setCaretVisible(true);
    midiMessageLog.setPopupMenuEnabled(true);
    midiMessageLog.setColour(juce::TextEditor::backgroundColourId, juce::Colours::black);
    midiMessageLog.setColour(juce::TextEditor::outlineColourId, juce::Colours::grey);
    // midiMessageLog.setColour(juce::TextEditor::shadowColourId, juce::Colours::grey);

    audioProcessor.keyboardState.addListener(this);

    for (auto device : juce::MidiInput::getAvailableDevices())
    { 
        deviceManager.setMidiInputDeviceEnabled(device.identifier, true);
        deviceManager.addMidiInputDeviceCallback(device.identifier, this);
    }
    
    setSize (1200, 400);
    startTimer(400); // to get computer keyboard focus
}

BasicSynth2AudioProcessorEditor::~BasicSynth2AudioProcessorEditor()
{}

//==============================================================================
void BasicSynth2AudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
}

void BasicSynth2AudioProcessorEditor::resized()
{
    auto bounds = getLocalBounds();
    keyboardComponent.setBounds(bounds.removeFromTop(bounds.getHeight() / 3));

    auto leftSideBounds = bounds.removeFromLeft(bounds.getWidth() * 0.2);
    waveformChoice.setBounds(leftSideBounds.removeFromTop(18));
    gainComponent.setBounds(leftSideBounds.removeFromTop(leftSideBounds.getHeight() / 3));
    fmComponent.setBounds(leftSideBounds);

    auto rightSideBounds = bounds;
    auto rightControlBounds = rightSideBounds.removeFromTop(rightSideBounds.getHeight() * 0.8);
    adsrComponent.setBounds(rightControlBounds.removeFromLeft(rightControlBounds.getWidth() / 2.5));

    auto filterBounds = rightControlBounds.removeFromLeft(rightControlBounds.getWidth() / 3);
    filterChoice.setBounds(filterBounds.removeFromTop(18));
    filterComponent.setBounds(filterBounds);
    filterAdsrComponent.setBounds(rightControlBounds);

    midiMessageLog.setBounds(rightSideBounds);
}

void BasicSynth2AudioProcessorEditor::timerCallback()
{
    keyboardComponent.grabKeyboardFocus();
    stopTimer();
}

//==============================================================================
// MIDI logging

void BasicSynth2AudioProcessorEditor::handleNoteOn(juce::MidiKeyboardState*, int midiChannel, int midiNoteNumber, float velocity)
{
    if (!isAddingFromMidiInput)
    {
        auto m = juce::MidiMessage::noteOn(midiChannel, midiNoteNumber, velocity);
        m.setTimeStamp(juce::Time::getMillisecondCounterHiRes() * 0.001);
        postMessageToList(m, "On-Screen Keyboard");
    }
}

void BasicSynth2AudioProcessorEditor::handleNoteOff(juce::MidiKeyboardState*, int midiChannel, int midiNoteNumber, float /*velocity*/)
{
    if (!isAddingFromMidiInput)
    {
        auto m = juce::MidiMessage::noteOff(midiChannel, midiNoteNumber);
        m.setTimeStamp(juce::Time::getMillisecondCounterHiRes() * 0.001);
        postMessageToList(m, "On-Screen Keyboard");
    }
}

void BasicSynth2AudioProcessorEditor::handleIncomingMidiMessage(juce::MidiInput* source, const juce::MidiMessage& message)
{
    juce::ScopedValueSetter<bool> scopedInputFlag(isAddingFromMidiInput, true);
    audioProcessor.keyboardState.processNextMidiEvent(message);
    postMessageToList(message, source->getName());
}

//==============================================================================
void BasicSynth2AudioProcessorEditor::postMessageToList(const juce::MidiMessage& message, 
    const juce::String& source)
{
    (new IncomingMessageCallback(this, message, source))->post();
}

void BasicSynth2AudioProcessorEditor::addMessageToList(const juce::MidiMessage& message, 
    const juce::String& source)
{
    auto time = message.getTimeStamp() - startTime;

    auto hours = ((int)(time / 3600.0)) % 24;
    auto minutes = ((int)(time / 60.0)) % 60;
    auto seconds = ((int)time) % 60;
    auto millis = ((int)(time * 1000.0)) % 1000;

    auto timecode = juce::String::formatted("%02d:%02d:%02d.%03d",
        hours,
        minutes,
        seconds,
        millis);

    juce::String description = getMidiMessageDescription(message);
    juce::String midiMessageString = timecode + " - " + description + " - (" + source + ")";
    logMessage(midiMessageString);
}

juce::String BasicSynth2AudioProcessorEditor::getMidiMessageDescription(const juce::MidiMessage& m)
{
    if (m.isNoteOn()) return "Note on " + m.getMidiNoteName(m.getNoteNumber(), true, true, 3);
    if (m.isNoteOff()) return "Note off " + m.getMidiNoteName(m.getNoteNumber(), true, true, 3);
    if (m.isProgramChange()) return "Program change" + juce::String(m.getProgramChangeNumber());
    if (m.isPitchWheel()) return "Pitch Wheel" + juce::String(m.getPitchWheelValue());
    if (m.isAftertouch()) return "Aftertouch" + m.getMidiNoteName(m.getNoteNumber(), true, true, 3) +
        " : " + juce::String(m.getAfterTouchValue());
    if (m.isChannelPressure()) return "Channel pressure" + juce::String(m.getChannelPressureValue());
    if (m.isAllNotesOff()) return "All notes off";
    if (m.isAllSoundOff()) return "All sound off";
    if (m.isMetaEvent()) return "Meta event";
    if (m.isController())
    {
        juce::String name = juce::MidiMessage::getControllerName(m.getControllerNumber());

        if (name.isEmpty())
        {
            name = "[" + juce::String(m.getControllerNumber()) + "]";
        }

        return "Controller " + name + ": " + juce::String(m.getControllerValue());
    }

    return juce::String::toHexString(m.getRawData(), m.getRawDataSize());
}

void BasicSynth2AudioProcessorEditor::logMessage(const juce::String& m)
{
    midiMessageLog.moveCaretToEnd();
    midiMessageLog.insertTextAtCaret(m + juce::newLine);
}