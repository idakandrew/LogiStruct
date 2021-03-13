#include <math.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_ttf.h>
#include "canvas.h"
#include "utils.h"
#include "sim.h"
#include "ui.h"

#define len(v) sizeof v / sizeof *v

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
            al_get_mouse_state(&mstate);

            switch(event.type) {
                case ALLEGRO_EVENT_TIMER:
                    redraw = true;
                    break;
                case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
                    for(int i = 0; i < len(mbtnlist); i++) {
                        btn_click(mbtnlist[i], event.mouse, &mcbtnlist[i]);
                    }
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
                al_draw_filled_rectangle(590, 0, 1330, 1080, nearblack);
                al_draw_bitmap(logo, 960 - al_get_bitmap_width(logo) / 2, 150, 0);
                al_draw_text(font, white, 960, 960, ALLEGRO_ALIGN_CENTER, "v0.3 (Feb 22, 2021)");
                al_draw_text(font, white, 960, 1000, ALLEGRO_ALIGN_CENTER, "By Andrew Idak");
                
                for(int i = 0; i < len(mbtnlist); i++) {
                    btn_draw(mbtnlist[i], font, &mcbtnlist[i], mstate);
                }

                al_flip_display();
                redraw = false;
            }
        }

        al_destroy_bitmap(logo);
        for(int i = 0; i < len(mbtnlist); i++) {
            al_destroy_bitmap(mbtnlist[i].bit);
        }
        al_destroy_font(font);
        al_flush_event_queue(queue);

        goto start;

    } else if(curr == settings) {
        int scbtn0 = 0, ignore = 0;

        ALLEGRO_FONT *font = al_load_ttf_font("data/mont.otf", 32, 0);
        int halfline = al_get_font_line_height(font) / 2;

        button sbtnlist[11] = {
            btn_build(250, 70, "Menu", "data/new.png"), btn_build(250, 330, "L Mouse", "data/new.png"), 
            btn_build(250, 470, "R Mouse", "data/new.png"), btn_build(250, 610, "L Shift", "data/new.png"), 
            btn_build(250, 750, "Tab", "data/new.png"), btn_build(250, 890, "Esc", "data/new.png"), 
            btn_build(1200, 330, "Backspace", "data/new.png"), btn_build(1200, 470, "Space + L/R Mouse", "data/new.png"),
            btn_build(1200, 610, "Mouse Wheel", "data/new.png"), btn_build(1200, 750, "R", "data/new.png"),
            btn_build(1200, 890, "Q", "data/new.png")
        };

        char const *textlist[10] = {
            "Place wire and objects.", "Erase wire and objects.", "Lock placement axis.", 
            "Toggle grid overlay.", "Deselect current object.", "Clear canvas.", 
            "Hold & drag to pan view.", "Toggle zoom level.", "Rotate gates horizontally.",
            "Select part on canvas."
        };

        int xlist[10] = {490, 490, 490, 490, 490, 1440, 1440, 1440, 1440, 1440};
        int ylist[10] = {330, 470, 610, 750, 890, 330, 470, 610, 750, 890};

        while(1) {
            al_wait_for_event(queue, &event);
            al_get_mouse_state(&mstate);

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

                al_draw_filled_rectangle(0, 0, 1920, 140, nearblack);
                al_draw_filled_rectangle(20, 240, 950, 980, nearblack);
                al_draw_filled_rectangle(970, 240, 1900, 980, nearblack);

                for (int i = 0; i < len(sbtnlist); i++) {
                    btn_draw(sbtnlist[i], font, (i == 0) ? &scbtn0 : &ignore, mstate);
                }

                for(int i = 0; i < len(textlist); i++) {
                    al_draw_text(font, white, xlist[i], ylist[i] - halfline, 0, textlist[i]);
                }

                al_flip_display();
                redraw = false;
            }
        }

        for(int i = 0; i < len(sbtnlist); i++) {
            al_destroy_bitmap(sbtnlist[i].bit);
        }
        al_destroy_font(font);
        al_flush_event_queue(queue);

        goto start;

    } else if (curr == canvas) {
        bool grid = false, pan = false, click = false, pen = true, ask = false;
        int wait = 0, lock = -1, select = -1, option = 0;
        int x = 0, y = 0, prevx = 0, prevy = 0, lx = 0, ly = 0;
        int cx = 500, cy = 499;
        int zm = 1, prevz = 0;
        int page = 0, rot = 1;
        al_set_mouse_z(0);
        static int map[MAP_X][MAP_Y];
        load_canvas(map);

        ALLEGRO_FONT *fontlrg = al_load_ttf_font("data/mont.otf", 26, 0);
        ALLEGRO_FONT *fontsml = al_load_ttf_font("data/mont.otf", 13, 0);
        ALLEGRO_FONT *fontmin = al_load_ttf_font("data/mont.otf", 7, 0);

        int ccbtnlist[8] = {0, 0, 0, 0, 0, 0, 0};
        int nopgclk[4] = {0, 0, 0, 0};

        button cbtnlist[8] = {
            btn_build(660, 1040, "NAND", "data/select.png"), btn_build(810, 1040, "NOR", "data/select.png"), 
            btn_build(960, 1040, "Switch", "data/select.png"), btn_build(1110, 1040, "Light", "data/select.png"), 
            btn_build(1260, 1040, "Crossing", "data/select.png"), btn_build(660, 1040, "Bridge G", "data/select.png"),
            btn_build(810, 1040, "Bridge B", "data/select.png"), btn_build(960, 1040, "8-Seg", "data/select.png")
        };

        button nopglst[4] = {
            btn_build(1840, 1040, "Menu", "data/select.png"), btn_build(550, 1040, "<", "data/next.png"), 
            btn_build(1370, 1040, ">", "data/next.png"), btn_build(1690, 1040, " ", "data/select.png")
        };

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
                    if(mtrx_range(mstate.x, mstate.y, 0, 1920, 0, 1000)) {
                        click = true;
                    } else {
                        for(int i = 0 + 5 * page; i < 5 + 5 * page; i++) {
                            if(i < len(cbtnlist)) {
                                if(btn_click(cbtnlist[i], event.mouse, &ccbtnlist[i])) {
                                    select = i;
                                }
                            }
                        }
                        for(int i = 0; i < len(nopglst); i++) {
                            if(btn_click(nopglst[i], event.mouse, &nopgclk[i])) {
                                option = i;
                            }
                        }
                        switch(option) {
                            case 1:
                                page = r_lim(0, --page, 1);
                                break;
                            case 2:
                                page = r_lim(0, ++page, 1);
                                break;
                            case 3:
                                pen = !pen;
                                break;
                        }
                        option = 0;
                    }
                    break;
                case ALLEGRO_EVENT_MOUSE_BUTTON_UP:
                    click = 0;
                    if(lock > -1) {
                        lock = 0;
                    }
                    break;
                case ALLEGRO_EVENT_MOUSE_AXES:
                    if(event.mouse.z > prevz) {
                        zm = r_lim(1, zm / 2, 4);
                    } else if(event.mouse.z < prevz) {
                        zm = r_lim(1, zm * 2, 4);
                    }
                    if(pan && click || event.mouse.z != prevz) {
                        cx = r_lim(0 + zm_adj(0, zm), cx - (event.mouse.x / (20 / zm) - prevx / (20 / zm)), MAP_X - VIEW_X - zm_adj(0, zm));
                        cy = r_lim(0 + zm_adj(1, zm), cy - (event.mouse.y / (20 / zm) - prevy / (20 / zm)), MAP_Y - VIEW_X - zm_adj(1, zm));
                    }
                    prevx = event.mouse.x;
                    prevy = event.mouse.y;
                    prevz = event.mouse.z;
                    break;
                case ALLEGRO_EVENT_KEY_DOWN:
                    if(event.keyboard.keycode == ALLEGRO_KEY_SPACE) {
                        pan = true;
                    } else if(event.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
                        select = -1;
                    } else if(event.keyboard.keycode == ALLEGRO_KEY_LSHIFT) {
                        lock = 0;
                    } else if(event.keyboard.keycode == ALLEGRO_KEY_TAB) {
                        grid = !grid;
                    } else if(event.keyboard.keycode == ALLEGRO_KEY_BACKSPACE) {
                        ask = !ask;
                    } else if(event.keyboard.keycode == ALLEGRO_KEY_ENTER) {
                        if(ask) {
                            memset(map, empty, sizeof(map));
                            ask = false;
                        }
                    } else if(event.keyboard.keycode == ALLEGRO_KEY_R) {
                        rot = (rot == 1) ? -1 : 1;
                    } else if(event.keyboard.keycode == ALLEGRO_KEY_Q) {
                        select = part_picker(map, mstate, cx, cy, zm);
                    }
                    break;
                case ALLEGRO_EVENT_KEY_UP:
                    if(event.keyboard.keycode == ALLEGRO_KEY_LSHIFT) {
                        lock = -1;
                    } else if(event.keyboard.keycode == ALLEGRO_KEY_SPACE) {
                        pan = false;
                    }
                    break;
                case ALLEGRO_EVENT_DISPLAY_CLOSE:
                    done = true;
                    break;
            }

            if(done || nopgclk[0] == 1) {
                curr = menu;    
                break;
            }

            if(click && !pan) {
                x = mstate.x / (20 / zm) + cx - zm_adj(0, zm);
                y = mstate.y / (20 / zm) + cy - zm_adj(1, zm);

                lock_axis(zm, &lock, &x, &y, lx, ly);

                wait = r_lim(0, wait, 20);

                click_handler(map, mstate, x, y, select, &wait, pen, rot);

                lx = x;
                ly = y;
            }

            if(redraw && al_is_event_queue_empty(queue)) {
                al_clear_to_color(bgcolor);
            
                draw_map(zm, grid, map, cx, cy, (zm == 1) ? fontlrg : (zm == 2) ? fontsml : fontmin);

                draw_ghost(select, cbtnlist, mstate.x, mstate.y, (zm == 1) ? fontlrg : (zm == 2) ? fontsml : fontmin, zm, rot);

                al_draw_filled_rectangle(0, 1000, 1920, 1080, nearblack);

                for(int i = 0 + 5 * page; i < 5 + 5 * page; i++) {
                    if(i < len(cbtnlist)) {
                        btn_draw(cbtnlist[i], fontlrg, &ccbtnlist[i], mstate);
                    }
                }
                for(int i = 0; i < len(nopglst); i++) {
                    btn_draw(nopglst[i], fontlrg, &nopgclk[i], mstate);
                }
                
                toolbar_text(select, cx, cy, fontlrg, pen);

                launch_codes(ask, fontlrg);
                
                al_flip_display();
                redraw = false;
            }
        }

        save_canvas(map);

        al_destroy_font(fontlrg);
        al_destroy_font(fontsml);
        for(int i = 0; i < len(cbtnlist); i++) {
            al_destroy_bitmap(cbtnlist[i].bit);
        }
        for(int i = 0; i < len(nopglst); i++) {
            al_destroy_bitmap(nopglst[i].bit);
        }
        al_flush_event_queue(queue);

        goto start;
    }
}