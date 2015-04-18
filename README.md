# citrace-player

A 3DS homebrew utility program to play back CiTrace files. CiTrace files are essentially logs of GPU commands sent to the PICA200 GPU used in the Nintendo 3DS. Such logs can be recorded by the emulator Citra, and being able to play them back is a powerful tool for debugging.

## Build Instructions

Install devkitARM and ctrulib, then type `make` in the project root folder.

## Usage

** This program does not run on an actual 3DS console, yet **

Copy the CiTrace file to be played back into the root folder of Citra's emulated SD card and name it `citrace.ctf`. Then run the compiled citrace-player.3dsx file with Citra. The application will listen for incoming connections on port 11113, so you can retrieve output for example via `netcat 127.0.0.1 11113`.
