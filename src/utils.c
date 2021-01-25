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

void save_canvas(int map[MAP_X][MAP_Y]) {
    FILE *fp = fopen("../data/save.bin", "wb");
    if(fp != NULL) {
        fseek(fp, 0, SEEK_SET);
        fwrite(map, sizeof(int)*MAP_X*MAP_Y, 1, fp);
        fclose(fp);
    }
}

void load_canvas(int map[MAP_X][MAP_Y]) {
    FILE *fp = fopen("../data/save.bin", "rb");
    if(fp != NULL) {
        fread(map, sizeof(int)*MAP_X*MAP_Y, 1, fp);
        fclose(fp);
    }
}