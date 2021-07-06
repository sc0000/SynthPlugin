/*
  ==============================================================================

    PluginProcessor.cpp
    Created: 6 Jul 2021 6:40:32pm
    Author:  Sebastian Cyliax

  ==============================================================================
*/

#include "ADSRComponent.h"
#include "../PluginProcessor.h"

//==============================================================================
ADSRComponent::ADSRComponent(juce::AudioProcessorValueTreeState& apvts) :
    attackSliderAttachment(apvts, "Attack", attackSlider),
    decaySliderAttachment(apvts, "Decay", decaySlider),
    sustainSliderAttachment(apvts, "Sustain", sustainSlider),
    releaseSliderAttachment(apvts, "Release", releaseSlider)
{
    addAndMakeVisible(attackSlider);
    addAndMakeVisible(decaySlider);
    addAndMakeVisible(sustainSlider);
    addAndMakeVisible(releaseSlider);

    attackLabel.attachToComponent(&attackSlider, true);
    attackLabel.setText("Attack", juce::dontSendNotification);

    decayLabel.attachToComponent(&decaySlider, true);
    decayLabel.setText("Decay", juce::dontSendNotification);

    sustainLabel.attachToComponent(&sustainSlider, true);
    sustainLabel.setText("Sustain", juce::dontSendNotification);

    releaseLabel.attachToComponent(&releaseSlider, true);
    releaseLabel.setText("Release", juce::dontSendNotification);
}

ADSRComponent::~ADSRComponent()
{
}

void ADSRComponent::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component
}

void ADSRComponent::resized()
{
    auto area = getLocalBounds();
    auto labelArea = area.removeFromLeft(area.getHeight() * 0.15);
    attackLabel.setBounds(labelArea.removeFromRight(getHeight() * 0.25));
    decayLabel.setBounds(labelArea.removeFromRight(getHeight() * 0.25));
    sustainLabel.setBounds(labelArea.removeFromRight(getHeight() * 0.25));
    releaseLabel.setBounds(labelArea);

    auto adsrArea = area.removeFromRight(getWidth() * 0.85);
    attackSlider.setBounds(adsrArea.removeFromTop(getHeight() * 0.25));
    decaySlider.setBounds(adsrArea.removeFromTop(getHeight() * 0.25));
    sustainSlider.setBounds(adsrArea.removeFromTop(getHeight() * 0.25));
    releaseSlider.setBounds(adsrArea/*.removeFromLeft(140)*/);
}