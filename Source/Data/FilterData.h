/*
  ==============================================================================

    FilterData.h
    Created: 7 Jul 2021 10:04:00am
    Author:  Sebastian Cyliax

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class FilterData
{
public:
    void prepareToPlay(double sampleRate, int samplesPerBlock, int numChannels);
    void process(juce::AudioBuffer<float>& buffer);
    void updateParameters(const int filterType, float cutoffFrequency, const float resonance, const float modulator = 1.0f);
    void reset();
private:
    juce::dsp::StateVariableTPTFilter<float> tptFilter;

    bool isPrepared = false;
};