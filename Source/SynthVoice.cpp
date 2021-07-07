/*
  ==============================================================================

    SynthVoice.cpp
    Created: 5 Jul 2021 5:30:17pm
    Author:  Sebastian Cyliax

  ==============================================================================
*/

#include "SynthVoice.h"

bool SynthVoice::canPlaySound(juce::SynthesiserSound* sound)
{
    return dynamic_cast<juce::SynthesiserSound*>(sound) != nullptr;
}

void SynthVoice::startNote(int midiNoteNumber,
    float velocity,
    juce::SynthesiserSound* sound,
    int /*currentPitchWheelPosition*/)
{
    oscillator.setOscillatorFrequency(midiNoteNumber);
    adsr.noteOn();
    filterAdsr.noteOn();
}

void SynthVoice::stopNote(float /*velocity*/, bool allowTailOff)
{
    adsr.noteOff();
    if (!allowTailOff || !adsr.isActive())
        clearCurrentNote();

    filterAdsr.noteOff();
}

void SynthVoice::prepareToPlay(double sampleRate, int samplesPerBlock, int outputChannels)
{
    adsr.setSampleRate(sampleRate);
    filterAdsr.setSampleRate(sampleRate);

    juce::dsp::ProcessSpec spec;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = outputChannels;
    spec.sampleRate = sampleRate;

    oscillator.prepareToPlay(spec);

    gain.prepare(spec);
    gain.setGainLinear(0.01f);

    filter.prepareToPlay(sampleRate, samplesPerBlock, outputChannels);

    isPrepared = true;
}

void SynthVoice::renderNextBlock(juce::AudioSampleBuffer& outputBuffer,
    int startSample,
    int numSamples)
{
    jassert(isPrepared);

    if (!isVoiceActive())
        return;

    synthBuffer.setSize(outputBuffer.getNumChannels(), numSamples, false, false, true);

    filterAdsr.applyEnvelopeToBuffer(synthBuffer, 0, synthBuffer.getNumSamples()); // doesn't do what it says, just activates the filter ADSR ;)

    synthBuffer.clear();

    juce::dsp::AudioBlock<float> audioBlock(synthBuffer);

    oscillator.getNextAudioBlock(audioBlock);
    filter.process(synthBuffer);
    gain.process(juce::dsp::ProcessContextReplacing<float>(audioBlock));

    adsr.applyEnvelopeToBuffer(synthBuffer, 0, synthBuffer.getNumSamples());

    for (auto channel = 0; channel < outputBuffer.getNumChannels(); ++channel)
    {
        outputBuffer.addFrom(channel, startSample, synthBuffer, channel, 0, numSamples);

        if (!adsr.isActive())
            clearCurrentNote();
    }

    filterAdsr.getNextSample();
}

void SynthVoice::updateADSR(ADSRData& instance, const float a, const float d, const float s, const float r)
{
    instance.updateADSR(a, d, s, r);
}

void SynthVoice::updateFilter(const int filterType, const float cutoffFrequency, const float resonance)
{
    auto modulator = filterAdsr.getNextSample();
    filter.updateParameters(filterType, cutoffFrequency, resonance, modulator);
}