![logo](https://github.com/idakandrew/logistruct/blob/main/data/logo.png?raw=true)

## Overview
LogiStruct is an in-development logic simulator designed around a pixel canvas system. It is written entirely in C, and relies on the Allegro 5 library for graphical and input features. 
## Version
Latest: v0.1
## Compilation
Compilation will require the Allegro 5 library to be installed locally.
To compile via GCC use the following:

gcc logistruct.c canvas.c canvas.h sim.c sim.h ui.c ui.h utils.c utils.h -o Logistruct -lallegro -lallegro_font -lallegro_ttf -lallegro_primitives -lallegro_image -mwindows -lm
