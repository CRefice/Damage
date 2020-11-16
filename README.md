# Damage

Yet another GameBoy emulator for Windows.

Damage features some unusual features for a "toy" emulator:
* Full, accurate audio emulation, streaming through XAudio2
* Remappable controls (keyboard and XInput gamepads)
* Integer pixel scaling

The emulation itself is fully accurate, capable of emulating most (I'd  say all, but I haven't tested all of them :))
of the GameBoy library of games.

## Screenshots
<p float="left">
  <img src="/screenshots/mario-land-2.png" width="400">
  <img src="/screenshots/tetris.png" width="400">
</p>

## Building

You'll need Microsoft Visual C++ to compile the project, and CMake to generate the build files.
Visual Studio 2019 allows you to open CMake projects directly from the IDE, so it's a good choice.
Compilation has been tested on Visual Studio 2019 version 16.8.1. I can't guarantee it will build on a later
version, but feel free to open a pull request if that's not the case.

## Configuration
The default control mapping uses the keyboard:
* **A:** X
* **B:** C
* **Start:** Enter
* **Select:** Backspace
* **D-Pad:** Arrow Keys

To change the control map or use a XInput controller, you'll need to create the file `Damage.ini`
in the same directory as the built executable. An example configuration file to map controls
to a controller is provided [in this repository.](/Damage.ini) Note that the .ini file is sensitive to both case
and whitespace, and the control names follow the XBox360 button names.
