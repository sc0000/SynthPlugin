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
WaveformChoice::WaveformChoice(juce::AudioProcessorValueTreeState& apvts) :
    waveformChoiceAttachment(apvts, "Waveform", waveformChoice)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    addAndMakeVisible(waveformChoice);
    waveformChoice.setText("Set Waveform");
    waveformChoice.addItemList(juce::StringArray{ "Sine", "Sawtooth", "Square", "Triangle" }, 1);
}

WaveformChoice::~WaveformChoice()
{
}

void WaveformChoice::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    //g.setColour (juce::Colours::grey);
    //g.drawRect (getLocalBounds(), 1);   // draw an outline around the component
}

void WaveformChoice::resized()
{
    waveformChoice.setBounds(getLocalBounds());
}
