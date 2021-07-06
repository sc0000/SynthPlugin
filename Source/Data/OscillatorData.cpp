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
        initialise([](float x) { return std::sin(x); });
        break;
    case 1:
        // Sawtooth Wave
        initialise([](float x) { return x / juce::MathConstants<float>::pi; });
        break;
    case 2:
        // Square Wave
        initialise([](float x) { return x < 0.0f ? -1.0f : 1.0f; });
        break;
    case 3:
        // Triangle Wave // working???
        initialise([](float x) { return 1.0 - std::abs(std::fmod(x, 2.0) - 1.0); });
        break;
    }

}
