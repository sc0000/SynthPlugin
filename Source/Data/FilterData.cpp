/*
  ==============================================================================

    FilterData.cpp
    Created: 7 Jul 2021 10:04:00am
    Author:  Sebastian Cyliax

  ==============================================================================
*/

#include "FilterData.h"


void FilterData::prepareToPlay(double sampleRate, int samplesPerBlock, int numChannels)
{
    tptFilter.reset();
    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = numChannels;
    
    tptFilter.prepare(spec);

    isPrepared = true;
}

void FilterData::process(juce::AudioBuffer<float>& buffer)
{
    jassert(isPrepared);

    juce::dsp::AudioBlock<float> audioBlock(buffer);

    tptFilter.process(juce::dsp::ProcessContextReplacing<float>(audioBlock));
}

void FilterData::updateParameters(const int filterType, const float cutoffFrequency, const float resonance, const float modulator)
{
    switch (filterType)
    {
    case 0:
        tptFilter.setType(juce::dsp::StateVariableTPTFilterType::lowpass);
        break;
    case 1:
        tptFilter.setType(juce::dsp::StateVariableTPTFilterType::bandpass);
        break;
    case 2:
        tptFilter.setType(juce::dsp::StateVariableTPTFilterType::highpass);
        break;
    }

    float modFrequency = cutoffFrequency * modulator;
    modFrequency = std::fmax(modFrequency, 20.0f);
    modFrequency = std::fmin(modFrequency, 20000.0f);

    tptFilter.setCutoffFrequency(modFrequency);
    tptFilter.setResonance(resonance);
}

void FilterData::reset()
{
    tptFilter.reset();
}