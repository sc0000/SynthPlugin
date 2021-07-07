/*
  ==============================================================================

    ChoiceBox.h
    Created: 6 Jul 2021 10:09:28pm
    Author:  Sebastian Cyliax

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class ChoiceBox  : public juce::Component
{
public:
    ChoiceBox(juce::AudioProcessorValueTreeState& apvts, const juce::String& id, juce::MidiKeyboardComponent& k);
    ~ChoiceBox() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    juce::ComboBox choiceBox;
    juce::AudioProcessorValueTreeState::ComboBoxAttachment choiceBoxAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ChoiceBox)
};
