#include <pthread.h>
#include "canvas.h"

void *chip_sim(int map[MAP_X][MAP_Y]) {
    for(int i = 0; i < MAP_X; i++) {
        for(int j = 0; j < MAP_Y; j++) {
            if(map[i][j] == nand) {
                if(map[i-3][j-1] != hipinin || map[i-3][j+1] != hipinin) {
                    map[i+3][j] = hipinout;
                } else if(map[i-3][j-1] == hipinin && map[i-3][j+1] == hipinin) {
                    map[i+3][j] = lopinout;
                }
            } else if(map[i][j] == nor) {
                if(map[i-3][j-1] == lopinin && map[i-3][j+1] == lopinin) {
                    map[i+3][j] = hipinout;
                } else if(map[i-3][j-1] == hipinin || map[i-3][j+1] == hipinin) {
                    map[i+3][j] = lopinout;
                }
            } else if(map[i][j] == seg) {
                if(map[i-3][j-7] == hipinin) {
                    for(int k = i - 2; k < i + 3; k++) {
                        map[k][j-5] = hilight;
                    }
                }
                if(map[i+3][j-7] == hipinin) {
                    for(int k = i - 2; k < i + 3; k++) {
                        map[k][j-1] = hilight;
                    }
                }
                if(map[i+1][j+7] == hipinin) {
                    for(int k = i - 2; k < i + 3; k++) {
                        map[k][j+3] = hilight;
                    }
                }
                if(map[i-1][j-7] == hipinin) {
                    for(int k = j - 4; k < j - 1; k++) {
                        map[i-2][k] = hilight;
                    }
                }
                if(map[i+1][j-7] == hipinin) {
                    for(int k = j - 4; k < j - 1; k++) {
                        map[i+2][k] = hilight;
                    }
                }
                if(map[i-3][j+7] == hipinin) {
                    for(int k = j; k < j + 3; k++) {
                        map[i-2][k] = hilight;
                    }
                }
                if(map[i-1][j+7] == hipinin) {
                    for(int k = j; k < j + 3; k++) {
                        map[i+2][k] = hilight;
                    }
                }
                if(map[i+3][j+7] == hipinin) {
                    map[i+1][j+5] = map[i+2][j+5] = hilight;
                }
            }
        }
    }
}

void *wire_sim(int map[MAP_X][MAP_Y], int x, int y, int mode) {
    if(!mode) {
        for(int i = 0; i < MAP_X; i++) {
            for(int j = 0; j < MAP_Y; j++) {
                if(map[i][j] == hiwire) {
                    map[i][j] = lowire;
                } else if(map[i][j] == hilight) {
                    map[i][j] = lolight;
                } else if(map[i][j] == hipinin) {
                    map[i][j] = lopinin;
                } else if(map[i][j] == hibridge) {
                    map[i][j] = lobridge;
                }
            }
        }

        for(int i = 0; i < MAP_X; i++) {
            for(int j = 0; j < MAP_Y; j++) {
                if(map[i][j] == hipinout || map[i][j] == hiflip) {
                    if(map[i+1][j] == lowire || map[i+1][j] == lolight || map[i+1][j] == lopinin || map[i+1][j] == lobridge) {
                        wire_sim(map, i+1, j, 1);
                    }
                    if(map[i-1][j] == lowire || map[i-1][j] == lolight || map[i-1][j] == lopinin || map[i-1][j] == lobridge) {
                        wire_sim(map, i-1, j, 1);
                    }
                    if(map[i][j+1] == lowire || map[i][j+1] == lolight || map[i][j+1] == lopinin || map[i][j+1] == lobridge) {
                        wire_sim(map, i, j+1, 1);
                    }
                    if(map[i][j-1] == lowire || map[i][j-1] == lolight || map[i][j-1] == lopinin || map[i][j-1] == lobridge) {
                        wire_sim(map, i, j-1, 1);
                    }
                }
            }
        }
        
    } else {
        if(map[x][y] == lowire) {
            map[x][y] = hiwire;
        } else if(map[x][y] == lolight) {
            map[x][y] = hilight;
        } else if(map[x][y] == lopinin) {
            map[x][y] = hipinin;
        } else if(map[x][y] == lobridge) {
            map[x][y] = hibridge;
            for(int i = -8; i < 9; i++) {
                for(int j = -8; j < 9; j++) {
                    if(map[x + i][y] == lobridge) {
                        wire_sim(map, x+i, y, 1);
                    }
                    if(map[x][y + j] == lobridge) {
                        wire_sim(map, x, y+j, 1);
                    }
                }
            }
        }

        if(map[x+1][y] == lowire || map[x+1][y] == lolight || map[x+1][y] == lopinin || map[x+1][y] == lobridge) {
            wire_sim(map, x+1, y, 1);
        } else if(map[x+1][y] == cross && map[x+2][y] == lowire) {
            wire_sim(map, x+2, y, 1);
        }
        if(map[x-1][y] == lowire || map[x-1][y] == lolight || map[x-1][y] == lopinin || map[x-1][y] == lobridge) {
            wire_sim(map, x-1, y, 1);
        } else if(map[x-1][y] == cross && map[x-2][y] == lowire) {
            wire_sim(map, x-2, y, 1);
        }
        if(map[x][y+1] == lowire || map[x][y+1] == lolight || map[x][y+1] == lopinin || map[x][y+1] == lobridge) {
            wire_sim(map, x, y+1, 1);
        } else if(map[x][y+1] == cross && map[x][y+2] == lowire) {
            wire_sim(map, x, y+2, 1);
        }
        if(map[x][y-1] == lowire || map[x][y-1] == lolight || map[x][y-1] == lopinin || map[x][y-1] == lobridge) {
            wire_sim(map, x, y-1, 1);
        } else if(map[x][y-1] == cross && map[x][y-2] == lowire) {
            wire_sim(map, x, y-2, 1);
        } 
    }
}