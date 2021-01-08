#include <math.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include "canvas.h"

bool m_range(ALLEGRO_MOUSE_STATE mouse, int xmin, int xmax, int ymin, int ymax) {
    if(mouse.x >= xmin && mouse.x <= xmax && mouse.y >= ymin && mouse.y <= ymax) {
        return true;
    }
    return false;
}

int r_lim(int min, int val, int max) {
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
            } else if(map[i][j] == hiwire) {
                al_draw_filled_rectangle(i*20, j*20, i*20+20, j*20+20, al_map_rgb(200, 30, 30)); 
            } else if(map[i][j] == and || map[i][j] == aboard) {
                al_draw_filled_rectangle(i*20, j*20, i*20+20, j*20+20, al_map_rgb(0, 180, 80));
                if(map[i][j] == and) {xs[saved] = i, ys[saved] = j, saved++;}
            } else if(map[i][j] == not || map[i][j] == nboard) {
                al_draw_filled_rectangle(i*20, j*20, i*20+20, j*20+20, al_map_rgb(0, 80, 180));
                if(map[i][j] == not) {xs[saved] = i, ys[saved] = j, saved++;}
            } else if(map[i][j] == cross) {
                al_draw_filled_rectangle(i*20, j*20, i*20+20, j*20+20, al_map_rgb(150, 150, 0));
            } else if(map[i][j] == lopinin || map[i][j] == lopinout || map[i][j] == loflip) {
                al_draw_filled_rectangle(i*20, j*20, i*20+20, j*20+20, al_map_rgb(0, 0, 0));
            } else if (map[i][j] == hipinin || map[i][j] == hipinout || map[i][j] == hiflip) {
                al_draw_filled_rectangle(i*20, j*20, i*20+20, j*20+20, al_map_rgb(80, 0, 0));
            } else if(map[i][j] == lolight) {
                al_draw_filled_rectangle(i*20, j*20, i*20+20, j*20+20, al_map_rgb(100, 100, 100));
            } else if(map[i][j] == hilight) {
                al_draw_filled_rectangle(i*20, j*20, i*20+20, j*20+20, al_map_rgb(200, 200, 200));
            }
        }
    }

    for(int i = 0; i < saved; i++) {
        if(map[xs[i]][ys[i]] == and) {
            al_draw_text(font, al_map_rgb(200, 200, 200), xs[i]*20+10, ys[i]*20+9 - al_get_font_line_height(font) / 2, ALLEGRO_ALIGN_CENTRE, "AND");
        } else if(map[xs[i]][ys[i]] == not) {
            al_draw_text(font, al_map_rgb(200, 200, 200), xs[i]*20+10, ys[i]*20+9 - al_get_font_line_height(font) / 2, ALLEGRO_ALIGN_CENTRE, "NOT");
        }
    }
}

void place_chip(int x, int y, comp chip, int map[96][50]) {
    if(chip == and) {
        x = r_lim(3, x, 92);
        y = r_lim(2, y, 47);
        for(int i = x - 4; i < x + 5; i++) {
            for(int j = y - 3; j < y + 4; j++) {
                if (map[r_lim(0, i, 95)][r_lim(0, j, 49)] > hiwire) {
                    goto trip;
                }
            }
        }
        for(int i = x - 2; i < x + 3; i++) {
            for(int j = y - 2; j < y + 3; j++) {
                map[i][j] = aboard;
            }
        }
        map[x][y] = and;
        map[x - 3][y + 1] = map[x - 3][y - 1] = lopinin;
        map[x + 3][y] = lopinout;
    } else if(chip == not) {
        x = r_lim(3, x, 92);
        y = r_lim(1, y, 48);
        for(int i = x - 4; i < x + 5; i++) {
            for(int j = y - 2; j < y + 3; j++) {
                if(map[r_lim(0, i, 95)][r_lim(0, j, 49)] > hiwire) {
                    goto trip;
                }
            }
        }
        for(int i = x - 2; i < x + 3; i++) {
            for(int j = y - 1; j < y + 2; j++) {
                map[i][j] = nboard;
            }
        }
        map[x][y] = not;
        map[x - 3][y] = lopinin;
        map[x + 3][y] = lopinout;
    } else if(chip == loflip) {
        x = r_lim(1, x, 94);
        y = r_lim(1, y, 48);
        for(int i = x - 2; i < x + 3; i++) {
            for(int j = y - 2; j < y + 3; j++) {
                if(map[r_lim(0, i, 95)][r_lim(0, j, 49)] > hiwire) {
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
        x = r_lim(1, x, 94);
        y = r_lim(1, y, 48);
        for(int i = x - 2; i < x + 3; i++) {
            for(int j = y - 2; j < y + 3; j++) {
                if(map[r_lim(0, i, 95)][r_lim(0, j, 49)] > hiwire) {
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
        x = r_lim(2, x, 93);
        y = r_lim(2, y, 47);
        for(int i = x - 1; i < x + 2; i++) {
            for(int j = y - 1; j < y + 2; j++) {
                if(map[r_lim(0, i, 95)][r_lim(0, j, 49)] > hiwire) {
                    goto trip;
                }
            }
        }
        map[x][y] = cross;
    }
    trip:
        return;
}

void remove_chip(int map[96][50], int x, int y) {
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

int flip_switch(int map[96][50], int x, int y, int mode) {
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
        return 10;
    }
    return 0;
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