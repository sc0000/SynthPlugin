/*
  ==============================================================================

    ChoiceBox.cpp
    Created: 6 Jul 2021 10:09:28pm
    Author:  Sebastian Cyliax

  ==============================================================================
*/

#include <JuceHeader.h>
#include "ChoiceBox.h"

//==============================================================================
ChoiceBox::ChoiceBox(juce::AudioProcessorValueTreeState& apvts, const juce::String& id, juce::MidiKeyboardComponent& k)
    : choiceBoxAttachment(apvts, id, choiceBox)
{
    addAndMakeVisible(choiceBox);
    choiceBox.setText("Set " + id);
    if (id == "Waveform")
    {
        choiceBox.addItemList(juce::StringArray{ "Sine", "Sawtooth", "Square", "Triangle" }, 1);
         
    }
        
    else if (id == "Filter Type")
        choiceBox.addItemList(juce::StringArray{ "Low Pass", "Bandpass", "Highpass" }, 1);

    choiceBox.onChange = [&k]() { k.grabKeyboardFocus(); };
}

ChoiceBox::~ChoiceBox()
{
}

void ChoiceBox::paint(juce::Graphics& g)
{
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
}

void ChoiceBox::resized()
{
    choiceBox.setBounds(getLocalBounds());
}
