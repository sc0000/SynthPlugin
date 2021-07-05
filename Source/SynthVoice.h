/*
  ==============================================================================

    SynthVoice.h
    Created: 5 Jul 2021 5:30:17pm
    Author:  sebas

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "SynthSound.h"

class SynthVoice : public juce::SynthesiserVoice
{
public:
    bool canPlaySound(juce::SynthesiserSound* sound) override;

    void startNote(int midiNoteNumber,
        float velocity,
        juce::SynthesiserSound* sound,
        int /*currentPitchWheelPosition*/) override;

    void stopNote(float /*velocity*/, bool allowTailOff) override;

    void prepareToPlay(double sampleRate, int samplesPerBlock, int outputChannels);

    void renderNextBlock(juce::AudioSampleBuffer& outputBuffer,
        int startSample,
        int numSamples) override;

    void pitchWheelMoved(int newPitchWheelValue) override {}
    void controllerMoved(int controllerNumber, int newControllerValue) override {}

private:
    juce::ADSR adsr;
    juce::ADSR::Parameters adsrParameters;
    juce::dsp::Oscillator<float> oscillator{ [](float x) { return x / juce::MathConstants<float>::pi; } };
    juce::dsp::Gain<float> gain;

    // Waveforms: 
    //===========
    // sine: std::sin(x)
    // saw: x / juce::MathConstants<T>::pi
    // square: x < 0.0f ? -1.0f : 1.0f

    bool isPrepared = false;
};


