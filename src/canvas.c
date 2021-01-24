#include <math.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_ttf.h>
#include "canvas.h"

bool mtrx_range(int x, int y, int xmin, int xmax, int ymin, int ymax) {
    if(x >= xmin && x <= xmax && y >= ymin && y <= ymax) {
        return true;
    }
    return false;
}

int r_lim(int min, int val, int max) {
    if (val < min) {return min;}
    else if (val > max) {return max;}
    else {return val;}
} 

// figure out how to only draw visible area, possibly multithread
void draw_map(bool grid, int map[MAP_X][MAP_Y], int cx, int cy, ALLEGRO_FONT *font) {
    ALLEGRO_COLOR colormap[18] = {
        bgcolor, mediumgrey, red, black, redblack, black, redblack, green,
        blue, green, blue, lightgrey, white, black, redblack, gold
    };
    int xs[200];
    int ys[200];
    int saved = 0;
    int absx = abs(cx);
    int absy = abs(cy);

    for(int i = absx; i < absx + VIEW_X; i++) {
        for(int j = absy; j < absy + VIEW_Y; j++) {
            if(grid) {
                al_draw_line((i-absx)*20, (j-absy)*20, (i-absx)*20+20, (j-absy)*20, nearblack, 1);
                al_draw_line((i-absx)*20, (j-absy)*20, (i-absx)*20, (j-absy)*20+20, nearblack, 1);
            }

            if(map[i][j] != empty) {
                al_draw_filled_rectangle((i+cx)*20, (j+cy)*20, (i+cx)*20+20, (j+cy)*20+20, colormap[map[i][j]]);
                if(map[i][j] == nand || map[i][j] == nor) {
                    xs[saved] = i, ys[saved] = j, saved++;
                }
            }
        }
    }

    for(int i = 0; i < saved; i++) {
        if(mtrx_range(xs[i], ys[i], absx, absx + VIEW_X, absy, absy + VIEW_Y)) {
            if(map[xs[i]][ys[i]] == nand) {
                al_draw_text(font, white, (xs[i] - absx)*20+10, (ys[i] - absy)*20+9 - al_get_font_line_height(font) / 2, ALLEGRO_ALIGN_CENTRE, "NAND");
            } else if(map[xs[i]][ys[i]] == nor) {
                al_draw_text(font, white, (xs[i] - absx)*20+10, (ys[i] - absy)*20+9 - al_get_font_line_height(font) / 2, ALLEGRO_ALIGN_CENTRE, "NOR");
            }
        }
    }
}

void place_chip(int x, int y, comp chip, int map[MAP_X][MAP_Y]) {
    if(chip == nand) {
        x = r_lim(3, x, MAP_X - 1 - 3);
        y = r_lim(1, y, MAP_Y - 1 - 1);
        for(int i = x - 4; i < x + 5; i++) {
            for(int j = y - 2; j < y + 3; j++) {
                if (map[r_lim(0, i, MAP_X - 1)][r_lim(0, j, MAP_Y - 1)] > hiwire) {
                    goto trip;
                }
            }
        }
        for(int i = x - 2; i < x + 3; i++) {
            for(int j = y - 1; j < y + 2; j++) {
                map[i][j] = aboard;
            }
        }
        map[x][y] = nand;
        map[x - 3][y + 1] = map[x - 3][y - 1] = lopinin;
        map[x + 3][y] = lopinout;
    } else if(chip == nor) {
        x = r_lim(3, x, MAP_X - 1 - 3);
        y = r_lim(1, y, MAP_Y - 1 - 1);
        for(int i = x - 4; i < x + 5; i++) {
            for(int j = y - 2; j < y + 3; j++) {
                if(map[r_lim(0, i, MAP_X - 1)][r_lim(0, j, MAP_Y - 1)] > hiwire) {
                    goto trip;
                }
            }
        }
        for(int i = x - 2; i < x + 3; i++) {
            for(int j = y - 1; j < y + 2; j++) {
                map[i][j] = oboard;
            }
        }
        map[x][y] = nor;
        map[x - 3][y + 1] = map[x - 3][y - 1] = lopinin;
        map[x + 3][y] = lopinout;
    } else if(chip == loflip) {
        x = r_lim(1, x, MAP_X - 1 - 1);
        y = r_lim(1, y, MAP_Y - 1 - 1);
        for(int i = x - 2; i < x + 3; i++) {
            for(int j = y - 2; j < y + 3; j++) {
                if(map[r_lim(0, i, MAP_X - 1)][r_lim(0, j, MAP_Y - 1)] > hiwire) {
                    goto trip;
                }
            }
        }
        for(int i = x - 1; i < x + 2; i++) {
            for(int j = y - 1; j < y + 2; j++) {
                map[i][j] = loflip;
            }
        }
    } else if(chip == lolight) {
        x = r_lim(1, x, MAP_X - 1 - 1);
        y = r_lim(1, y, MAP_Y - 1 - 1);
        for(int i = x - 2; i < x + 3; i++) {
            for(int j = y - 2; j < y + 3; j++) {
                if(map[r_lim(0, i, MAP_X - 1)][r_lim(0, j, MAP_Y - 1)] > hiwire) {
                    goto trip;
                }
            }
        }
        for(int i = x - 1; i < x + 2; i++) {
            for(int j = y - 1; j < y + 2; j++) {
                map[i][j] = lolight;
            }
        }
    } else if(chip == cross) {
        x = r_lim(0, x, MAP_X - 1);
        y = r_lim(0, y, MAP_Y - 1);
        for(int i = x - 1; i < x + 2; i++) {
            for(int j = y - 1; j < y + 2; j++) {
                if(map[r_lim(0, i, MAP_X - 1)][r_lim(0, j, MAP_Y - 1)] > hiwire) {
                    goto trip;
                }
            }
        }
        map[x][y] = cross;
    }
    trip:
        return;
}

