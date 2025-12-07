# Building

This project can be built either for windows or for the DTEK board.

## Building for DTEK-board

To compile for the DTEK-board, you need the `riscv32-unknown-elf-gcc`
toolchain. Make sure to install it if you do not have it.

To compile the game for the DTEK-board, enter the `build-dtek` directory
and run `make`.

## Building for Windows 

This project contains a build system for cross compiling from linux (ubuntu)
to windows using mingw.

### Installing dependencies

*make sure that make and cmake are installed on your system.*

#### 1. Get SDL3
https://github.com/libsdl-org/SDL/releases
- download SDL3-devel-3.2.26-mingw.tar.gz
- put the `SDL3-x.x.x` folder in a folder called "Libraries"

#### 2. Install the mingw compiler
- run `sudo apt-get install gcc-mingw-w64` to install the mingw c compiler.
- run `sudo apt-get install g++-mingw-w64` to install the mingw c++ compiler.

#### 3. Generate makefiles using cmake
- enter the `build_windows` directory.
- run the command `chmod +x cmake.sh` here to make the cmake script executable.
- create a new folder within this one, in which the project will be build. Suitably, call it `build`.
- enter the newly created folder.
- run the cmake script to generate the makefiles: Enter the command `../cmake.sh`
- build the project by running `make`.
- the output binary will be called `out.exe`. If you want to move this to another folder and
  run it from there, make sure to also move the SDL3.dll file to that directory (the SDL binary)
  which is dynamically linked against at runtime.


# Used assets

The chess piece models were not created by us. They can for example be found here:
https://sketchfab.com/3d-models/knight-low-poly-431d71b3c88f4461bf7848d474b8367f

Credits:
"Knight - Low Poly" (https://skfb.ly/XHzT) by Chris is licensed under Creative Commons Attribution (http://creativecommons.org/licenses/by/4.0/).