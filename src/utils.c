#include <stdio.h>
#include <unistd.h>
#include <libloaderapi.h>
#include "utils.h"

void fix_dir(void) {
    char path[1024], *occ;
    GetModuleFileNameA(NULL, path, 1023);
    occ = strrchr(path, '\\');
    path[occ - path] = '\0';
    chdir(path); 
}

void save_canvas(int map[MAP_X][MAP_Y], int *posx, int *posy) {
    FILE *fp = fopen("data/save.bin", "wb");
    if(fp != NULL) {
        fseek(fp, 0, SEEK_SET);
        fwrite(posx, sizeof(int), 1, fp);
        fwrite(posy, sizeof(int), 1, fp);
        fwrite(map, sizeof(int)*MAP_X*MAP_Y, 1, fp);
        fclose(fp);
    }
}

void load_canvas(int map[MAP_X][MAP_Y], int *posx, int *posy) {
    FILE *fp = fopen("data/save.bin", "rb");
    if(fp != NULL) {
        fseek(fp, 0, SEEK_SET);
        fread(posx, sizeof(int), 1, fp);
        fread(posy, sizeof(int), 1, fp);
        fread(map, sizeof(int)*MAP_X*MAP_Y, 1, fp);
        fclose(fp);
    }
}