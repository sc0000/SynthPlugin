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
    void prepareToPlay(juce::dsp::ProcessSpec& spec);
    void getNextAudioBlock(juce::dsp::AudioBlock<float>& audioBlock);
    void setOscillatorFrequency(const int& mNN);
    void setFMParameters(const float freq, const float depth);

private:
    juce::dsp::Oscillator<float> fmOscillator{ [](float x) { return std::sin(x); } };
    float fmMod = 0.0f, fmDepth = 0.0f;
    int midiNoteNumber;
};
