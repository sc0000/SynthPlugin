/*
  ==============================================================================

    SynthVoice.cpp
    Created: 5 Jul 2021 5:30:17pm
    Author:  sebas

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
    oscillator.setFrequency(juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber));
}

void SynthVoice::stopNote(float /*velocity*/, bool allowTailOff)
{
    adsr.noteOff();
}

void SynthVoice::prepareToPlay(double sampleRate, int samplesPerBlock, int outputChannels)
{
    adsr.setSampleRate(sampleRate);

    juce::dsp::ProcessSpec spec;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = outputChannels;
    spec.sampleRate = sampleRate;

    oscillator.prepare(spec);
    

    gain.prepare(spec);
    gain.setGainLinear(0.01f);

    isPrepared = true;
}

void SynthVoice::renderNextBlock(juce::AudioSampleBuffer& outputBuffer,
    int startSample,
    int numSamples)
{
    jassert(isPrepared);

    juce::dsp::AudioBlock<float> audioBlock(outputBuffer);

    oscillator.process(juce::dsp::ProcessContextReplacing<float>(audioBlock));
    gain.process(juce::dsp::ProcessContextReplacing<float>(audioBlock));

    adsr.applyEnvelopeToBuffer(outputBuffer, startSample, numSamples);
}
