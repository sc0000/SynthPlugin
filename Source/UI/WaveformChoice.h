/*
  ==============================================================================

    WaveformChoice.h
    Created: 6 Jul 2021 10:09:28pm
    Author:  Sebastian Cyliax

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class WaveformChoice  : public juce::Component
{
public:
    WaveformChoice(juce::AudioProcessorValueTreeState& apvts);
    ~WaveformChoice() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    juce::ComboBox waveformChoice;
    juce::AudioProcessorValueTreeState::ComboBoxAttachment waveformChoiceAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WaveformChoice)
};
