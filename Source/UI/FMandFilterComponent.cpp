/*
  ==============================================================================

    FMandFilterComponent.cpp
    Created: 7 Jul 2021 2:33:32am
    Author:  Sebastian Cyliax

  ==============================================================================
*/

#include <JuceHeader.h>
#include "FMandFilterComponent.h"

//==============================================================================
FMandFilterComponent::FMandFilterComponent(juce::AudioProcessorValueTreeState& apvts, const juce::StringArray& id)
    : slider1Attachment(apvts, id[0], slider1),
    slider2Attachment(apvts, id[1], slider2)
{
    addAndMakeVisible(slider1);
    addAndMakeVisible(slider2);

    addAndMakeVisible(label1);
    label1.setText(id[0], juce::dontSendNotification);
    label1.attachToComponent(&slider1, true);

    addAndMakeVisible(label2);
    label2.setText(id[1], juce::dontSendNotification);
    label2.attachToComponent(&slider2, true);
}

FMandFilterComponent::~FMandFilterComponent()
{
}

void FMandFilterComponent::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component
}

void FMandFilterComponent::resized()
{
    auto area = getLocalBounds();
    auto freqArea = area.removeFromTop(area.getHeight() * 0.5);
    slider1.setBounds(freqArea.removeFromRight(getWidth() * 0.8));
    //label1.setBounds(area);

    slider2.setBounds(area.removeFromRight(getWidth() * 0.8));
    //label2.setBounds(area);
}
