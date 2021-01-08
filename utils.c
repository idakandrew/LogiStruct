#include <unistd.h>
#include <libloaderapi.h>

void fix_dir(void) {
    char path[1024], *occ;
    GetModuleFileNameA(NULL, path, 1023);
    occ = strrchr(path, '\\');
    path[occ - path] = '\0';
    chdir(path); 
}

