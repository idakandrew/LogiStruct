#include <allegro5/allegro_primitives.h>
#include "ui.h"
#include "canvas.h"


int max(int a, int b) {
    if(a > b) {
        return a;
    } else {
        return b;
    }
}

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

void btn_draw(button btn, ALLEGRO_FONT *font, int *time, ALLEGRO_MOUSE_STATE state) {
    al_draw_bitmap(btn.bit, btn.x, btn.y, 0);
    int xrange = (btn.cex - btn.x) * 2 + btn.x;
    int yrange = (btn.cey - btn.y) * 2 + btn.y;
    ALLEGRO_COLOR color = (*time > 0) ? al_map_rgba_f(.7, .7, .7, .7) : al_map_rgba_f(.85, .85, .85, .85);

    
    if(*time > 0 || mtrx_range(state.x, state.y, btn.x, xrange, btn.y, yrange)) {
       al_draw_filled_rectangle(btn.cex - al_get_bitmap_width(btn.bit)/2, btn.cey - al_get_bitmap_height(btn.bit)/2, btn.cex + al_get_bitmap_width(btn.bit)/2, 
            btn.cey + al_get_bitmap_height(btn.bit)/2, al_map_rgb(250, 80, 80));
        al_draw_tinted_bitmap(btn.bit, color, btn.x, btn.y, 0);
        if(time > 0) {(*time)--;}
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

// clean this up
void toolbar_text(int select, int cx, int cy, ALLEGRO_FONT *font, bool pen) {
    al_draw_textf(font, white, 240, 1055 - al_get_font_line_height(font) / 2, ALLEGRO_ALIGN_CENTER, "Coordinates: (%d : %d)", cx, MAP_Y - cy - 1);
    
    if(select == -1) {al_draw_text(font, white, 240, 1025 - al_get_font_line_height(font) / 2, ALLEGRO_ALIGN_CENTER, "Selected: Wire");}
    else if(select == 0) {al_draw_text(font, white, 240, 1025 - al_get_font_line_height(font) / 2, ALLEGRO_ALIGN_CENTER, "Selected: NAND");}
    else if(select == 1) {al_draw_text(font, white, 240, 1025 - al_get_font_line_height(font) / 2, ALLEGRO_ALIGN_CENTER, "Selected: NOR");}
    else if(select == 2) {al_draw_text(font, white, 240, 1025 - al_get_font_line_height(font) / 2, ALLEGRO_ALIGN_CENTER, "Selected: Switch");}
    else if(select == 3) {al_draw_text(font, white, 240, 1025 - al_get_font_line_height(font) / 2, ALLEGRO_ALIGN_CENTER, "Selected: Light");}
    else if(select == 4) {al_draw_text(font, white, 240, 1025 - al_get_font_line_height(font) / 2, ALLEGRO_ALIGN_CENTER, "Selected: Crossing");}
    else if(select == 5) {al_draw_text(font, white, 240, 1025 - al_get_font_line_height(font) / 2, ALLEGRO_ALIGN_CENTER, "Selected: Bridge G");}
    else if(select == 6) {al_draw_text(font, white, 240, 1025 - al_get_font_line_height(font) / 2, ALLEGRO_ALIGN_CENTER, "Selected: Bridge B");}
    else if(select == 7) {al_draw_text(font, white, 240, 1025 - al_get_font_line_height(font) / 2, ALLEGRO_ALIGN_CENTER, "Selected: 8-Seg");}
    else if(select == 8) {al_draw_text(font, white, 240, 1025 - al_get_font_line_height(font) / 2, ALLEGRO_ALIGN_CENTER, "Selected: One-Way");}

    al_draw_textf(font, white, 1690, 1040 - al_get_font_line_height(font) / 2 - 4, ALLEGRO_ALIGN_CENTER, "Edit: %s", pen ? "On" : "Off");
}

void draw_ghost(int select, button *cbtnlist, int x, int y, ALLEGRO_FONT *font, int zm, int rot) {
    int fact = 20 / zm;
    int adjx = x / fact * fact, adjy = y / fact * fact;
    char *left = (rot == 1) ? "" : "<<";
    char *right = (rot == 1) ? ">>" : "";

    if((select == 0 || select == 1) && mtrx_range(x, y, 0, 1920, 0, 1000)) {
        al_draw_rectangle(adjx - 60 / zm, adjy - 20 / zm, adjx + 80 / zm, adjy + 40 / zm, white, 2);
        al_draw_textf(font, white, adjx + fact / 2, adjy - 24 / zm - al_get_font_line_height(font), ALLEGRO_ALIGN_CENTER, 
            "%s %s %s", left, cbtnlist[select].text, right);
    } else if((select == 2 || select == 3) && mtrx_range(x, y, 0, 1920, 0, 1000)) {
        al_draw_rectangle(adjx - 20 / zm, adjy - 20 / zm, adjx + 40 / zm, adjy + 40 / zm, white, 2);
        al_draw_text(font, white, adjx + fact / 2, adjy - 24 / zm - al_get_font_line_height(font), ALLEGRO_ALIGN_CENTER, cbtnlist[select].text);
    } else if((select > 3 && select < 7) && mtrx_range(x, y, 0, 1920, 0, 1000)) {
        al_draw_rectangle(adjx, adjy, adjx + 20 / zm, adjy + 20 / zm, white, 2);
        if(select == 5 || select == 6) {
            al_draw_line(adjx - 160 / zm, adjy, adjx - 160 / zm, adjy + 20 / zm, white, 2);
            al_draw_line(adjx + 180 / zm, adjy, adjx + 180 / zm, adjy + 20 / zm, white, 2);
            al_draw_line(adjx, adjy - 160 / zm, adjx + 20 / zm, adjy - 160 / zm, white, 2);
            al_draw_line(adjx, adjy + 180 / zm, adjx + 20 / zm, adjy + 180 / zm, white, 2);
        }
        al_draw_text(font, white, adjx + fact / 2, adjy - 12 / zm - al_get_font_line_height(font), ALLEGRO_ALIGN_CENTER, cbtnlist[select].text);
    } else if(select == 7) {
        al_draw_rectangle(adjx - 60 / zm, adjy - 140 / zm, adjx + 80 / zm, adjy + 160 / zm, white, 2);
        al_draw_text(font, white, adjx + fact / 2, adjy - 144 / zm - al_get_font_line_height(font), ALLEGRO_ALIGN_CENTER, cbtnlist[select].text);
    } else if(select == 8) {
        al_draw_rectangle(adjx - 40 / zm, adjy, adjx + 60 / zm, adjy + 20 / zm, white, 2);
        al_draw_textf(font, white, adjx + fact / 2, adjy - 12 / zm - al_get_font_line_height(font), ALLEGRO_ALIGN_CENTER, 
            "%s %s %s", left, cbtnlist[select].text, right);
    } else if(select == 9) {
        al_draw_rectangle(adjx, adjy - 40 / zm, adjx + 20 / zm, adjy + 60 / zm, white, 2);
        al_draw_textf(font, white, adjx + fact / 2, adjy - 48 / zm - al_get_font_line_height(font), ALLEGRO_ALIGN_CENTER, 
            "%s : %s", rot == 1 ? "^" : "v", cbtnlist[select].text);
    }
}

void launch_codes(bool ask, ALLEGRO_FONT *font) {
    if(ask) {
        al_draw_text(font, red, 960, 96, ALLEGRO_ALIGN_CENTER, "CLEAR :: ARE YOU SURE?");
        al_draw_text(font, white, 960, 100, ALLEGRO_ALIGN_CENTER, "CLEAR :: ARE YOU SURE?");

        al_draw_text(font, red, 960, 134, ALLEGRO_ALIGN_CENTER, "[BACKSPACE] TO CANCEL.");
        al_draw_text(font, white, 960, 138, ALLEGRO_ALIGN_CENTER, "[BACKSPACE] TO CANCEL.");
        
        al_draw_text(font, red, 960, 172, ALLEGRO_ALIGN_CENTER, "[ENTER] TO CONFIRM.");
        al_draw_text(font, white, 960, 176, ALLEGRO_ALIGN_CENTER, "[ENTER] TO CONFIRM.");
    }
}

void save_text(ALLEGRO_FONT *font) {
    al_draw_text(font, red, 960, 96, ALLEGRO_ALIGN_CENTER, "AUTO-SAVING");
    al_draw_text(font, white, 960, 100, ALLEGRO_ALIGN_CENTER, "AUTO-SAVING");
}

void draw_box(int x, int y, int zm, ALLEGRO_MOUSE_STATE mstate, ALLEGRO_FONT *font, int mode) {
    int fact = 20 / zm;
    int adjx = x / fact * fact, adjy = y / fact * fact;
    int textx = ((mstate.x+1) / fact * fact - adjx) / 2 + adjx, texty = max((mstate.y+1) / fact * fact, adjy) + al_get_font_line_height(font) / 2;

    if(mode == 0) {
        al_draw_text(font, white, textx, texty, ALLEGRO_ALIGN_CENTER, "[COPY]");
    } else if(mode == 1) {
        al_draw_text(font, white, textx, texty, ALLEGRO_ALIGN_CENTER, "[DELETE]");
    }
    al_draw_rectangle(adjx, adjy, (mstate.x+1) / fact * fact, (mstate.y+1) / fact * fact, white, 2);
}