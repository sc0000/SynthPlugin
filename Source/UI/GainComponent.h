/*
  ==============================================================================

    GainComponent.h
    Created: 7 Jul 2021 1:51:03am
    Author:  Sebastian Cyliax

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class GainComponent  : public juce::Component
{
public:
    GainComponent(juce::AudioProcessorValueTreeState& apvts);
    ~GainComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:

    struct CustomRotarySlider : public juce::Slider
    {
        CustomRotarySlider() : juce::Slider(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag,
            juce::Slider::TextEntryBoxPosition::NoTextBox)
        {}
    };

    CustomRotarySlider gainSlider;
    juce::AudioProcessorValueTreeState::SliderAttachment gainSliderAttachment;
    juce::Label gainLabel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GainComponent)
};
