/*
  ==============================================================================

    Chorus.cpp
    Created: 5 Nov 2023 11:48:39pm
    Author:  ashay

  ==============================================================================
*/

#include "Chorus.h"

Chorus::Chorus()
{
    delayTime = 0.030f;
    modulationRate = 4.0f;
    modulationDepth = 0.003f;
    feedback = 0.7f;
    dryWetMix = 0.4f;

    delayLengthSamples = 0;
    modulationDepthSamples = 0;

    delaySampleRate = 0;
    delaySamplesPerBlock = 0;
}

Chorus::~Chorus()
{
}

void Chorus::process(juce::AudioBuffer<float>& buffer, float modulationRateParameter, float feedbackParameter, float dryWetMixParameter)
{
    float modulationWave = 2 * juce::MathConstants<float>::pi * (modulationRateParameter / sampleRate);
    

    for (int i = 0; i < buffer.getNumSamples(); i++)
    {
        float leftDry = buffer.getReadPointer(0)[i];
        float rightDry = buffer.getReadPointer(1)[i];
        float modulatedSample = modulationDepthSamples * sinf(modulationWave * i + 1);
        modulatedSample += delayLengthSamples;

        float interp_y1_left = delayBufferLeft[juce::jmax(0, (int)floor(modulatedSample))];
        float interp_y2_left = delayBufferLeft[juce::jmin((int)size(delayBufferLeft), (int)ceil(modulatedSample))];
        float querySampleLeft = modulatedSample - floor(modulatedSample);
        float leftInput = buffer.getReadPointer(0)[i];

        float interLeft = interp_y1_left + (interp_y2_left - interp_y1_left) * querySampleLeft;

        float interp_y1_right = delayBufferRight[juce::jmax(0, (int)floor(modulatedSample))];
        float interp_y2_right = delayBufferRight[juce::jmin((int)size(delayBufferRight), (int)ceil(modulatedSample))];
        float querySampleRight = modulatedSample - floor(modulatedSample);
        float rightInput = buffer.getReadPointer(1)[i];

        float interRight = interp_y1_right + (interp_y2_right - interp_y1_right) * querySampleRight;

        float newSampleLeft = (1 - feedbackParameter) * (leftInput) + (interLeft * feedbackParameter);
        float newSampleRight = (1 - feedbackParameter) * (rightInput) + (interRight * feedbackParameter);

  /*     for (int j = 1; j < delayBufferLeft.size(); j++)
        {
            delayBufferLeft[j] = delayBufferLeft[j - 1];
        }*/

        rotate(delayBufferLeft.begin(), delayBufferLeft.end() - 1, delayBufferLeft.end());
        delayBufferLeft[0] = newSampleLeft;

    /*    for (int j = 1; j < delayBufferRight.size(); j++)
        {
            delayBufferRight[j] = delayBufferRight[j - 1];
        }*/

        rotate(delayBufferRight.begin(), delayBufferRight.end() - 1, delayBufferRight.end());
        delayBufferRight[0] = newSampleRight;

        buffer.getWritePointer(0)[i] = (1 - dryWetMixParameter) * (leftDry)+ (dryWetMixParameter * newSampleLeft);
        buffer.getWritePointer(1)[i] = (1 - dryWetMixParameter) * (rightDry) + (dryWetMixParameter * newSampleRight);

    }

}

void Chorus::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    this->sampleRate = sampleRate;
    this->delayLengthSamples = (int)(round(delayTime * sampleRate));
    this->modulationDepthSamples = modulationDepth * sampleRate;

}

void Chorus::reset(float delayTimeParameter, float modulationDepthParameter)
{
    this->delayLengthSamples = delayTimeParameter * sampleRate;
    this->modulationDepthSamples = round(modulationDepthParameter * sampleRate);
    this->delayBufferLeft.resize((delayTimeParameter * sampleRate) + modulationDepthSamples, 0.0f);
    this->delayBufferRight.resize((delayTimeParameter * sampleRate) + modulationDepthSamples, 0.0f);
}
