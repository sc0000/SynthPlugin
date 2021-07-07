/*
  ==============================================================================

    PluginProcessor.h
    Created: 6 Jul 2021 6:40:32pm
    Author:  Sebastian Cyliax

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class ADSRComponent  : public juce::Component
{
public:
    ADSRComponent(juce::AudioProcessorValueTreeState& apvts, const juce::StringArray& id);
    ~ADSRComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    struct CustomLinearSlider : public juce::Slider
    {
        CustomLinearSlider() : juce::Slider(juce::Slider::SliderStyle::LinearHorizontal,
            juce::Slider::TextEntryBoxPosition::NoTextBox)
        {}
    };

    // ADSR sliders
    CustomLinearSlider attackSlider, decaySlider, sustainSlider, releaseSlider;

    juce::AudioProcessorValueTreeState::SliderAttachment attackSliderAttachment, decaySliderAttachment,
        sustainSliderAttachment, releaseSliderAttachment;

    juce::Label attackLabel, decayLabel, sustainLabel, releaseLabel;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ADSRComponent)
};
