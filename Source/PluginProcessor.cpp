/*
  ==============================================================================

    This file was auto-generated by the Introjucer!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================

// Default parameter values
const float defaultFilterType = SVFLowpass;
const float defaultCutoff = 40.0f;
const float defaultResonance = 0.5f;
const float defaultShelfGain = 0.0f;

//==============================================================================
VAStateVariableFilterProcessor::VAStateVariableFilterProcessor()
{
    addParameter(filterType = new FilterTypeParam{ svfFilter, defaultFilterType, "Filter Type" });
    addParameter(cutoff = new CutoffPitchParam{ svfFilter, defaultCutoff, "Cutoff Pitch" });
    addParameter(resonance = new ResonanceParam{ svfFilter, defaultResonance, "Resonance" });
    addParameter(shelfGain = new ShelfGainParam{ svfFilter, defaultShelfGain, "Shelf Gain" });

    lastUIWidth = 500;
    lastUIHeight = 200;
}

VAStateVariableFilterProcessor::~VAStateVariableFilterProcessor()
{
}

//==============================================================================
const String VAStateVariableFilterProcessor::getInputChannelName (int channelIndex) const
{
    return String (channelIndex + 1);
}

const String VAStateVariableFilterProcessor::getOutputChannelName (int channelIndex) const
{
    return String (channelIndex + 1);
}

bool VAStateVariableFilterProcessor::isInputChannelStereoPair (int index) const
{
    return true;
}

bool VAStateVariableFilterProcessor::isOutputChannelStereoPair (int index) const
{
    return true;
}

bool VAStateVariableFilterProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool VAStateVariableFilterProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool VAStateVariableFilterProcessor::silenceInProducesSilenceOut() const
{
    return false;
}

double VAStateVariableFilterProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

//==============================================================================

void VAStateVariableFilterProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialization that you need..
    svfFilter.setSampleRate(static_cast<float>(sampleRate));
}

void VAStateVariableFilterProcessor::releaseResources()
{
}

void VAStateVariableFilterProcessor::processBlock(AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
    const int numSamples = buffer.getNumSamples();

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    for (int channel = 0; channel < getNumInputChannels(); ++channel) {

        float* channelData = buffer.getWritePointer(channel);

        // Process audio sample block through filter
        svfFilter.processAudioBlock(channelData, numSamples, channel);
    }

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    for (int i = getNumInputChannels(); i < getNumOutputChannels(); ++i)
        buffer.clear(i, 0, buffer.getNumSamples());
}

//==============================================================================
AudioProcessorEditor* VAStateVariableFilterProcessor::createEditor()
{
    return new VAStateVariableFilterProcessorEditor (*this);
}

//==============================================================================
void VAStateVariableFilterProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // Here's an example of how you can use XML to make it easy and more robust:

    // Create an outer XML element..
    XmlElement xml("MYPLUGINSETTINGS");

    // add some attributes to it..
    xml.setAttribute("uiWidth", lastUIWidth);
    xml.setAttribute("uiHeight", lastUIHeight);
    xml.setAttribute("filterType", filterType->getValue());
    xml.setAttribute("cutoff", cutoff->getValue());
    xml.setAttribute("resonance", resonance->getValue());
    xml.setAttribute("shelfGain", shelfGain->getValue());

    // then use this helper function to stuff it into the binary blob and return it..
    copyXmlToBinary(xml, destData);
}

void VAStateVariableFilterProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.

    // This getXmlFromBinary() helper function retrieves our XML from the binary blob..
    ScopedPointer<XmlElement> xmlState{ getXmlFromBinary(data, sizeInBytes) };

    if (xmlState != nullptr) {
        // make sure that it's actually our type of XML object..
        if (xmlState->hasTagName("MYPLUGINSETTINGS")) {
            // ok, now pull out our parameters..
            lastUIWidth = xmlState->getIntAttribute("uiWidth", lastUIWidth);
            lastUIHeight = xmlState->getIntAttribute("uiHeight", lastUIHeight);
            filterType->setValue((float)xmlState->getDoubleAttribute("filterType", filterType->getValue()));
            cutoff->setValue((float)xmlState->getDoubleAttribute("cutoff", cutoff->getValue()));
            resonance->setValue((float)xmlState->getDoubleAttribute("resonance", resonance->getValue()));
            shelfGain->setValue((float)xmlState->getDoubleAttribute("shelfGain", shelfGain->getValue()));
        }
    }
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new VAStateVariableFilterProcessor();
}
