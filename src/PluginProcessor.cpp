#include "PluginProcessor.h"
#include "PluginEditor.h"

PluginProcessor::PluginProcessor()
    : AudioProcessor(BusesProperties()
          .withInput("Input",  juce::AudioChannelSet::stereo(), true)
          .withOutput("Output", juce::AudioChannelSet::stereo(), true)),
      apvts(*this, nullptr, "Parameters", createParameterLayout())
{
}

PluginProcessor::~PluginProcessor() {}

void PluginProcessor::prepareToPlay(double, int) {}
void PluginProcessor::releaseResources() {}

void PluginProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer&)
{
    juce::ScopedNoDenormals noDenormals;
    // Pass-through: copy input to output unchanged
    for (int ch = 0; ch < buffer.getNumChannels(); ++ch)
        buffer.copyFrom(ch, 0, buffer.getReadPointer(ch), buffer.getNumSamples());
}

juce::AudioProcessorEditor* PluginProcessor::createEditor()
{
    return new PluginEditor(*this);
}

juce::AudioProcessorValueTreeState::ParameterLayout PluginProcessor::createParameterLayout()
{
    // Example parameter — replace or extend for your plugin.
    // Pattern: AudioParameterFloat(id, name, NormalisableRange, default)
    return {
        std::make_unique<juce::AudioParameterFloat>(
            "gain",          // Parameter ID — use in getRawParameterValue("gain")
            "Gain",          // Display name shown in DAW automation lanes
            juce::NormalisableRange<float>(0.0f, 1.0f, 0.01f),
            1.0f             // Default: unity gain
        )
    };
}

void PluginProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    auto state = apvts.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void PluginProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xml(getXmlFromBinary(data, sizeInBytes));
    if (xml != nullptr)
        apvts.replaceState(juce::ValueTree::fromXml(*xml));
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new PluginProcessor();
}
