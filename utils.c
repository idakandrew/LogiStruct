#include <stdio.h>
#include <unistd.h>
#include <libloaderapi.h>

void fix_dir(void) {
    char path[1024], *occ;
    GetModuleFileNameA(NULL, path, 1023);
    occ = strrchr(path, '\\');
    path[occ - path] = '\0';
    chdir(path); 
}

void save_canvas(int map[96][50]) {
    FILE *fp = fopen("data/save.bin", "wb");
    if(fp != NULL) {
        fseek(fp, 0, SEEK_SET);
        fwrite(map, sizeof(int)*96*50, 1, fp);
        fclose(fp);
    }
}

void load_canvas(int map[95][50]) {
    FILE *fp = fopen("data/save.bin", "rb");
    if(fp != NULL) {
        fread(map, sizeof(int)*96*50, 1, fp);
        fclose(fp);
    } else {
        memset(map, 0, sizeof(int)*96*50);
    }
}