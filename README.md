# HotColdGame – C++ Number Guessing Game
A simple CLI game written in C++11.  Player guesses the number and receives a feedback (cold, hot, hotter) based on a configurable [section.cfg](section.cfg) file (details and how-to inside the file).

This project has been made purely for fun and practice and is probably slightly overengineered for the basic gameplay it provides.

## Build

### Using CMake Presets 
Creates a /build folder with .exe inside and copies the [sections.cfg](section.cfg).
This will also run unit tests during build.
```bash
cmake --preset default
cmake --build build --preset buildAndTest
build\HotColdGame.exe
```
### Using g++
```bash
g++ main.cpp src/Section.cpp src/Guess.cpp src/Game.cpp src/ConfigLoader.cpp -Iinclude -std=c++11 -o HotColdGame
HotColdGame.exe
```
## License
This project is open source under the [MIT License](LICENSE).
