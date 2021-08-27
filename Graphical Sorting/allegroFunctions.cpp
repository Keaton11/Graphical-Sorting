/*  Name: Keaton Lee
 *  Date: June 18, 2021
 *  Title: Graphical Sorting
 *
 *  Description: Contains functions used to initialize and close allegro
*/

// imports header files
#include "header.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_native_dialog.h>

extern ALLEGRO_EVENT_QUEUE *event_queue;
extern ALLEGRO_DISPLAY *display;
extern ALLEGRO_FONT *basic_font;
extern ALLEGRO_FONT *title_font;
extern ALLEGRO_TIMER *timer;

/// Allegro Initialization Function

bool initializeAllegro() {
// function recieves no parameters
// allegro is initialzed
// displays error message if allegro fails to initialize and returns false
// returns true if allegro is correctly initialized

    // allegro is initialized
	al_init();

    // allegro display is initialized
    display = al_create_display(1280, 720);
    if (!display) {
        al_show_native_message_box(display, "Error", "Error", "Failed to initialize display", nullptr, ALLEGRO_MESSAGEBOX_ERROR);
        return false;
    }

    // title is added to display
    al_set_window_title(display, "Keaton Lee: ICS4U - Sorting Assignment");

    // primitives addon is initialized
    if (!al_init_primitives_addon()) {
    	al_show_native_message_box(display, "Error", "Error", "Failed to initialize primatives addon", nullptr, ALLEGRO_MESSAGEBOX_ERROR);
    	return false;
	}

	// font addon is initialized
    if (!al_init_font_addon()) {
        al_show_native_message_box(display, "Error", "Error", "Failed to initialize font add on", nullptr, ALLEGRO_MESSAGEBOX_ERROR);
        return false;
    }

    // ttf addon is initialized
    if (!al_init_ttf_addon()) {
        al_show_native_message_box(display, "Error", "Error", "Failed to initialize font ttf on", nullptr, ALLEGRO_MESSAGEBOX_ERROR);
        return false;
    }

    // basic font (kingthings-clarity) is initialized
    basic_font = al_load_ttf_font("kingthings-clarity.ttf", 22, 0);
    if (!basic_font){
        al_show_native_message_box(display, "Error", "Error", "Could not load kingthings-clarity.ttf", nullptr, ALLEGRO_MESSAGEBOX_ERROR);
        return false;
    }

    // title font (lemon-milk) is initialized
    title_font = al_load_ttf_font("lemon-milk.otf", 60, 0);
    if (!title_font){
        al_show_native_message_box(display, "Error", "Error", "Could not load lemon-milk.otf", nullptr, ALLEGRO_MESSAGEBOX_ERROR);
        return false;
    }

    // keyboard is initialized
    if (!al_install_keyboard()) {
        al_show_native_message_box(display, "Error", "Error", "Failed to initialize the keyboard", nullptr, ALLEGRO_MESSAGEBOX_ERROR);
        return false;
    }

    // mouse is initialized
    if (!al_install_mouse()){
        al_show_native_message_box(display, "Error", "Error", "Failed to initialize mouse", nullptr, ALLEGRO_MESSAGEBOX_ERROR);
        return false;
    }

    // event queue is created
    event_queue = al_create_event_queue();
    if (!event_queue) {
        al_show_native_message_box(display, "Error", "Error", "Failed to create event_queue", nullptr, ALLEGRO_MESSAGEBOX_ERROR);
        return false;
    }

    // timer is created
    timer = al_create_timer(1.0 / 20);
    if (!timer) {
        al_show_native_message_box(display, "Error", "Error", "Failed to create timer", nullptr, ALLEGRO_MESSAGEBOX_ERROR);
        return false;
    }

    // event sources are registered
    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue, al_get_mouse_event_source());

 	// timer is started
 	al_start_timer(timer);

    // returns true if allegro is correctly initialized
	return true;
}

/// Close Allegro Function

void closeAllegro() {
// function recieves no parameters
// function destroys display, fonts, timer, and event queue

    // display, fonts, timer, and event queue is destroyed
    al_destroy_display(display);
    al_destroy_font(title_font);
    al_destroy_font(basic_font);
    al_destroy_timer(timer);
    al_destroy_event_queue(event_queue);
}
