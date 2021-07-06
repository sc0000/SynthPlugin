/*
  ==============================================================================

    WaveformChoice.cpp
    Created: 6 Jul 2021 10:09:28pm
    Author:  Sebastian Cyliax

  ==============================================================================
*/

#include <JuceHeader.h>
#include "WaveformChoice.h"

//==============================================================================
WaveformChoice::WaveformChoice(juce::AudioProcessorValueTreeState& apvts, juce::MidiKeyboardComponent& k) 
    : waveformChoiceAttachment(apvts, "Waveform", waveformChoice)
{
    addAndMakeVisible(waveformChoice);
    waveformChoice.setText("Set Waveform");
    waveformChoice.addItemList(juce::StringArray{ "Sine", "Sawtooth", "Square", "Triangle" }, 1);
    waveformChoice.onChange = [&k]() { k.grabKeyboardFocus(); }; // to be instantly able to use the computer keyboard 
                                                                 // again after switching waveforms
}

WaveformChoice::~WaveformChoice()
{
}

void WaveformChoice::paint(juce::Graphics& g)
{
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
}

void WaveformChoice::resized()
{
    waveformChoice.setBounds(getLocalBounds());
}
