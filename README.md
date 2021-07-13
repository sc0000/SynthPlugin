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

## Code Structure
The two classes given by the framework, PluginProcessor and PluginEditor, deal with Audio Processing and UI respectively. 

PluginProcessor has a juce::Synthesiser member that holds 16 instances of SynthVoice, each of which producing audio via its own 2 oscillators (w/ switchable waveforms) and processing its manipulations via Frequency Modulation and a Filter section. The synthesiser also requires an instance of SynthSound to work properly.
Plugin Processor also holds an instance of juce::AudioProcessorValueTreeState which works as an interface between the UI and the Audio Processing. The functions updating the different parameters are organized in classes in the Data folder.

PluginEditor holds instances of all the UI elements, which, with the exception of the on-screen-keyboard, which requires very little setup compared to the others, are organized in classes in the UI folder.

## Rubric

__README (All Rubric Points REQUIRED)__

|DONE | CRITERIA | MEETS SPECIFICATIONS| WHERE |
|-- | -- | --| -- |
| :heavy_check_mark: | A README with instructions is included with the project |The README is included with the project and has instructions for building/running the project. If any additional libraries are needed to run the project, these are indicated with cross-platform installation instructions. You can submit your writeup as markdown or pdf.| |
| :heavy_check_mark: | The README indicates which project is chosen. | The README describes the project you have built. The README also indicates the file and class structure, along with the expected behavior or output of the program. | |
| :heavy_check_mark: | The README includes information about each rubric point addressed. | The README indicates which rubric points are addressed. The README also indicates where in the code (i.e. files and line numbers) that the rubric points are addressed. | |

__Compiling and Testing (All Rubric Points REQUIRED)__

|DONE | CRITERIA | MEETS SPECIFICATIONS| WHERE |
|-- | -- | --| -- |
| :heavy_check_mark: | The submission must compile and run. | The project code must compile and run without errors. We strongly recommend using cmake and make, as provided in the starter repos. If you choose another build system, the code must compile on any reviewer platform. |

__Loops, Functions, I/O__

|DONE | CRITERIA | MEETS SPECIFICATIONS| WHERE |
|-- | -- | --| -- |
| :heavy_check_mark: | The project demonstrates an understanding of C++ functions and control structures.| A variety of control structures are used in the project. The project code is clearly organized into functions.| Everywhere |
|  | The project reads data from a file and process the data, or the program writes data to a file. | The project reads data from an external file or writes data to a file as part of the necessary operation of the program.|  |
| :heavy_check_mark: | The project accepts user input and processes the input.|The project accepts input from a user as part of the necessary operation of the program.|  UI components can be used to change audio parameters. MIDI messages from external device or on-screen keyboard are routed to synth in PluginProcessor::processBlock and logged in the Message Box |

__Object Oriented Programming__

|DONE | CRITERIA | MEETS SPECIFICATIONS| WHERE |
|-- | -- | --| -- |
| :heavy_check_mark: | The project uses Object Oriented Programming techniques. | The project code is organized into classes with class attributes to hold the data, and class methods to perform tasks. | All *.cpp and *.h files |
| :heavy_check_mark: | Classes use appropriate access specifiers for class members. | All class data members are explicitly specified as public, protected, or private.| All *.cpp and *.h files |
| :heavy_check_mark: | Class constructors utilize member initialization lists. | All class members that are set to argument values are initialized through member initialization lists.| PluginEditor and all Component classes |
| :heavy_check_mark: | Classes abstract implementation details from their interfaces. | All class member functions document their effects, either through function names, comments, or formal documentation. Member functions do not change program state in undocumented ways.| All *.cpp and *.h files; however I did not document basic framework functionality |
| :heavy_check_mark: | Classes encapsulate behavior. | Appropriate data and functions are grouped into classes. Member data that is subject to an invariant is hidden from the user. State is accessed via member functions.| All *.cpp and *.h files |
| :heavy_check_mark: | Classes follow an appropriate inheritance hierarchy. | Inheritance hierarchies are logical. Composition is used instead of inheritance when appropriate. Abstract classes are composed of pure virtual functions. Override functions are specified.| All classes except FilterData inherit from framework classes and feature many overridden functions  |
|  | Overloaded functions allow the same function to operate on different parameters. |  |
| :heavy_check_mark: | Derived class functions override virtual base class functions. |One member function in an inherited class overrides a virtual base class member function.| All classes except FilterData inherit from framework classes and feature many overridden functions|
|  | Templates generalize functions in the project. | One function is declared with a template that allows it to accept a generic parameter.| |

__Memory Management__

|DONE | CRITERIA | MEETS SPECIFICATIONS| WHERE |
|-- | -- | --| -- |
| :heavy_check_mark: | The project makes use of references in function declarations. | At least two variables are defined as references, or two functions use pass-by-reference in the project code.| Everywhere in the project, especially UI component constructors and all the getters in SynthVoice |
|  | The project uses destructors appropriately. | At least one class that uses unmanaged dynamically allocated memory, along with any class that otherwise needs to modify state upon the termination of an object, uses a destructor. | (Most memory management is done by the framework) |
|  | The project uses scope / Resource Acquisition Is Initialization (RAII) where appropriate. | The project follows the Resource Acquisition Is Initialization pattern where appropriate, by allocating objects at compile-time, initializing objects when they are declared, and utilizing scope to ensure their automatic destruction.| |
| | The project follows the Rule of 5. | For all classes, if any one of the copy constructor, copy assignment operator, move constructor, move assignment operator, and destructor are defined, then all of these functions are defined.|  |
|  | The project uses move semantics to move data, instead of copying it, where possible. | For classes with move constructors, the project returns objects of that class by value, and relies on the move constructor, instead of copying the object. |  |
| :heavy_check_mark: | The project uses smart pointers instead of raw pointers. | The project uses at least one smart pointer: unique_ptr, shared_ptr, or weak_ptr. The project does not use raw pointers.| The Parameter Layout of the AudioProcessorValueTreeState is put together using unique pointers (PluginProcessor.cpp); The project does use raw pointers when they're managed by the framework afterwards (when adding Voices and Sound to the Synthesier, PluginProcessor.cpp)  |

__Concurrency__

|DONE | CRITERIA | MEETS SPECIFICATIONS| WHERE |
|-- | -- | --| -- |
| | The project uses multithreading. | The project uses multiple threads in the execution.| (It does, but its organized by the framework) |
|  | A promise and future is used in the project. | A promise and future is used to pass data from a worker thread to a parent thread in the project code.| |
|  | A mutex or lock is used in the project. | A mutex or lock (e.g. std::lock_guard or `std::unique_lock) is used to protect data that is shared across multiple threads in the project code.| |
|  | A condition variable is used in the project. | A std::condition_variable is used in the project code to synchronize thread execution.| |

## Reference
