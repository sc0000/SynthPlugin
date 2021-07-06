/*
  ==============================================================================

    ADSRData.cpp
    Created: 6 Jul 2021 8:43:10pm
    Author:  sebas

  ==============================================================================
*/

#include "ADSRData.h"

void ADSRData::updateADSR(const float a,
    const float d, const float s, const float r)
{
    adsrParameters.attack = a;
    adsrParameters.decay = d;
    adsrParameters.sustain = s;
    adsrParameters.release = r;

    setParameters(adsrParameters);
}
