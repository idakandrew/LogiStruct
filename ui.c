#include <allegro5/allegro5.h>
#include "ui.h"


button btn_build(float cx, float cy, char *text, char *file) {
    button out;
    ALLEGRO_BITMAP *bit = al_load_bitmap(file);

    out.cx = cx;
    out.cy = cy;
    out.x = cx - al_get_bitmap_width(bit) / 2;
    out.y = cy - al_get_bitmap_height(bit) / 2;
    out.text = text;
    out.bit = bit;

    return out;
}

float btn_draw(button btn, ALLEGRO_FONT *font, float click) {
    al_draw_bitmap(btn.bit, btn.x, btn.y, 0);

    if(click >= .495) {
        al_draw_filled_rectangle(btn.cx - al_get_bitmap_width(btn.bit) * (1 - click), btn.cy - al_get_bitmap_height(btn.bit) * (1 - click), 
            btn.cx + al_get_bitmap_width(btn.bit) * (1 - click), btn.cy + al_get_bitmap_height(btn.bit) * (1 - click), al_map_rgb(200, 200, 200));
        al_draw_tinted_bitmap(btn.bit, al_map_rgba_f(.8, .8, .8, .8), btn.x, btn.y, 0);
        click-= .05;
    } 

    al_draw_text(font, al_map_rgb(200, 200, 200), btn.cx, btn.cy - al_get_font_line_height(font) / 2, ALLEGRO_ALIGN_CENTRE, btn.text);
    
    return click;
}

void btn_click(button btn, ALLEGRO_MOUSE_EVENT click, float *time) {
    float xrange = btn.x + al_get_bitmap_width(btn.bit);
    float yrange = btn.y + al_get_bitmap_height(btn.bit);

    if(click.x >= btn.x && click.x <= xrange && click.y >= btn.y && click.y <= yrange) {
        *time = 1;
    }
}
