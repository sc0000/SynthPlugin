/*
  ==============================================================================

    GainComponent.cpp
    Created: 7 Jul 2021 1:51:03am
    Author:  Sebastian Cyliax

  ==============================================================================
*/

#include <JuceHeader.h>
#include "GainComponent.h"

//==============================================================================
GainComponent::GainComponent(juce::AudioProcessorValueTreeState& apvts)
    : gainSliderAttachment(apvts, "Gain", gainSlider)
{
    addAndMakeVisible(gainSlider);
    gainSlider.setSkewFactorFromMidPoint(0.1);

    addAndMakeVisible(gainLabel);
    gainLabel.setText("Gain", juce::dontSendNotification);
    gainLabel.attachToComponent(&gainSlider, true);
}

GainComponent::~GainComponent()
{
}

void GainComponent::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component
}

void GainComponent::resized()
{
    auto area = getLocalBounds();
    gainSlider.setBounds(area.removeFromRight(getWidth() * 0.8));
    gainLabel.setBounds(area);
    
}
