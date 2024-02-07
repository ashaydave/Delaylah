/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
HeyThereDelaylahAudioProcessorEditor::HeyThereDelaylahAudioProcessorEditor (HeyThereDelaylahAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (700, 300);

   /* onValueChange = [this]
    {
        audioProcessor.delayParameter = delaySlider.getValue();
    }*/

    auto parameterTree = audioProcessor.getParameters();

    addAndMakeVisible(delayTimeLabel);
    delayTimeLabel.setText("", juce::dontSendNotification);
    delayTimeLabel.setColour(juce::Label::textColourId, juce::Colours::lightgoldenrodyellow);
    delayTimeLabel.setJustificationType(juce::Justification::centred);
    delayTimeLabel.setFont(juce::Font(14.0f, juce::Font::bold));

    addAndMakeVisible(delayTimeSlider);
    delayTimeSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    delayTimeSlider.setColour(juce::Slider::ColourIds::rotarySliderFillColourId, juce::Colours::ghostwhite);
    delayTimeSlider.setColour(juce::Slider::ColourIds::backgroundColourId, juce::Colours::floralwhite);
    delayTimeSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 40, 30);

    delayTimeSliderParameter = (juce::AudioParameterFloat*)parameterTree.getUnchecked(0);
    delayTimeSlider.setRange(delayTimeSliderParameter->range.start, delayTimeSliderParameter->range.end, 0.001);

    delayTimeSlider.setValue(*delayTimeSliderParameter, juce::NotificationType::dontSendNotification);

    delayTimeSlider.onValueChange = [this]
    {
        *audioProcessor.delayParameter = delayTimeSlider.getValue();
        
    };

    /*------------*/

    addAndMakeVisible(modulationDepthLabel);
    modulationDepthLabel.setText("", juce::dontSendNotification);
    modulationDepthLabel.setColour(juce::Label::textColourId, juce::Colours::lightgoldenrodyellow);
    modulationDepthLabel.setJustificationType(juce::Justification::centred);
    modulationDepthLabel.setFont(juce::Font(14.0f, juce::Font::bold));

    addAndMakeVisible(modulationDepthSlider);
    modulationDepthSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    modulationDepthSlider.setColour(juce::Slider::ColourIds::rotarySliderFillColourId, juce::Colours::ghostwhite);
    modulationDepthSlider.setColour(juce::Slider::ColourIds::backgroundColourId, juce::Colours::floralwhite);
    modulationDepthSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 30);

    modulationDepthSliderParameter = (juce::AudioParameterFloat*)parameterTree.getUnchecked(1);
    modulationDepthSlider.setRange(modulationDepthSliderParameter->range.start, modulationDepthSliderParameter->range.end, 0.001);

    modulationDepthSlider.setValue(*modulationDepthSliderParameter, juce::NotificationType::dontSendNotification);

    modulationDepthSlider.onValueChange = [this]
    {
        *audioProcessor.modulationDepthParameter = modulationDepthSlider.getValue();
    };

    /*-------------*/

    addAndMakeVisible(modulationRateLabel);
    modulationRateLabel.setText("", juce::dontSendNotification);
    modulationRateLabel.setColour(juce::Label::textColourId, juce::Colours::lightgoldenrodyellow);
    modulationRateLabel.setJustificationType(juce::Justification::centred);
    modulationRateLabel.setFont(juce::Font(14.0f, juce::Font::bold));

    addAndMakeVisible(modulationRateSlider);
    modulationRateSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    modulationRateSlider.setColour(juce::Slider::ColourIds::rotarySliderFillColourId, juce::Colours::ghostwhite);
    modulationRateSlider.setColour(juce::Slider::ColourIds::backgroundColourId, juce::Colours::floralwhite);
    modulationRateSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 30, 30);

    modulationRateSliderParameter = (juce::AudioParameterFloat*)parameterTree.getUnchecked(2);
    modulationRateSlider.setRange(modulationRateSliderParameter->range.start, modulationRateSliderParameter->range.end, 0.1);

    modulationRateSlider.setValue(*modulationRateSliderParameter, juce::NotificationType::dontSendNotification);

    modulationRateSlider.onValueChange = [this]
    {
        *audioProcessor.modulationRateParameter = modulationRateSlider.getValue();

    };

    /*------------*/

    addAndMakeVisible(feedbackLabel);
    feedbackLabel.setText("", juce::dontSendNotification);
    feedbackLabel.setColour(juce::Label::textColourId, juce::Colours::lightgoldenrodyellow);
    feedbackLabel.setJustificationType(juce::Justification::centred);
    feedbackLabel.setFont(juce::Font(14.0f, juce::Font::bold));

    addAndMakeVisible(feedbackSlider);
    feedbackSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    feedbackSlider.setColour(juce::Slider::ColourIds::rotarySliderFillColourId, juce::Colours::ghostwhite);
    feedbackSlider.setColour(juce::Slider::ColourIds::backgroundColourId, juce::Colours::floralwhite);
    feedbackSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 30, 30);

    feedbackSliderParameter = (juce::AudioParameterFloat*)parameterTree.getUnchecked(3);
    feedbackSlider.setRange(feedbackSliderParameter->range.start, feedbackSliderParameter->range.end, 0.01);

    feedbackSlider.setValue(*feedbackSliderParameter, juce::NotificationType::dontSendNotification);

    feedbackSlider.onValueChange = [this]
    {
        *audioProcessor.feedbackParameter = feedbackSlider.getValue();
    };

    /*------------*/

    addAndMakeVisible(dryWetMixLabel);
    dryWetMixLabel.setText("", juce::dontSendNotification);
    dryWetMixLabel.setColour(juce::Label::textColourId, juce::Colours::lightgoldenrodyellow);
    dryWetMixLabel.setJustificationType(juce::Justification::centred);
    dryWetMixLabel.setFont(juce::Font(14.0f, juce::Font::bold));

    addAndMakeVisible(dryWetMixSlider);
    dryWetMixSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    dryWetMixSlider.setColour(juce::Slider::ColourIds::rotarySliderFillColourId, juce::Colours::ghostwhite);
    dryWetMixSlider.setColour(juce::Slider::ColourIds::backgroundColourId, juce::Colours::floralwhite);
    dryWetMixSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 30, 30);

    dryWetMixSliderParameter = (juce::AudioParameterFloat*)parameterTree.getUnchecked(4);
    dryWetMixSlider.setRange(dryWetMixSliderParameter->range.start, dryWetMixSliderParameter->range.end, 0.01);

    dryWetMixSlider.setValue(*dryWetMixSliderParameter, juce::NotificationType::dontSendNotification);

    dryWetMixSlider.onValueChange = [this]
    {
        *audioProcessor.dryWetMixParameter = dryWetMixSlider.getValue();
    };


    getLookAndFeel().setColour(juce::Slider::thumbColourId, juce::Colours::mediumpurple);
    //getLookAndFeel().setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colours::whitesmoke);
    //getLookAndFeel().setColour(juce::Slider::rotarySliderFillColourId, juce::Colours::palevioletred);
    //getLookAndFeel().setColour(juce::Slider::backgroundColourId, juce::Colours::antiquewhite);
    //getLookAndFeel().setColour(juce::Slider::trackColourId, juce::Colours::red);

}


