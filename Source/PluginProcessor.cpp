/*
  ==============================================================================

    JUCE plugin processor

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
    for (int i = 0; i < 16; ++i)
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
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool BasicSynth2AudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This checks if the layout is supported.
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

            // Gain
            auto g = apvts.getRawParameterValue("Gain")->load();

            voice->getOscillator().getGain().setGainLinear(g);

            // ADSR
            auto a = apvts.getRawParameterValue("Attack")->load();
            auto d = apvts.getRawParameterValue("Decay")->load();
            auto s = apvts.getRawParameterValue("Sustain")->load();
            auto r = apvts.getRawParameterValue("Release")->load();

            voice->updateADSR(voice->getADSR(), a, d, s, r);
           
            // FM
            auto freq = apvts.getRawParameterValue("FM Freq")->load();
            auto depth = apvts.getRawParameterValue("FM Depth")->load();

            voice->getOscillator().setFMParameters(freq, depth);

            // Filter
            int filterType = apvts.getRawParameterValue("Filter Type")->load();
            auto cutoff = apvts.getRawParameterValue("Filter Cutoff")->load();
            auto resonance = apvts.getRawParameterValue("Filter Reso")->load();

            voice->updateFilter(filterType, cutoff, resonance);

            // Filter ADSR
            auto fa = apvts.getRawParameterValue("Filter Attack")->load();
            auto fd = apvts.getRawParameterValue("Filter Decay")->load();
            auto fs = apvts.getRawParameterValue("Filter Sustain")->load();
            auto fr = apvts.getRawParameterValue("Filter Release")->load();

            voice->updateADSR(voice->getFilterADSR(), fa, fd, fs, fr);

        }
    }

    midiMessageCollector.removeNextBlockOfMessages(midiMessages, buffer.getNumSamples());
    keyboardState.processNextMidiBuffer(midiMessages, 0, buffer.getNumSamples(), true);
    synth.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());
}

//==============================================================================
bool BasicSynth2AudioProcessor::hasEditor() const
{
    return true;
}

juce::AudioProcessorEditor* BasicSynth2AudioProcessor::createEditor()
{
    return new BasicSynth2AudioProcessorEditor (*this);
}

//==============================================================================
void BasicSynth2AudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    juce::MemoryOutputStream mos(destData, true);
    apvts.state.writeToStream(mos);
}

void BasicSynth2AudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // saves apvts state after closing the program

    auto tree = juce::ValueTree::readFromData(data, sizeInBytes);
    if (tree.isValid())
    {
        apvts.replaceState(tree);
    }
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
    // Gain
    parameterLayout.add(std::make_unique<juce::AudioParameterFloat>("Gain",
        "Gain", juce::NormalisableRange<float>{0.0f, 1.0f}, 0.1f));
    // ADSR
    parameterLayout.add(std::make_unique<juce::AudioParameterFloat>("Attack",
        "Attack", juce::NormalisableRange<float>{0.1f, 1.0f}, 0.3f));
    parameterLayout.add(std::make_unique<juce::AudioParameterFloat>("Decay",
        "Decay", juce::NormalisableRange<float>{0.1f, 1.0f}, 0.9f));
    parameterLayout.add(std::make_unique<juce::AudioParameterFloat>("Sustain",
        "Sustain", juce::NormalisableRange<float>{0.1f, 1.0f}, 0.8f));
    parameterLayout.add(std::make_unique<juce::AudioParameterFloat>("Release",
        "Release", juce::NormalisableRange<float>{0.1f, 3.0f}, 0.5f));
    // FM
    parameterLayout.add(std::make_unique<juce::AudioParameterFloat>("FM Freq",
        "FM Freq", juce::NormalisableRange<float>{0.1f, 1000.0f}, 0.1f));
    parameterLayout.add(std::make_unique<juce::AudioParameterFloat>("FM Depth",
        "FM Depth", juce::NormalisableRange<float>{0.1f, 1000.0f}, 0.1f));
    //Filter
    parameterLayout.add(std::make_unique<juce::AudioParameterChoice>("Filter Type",
        "Filter Type", juce::StringArray{ "Low Pass", "Band Pass", "High Pass" }, 0));
    parameterLayout.add(std::make_unique<juce::AudioParameterFloat>("Filter Cutoff",
        "Filter Cutoff", juce::NormalisableRange<float>{20.0f, 20000.0f, 1.0f, 0.6f}, 440.0f));
    parameterLayout.add(std::make_unique<juce::AudioParameterFloat>("Filter Reso",
        "Filter Reso", juce::NormalisableRange<float>{0.1f, 10.0f, 0.1}, 5.0f));
    // Filter ADSR
    parameterLayout.add(std::make_unique<juce::AudioParameterFloat>("Filter Attack",
        "Filter Attack", juce::NormalisableRange<float>{0.1f, 1.0f}, 0.3f));
    parameterLayout.add(std::make_unique<juce::AudioParameterFloat>("Filter Decay",
        "Filter Decay", juce::NormalisableRange<float>{0.1f, 1.0f}, 0.9f));
    parameterLayout.add(std::make_unique<juce::AudioParameterFloat>("Filter Sustain",
        "Filter Sustain", juce::NormalisableRange<float>{0.1f, 1.0f}, 0.8f));
    parameterLayout.add(std::make_unique<juce::AudioParameterFloat>("Filter Release",
        "Filter Release", juce::NormalisableRange<float>{0.1f, 3.0f}, 0.5f));

    return parameterLayout;
}

void BasicSynth2AudioProcessor::logMessage(const juce::String& m)
{
    midiLog.moveCaretToEnd();
    midiLog.insertTextAtCaret(m + juce::newLine);
}

