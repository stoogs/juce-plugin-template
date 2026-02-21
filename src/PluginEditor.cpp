#include "PluginEditor.h"

PluginEditor::PluginEditor(PluginProcessor& p)
    : AudioProcessorEditor(&p), processorRef(p)
{
    setSize(400, 300);
}

PluginEditor::~PluginEditor() {}

void PluginEditor::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::darkgrey);
    g.setColour(juce::Colours::white);
    g.setFont(16.0f);
    g.drawText("Plugin Template", getLocalBounds(), juce::Justification::centred, true);
}

void PluginEditor::resized() {}
