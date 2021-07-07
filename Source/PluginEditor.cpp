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
    startTime(juce::Time::getMillisecondCounterHiRes() * 0.001),
    keyboardComponent(audioProcessor.keyboardState, juce::MidiKeyboardComponent::horizontalKeyboard),
    waveformChoice(audioProcessor.apvts, audioProcessor.choiceIdentifiers[0], keyboardComponent),
    filterChoice(audioProcessor.apvts, audioProcessor.choiceIdentifiers[1], keyboardComponent),
    gainComponent(audioProcessor.apvts),
    adsrComponent(audioProcessor.apvts, audioProcessor.adsrIdentifiers),
    filterAdsrComponent(audioProcessor.apvts, audioProcessor.filterAdsrIdentifiers),
    fmComponent(audioProcessor.apvts, audioProcessor.fmIdentifiers),
    filterComponent(audioProcessor.apvts, audioProcessor.filterIdentifiers),
    midiMessageLogComponent(audioProcessor.keyboardState, startTime)    
{
    for (auto* component : getComponents())
    {
        addAndMakeVisible(component);
    }
    
    midiMessageLogComponent.setup();
    
    setSize (1200, 400);
    startTimer(400); // to get computer keyboard focus; only possible when everything else is alread visible
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
    adsrComponent.setBounds(rightControlBounds.removeFromLeft(rightControlBounds.getWidth() / 2.7));

    auto filterBounds = rightControlBounds.removeFromLeft(rightControlBounds.getWidth() / 2.7);
    filterChoice.setBounds(filterBounds.removeFromTop(18));
    filterComponent.setBounds(filterBounds);
    filterAdsrComponent.setBounds(rightControlBounds);

    midiMessageLogComponent.setBounds(rightSideBounds);
}

void BasicSynth2AudioProcessorEditor::timerCallback()
{
    keyboardComponent.grabKeyboardFocus();
    stopTimer();
}