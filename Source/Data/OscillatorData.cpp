/*
  ==============================================================================

    OscillatorData.cpp
    Created: 6 Jul 2021 9:03:05pm
    Author:  Sebastian Cyliax

  ==============================================================================
*/

#include "OscillatorData.h"

void OscillatorData::setWaveform(const int choice)
{
    switch (choice)
    {
    case 0:
        // Sine Wave
        processorChain.get<oscillator1Index>().initialise([](float x) { return std::sin(x); });
        processorChain.get<oscillator2Index>().initialise([](float x) { return std::sin(x); });
        break;
    case 1:
        // Sawtooth Wave
        processorChain.get<oscillator1Index>().initialise([](float x) 
            {
            return juce::jmap(x,
                -juce::MathConstants<float>::pi,
                juce::MathConstants<float>::pi,
                -1.0f,
                1.0f);
            }, 2);
        processorChain.get<oscillator2Index>().initialise([](float x)
            {
                return juce::jmap(x,
                    -juce::MathConstants<float>::pi,
                    juce::MathConstants<float>::pi,
                    -1.0f,
                    1.0f);
            }, 2);
        break;
    case 2:
        // Square Wave
        processorChain.get<oscillator1Index>().initialise([](float x) { return x < 0.0f ? -1.0f : 1.0f; });
        processorChain.get<oscillator2Index>().initialise([](float x) { return x < 0.0f ? -1.0f : 1.0f; });
        break;
    case 3:
        // Triangle Wave
        processorChain.get<oscillator1Index>().initialise([](float x) { return 1.0 - std::abs(std::fmod(x, 2.0) - 1.0); });
        processorChain.get<oscillator2Index>().initialise([](float x) { return 1.0 - std::abs(std::fmod(x, 2.0) - 1.0); });
        break;
    }
}

void OscillatorData::prepareToPlay(juce::dsp::ProcessSpec& spec) 
{ 
    processorChain.prepare(spec);
    fmOscillator.prepare(spec);
    
    processorChain.get<masterGainIndex>().setGainLinear(0.01f);

}

void OscillatorData::getNextAudioBlock(juce::dsp::AudioBlock<float>& audioBlock) 
{ 
    for (int channel = 0; channel < audioBlock.getNumChannels(); ++channel)
    {
        for (int sample = 0; sample < audioBlock.getNumSamples(); ++sample)
        {
            fmMod = fmOscillator.processSample(audioBlock.getSample(channel, sample)) * fmDepth;
        }
    }
    
    processorChain.process(juce::dsp::ProcessContextReplacing<float>(audioBlock));
}

void OscillatorData::setOscillatorFrequency(const int& mNN)
{ 
    midiNoteNumber = mNN;
    processorChain.get<oscillator1Index>().setFrequency(std::abs(juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber) + fmMod));
    processorChain.get<oscillator2Index>().setFrequency(std::abs(juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber) * 1.005f + fmMod));
}

void OscillatorData::setFMParameters(const float freq, const float depth)
{
    fmOscillator.setFrequency(freq);
    fmDepth = depth;
    processorChain.get<oscillator1Index>().setFrequency(std::abs(juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber) + fmMod));
    processorChain.get<oscillator2Index>().setFrequency(std::abs(juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber) * 1.005f + fmMod));
}
