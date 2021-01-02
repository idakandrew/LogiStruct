#include <math.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include "canvas.h"

int range(int min, int val, int max) {
    if (val < min) {return min;}
    else if (val > max) {return max;}
    else {return val;}
} 

void draw_map(bool grid, int map[96][50], ALLEGRO_FONT *font) {
    int xs[192];
    int ys[192];
    int saved = 0;

    al_draw_filled_rectangle(0, 1000, 1920, 1080, al_map_rgb(15, 15, 15));

    for(int i = 0; i < 96; i++) {
        for(int j = 0; j < 50; j++) {
            if(grid) {
                al_draw_line(i*20, j*20, i*20+20, j*20, al_map_rgb(15, 15, 15), 1);
                al_draw_line(i*20, j*20, i*20, j*20+20, al_map_rgb(15, 15, 15), 1);
            }
            
            if(map[i][j] == lowire) {
                al_draw_filled_rectangle(i*20, j*20, i*20+20, j*20+20, al_map_rgb(30, 30, 30));
            } else if(map[i][j] == and || map[i][j] == aboard) {
                al_draw_filled_rectangle(i*20, j*20, i*20+20, j*20+20, al_map_rgb(0, 180, 80));
                if(map[i][j] == and) {xs[saved] = i, ys[saved] = j, saved++;}
            } else if(map[i][j] == not || map[i][j] == nboard) {
                al_draw_filled_rectangle(i*20, j*20, i*20+20, j*20+20, al_map_rgb(0, 80, 180));
                if(map[i][j] == not) {xs[saved] = i, ys[saved] = j, saved++;}
            } else if(map[i][j] == pin) {
                al_draw_filled_rectangle(i*20, j*20, i*20+20, j*20+20, al_map_rgb(0, 0, 0));
            }
        }
    }

    for(int i = 0; i < saved; i++) {
        if(map[xs[i]][ys[i]] == and) {
            al_draw_text(font, al_map_rgb(200, 200, 200), xs[i]*20+10, ys[i]*20+10 - al_get_font_line_height(font) / 2, ALLEGRO_ALIGN_CENTRE, "AND");
        } else if(map[xs[i]][ys[i]] == not) {
            al_draw_text(font, al_map_rgb(200, 200, 200), xs[i]*20+10, ys[i]*20+10 - al_get_font_line_height(font) / 2, ALLEGRO_ALIGN_CENTRE, "NOT");
        }
    }
}

void place_chip(int x, int y, comp chip, int map[96][50], int mode) {
    if(chip == and) {
        x = range(2, x, 93);
        y = range(2, y, 47);
        for(int i = x - 3; i < x + 4; i++) {
            for(int j = y - 3; j < y + 4; j++) {
                if (map[range(0, i, 95)][range(0, j, 49)] != empty && mode) {
                    goto trip;
                }
            }
        }
        for(int i = x - 2; i < x + 3; i++) {
            for(int j = y - 2; j < y + 3; j++) {
                map[i][j] = aboard * mode;
            }
        }
        map[x][y] = and * mode;
        map[x - 2][y + 1] = map[x - 2][y - 1] = map[x + 2][y] = pin * mode;
    } else if(chip == not) {
        x = range(2, x, 93);
        y = range(1, y, 48);
        for(int i = x - 3; i < x + 4; i++) {
            for(int j = y - 2; j < y + 3; j++) {
                if(map[range(0, i, 95)][range(0, j, 49)] != empty && mode) {
                    goto trip;
                }
            }
        }
        for(int i = x - 2; i < x + 3; i++) {
            for(int j = y - 1; j < y + 2; j++) {
                map[i][j] = nboard * mode;
            }
        }
        map[x][y] = not * mode;
        map[x - 2][y] = map[x + 2][y] = pin * mode;
    }
    trip:
        return;
}

void lock_coords(int *lock, int *lx, int *ly, ALLEGRO_MOUSE_STATE state) {
    *lock = 2;
    *lx = floor(state.x / 20); 
    *ly = floor(state.y / 20);
}

void lock_handler(int *lock, int lx, int ly, int *x, int *y, int *dirx, int *diry) {
    if(*lock == 2) {
        *dirx = abs(lx - *x);
        *diry = abs(ly - *y);
        *lock -= floor(*dirx);
        *lock -= floor(*diry);
    }
    if(*dirx == 1) {
        *y = ly;
    } else if(*diry == 1) {
        *x = lx;
    }
}