HeyThereDelaylahAudioProcessorEditor::~HeyThereDelaylahAudioProcessorEditor()
{
}

//==============================================================================
void HeyThereDelaylahAudioProcessorEditor::paint (juce::Graphics& g)
{
    auto whiteHue = juce::Colours::floralwhite.getHue();
    auto backgroundImage = juce::ImageCache::getFromMemory(BinaryData::Delaylah_png, BinaryData::Delaylah_pngSize);
    g.drawImage(backgroundImage, 0, 0, getWidth(), getHeight(), 0, 0, 600, 200);
}

void HeyThereDelaylahAudioProcessorEditor::resized()
{
    auto bounds = getLocalBounds();
    auto width = bounds.getWidth();
    auto height = bounds.getHeight();

    float labelWidth = 150.0f;
    float labelHeight = 20.0f;
    float sliderWidth = 100.0f;
    float sliderHeight = 150.0f;
    float xMargin = 10.0f;
    float yMargin = 10.0f;

    float totalWidth = (5 * sliderWidth) + (4 * xMargin);
    // Chen's horizontal layout code:
    // Calculate the horizontal position of the first label
    float startX = (width - totalWidth) / 2;

    delayTimeLabel.setBounds(startX, height / 2 - sliderHeight / 2 - labelHeight - yMargin, labelWidth, labelHeight);
    startX += sliderWidth + xMargin;
    modulationDepthLabel.setBounds(startX, height / 2 - sliderHeight / 2 - labelHeight - yMargin, labelWidth, labelHeight);
    startX += sliderWidth + xMargin;
    modulationRateLabel.setBounds(startX, height / 2 - sliderHeight / 2 - labelHeight - yMargin, labelWidth, labelHeight);
    startX += sliderWidth + xMargin;
    feedbackLabel.setBounds(startX, height / 2 - sliderHeight / 2 - labelHeight - yMargin, labelWidth, labelHeight);
    startX += sliderWidth + xMargin;
    dryWetMixLabel.setBounds(startX, height / 2 - sliderHeight / 2 - labelHeight - yMargin, labelWidth, labelHeight);

    startX = (width - totalWidth) / 2; // Reset the horizontal position

    delayTimeSlider.setBounds(startX - 50, height / 2 - sliderHeight / 7 + yMargin, sliderWidth, sliderHeight);
    startX += sliderWidth + xMargin;
    modulationDepthSlider.setBounds(startX - 25, height / 2 - sliderHeight / 7 + yMargin, sliderWidth, sliderHeight);
    startX += sliderWidth + xMargin;
    modulationRateSlider.setBounds(startX, height / 2 - sliderHeight / 7 + yMargin, sliderWidth, sliderHeight);
    startX += sliderWidth + xMargin;
    feedbackSlider.setBounds(startX + 23, height / 2 - sliderHeight / 7 + yMargin, sliderWidth, sliderHeight);
    startX += sliderWidth + xMargin;
    dryWetMixSlider.setBounds(startX + 45, height / 2 - sliderHeight / 7 + yMargin, sliderWidth, sliderHeight);
}
