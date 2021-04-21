#ifndef UTILS
#define UTILS

#include "canvas.h"

void fix_dir(void);

void save_canvas(int map[MAP_X][MAP_Y], int *posx, int *posy);

void load_canvas(int map[MAP_X][MAP_Y], int *posx, int *posy);

#endif