#ifndef ASHAY_CHORUS
#define ASHAY_CHORUS
/*
  ==============================================================================

    Chorus.h
    Created: 5 Nov 2023 11:48:39pm
    Author:  ashay

  ==============================================================================
*/

#include "JuceHeader.h"
#include <math.h>

class Chorus
{
public:
    Chorus();
    ~Chorus();
    void process(juce::AudioBuffer<float>& buffer, float modulationRateParameter, float feedbackParameter, float dryWetMixParameter);
    void prepareToPlay(double sampleRate, int samplesPerBlock);
    void reset(float delayTimeParameter, float modulationDepthParameter);
private:
    int modulationDepthSamples, delayLengthSamples;
    float delayTime, modulationRate, modulationDepth, feedback, dryWetMix;
    double delaySampleRate;
    int delaySamplesPerBlock;
    double sampleRate;

    std::vector<float> delayBufferLeft, delayBufferRight;
};

#endif
