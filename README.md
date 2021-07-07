# SynthPlugin
A Juce Synth Plugin; will be outfitted with new features as a learn them.
It can be used with external MIDI input devices, the computer keyboard (middle and upper letter row) and mouse.

## Dependencies
* [Clone the JUCE Repo](https://github.com/juce-framework/JUCE)
* Build the Projucer: Use the appropriate build file in `JUCE/extras/Projucer/Builds/`

## Build & Run
* Clone this Repo
* Open `BasicSynth2.jucer` with the projucer, select the appropriate exporter (use the `+` sign in the bottom left corner to add a new one if necessary)
* Press the exporter's symbol next to the selector to create a solution for the chosen IDE
* Build that solution
* Run the standalone build or add the VST3 build to your VST3 folder to use it in your DAW

## Code Structure
The two classes given by the framework, PluginProcessor and PluginEditor, deal with Audio Processing and UI respectively. 

Plugin Processor has a juce::Synthesiser member that holds 16 instances of SynthVoice, each of which producing audio via its own oscillator (w/ switchable waveforms) and processing its manipulations via Frequency Modulation and a Filter section. The synthesiser also requires an instance of SynthSound to work properly.
Plugin Processor also holds an instance of juce::AudioProcessorValueTreeState which works as an interface between the UI and the Audio Processing. The functions updating the different parameters are organized in classes in the Data folder, except for Gain, which is directly accessed via SynthVoice.

PluginEditor holds instances of all the UI elements, which, with the exception of the on-scree-keyboard, which requires very little setup compared to the others, are organized in classes in the UI folder.
