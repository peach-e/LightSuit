# Arduino Light Suit

The Arduino Light Suit project combines hardware, software, and sewing to create a wearable vest with blinking lights that respond to ambient sound. The completed system filters audio received from a microphone and modulates the brightness of the red, green and blue channels of the light strips to beat along with the detected music.

## Technology Description
The system receives the audio signal by listening through an input microphone and filtering the signal into seven frequency bins. The audio filtering is performed by an off-the-shelf chip in order to save the Arduino the trouble of continuously performing a Fourier transform.

The Arduino sorts through the frequency bins, allocating them to red, green and blue channels, and performs normalization and balancing so that the lights provide an appealing display in different volumes of sound and in silence.

The hardware is designed to be modular. The circuit can be modified to accommodate different kinds of lights, as long as they have individually adjustable RGB channels, and different sources of audio can be delivered to the input.

For more detailed designs about circuit design, part list and construction, refer to the design PDF.


## Installation

The code has been written in such a way that it's possible to deploy and test most of the code without requiring an Arduino or configured hardware. That makes it easy for debugging when preparing the Arduino libraries.

The software was developed using Ubuntu Linux and the Eclipse CDT. These instructions assume that the reader is using a similar environment, however it should be possible to deploy on a Windows system but with some minor changes.

### Prerequisites

For deployment to Arduino:
* A recent version of the Arduino IDE

For development on Eclipse:
* Recommended: Debian-based Linux Operating System
* Recommended: Python3, iPython and SciPy
* A working deployment of the Eclipse CDT (https://www.eclipse.org/cdt/)
* A Recent version of the Arduino IDE (https://www.arduino.cc/en/Main/Software)

### Deployment to Arduino

It is assumed that you have a working Arduino that you can already communicate with and upload basic examples.
The Arduino IDE specifies a default directory where all the main sketches are stored. Inside this directory, there is a subdirectory called ‘libraries’, which is where all the libraries need to be stored. You must either copy the Light Suit libraries into the Arduino/libraries directory, or create symbolic links to point to the true project directory.

1. Unzip the source code into a development folder.
```bash
~$ ls LightSuit
analysis  README.md  src
```
2. In the Arduino sketchbook location, enter or create the directory "libraries".
3. Create a symbolic link to each library folder.
```bash
~/Arduino/libraries$ ln -s ../../LightSuit/src/cpp/MachineState MachineState
~/Arduino/libraries$ ln -s ../../LightSuit/src/cpp/LevelAnalyzer LevelAnalyzer
~/Arduino/libraries$ ln -s ../../LightSuit/src/cpp/Constants Constants
```
4. Restart the Arduino IDE, and then open the main Arduino sketch, 'LightSuit.ino', in the Lightsuit project.
5. Try verifying the sketch.
   * The sketch should correctly locate MachineState.h, FadeHSV.h, MusicPulse.h and Constants.h if the libraries directory has been correctly configured.
6. Flash the sketch to the Arduino.
   * Note: You may need to double-check some of the pin assignments, in Constants.h, depending on your circuit setup.

### Deployment to Eclipse

Please wait until I've tried it before I try to explain how...
