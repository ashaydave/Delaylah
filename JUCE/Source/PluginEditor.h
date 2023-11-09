/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class HeyThereDelaylahAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    HeyThereDelaylahAudioProcessorEditor (HeyThereDelaylahAudioProcessor&);
    ~HeyThereDelaylahAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    HeyThereDelaylahAudioProcessor& audioProcessor;

    juce::Label delayTimeLabel, modulationDepthLabel, modulationRateLabel, feedbackLabel, dryWetMixLabel;
    juce::Slider delayTimeSlider, modulationDepthSlider, modulationRateSlider, feedbackSlider, dryWetMixSlider;

    juce::AudioParameterFloat* delayTimeSliderParameter;
    juce::AudioParameterFloat* modulationDepthSliderParameter;
    juce::AudioParameterFloat* modulationRateSliderParameter;
    juce::AudioParameterFloat* feedbackSliderParameter;
    juce::AudioParameterFloat* dryWetMixSliderParameter;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (HeyThereDelaylahAudioProcessorEditor)
};
