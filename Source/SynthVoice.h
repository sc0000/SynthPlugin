/*
  ==============================================================================

    SynthVoice.h
    Created: 5 Jul 2021 5:30:17pm
    Author:  Sebastian Cyliax

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "SynthSound.h"
#include "Data/ADSRData.h"
#include "Data/OscillatorData.h"

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

    void update(const float a, const float d, const float s, const float r);

    OscillatorData& getOscillator() { return oscillator; }

private:
    juce::AudioBuffer<float> synthBuffer;
  
    ADSRData adsr;
    /*juce::dsp::Oscillator<float> oscillator{ [](float x) { return x / juce::MathConstants<float>::pi; } };*/
    OscillatorData oscillator;
    juce::dsp::Gain<float> gain;

    bool isPrepared = false;
};