void remove_chip(int map[MAP_X][MAP_Y], int x, int y) {
    x = r_lim(0, x, MAP_X - 1);
    y = r_lim(0, y, MAP_Y - 1);
    if(map[x][y] > hipinout) {
        map[x][y] = empty;
        remove_chip(map, x+1, y);
        remove_chip(map, x-1, y);
        remove_chip(map, x, y+1);
        remove_chip(map, x, y-1);
    } else if(map[x][y] > hiwire && map[x][y] < aboard) {
        map[x][y] = empty;
    }
}

int flip_switch(int map[MAP_X][MAP_Y], int x, int y, int mode) {
    if(map[x][y] == loflip && (mode == 0 || mode == 1)) {
        map[x][y] = hiflip;
        flip_switch(map, x+1, y, 1);
        flip_switch(map, x-1, y, 1);
        flip_switch(map, x, y+1, 1);
        flip_switch(map, x, y-1, 1);
    } else if(map[x][y] == hiflip && (mode == 0 || mode == 2)) {
        map[x][y] = loflip;
        flip_switch(map, x+1, y, 2);
        flip_switch(map, x-1, y, 2);
        flip_switch(map, x, y+1, 2);
        flip_switch(map, x, y-1, 2);
    }
    if(!mode) {
        return 20;
    }
    return 0;
}

void lock_coords(int *lock, int *lx, int *ly, ALLEGRO_MOUSE_STATE state) {
    *lock = 2;
    *lx = state.x / 20; 
    *ly = state.y / 20;
}

void lock_handler(int *lock, int lx, int ly, int *x, int *y, int *dirx, int *diry) {
    if(*lock == 2) {
        *dirx = abs(lx - *x);
        *diry = abs(ly - *y);
        *lock -= *dirx;
        *lock -= *diry;
    }
    if(*dirx == 1) {
        *y = ly;
    } else if(*diry == 1) {
        *x = lx;
    }
}

void click_handler(int map[MAP_X][MAP_Y], ALLEGRO_MOUSE_STATE state, int x, int y, int select, int *wait) {
    if(mtrx_range(state.x, state.y, 0, 1920, 0, 1000)) {
        if(*wait == 0 && select == 0 && (map[x][y] == loflip || map[x][y] == hiflip) && state.buttons & 1) {*wait = flip_switch(map, x, y, 0);}
        else if(map[x][y] > hipinout && state.buttons & 2) {remove_chip(map, x, y);}
        else if(select == 1 && map[x][y] < lopinin && state.buttons & 1) {place_chip(x, y, nand, map);}
        else if(select == 2 && map[x][y] < lopinin && state.buttons & 1) {place_chip(x, y, nor, map);}
        else if(select == 3 && map[x][y] < lopinin && state.buttons & 1) {place_chip(x, y, loflip, map);}
        else if(select == 4 && map[x][y] < lopinin && state.buttons & 1) {place_chip(x, y, lolight, map);}
        else if(select == 5 && map[x][y] < lopinin && state.buttons & 1) {place_chip(x, y, cross, map);}
        else if(state.buttons & 2 && map[x][y] < lopinin) {map[x][y] = empty;}
        else if(state.buttons & 1 && map[x][y] == empty) {map[x][y] = lowire;}
    }
}