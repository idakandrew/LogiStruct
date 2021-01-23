![logo](https://github.com/idakandrew/logistruct/blob/main/data/logo.png?raw=true)

## Overview
LogiStruct is an in-development logic simulator designed around a pixel canvas system. It is written entirely in C, and relies on the Allegro 5 library for graphical and input features. Screenshots below.
## Version
Latest: v0.1 (Jan 19, 2021)
## Installation
### Compiled Downloads
Itch.io page: https://aleainfinitus.itch.io/logistruct

Github releases: https://github.com/idakandrew/LogiStruct/releases

### Compilation
1. Download and install Allegro version 5.2.6.0 from https://github.com/liballeg/allegro5/releases. 
2. Ensure that Allegro library files are on your include path. 
3. Clone this repository locally. 
4. Create /bin/ in /Logistruct/.
5. Navigate to /data/ & compile the icon using:
```
windres icon.rc -O coff -o icon.res
```
6. Navigate to /src/ & compile via GCC using: 
```
gcc logistruct.c canvas.c canvas.h sim.c sim.h ui.c ui.h utils.c utils.h ../data/icon.res -o ../bin/Logistruct -lallegro -lallegro_font -lallegro_ttf -lallegro_primitives -lallegro_image -mwindows -lm
```
7. Copy the following .dlls from your Allegro / MinGW install to /bin/ (optional):
```
allegro_font-5.2.dll
allegro_image-5.2.dll
allegro_primitives-5.2.dll
allegro_ttf-5.2.dll
allegro-5.2.dll
libgcc_s_seh-1.dll
libstdc++-6.dll
libwinpthread-1.dll
```
## TODO
- Middle click to select part on canvas.
- Select and save canvas region as chip.
- Redo UI to handle more chip types.
- Increase canvas size & add zoom / pan features.
## Screenshots
![menu](https://github.com/idakandrew/logistruct/blob/main/media/menuscreen.png?raw=true)

![screen](https://github.com/idakandrew/logistruct/blob/main/media/screenshot.png?raw=true)
![screen2](https://github.com/idakandrew/logistruct/blob/main/media/screenshot2.png?raw=true)
