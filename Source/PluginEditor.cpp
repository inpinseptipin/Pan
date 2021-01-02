/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
MainWindow::MainWindow(PanAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    setSize (400, 300);

    auto& params = processor.getParameters();
    juce::AudioParameterFloat* m_Pan = (juce::AudioParameterFloat*)params.getUnchecked(0);

    addAndMakeVisible(mc_Presets);
    mc_Presets.setTextWhenNothingSelected("Presets");
    mc_Presets.setColour(juce::ComboBox::ColourIds::backgroundColourId, juce::Colours::black);
    mc_Presets.setColour(juce::ComboBox::ColourIds::textColourId, juce::Colours::white);
    mc_Presets.setColour(juce::ComboBox::ColourIds::arrowColourId, juce::Colours::green);
    mc_Presets.setColour(juce::ComboBox::ColourIds::outlineColourId, juce::Colours::green);

    initPresets();
   
    
    addAndMakeVisible(m_panslider);
    m_panslider.setSliderStyle(juce::Slider::SliderStyle::LinearHorizontal);
    m_panslider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, false, 0, 0);
    m_panslider.setColour(juce::Slider::ColourIds::backgroundColourId, juce::Colours::black);
    m_panslider.setColour(juce::Slider::ColourIds::trackColourId, juce::Colours::green);
    m_panslider.setRange(m_Pan->range.start, m_Pan->range.end);
    m_panslider.setValue(*m_Pan);


    addAndMakeVisible(ml_header);
    ml_header.setText("Plugin : Pan", juce::dontSendNotification);
    ml_header.setColour(juce::Label::ColourIds::backgroundColourId, juce::Colours::black);
    ml_header.setColour(juce::Label::ColourIds::outlineColourId, juce::Colours::white);
    ml_header.setColour(juce::Label::ColourIds::textColourId, juce::Colours::white);
    ml_header.setFont(juce::Font(24.0f, 1));
    ml_header.setJustificationType(juce::Justification::horizontallyCentred);
    ml_header.setEditable(false);


    addAndMakeVisible(ml_panLeftSliderValue);
    ml_panLeftSliderValue.setText("Left = 1",juce::dontSendNotification);
    ml_panLeftSliderValue.setColour(juce::Label::ColourIds::backgroundColourId, juce::Colours::black);
    ml_panLeftSliderValue.setColour(juce::Label::ColourIds::outlineColourId, juce::Colours::blue);
    ml_panLeftSliderValue.setColour(juce::Label::ColourIds::textColourId, juce::Colours::white);
    ml_panLeftSliderValue.setFont(juce::Font(24.0f, 1));
    ml_panLeftSliderValue.setJustificationType(juce::Justification::horizontallyCentred);
    ml_panLeftSliderValue.setEditable(false);


    addAndMakeVisible(ml_panRightSliderValue);
    ml_panRightSliderValue.setText("Right = 1", juce::dontSendNotification);
    ml_panRightSliderValue.setColour(juce::Label::ColourIds::backgroundColourId, juce::Colours::black);
    ml_panRightSliderValue.setColour(juce::Label::ColourIds::outlineColourId, juce::Colours::blue);
    ml_panRightSliderValue.setColour(juce::Label::ColourIds::textColourId, juce::Colours::white);
    ml_panRightSliderValue.setFont(juce::Font(24.0f, 1));
    ml_panRightSliderValue.setJustificationType(juce::Justification::horizontallyCentred);
    ml_panRightSliderValue.setEditable(false);

        
    m_panslider.onValueChange = [this,m_Pan]
    {
        *m_Pan = m_panslider.getValue();
        if (m_panslider.getValue() < 0.0 )
        {
            ml_panLeftSliderValue.setText("Left = 1" , juce::dontSendNotification);
            ml_panRightSliderValue.setText("Right = " + juce::String(1-abs(*m_Pan )), juce::dontSendNotification);
            m_panslider.setColour(juce::Slider::ColourIds::trackColourId, juce::Colours::red);
        }
        if (m_panslider.getValue() == 0.0)
        {
            ml_panLeftSliderValue.setText("Left = 1", juce::dontSendNotification);
            ml_panRightSliderValue.setText("Right = 1", juce::dontSendNotification);
            m_panslider.setColour(juce::Slider::ColourIds::trackColourId, juce::Colours::green);
        }
        if (m_panslider.getValue() > 0.0)
        {
            ml_panLeftSliderValue.setText("Left = "+ juce::String(1 - *m_Pan), juce::dontSendNotification);
            ml_panRightSliderValue.setText("Right = 1", juce::dontSendNotification);
            m_panslider.setColour(juce::Slider::ColourIds::trackColourId, juce::Colours::blue);
        }
    };

    
    mc_Presets.onChange = [this, m_Pan]
    {
        if (mc_Presets.getText() == "Reset")
        {
            *m_Pan = 0.0;
            m_panslider.setValue(*m_Pan);
            m_panslider.setColour(juce::Slider::ColourIds::trackColourId, juce::Colours::green);
            ml_panLeftSliderValue.setText("Left = 1", juce::dontSendNotification);
            ml_panRightSliderValue.setText("Right = 1", juce::dontSendNotification);
        }

        if (mc_Presets.getText() == "Only Left")
        {
            *m_Pan = -1.0;
            m_panslider.setValue(*m_Pan);
            m_panslider.setColour(juce::Slider::ColourIds::trackColourId, juce::Colours::red);
            ml_panLeftSliderValue.setText("Left = 1", juce::dontSendNotification);
            ml_panRightSliderValue.setText("Right = 0", juce::dontSendNotification);
        }

        if (mc_Presets.getText() == "Only Right")
        {
            *m_Pan = 1.0;
            m_panslider.setValue(*m_Pan);
            m_panslider.setColour(juce::Slider::ColourIds::trackColourId, juce::Colours::blue);
            ml_panLeftSliderValue.setText("Left = 0", juce::dontSendNotification);
            ml_panRightSliderValue.setText("Right = 1", juce::dontSendNotification);
        }

        

    };
    

}

MainWindow::~MainWindow()
{
}

//==============================================================================
void MainWindow::paint (juce::Graphics& g)
{
    g.setColour(juce::Colours::black);
    g.fillAll();
}

void MainWindow::resized()
{
    ml_header.setBounds(0, 0, getRight(), 2*getBottom() / 10);
    m_panslider.setBounds(0, 2*getBottom()/10 , getRight() ,2*getBottom() / 10);
    ml_panLeftSliderValue.setBounds(0, m_panslider.getBottom(), getRight()/2, getBottom() / 10);
    ml_panRightSliderValue.setBounds(getRight()/2, m_panslider.getBottom(), getRight()/2, getBottom() / 10);
    mc_Presets.setBounds(getRight()/4, ml_panLeftSliderValue.getBottom() + getBottom()/10, getRight() / 2, getBottom() / 10);

}

void MainWindow::initPresets()
{
    mc_Presets.addItemList({ "Reset","Only Left","Only Right"}, 1);
}

