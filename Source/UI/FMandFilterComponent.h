/*
  ==============================================================================

    FMandFilterComponent.h
    Created: 7 Jul 2021 2:33:32am
    Author:  Sebastian Cyliax

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class FMandFilterComponent  : public juce::Component
{
public:
    FMandFilterComponent(juce::AudioProcessorValueTreeState& apvts, const juce::StringArray& id);
    ~FMandFilterComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    struct CustomRotarySlider : public juce::Slider
    {
        CustomRotarySlider() : juce::Slider(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag,
            juce::Slider::TextEntryBoxPosition::NoTextBox)
        {}
    };

    CustomRotarySlider slider1, slider2;
    juce::AudioProcessorValueTreeState::SliderAttachment slider1Attachment, slider2Attachment;
    juce::Label label1, label2;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FMandFilterComponent)
};
