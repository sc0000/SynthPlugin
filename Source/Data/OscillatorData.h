/*
  ==============================================================================

    OscillatorData.h
    Created: 6 Jul 2021 9:03:05pm
    Author:  Sebastian Cyliax

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class OscillatorData : public juce::dsp::Oscillator<float>
{
public:
    void setWaveform(const int choice);
    void prepareToPlay(juce::dsp::ProcessSpec& spec) { prepare(spec); }
    void getNextAudioBlock(juce::dsp::AudioBlock<float>& audioBlock) { process(juce::dsp::ProcessContextReplacing<float>(audioBlock)); }
    void setOscillatorFrequency(double midiNoteInHertz) { setFrequency(midiNoteInHertz); }

private:

};
