#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>

typedef struct button {
    float cx;
    float cy;
    float x;
    float y;
    char *text;
    ALLEGRO_BITMAP *bit;
} button;

button btn_build(float cx, float cy, char *text, char *file);

float btn_draw(button btn, ALLEGRO_FONT *font, float click);

float btn_click(button btn, ALLEGRO_MOUSE_EVENT click);