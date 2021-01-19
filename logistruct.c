#include <math.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_ttf.h>
#include "canvas.h"
#include "utils.h"
#include "sim.h"
#include "ui.h"

#define length(v) (sizeof(v) / sizeof(*v))
#define txtcolor al_map_rgb(200, 200, 200)
#define bgcolor al_map_rgb(45, 45, 45)

int main(void) {
    fix_dir();
    al_init();
    al_init_primitives_addon();
    al_init_image_addon();
    al_init_font_addon();
    al_init_ttf_addon();
    al_install_keyboard();
    al_install_mouse();

    ALLEGRO_TIMER *timer = al_create_timer(1.0 / 60.0);
    ALLEGRO_EVENT_QUEUE *queue = al_create_event_queue();
    al_set_new_display_flags(ALLEGRO_FULLSCREEN_WINDOW);
    ALLEGRO_DISPLAY *disp = al_create_display(1920, 1080);

    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_mouse_event_source());
    al_register_event_source(queue, al_get_display_event_source(disp));
    al_register_event_source(queue, al_get_timer_event_source(timer));

    ALLEGRO_EVENT event;
    ALLEGRO_MOUSE_STATE mstate;
    ALLEGRO_KEYBOARD_STATE kstate;

    typedef enum scenes {exit, menu, settings, canvas} scene;

    scene curr = menu;
    bool done = false;
    bool redraw = true;

    al_start_timer(timer);

    start:

    if(curr == menu) {
        ALLEGRO_FONT *font = al_load_ttf_font("data/mont.otf", 32, 0);

        int mcbtnlist[3] = {0, 0, 0};

        button mbtnlist[3] = {
            btn_build(960, 500, "Canvas", "data/new.png"), btn_build(960, 650, "Controls", "data/new.png"),
            btn_build(960, 800, "Exit", "data/new.png")
        };
        
        ALLEGRO_BITMAP *logo = al_load_bitmap("data/logo.png");
        ALLEGRO_BITMAP *bg = al_load_bitmap("data/bg.png");

        while(1) {
            al_wait_for_event(queue, &event);

            switch(event.type) {
                case ALLEGRO_EVENT_TIMER:
                    redraw = true;
                    break;
                case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
                    for(int i = 0; i < 3; i++) {btn_click(mbtnlist[i], event.mouse, &mcbtnlist[i]);}
                    break;
                case ALLEGRO_EVENT_DISPLAY_CLOSE:
                    done = true;
                    break;
            }

            if(mcbtnlist[0] == 1) {
                curr = canvas;
                break;
            } else if(mcbtnlist[1] == 1) {
                curr = settings;
                break;
            } else if(done || mcbtnlist[2] == 1) {
                curr = exit;
                break;
            }

            if(redraw && al_is_event_queue_empty(queue)) {
                al_clear_to_color(bgcolor);
                al_draw_bitmap(bg, 0, 0, 0);
                al_draw_filled_rectangle(590, 0, 1330, 1080, al_map_rgb(15, 15, 15));
                al_draw_bitmap(logo, 960 - al_get_bitmap_width(logo) / 2, 150, 0);
                al_draw_text(font, txtcolor, 960, 960, ALLEGRO_ALIGN_CENTER, "v0.1 (Jan 8, 2021)");
                al_draw_text(font, txtcolor, 960, 1000, ALLEGRO_ALIGN_CENTER, "By Andrew Idak");
                
                for(int i = 0; i < 3; i++) {btn_draw(mbtnlist[i], font, &mcbtnlist[i]);}

                al_flip_display();
                redraw = false;
            }
        }

        al_destroy_bitmap(logo);
        for(int i = 0; i < 3; i++) {al_destroy_bitmap(mbtnlist[i].bit);}
        al_destroy_font(font);
        al_flush_event_queue(queue);

        goto start;

    } else if(curr == settings) {
        int scbtn0 = 0, ignore = 0;

        ALLEGRO_FONT *font = al_load_ttf_font("data/mont.otf", 32, 0);
        int halfline = al_get_font_line_height(font) / 2;

        button sbtnlist[7] = {
            btn_build(250, 70, "Menu", "data/new.png"), btn_build(250, 330, "Mouse 1", "data/new.png"), 
            btn_build(250, 470, "Mouse 2", "data/new.png"), btn_build(250, 610, "L Shift", "data/new.png"), 
            btn_build(250, 750, "Tab", "data/new.png"), btn_build(250, 890, "Esc", "data/new.png"), 
            btn_build(1200, 330, "Backspace", "data/new.png")
        };

        char const *textlist[6] = {
            "Place wire and objects.", "Erase wire and objects.", "Lock placement axis.", 
            "Toggle grid overlay.", "Deselect current object.", "Clear canvas."
        };

        int xlist[6] = {490, 490, 490, 490, 490, 1440};
        int ylist[6] = {330, 470, 610, 750, 890, 330};

        while(1) {
            al_wait_for_event(queue, &event);

            switch(event.type) {
                case ALLEGRO_EVENT_TIMER:
                    redraw = true;
                    break;
                case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
                    btn_click(sbtnlist[0], event.mouse, &scbtn0);
                    break;
                case ALLEGRO_EVENT_DISPLAY_CLOSE:
                    done = true;
                    break;
            }

            if(scbtn0 == 1) {
                curr = menu;
                break;
            } else if(done) {
                curr = exit;
                break;
            }

            if(redraw && al_is_event_queue_empty(queue)) {
                al_clear_to_color(bgcolor);

                al_draw_filled_rectangle(0, 0, 1920, 140, al_map_rgb(15, 15, 15));
                al_draw_filled_rectangle(20, 240, 950, 980, al_map_rgb(15, 15, 15));
                al_draw_filled_rectangle(970, 240, 1900, 980, al_map_rgb(15, 15, 15));

                for (int i = 0; i < length(sbtnlist); i++) {
                    btn_draw(sbtnlist[i], font, (i == 0) ? &scbtn0 : &ignore);
                }

                for(int i = 0; i < length(textlist); i++) {
                    al_draw_text(font, txtcolor, xlist[i], ylist[i] - halfline, 0, textlist[i]);
                }

                al_flip_display();
                redraw = false;
            }
        }

        for(int i = 0; i < 7; i++) {al_destroy_bitmap(sbtnlist[i].bit);}
        al_destroy_font(font);
        al_flush_event_queue(queue);

        goto start;

    } else if (curr == canvas) {
        bool grid = false;
        int click = 0, wait = 0, lock = 0, x = 0, y = 0, lx = 0, ly = 0, dirx = 0, diry = 0;
        int cbtna = 0, cbtnn = 0, cbtnf = 0, cbtnl = 0, cbtnr = 0, cbtnx = 0, select = 0;
        int map[96][50];
        load_canvas(map);

        ALLEGRO_FONT *font = al_load_ttf_font("data/mont.otf", 26, 0);
        button abtn = btn_build(660, 1040, "AND", "data/select.png");
        button nbtn = btn_build(810, 1040, "NOT", "data/select.png");
        button fbtn = btn_build(960, 1040, "Switch", "data/select.png");
        button lbtn = btn_build(1110, 1040, "Light", "data/select.png");
        button rbtn = btn_build(1260, 1040, "Crossing", "data/select.png");
        button xbtn = btn_build(1680, 1040, "Menu", "data/select.png");

        while(1) {
            al_wait_for_event(queue, &event);
            al_get_mouse_state(&mstate);
            al_get_keyboard_state(&kstate);

            switch (event.type) {
                case ALLEGRO_EVENT_TIMER:
                    wire_sim(map, 0, 0, 0);
                    chip_sim(map);
                    wait--;
                    redraw = true;
                    break;
                case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
                    if(m_range(mstate, 0, 1920, 0, 1000)) {
                        if(lock == 1) {lock_coords(&lock, &lx, &ly, mstate);}
                        click = 1;
                    } else if(btn_click(abtn, event.mouse, &cbtna)) {
                        select = (select == 1) ? 0 : 1;
                    } else if(btn_click(nbtn, event.mouse, &cbtnn)) {
                        select = (select == 2) ? 0 : 2;
                    } else if(btn_click(fbtn, event.mouse, &cbtnf)) {
                        select = (select == 3) ? 0 : 3;
                    } else if(btn_click(lbtn, event.mouse, &cbtnl)) {
                        select = (select == 4) ? 0 : 4;
                    } else if(btn_click(rbtn, event.mouse, &cbtnr)) {
                        select = (select == 5) ? 0 : 5;
                    }
                    btn_click(xbtn, event.mouse, &cbtnx);
                    break;
                case ALLEGRO_EVENT_MOUSE_BUTTON_UP:
                    click = 0;
                    break;
                case ALLEGRO_EVENT_KEY_DOWN:
                    if(event.keyboard.keycode == ALLEGRO_KEY_SPACE) {
                        done = true;
                    } else if(event.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
                        select = 0;
                    } else if(event.keyboard.keycode == ALLEGRO_KEY_LSHIFT) {
                        lock_coords(&lock, &lx, &ly, mstate);
                    } else if(event.keyboard.keycode == ALLEGRO_KEY_TAB) {
                        grid = !grid;
                    } else if(event.keyboard.keycode == ALLEGRO_KEY_BACKSPACE) {
                        memset(map, empty, sizeof(map));
                    }
                    break;
                case ALLEGRO_EVENT_KEY_UP:
                    if(event.keyboard.keycode == ALLEGRO_KEY_LSHIFT) {
                        lock = 0;
                        dirx = 0;
                        diry = 0;
                    }
                    break;
                case ALLEGRO_EVENT_DISPLAY_CLOSE:
                    done = true;
                    break;
            }

            if(done || cbtnx == 1) {
                curr = menu;
                break;
            }

            if(click) {
                x = r_lim(0, mstate.x / 20, 95);
                y = r_lim(0, mstate.y / 20, 49);
                
                lock_handler(&lock, lx, ly, &x, &y, &dirx, &diry);

                wait = r_lim(0, wait, 20);

                click_handler(map, mstate, x, y, select, &wait);
            }

            if(redraw && al_is_event_queue_empty(queue)) {
                al_clear_to_color(bgcolor);
            
                draw_map(grid, map, font);

                btn_draw(abtn, font, &cbtna);
                btn_draw(nbtn, font, &cbtnn);
                btn_draw(fbtn, font, &cbtnf);
                btn_draw(lbtn, font, &cbtnl);
                btn_draw(rbtn, font, &cbtnr);
                btn_draw(xbtn, font, &cbtnx);
                
                text_select(select, font);

                // this red doesnt match up with wires (170 vs 200)

                al_draw_filled_rectangle(475, 1000, 485, 1080, al_map_rgb(170, 30, 30));
                al_draw_filled_rectangle(1435, 1000, 1445, 1080, al_map_rgb(170, 30, 30));
                
                al_flip_display();
                redraw = false;
            }
        }

        save_canvas(map);

        al_destroy_font(font);
        al_destroy_bitmap(abtn.bit);
        al_destroy_bitmap(nbtn.bit);
        al_destroy_bitmap(fbtn.bit);
        al_destroy_bitmap(lbtn.bit);
        al_destroy_bitmap(rbtn.bit);
        al_destroy_bitmap(xbtn.bit);
        al_flush_event_queue(queue);

        goto start;
    }
}