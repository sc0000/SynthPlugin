/*
  ==============================================================================

    JUCE plugin editor

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "UI/ADSRComponent.h"
#include "UI/ChoiceBox.h"
#include "UI/GainComponent.h"
#include "UI/FMandFilterComponent.h"
#include "UI/MidiMessageLogComponent.h"

//==============================================================================
/**
*/
class BasicSynth2AudioProcessorEditor  : public juce::AudioProcessorEditor, private juce::Timer
{
public:
    BasicSynth2AudioProcessorEditor (BasicSynth2AudioProcessor&);
    ~BasicSynth2AudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

    void timerCallback() override;

private:
    // Reference to the plugin processor
    BasicSynth2AudioProcessor& audioProcessor;


    double startTime;

    juce::MidiKeyboardComponent keyboardComponent;

    ChoiceBox waveformChoice;
    ChoiceBox filterChoice;
    GainComponent gainComponent;
    ADSRComponent adsrComponent;
    ADSRComponent filterAdsrComponent;
    FMandFilterComponent fmComponent;
    FMandFilterComponent filterComponent;
    MidiMessageLogComponent midiMessageLogComponent;

    std::vector<juce::Component*> getComponents() 
    { 
        return { &keyboardComponent, &waveformChoice, &filterChoice, &gainComponent, 
            &adsrComponent, &filterAdsrComponent, &fmComponent, &filterComponent, &midiMessageLogComponent }; 
    }

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BasicSynth2AudioProcessorEditor)
};
