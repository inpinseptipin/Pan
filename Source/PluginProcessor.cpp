/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
PanAudioProcessor::PanAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
    addParameter(m_Pan = new juce::AudioParameterFloat("pan", "Pan", -1.0, 1.0, 0.0));
    m_smoothed = m_Pan->get();
    
}

PanAudioProcessor::~PanAudioProcessor()
{
}

//==============================================================================
const juce::String PanAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool PanAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool PanAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool PanAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double PanAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int PanAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int PanAudioProcessor::getCurrentProgram()
{
    return 0;
}

void PanAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String PanAudioProcessor::getProgramName (int index)
{
    return {};
}

void PanAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void PanAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    
}

void PanAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool PanAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
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

void PanAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    


    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    
    m_smoothed = m_smoothed - 0.05 * (m_smoothed - m_Pan->get());
    if (*m_Pan < 0.0 && *m_Pan >= -1.0)
    {
        m_right = 1 - abs(*m_Pan);
        m_left = 1;
    }
    else if(*m_Pan > 0.0 && *m_Pan <= 1.0)
    {
        m_left = 1.0-*m_Pan;
        m_right = 1;
    }
    else if (*m_Pan == 0.0)
    {
        m_left = 1.0;
        m_right = 1.0;
    }
    for (int i = 0; i < buffer.getNumSamples(); i++)
    {
        buffer.setSample(0, i, buffer.getSample(0, i) * m_left);
        buffer.setSample(1, i, buffer.getSample(1, i) * m_right); 
    }
    
    
}

//==============================================================================
bool PanAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* PanAudioProcessor::createEditor()
{
    return new MainWindow(*this);
}

//==============================================================================
void PanAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void PanAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new PanAudioProcessor();
}
