/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
HeyThereDelaylahAudioProcessor::HeyThereDelaylahAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
     ), parameters(*this, nullptr, juce::Identifier("DelayLah"), {
         std::make_unique<juce::AudioParameterFloat>("delayParameter", "Delay", 0.010, 0.050, 0.025),
         std::make_unique<juce::AudioParameterFloat>("modulationDepthParameter", "Depth", 0.003, 0.030, 0.003),
         std::make_unique<juce::AudioParameterFloat>("modulationRateParameter", "Rate", 1, 5, 4),
         std::make_unique<juce::AudioParameterFloat>("feedbackParameter", "Feedback", 0.01, 0.3, 0.1),
         std::make_unique<juce::AudioParameterFloat>("dryWetMixParameter", "Mix", 0, 1, 0.5)
                           })
#endif
{
    delayParameter = parameters.getRawParameterValue("delayParameter");
    modulationDepthParameter = parameters.getRawParameterValue("modulationDepthParameter");
    modulationRateParameter = parameters.getRawParameterValue("modulationRateParameter");
    feedbackParameter = parameters.getRawParameterValue("feedbackParameter");
    dryWetMixParameter = parameters.getRawParameterValue("dryWetMixParameter");

    delayParameterValue = 0;
    modulationDepthParameterValue = 0;

}

HeyThereDelaylahAudioProcessor::~HeyThereDelaylahAudioProcessor()
{
}

//==============================================================================
const juce::String HeyThereDelaylahAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool HeyThereDelaylahAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool HeyThereDelaylahAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool HeyThereDelaylahAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double HeyThereDelaylahAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int HeyThereDelaylahAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int HeyThereDelaylahAudioProcessor::getCurrentProgram()
{
    return 0;
}

void HeyThereDelaylahAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String HeyThereDelaylahAudioProcessor::getProgramName (int index)
{
    return {};
}

void HeyThereDelaylahAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void HeyThereDelaylahAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    chorus.prepareToPlay(sampleRate, samplesPerBlock);
}

void HeyThereDelaylahAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool HeyThereDelaylahAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void HeyThereDelaylahAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    if (*delayParameter != delayParameterValue || *modulationDepthParameter != modulationDepthParameterValue)
    {
        chorus.reset(*delayParameter, *modulationDepthParameter);
        delayParameterValue = *delayParameter;
        modulationDepthParameterValue = *modulationDepthParameter;
    }
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    chorus.process(buffer, *modulationRateParameter, *feedbackParameter, *dryWetMixParameter); //idhar pointer karke call your mod rate, feedback, drywet

}

//==============================================================================
bool HeyThereDelaylahAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* HeyThereDelaylahAudioProcessor::createEditor()
{
    return new HeyThereDelaylahAudioProcessorEditor (*this);
    //return new juce::GenericAudioProcessorEditor(*this);
}

//==============================================================================
void HeyThereDelaylahAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void HeyThereDelaylahAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new HeyThereDelaylahAudioProcessor();
}
