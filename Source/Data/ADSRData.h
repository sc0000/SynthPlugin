/*
  ==============================================================================

    ADSRData.h
    Created: 6 Jul 2021 8:43:10pm
    Author:  Sebastian Cyliax

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class ADSRData : public juce::ADSR
{
public:
    void updateADSR(const float a, const float d, const float s, const float r);
private:
    juce::ADSR::Parameters adsrParameters;
};