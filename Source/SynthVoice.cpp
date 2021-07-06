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
    adsr.noteOn();
    oscillator.setOscillatorFrequency(midiNoteNumber);
}

void SynthVoice::stopNote(float /*velocity*/, bool allowTailOff)
{
    adsr.noteOff();
    if (!allowTailOff || !adsr.isActive())
        clearCurrentNote();
}

void SynthVoice::prepareToPlay(double sampleRate, int samplesPerBlock, int outputChannels)
{
    adsr.setSampleRate(sampleRate);

    juce::dsp::ProcessSpec spec;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = outputChannels;
    spec.sampleRate = sampleRate;

    oscillator.prepareToPlay(spec);

    gain.prepare(spec);
    gain.setGainLinear(0.01f);

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
    synthBuffer.clear();

    juce::dsp::AudioBlock<float> audioBlock(synthBuffer);

    oscillator.getNextAudioBlock(audioBlock);
    gain.process(juce::dsp::ProcessContextReplacing<float>(audioBlock));

    adsr.applyEnvelopeToBuffer(synthBuffer, 0, synthBuffer.getNumSamples());

    for (auto channel = 0; channel < outputBuffer.getNumChannels(); ++channel)
    {
        outputBuffer.addFrom(channel, startSample, synthBuffer, channel, 0, numSamples);

        if (!adsr.isActive())
            clearCurrentNote();
    }
}

void SynthVoice::update(const float a, const float d, const float s, const float r)
{
    adsr.updateADSR(a, d, s, r);
}
