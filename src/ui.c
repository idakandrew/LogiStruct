#include <allegro5/allegro_primitives.h>
#include "ui.h"
#include "canvas.h"


button btn_build(float cex, float cey, char *text, char *file) {
    button out;
    ALLEGRO_BITMAP *bit = al_load_bitmap(file);

    out.cex = cex;
    out.cey = cey;
    out.x = cex - al_get_bitmap_width(bit) / 2;
    out.y = cey - al_get_bitmap_height(bit) / 2;
    out.text = text;
    out.bit = bit;

    return out;
}

void btn_draw(button btn, ALLEGRO_FONT *font, int *time) {
    al_draw_bitmap(btn.bit, btn.x, btn.y, 0);

    
    if(*time > 0) {
       al_draw_filled_rectangle(btn.cex - al_get_bitmap_width(btn.bit)/2, btn.cey - al_get_bitmap_height(btn.bit)/2, btn.cex + al_get_bitmap_width(btn.bit)/2, 
            btn.cey + al_get_bitmap_height(btn.bit)/2, al_map_rgb(250, 80, 80));
        al_draw_tinted_bitmap(btn.bit, al_map_rgba_f(.7, .7, .7, .7), btn.x, btn.y, 0);
        (*time)--;
    } 

    al_draw_text(font, white, btn.cex, btn.cey - al_get_font_line_height(font) / 2 - 4, ALLEGRO_ALIGN_CENTRE, btn.text);
}

bool btn_click(button btn, ALLEGRO_MOUSE_EVENT click, int *time) {
    float xrange = btn.x + al_get_bitmap_width(btn.bit);
    float yrange = btn.y + al_get_bitmap_height(btn.bit);

    if(click.x >= btn.x && click.x <= xrange && click.y >= btn.y && click.y <= yrange) {
        *time = 10;
        return true;
    }
    return false;
}

void toolbar_text(int select, int cx, int cy, ALLEGRO_FONT *font) {
    al_draw_textf(font, white, 240, 1055 - al_get_font_line_height(font) / 2, ALLEGRO_ALIGN_CENTER, "Coordinates: (%d : %d)", cx, MAP_Y - cy - 1);
    
    if(select == -1) {al_draw_text(font, white, 240, 1025 - al_get_font_line_height(font) / 2, ALLEGRO_ALIGN_CENTER, "Selected: Wire");}
    else if(select == 0) {al_draw_text(font, white, 240, 1025 - al_get_font_line_height(font) / 2, ALLEGRO_ALIGN_CENTER, "Selected: NAND");}
    else if(select == 1) {al_draw_text(font, white, 240, 1025 - al_get_font_line_height(font) / 2, ALLEGRO_ALIGN_CENTER, "Selected: NOR");}
    else if(select == 2) {al_draw_text(font, white, 240, 1025 - al_get_font_line_height(font) / 2, ALLEGRO_ALIGN_CENTER, "Selected: Switch");}
    else if(select == 3) {al_draw_text(font, white, 240, 1025 - al_get_font_line_height(font) / 2, ALLEGRO_ALIGN_CENTER, "Selected: Light");}
    else if(select == 4) {al_draw_text(font, white, 240, 1025 - al_get_font_line_height(font) / 2, ALLEGRO_ALIGN_CENTER, "Selected: Crossing");}
}
