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
#include "Data/FilterData.h"

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

    void updateADSR(ADSRData& instance, const float a, const float d, const float s, const float r);
    void updateFilter(const int filterType, const float cutoffFrequency, const float resonance);
    
    OscillatorData& getOscillator() { return oscillator; }
    // juce::dsp::Gain<float>& getGain() { return gain; }
    ADSRData& getADSR() { return adsr; }
    ADSRData& getFilterADSR() { return filterAdsr; }
    
private:
    juce::AudioBuffer<float> synthBuffer;
    OscillatorData oscillator;
    // juce::dsp::Gain<float> gain;
    ADSRData adsr;
    ADSRData filterAdsr;
    FilterData filter;

    bool isPrepared = false;
};


