#ifndef CANVAS
#define CANVAS

#include <allegro5/allegro_font.h>

typedef enum component {empty, lowire, hiwire, source, pin, fboard, aboard, nboard, flip, and, not} comp;

int range(int min, int val, int max);

void draw_map(bool grid, int map[96][50], ALLEGRO_FONT *font);

void place_chip(int x, int y, comp chip, int map[96][50], int mode);

void lock_coords(int *lock, int *lx, int *ly, ALLEGRO_MOUSE_STATE state);

void lock_handler(int *lock, int lx, int ly, int *x, int *y, int *dirx, int *diry);

#endif