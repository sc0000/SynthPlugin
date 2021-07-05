/*
  ==============================================================================

    SynthSound.h
    Created: 5 Jul 2021 5:31:11pm
    Author:  sebas

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

struct SynthSound : public juce::SynthesiserSound
{
    SynthSound() {}

    bool appliesToNote(int midiNoteNumber) override { return true; }
    bool appliesToChannel(int midiChannel) override { return true; }
};