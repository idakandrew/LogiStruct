#ifndef CANVAS
#define CANVAS

#include <allegro5/allegro_font.h>

#define white al_map_rgb(200, 200, 200)
#define lightgrey al_map_rgb(100, 100, 100)
#define mediumgrey al_map_rgb(30, 30, 30)
#define bgcolor al_map_rgb(45, 45, 45)
#define nearblack al_map_rgb(15, 15, 15)
#define black al_map_rgb(0, 0, 0)
#define redblack al_map_rgb(80, 0, 0)
#define red al_map_rgb(170, 30, 30)
#define gold al_map_rgb(150, 150, 0)
#define green al_map_rgb(0, 180, 80)
#define blue al_map_rgb(0, 80, 180)

#define MAP_X 300
#define MAP_Y 300
#define VIEW_X 96
#define VIEW_Y 50

typedef enum component {
    empty, lowire, hiwire, lopinin, hipinin, lopinout, hipinout, aboard, oboard, 
    nand, nor, lolight, hilight, loflip, hiflip, cross
} comp;


bool mtrx_range(int x, int y, int xmin, int xmax, int ymin, int ymax);

int r_lim(int min, int val, int max);

void draw_map(bool grid, int map[MAP_X][MAP_Y], int cx, int cy, ALLEGRO_FONT *font);

void place_chip(int x, int y, comp chip, int map[MAP_X][MAP_Y]);

void remove_chip(int map[MAP_X][MAP_Y], int x, int y);

int flip_switch(int map[MAP_X][MAP_Y], int x, int y, int mode);

void lock_coords(int *lock, int *lx, int *ly, ALLEGRO_MOUSE_STATE state);

void lock_handler(int *lock, int lx, int ly, int *x, int *y, int *dirx, int *diry);

void click_handler(int map[MAP_X][MAP_Y], ALLEGRO_MOUSE_STATE state, int x, int y, int select, int *wait);

#endif