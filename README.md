# SynthPlugin
A JUCE Synth Plugin; will be outfitted with new features as I learn them.
It can be used with external MIDI input devices, the computer keyboard (middle and upper letter row) and mouse.

## Installing dependencies, building & running on Windows and macOS
* Clone this Repo
* [Clone the JUCE Repo](https://github.com/juce-framework/JUCE)
* Build the Projucer (JUCE' project management tool): Use the appropriate solution in `JUCE/extras/Projucer/Builds/...`
* Run the Projucer; within it, open the `BasicSynth2.jucer` file in this Repo (File -> Open...)
* Select the appropriate exporter (use the `+` sign in the bottom left corner to add a new one if necessary)
* Press the exporter's symbol next to the selector (at the top of the window) to create a solution for the chosen IDE
* Build that solution
* Run the standalone build or add the VST3 build to your VST3 folder to use it in your DAW
