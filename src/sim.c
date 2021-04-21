#include <pthread.h>
#include "canvas.h"

void *chip_sim(int map[MAP_X][MAP_Y]) {
    int rot = 0;

    for(int i = 0; i < MAP_X; i++) {
        for(int j = 0; j < MAP_Y; j++) {
            if(map[i][j] == nand) {
                rot = (map[i + 1][j] == nandrot) ? 1 : -1;
                if(map[i-3 * rot][j-1] != hipinin || map[i-3 * rot][j+1] != hipinin) {
                    map[i+3 * rot][j] = hipinout;
                } else if(map[i-3 * rot][j-1] == hipinin && map[i-3 * rot][j+1] == hipinin) {
                    map[i+3 * rot][j] = lopinout;
                }
            } else if(map[i][j] == nor) {
                rot = (map[i + 1][j] == norrot) ? 1 : -1;
                if(map[i-3 * rot][j-1] == lopinin && map[i-3 * rot][j+1] == lopinin) {
                    map[i+3 * rot][j] = hipinout;
                } else if(map[i-3 * rot][j-1] == hipinin || map[i-3 * rot][j+1] == hipinin) {
                    map[i+3 * rot][j] = lopinout;
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
            } else if(map[i][j] == onewayh) {
                rot = (map[i + 1][j] == onerot) ? 1 : -1;
                if(map[i-2*rot][j] == hipinin) {
                    map[i+2*rot][j] = hipinout;
                } else if(map[i-2*rot][j] == lopinin) {
                    map[i+2*rot][j] = lopinout;
                }
            } else if(map[i][j] == onewayv) {
                rot = (map[i][j - 1] == onerot) ? 1 : -1;
                if(map[i][j+2*rot] == hipinin) {
                    map[i][j-2*rot] = hipinout;
                } else if(map[i][j+2*rot] == lopinin) {
                    map[i][j-2*rot] = lopinout;
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
                } else if(map[i][j] == hibridge1 || map[i][j] == hibridge2) {
                    map[i][j] = map[i][j] - 1;
                }
            }
        }

        for(int i = 0; i < MAP_X; i++) {
            for(int j = 0; j < MAP_Y; j++) {
                if(map[i][j] == hipinout || map[i][j] == hiflip) {
                    if(map[i+1][j] == lowire || map[i+1][j] == lolight || map[i+1][j] == lopinin || map[i+1][j] == lobridge1 || map[i+1][j] == lobridge2) {
                        wire_sim(map, i+1, j, 1);
                    }
                    if(map[i-1][j] == lowire || map[i-1][j] == lolight || map[i-1][j] == lopinin || map[i-1][j] == lobridge1 || map[i-1][j] == lobridge2) {
                        wire_sim(map, i-1, j, 1);
                    }
                    if(map[i][j+1] == lowire || map[i][j+1] == lolight || map[i][j+1] == lopinin || map[i][j+1] == lobridge1 || map[i][j+1] == lobridge2) {
                        wire_sim(map, i, j+1, 1);
                    }
                    if(map[i][j-1] == lowire || map[i][j-1] == lolight || map[i][j-1] == lopinin || map[i][j-1] == lobridge1 || map[i][j-1] == lobridge2) {
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
        } else if(map[x][y] == lobridge1 || map[x][y] == lobridge2) {
            map[x][y] = map[x][y] + 1;
            for(int i = -8; i < 9; i++) {
                for(int j = -8; j < 9; j++) {
                    if(map[x + i][y] == map[x][y] - 1) {
                        wire_sim(map, x+i, y, 1);
                    }
                    if(map[x][y + j] == map[x][y] - 1) {
                        wire_sim(map, x, y+j, 1);
                    }
                }
            }
        }

        if(map[x+1][y] == lowire || map[x+1][y] == lolight || map[x+1][y] == lopinin || map[x+1][y] == lobridge1 || map[x+1][y] == lobridge2) {
            wire_sim(map, x+1, y, 1);
        } else if(map[x+1][y] == cross && map[x+2][y] == lowire) {
            wire_sim(map, x+2, y, 1);
        }
        if(map[x-1][y] == lowire || map[x-1][y] == lolight || map[x-1][y] == lopinin || map[x-1][y] == lobridge1 || map[x-1][y] == lobridge2) {
            wire_sim(map, x-1, y, 1);
        } else if(map[x-1][y] == cross && map[x-2][y] == lowire) {
            wire_sim(map, x-2, y, 1);
        }
        if(map[x][y+1] == lowire || map[x][y+1] == lolight || map[x][y+1] == lopinin || map[x][y+1] == lobridge1 || map[x][y+1] == lobridge2) {
            wire_sim(map, x, y+1, 1);
        } else if(map[x][y+1] == cross && map[x][y+2] == lowire) {
            wire_sim(map, x, y+2, 1);
        }
        if(map[x][y-1] == lowire || map[x][y-1] == lolight || map[x][y-1] == lopinin || map[x][y-1] == lobridge1 || map[x][y-1] == lobridge2) {
            wire_sim(map, x, y-1, 1);
        } else if(map[x][y-1] == cross && map[x][y-2] == lowire) {
            wire_sim(map, x, y-2, 1);
        } 
    }
}