/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "SynthVoice.h"
#include "SynthSound.h"

//==============================================================================
BasicSynth2AudioProcessor::BasicSynth2AudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ), apvts(*this, nullptr, "Parameters", createParameterLayout())
#endif
{
    synth.addVoice(new SynthVoice());
    synth.addSound(new SynthSound());
}

BasicSynth2AudioProcessor::~BasicSynth2AudioProcessor()
{
}

//==============================================================================
const juce::String BasicSynth2AudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool BasicSynth2AudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool BasicSynth2AudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool BasicSynth2AudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double BasicSynth2AudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int BasicSynth2AudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int BasicSynth2AudioProcessor::getCurrentProgram()
{
    return 0;
}

void BasicSynth2AudioProcessor::setCurrentProgram (int index)
{
}

const juce::String BasicSynth2AudioProcessor::getProgramName (int index)
{
    return {};
}

void BasicSynth2AudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void BasicSynth2AudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    synth.setCurrentPlaybackSampleRate(sampleRate);
    for (size_t i = 0; i < synth.getNumVoices(); ++i)
    {
        if (auto* voice = dynamic_cast<SynthVoice*>(synth.getVoice(i)))
            voice->prepareToPlay(sampleRate, samplesPerBlock, getNumOutputChannels());
    }

    midiMessageCollector.reset(sampleRate);
}

void BasicSynth2AudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool BasicSynth2AudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void BasicSynth2AudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    for (size_t i = 0; i < synth.getNumVoices(); ++i)
    {
        if (auto* voice = dynamic_cast<SynthVoice*>(synth.getVoice(i)))
        {
            // Waveform
            auto waveform = apvts.getRawParameterValue("Waveform")->load();

            voice->getOscillator().setWaveform(int(waveform));

            // ADSR
            auto a = apvts.getRawParameterValue("Attack")->load();
            auto d = apvts.getRawParameterValue("Decay")->load();
            auto s = apvts.getRawParameterValue("Sustain")->load();
            auto r = apvts.getRawParameterValue("Release")->load();

            voice->update(a, d, s, r);
           
            // LFO
        }
    }

    midiMessageCollector.removeNextBlockOfMessages(midiMessages, buffer.getNumSamples());
    keyboardState.processNextMidiBuffer(midiMessages, 0, buffer.getNumSamples(), true);
    synth.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());
}

//==============================================================================
bool BasicSynth2AudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* BasicSynth2AudioProcessor::createEditor()
{
    return new BasicSynth2AudioProcessorEditor (*this);
}

//==============================================================================
void BasicSynth2AudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void BasicSynth2AudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new BasicSynth2AudioProcessor();
}

//==============================================================================
juce::AudioProcessorValueTreeState::ParameterLayout BasicSynth2AudioProcessor::createParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout parameterLayout;

    // Waveform Combobox
    parameterLayout.add(std::make_unique<juce::AudioParameterChoice>("Waveform", 
        "Waveform", juce::StringArray{ "Sine", "Sawtooth", "Triangle", "Square" }, 0));
    // ADSR
    parameterLayout.add(std::make_unique<juce::AudioParameterFloat>("Attack",
        "Attack", juce::NormalisableRange<float>{0.1f, 1.0f}, 0.1f));
    parameterLayout.add(std::make_unique<juce::AudioParameterFloat>("Decay",
        "Decay", juce::NormalisableRange<float>{0.1f, 1.0f}, 0.1f));
    parameterLayout.add(std::make_unique<juce::AudioParameterFloat>("Sustain",
        "Sustain", juce::NormalisableRange<float>{0.1f, 1.0f}, 1.0f));
    parameterLayout.add(std::make_unique<juce::AudioParameterFloat>("Release",
        "Release", juce::NormalisableRange<float>{0.1f, 3.0f}, 0.1f));

    return parameterLayout;
}

void BasicSynth2AudioProcessor::logMessage(const juce::String& m)
{
    midiLog.moveCaretToEnd();
    midiLog.insertTextAtCaret(m + juce::newLine);
}

