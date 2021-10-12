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

## Installing dependencies, building & running on Linux
* Clone this Repo
* [Clone the JUCE Repo](https://github.com/juce-framework/JUCE)
* Build the Projucer (JUCE' project management tool): Use the makefile in `JUCE/extras/Projucer/Builds/LinuxMakefile`
* Install dependencies:
  * run `sudo apt-get install -y g++ libgtk-3-dev libfreetype6-dev libx11-dev libxinerama-dev libxrandr-dev libxcursor-dev mesa-common-dev libasound2-dev freeglut3-dev libxcomposite-dev libcurl4-openssl-dev`

  * and `sudo add-apt-repository -y ppa:webkit-team/ppa && sudo apt-get install libwebkit2gtk-4.0-37 libwebkit2gtk-4.0-dev`
* Run the Projucer; the executable is in `/JUCE/extras/Projucer/Builds/LinuxMakefile/build`
* within it, open the `BasicSynth2.jucer` file in this Repo (File -> Open...)
* Select Linux Makefile as the exporter (use the `+` sign in the bottom left corner)
* Save the project (`File -> Save Project`) to create a makefile
* Use the makefile in `/SynthPlugin/Builds/LinuxMakefile` to build the project
* Run; the executable is `BasicSynth2` in `/SynthPlugin/Builds/LinuxMakefile/build/`
