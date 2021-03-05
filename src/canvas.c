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

// multithread?
void draw_map(int zm, bool grid, int map[MAP_X][MAP_Y], int cx, int cy, ALLEGRO_FONT *font) {
    ALLEGRO_COLOR colormap[24] = {
        bgcolor, mediumgrey, red, black, redblack, black, redblack, green,
        blue, green, blue, green, blue, lightgrey, white, black, redblack, gold, green, 
        redblack, blue, redblack, purp, purp
    };
    static int xs[1000];
    static int ys[1000];
    int saved = 0;
    int vxzm = VIEW_X - VIEW_X / zm, vyzm = VIEW_Y - VIEW_Y / zm;

    for(int i = cx - vxzm; i < cx + VIEW_X + vxzm; i++) {
        if(grid) {
            al_draw_line((i-cx + vxzm)*20/zm, 0, (i-cx + vxzm)*20/zm, 1000, nearblack, 1);
        }

        for(int j = cy - vyzm; j < cy + VIEW_Y + vyzm; j++) {
            if(grid && i == cx + VIEW_X + vxzm - 1) {
                al_draw_line(0, (j-cy + vyzm)*20/zm, 1920, (j-cy + vyzm)*20/zm, nearblack, 1);
            }

            if(map[i][j] != empty) {
                if(zm == 2) {
                    al_draw_filled_rectangle((i-cx + vxzm)*10, (j-cy + vyzm)*10, (i-cx + vxzm)*10+10, (j-cy + vyzm)*10+10, colormap[map[i][j]]);
                } else {
                    al_draw_filled_rectangle((i-cx)*20, (j-cy)*20, (i-cx)*20+20, (j-cy)*20+20, colormap[map[i][j]]);
                }
                if(map[i][j] == nand || map[i][j] == nor) {
                    xs[saved] = i, ys[saved] = j, saved++;
                }
            }
        }
    }

    for(int i = 0; i < saved; i++) {
        if(mtrx_range(xs[i], ys[i], cx - vxzm, cx + VIEW_X + vxzm, cy - vyzm, cy + VIEW_Y + vxzm)) {
            if(map[xs[i]][ys[i]] == nand) {
                al_draw_text(font, white, (xs[i] - cx + vxzm)*20/zm+10/zm, 
                    (ys[i] - cy + vyzm)*20/zm+9/zm - al_get_font_line_height(font) / 2, ALLEGRO_ALIGN_CENTRE, "NAND");
            } else if(map[xs[i]][ys[i]] == nor) {
                al_draw_text(font, white, (xs[i] - cx + vxzm)*20/zm+10/zm, 
                    (ys[i] - cy + vyzm)*20/zm+9/zm - al_get_font_line_height(font) / 2, ALLEGRO_ALIGN_CENTRE, "NOR");
            }
        }
    }
}

void place_chip(int x, int y, comp chip, int map[MAP_X][MAP_Y], int rot) {
    if(chip == nand) {
        x = r_lim(3, x, MAP_X - 1 - 4);
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
        map[x + rot][y] = nandrot;
        map[x - 3 * rot][y + 1] = map[x - 3 * rot][y - 1] = lopinin;
        map[x + 3 * rot][y] = lopinout;
    } else if(chip == nor) {
        x = r_lim(3, x, MAP_X - 1 - 4);
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
        map[x + rot][y] = norrot;
        map[x - 3 * rot][y + 1] = map[x - 3 * rot][y - 1] = lopinin;
        map[x + 3 * rot][y] = lopinout;
    } else if(chip == loflip) {
        x = r_lim(1, x, MAP_X - 1 - 2);
        y = r_lim(1, y, MAP_Y - 1 - 2);
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
        x = r_lim(1, x, MAP_X - 1 - 2);
        y = r_lim(1, y, MAP_Y - 1 - 2);
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
    } else if(chip == lobridge1 || chip == lobridge2) {
        x = r_lim(0, x, MAP_X - 1);
        y = r_lim(0, y, MAP_Y - 1);
        for(int i = x; i < x + 1; i++) {
            for(int j = y; j < y + 1; j++) {
                if(map[r_lim(0, i, MAP_X - 1)][r_lim(0, j, MAP_Y - 1)] > hiwire) {
                    goto trip;
                }
            }
        }
        map[x][y] = chip;
    } else if(chip == seg) {
        x = r_lim(3, x, MAP_X - 1 - 4);
        y = r_lim(7, y, MAP_Y - 1 - 8);
        for(int i = x - 4; i < x + 5; i++) {
            for(int j = y - 8; j < y + 9; j++) {
                if(map[r_lim(0, i, MAP_X - 1)][r_lim(0, j, MAP_Y - 1)] > hiwire) {
                    goto trip;
                }
            }
        }
        for(int i = x - 3; i < x + 4; i++) {
            for(int j = y - 6; j < y + 7; j ++)  {
                map[i][j] = segboard;
            }
        }
        for(int i = x - 2; i < x + 3; i++) {
            for(int j = y - 5; j < y + 4; j++) {
                map[i][j] = lolight;
            }
        }
        for(int i = x - 1; i < x + 2; i++) {
            for(int j = y - 4; j < y - 1; j++) {
                map[i][j] = segboard;
                map[i][j + 4] = segboard;
            }
        }
        for(int i = x - 3; i < x + 4; i += 2) {
            map[i][y - 7] = map[i][y + 7] = lopinin;
        }
        map[x + 1][y + 5] = map[x + 2][y + 5] = lolight;
        map[x][y] = seg;
    }
    trip:
        return;
}

