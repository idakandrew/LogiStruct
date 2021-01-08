![logo](https://github.com/idakandrew/logistruct/blob/main/data/logo.png?raw=true)

## Description
Repository for the in-development circuit/logic gate simulator LogiStruct, built in C with Allegro 5.
## Version
Latest: v0.1
## Compilation
Compilation will (for the time being, while library file packaging is sorted out) require the Allegro 5 library to be installed locally.
To compile via GCC use the following:

gcc logistruct.c canvas.c canvas.h sim.c sim.h ui.c ui.h utils.c utils.h -o Logistruct -lallegro -lallegro_font -lallegro_ttf -lallegro_primitives -lallegro_image -mwindows -lm
