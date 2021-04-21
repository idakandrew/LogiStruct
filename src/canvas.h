#ifndef CANVAS
#define CANVAS

#include <allegro5/allegro_font.h>

#define white al_map_rgb(200, 200, 200)
#define lightgrey al_map_rgb(85, 85, 85)
#define mediumgrey al_map_rgb(30, 30, 30)
#define bgcolor al_map_rgb(45, 45, 45)
#define nearblack al_map_rgb(15, 15, 15)
#define black al_map_rgb(0, 0, 0)
#define redblack al_map_rgb(120, 0, 0)
#define red al_map_rgb(170, 30, 30)
#define gold al_map_rgb(150, 150, 0)
#define green al_map_rgb(0, 145, 45)
#define blue al_map_rgb(0, 80, 180)
#define purp al_map_rgb(70, 35, 120)
#define orange al_map_rgb(220, 80, 0)

#define MAP_X 1000
#define MAP_Y 1000
#define VIEW_X 96
#define VIEW_Y 50

typedef enum component {
    empty, lowire, hiwire, lopinin, hipinin, lopinout, hipinout, aboard, oboard, 
    nand, nor, nandrot, norrot, lolight, hilight, loflip, hiflip, cross, lobridge1, hibridge1, 
    lobridge2, hibridge2, seg, segboard, onewayh, onewayv, onerot
} comp;


bool mtrx_range(int x, int y, int xmin, int xmax, int ymin, int ymax);

int r_lim(int min, int val, int max);

int zm_adj(int mode, int zm);

void draw_map(int zm, bool grid, int map[MAP_X][MAP_Y], int cx, int cy, ALLEGRO_FONT *font);

void place_chip(int x, int y, comp chip, int map[MAP_X][MAP_Y], int rot);

void remove_chip(int map[MAP_X][MAP_Y], int x, int y);

int flip_switch(int map[MAP_X][MAP_Y], int x, int y, int mode);

void lock_axis(int zm, int *lock, int *x, int *y, int lx, int ly);

void click_handler(int map[MAP_X][MAP_Y], ALLEGRO_MOUSE_STATE state, int x, int y, int select, int *wait, bool pen, int rot);

int part_picker(int map[MAP_X][MAP_Y], ALLEGRO_MOUSE_STATE state, int cx, int cy, int zm);

void region_delete(int map[MAP_X][MAP_Y], int ox, int oy, int ex, int ey, bool ask);

int *region_copy(int map[MAP_X][MAP_Y], int ox, int oy, int ex, int ey, int *boxsx, int *boxsy);

void region_paste(int map[MAP_X][MAP_Y], int *box, int boxsx, int boxsy, int x, int y);

#endif