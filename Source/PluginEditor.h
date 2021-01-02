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
class MainWindow  : public juce::AudioProcessorEditor
{
public:
    MainWindow(PanAudioProcessor&);
    ~MainWindow() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    void initPresets();

private:
    PanAudioProcessor& audioProcessor;
    juce::Label ml_header;
    juce::Label ml_panLeftSliderValue;
    juce::Label ml_panRightSliderValue;
    juce::ComboBox mc_Presets;
    juce::Slider m_panslider;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainWindow)
};
