#ifndef UI
#define UI

#include <allegro5/allegro_font.h>

typedef struct button {
    float cex;
    float cey;
    float x;
    float y;
    char *text;
    ALLEGRO_BITMAP *bit;
} button;

button btn_build(float cex, float cey, char *text, char *file);

void btn_draw(button btn, ALLEGRO_FONT *font, int *time);

bool btn_click(button btn, ALLEGRO_MOUSE_EVENT click, int *time);

void toolbar_text(int select, int cx, int cy, ALLEGRO_FONT *font, bool pen);

void draw_ghost(int select, button *cbtnlist, int x, int y, ALLEGRO_FONT *font, int zm, int rot);

#endif 