void remove_chip(int map[MAP_X][MAP_Y], int x, int y) {
    x = r_lim(0, x, MAP_X - 1);
    y = r_lim(0, y, MAP_Y - 1);
    if(map[x][y] > hipinout && !(map[x][y] > cross && map[x][y] < seg)) {
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

void lock_axis(int zm, int *lock, int *x, int *y, int lx, int ly) {
    int testx = 0, testy = 0;

    if(*lock == 0) {
        testx = abs(*x - lx);
        testy = abs(*y - ly);
        
        if(testx == 1) {
            *lock = 1;
        } else if(testy == 1) {
            *lock = 2;
        }
    } else if(*lock == 1) {
        *y = ly;
    } else if(*lock == 2) {
        *x = lx;
    }
}

// maybe clean also?
void click_handler(int map[MAP_X][MAP_Y], ALLEGRO_MOUSE_STATE state, int x, int y, int select, int *wait, bool pen, int rot) {
    if(mtrx_range(state.x, state.y, 0, 1920, 0, 1000)) {
        if(*wait == 0 && select == -1 && (map[x][y] == loflip || map[x][y] == hiflip) && state.buttons & 1) {*wait = flip_switch(map, x, y, 0);}
        else if(pen) {
            if(map[x][y] > hipinout && !(map[x][y] > cross && map[x][y] < seg) && state.buttons & 2) {remove_chip(map, x, y);}
            else if(select == 0 && map[x][y] < lopinin && state.buttons & 1) {place_chip(x, y, nand, map, rot);}
            else if(select == 1 && map[x][y] < lopinin && state.buttons & 1) {place_chip(x, y, nor, map, rot);}
            else if(select == 2 && map[x][y] < lopinin && state.buttons & 1) {place_chip(x, y, loflip, map, 0);}
            else if(select == 3 && map[x][y] < lopinin && state.buttons & 1) {place_chip(x, y, lolight, map, 0);}
            else if(select == 4 && map[x][y] < lopinin && state.buttons & 1) {place_chip(x, y, cross, map, 0);}
            else if(select == 5 && map[x][y] < lopinin && state.buttons & 1) {place_chip(x, y, lobridge1, map, 0);}
            else if(select == 6 && map[x][y] < lopinin && state.buttons & 1) {place_chip(x, y, lobridge2, map, 0);}
            else if(select == 7 && map[x][y] < lopinin && state.buttons & 1) {place_chip(x, y, seg, map, 0);}
            else if(state.buttons & 2 && (map[x][y] < lopinin || (map[x][y] > cross && map[x][y] < seg))) {map[x][y] = empty;}
            else if(state.buttons & 1 && map[x][y] == empty) {map[x][y] = lowire;}
        }
    }
}

int part_picker(int map[MAP_X][MAP_Y], ALLEGRO_MOUSE_STATE state, int cx, int cy, int zm) {
    int x = state.x / (20 / zm) + cx - (VIEW_X - VIEW_X / zm);
    int y = state.y / (20 / zm) + cy - (VIEW_Y - VIEW_Y / zm);

    if(map[x][y] == lowire || map[x][y] == hiwire) {
        return -1;
    } else if(map[x][y] == nand || map[x][y] == nandrot || map[x][y] == aboard) {
        return 0;
    } else if(map[x][y] == nor || map[x][y] == norrot || map[x][y] == oboard) {
        return 1;
    } else if(map[x][y] == loflip || map[x][y] == hiflip) {
        return 2;
    } else if(map[x][y] == lolight || map[x][y] == hilight) {
        return 3;
    } else if(map[x][y] == cross) {
        return 4;
    } else if(map[x][y] == lobridge1 || map[x][y] == hibridge1) {
        return 5;
    } else if(map[x][y] == lobridge2 || map[x][y] == hibridge2) {
        return 6;
    } else if(map[x][y] == seg || map[x][y] == segboard) {
        return 7;
    }

    return -1;
}