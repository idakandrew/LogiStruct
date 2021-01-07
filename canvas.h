#ifndef CANVAS
#define CANVAS

#include <allegro5/allegro_font.h>

typedef enum component {empty, lowire, hiwire, lopinin, hipinin, lopinout, hipinout, aboard, nboard, and, not, lolight, hilight, loflip, hiflip} comp;

int range(int min, int val, int max);

void draw_map(bool grid, int map[96][50], ALLEGRO_FONT *font);

void place_chip(int x, int y, comp chip, int map[96][50]);

void remove_chip(int map[96][50], int x, int y);

int flip_switch(int map[96][50], int x, int y, int mode);

void lock_coords(int *lock, int *lx, int *ly, ALLEGRO_MOUSE_STATE state);

void lock_handler(int *lock, int lx, int ly, int *x, int *y, int *dirx, int *diry);

#endif