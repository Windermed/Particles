# Particles

a particle system and bullet hell game that was built using C++ and SFML 3.1.
The bullet hell section is heavily inspired by Undertale/Deltarune

<img src="media/particles_preview01.gif">

# Features
* a Particle system with performance optimizations and configurable gravity (between zero gravity and gravity)
* a Bullet hell mode which contains a spiral, rain, and 1 full-on attack pattern.
* a migrated codebase from SFML 2.5.1 to SFML 3.1
* a per-character rainbow text that uses SFML 3.1's setGlyphPreProcessor.
* a randomized splash screen text that is loaded from an external text file.
* a NEW build system that utilizes CMake and automatically fetches the SFML dependency.
	
# Compiling The Project
> **Note:** First build will download and compile SFML 3.1 alongside
> its dependencies which can be a minimum of (~2GB). Subsequent builds are significantly faster.

## Build Instructions

### Requirements
* Cmake 3.28+
* Visual Studio 2022 (for Windows) or GCC/Clang with C++17 Support.

### Windows
Start by running `build.bat` and open `build/Particles.sln` using Visual Studio 2022.

### Mac/Linux
```bash
# give the build script permission to execute
chmod +x build.sh

# now run the build script
./build.sh
```

## Legacy
Previously, this project was made using SFML 2.5.1. It has since been upgraded to 3.1 and you
can go ahead and check it out by switching over to the `legacy/sfml-2.5.1` branch.

## Credits
If you use or reference this project please provide credit. that's all I ask.

## License
MIT License. see the LICENSE file for more info.