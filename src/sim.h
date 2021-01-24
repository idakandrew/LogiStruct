#ifndef SIM
#define SIM

#include "canvas.h"

void chip_sim(int map[MAP_X][MAP_Y]);

void wire_sim(int map[MAP_X][MAP_Y], int x, int y, int mode);

#endif