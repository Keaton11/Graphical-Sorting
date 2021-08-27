/*  Name: Keaton Lee
 *  Date: June 18, 2021
 *  Title: Graphical Sorting
 *
 *  Description: Protoype for Button class
*/

// checks if header file was defined to avoid double inclusions
#ifndef BUTTON_H
#define BUTTON_H

// imports header files
#include "colours.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_native_dialog.h>
#include <string>
#include <vector>

using namespace std;

extern ALLEGRO_EVENT_QUEUE *event_queue;
extern ALLEGRO_DISPLAY *display;
extern ALLEGRO_FONT *basic_font;

class Button {
    public:

        /// Constructors
        Button(string t, int l, int w, float x, float y, ALLEGRO_COLOR c, ALLEGRO_COLOR tc);
        Button() {};

        /// Setters (Mutators)
        void setText(string t);
        void setColour(ALLEGRO_COLOR c);
        void setHover(bool h);
        void setSelected(bool s);
        void setBorder(bool b);
        void setXPos(float x);
        void setYPos(float y);
        void setCircle(float r, float x, float y, ALLEGRO_COLOR c);
        void setTriangle(float b, float h, int d, float x, float y, ALLEGRO_COLOR c);

        /// Getters (Accessors)
        string getText();
        bool getLastHover();
        ALLEGRO_COLOR getColour();

        /// Public Method Prototypes: Drawing
        void draw(ALLEGRO_EVENT &ev);
        void drawLast();
        void drawCircle(ALLEGRO_EVENT &ev);
        void drawTriangle(ALLEGRO_EVENT &ev);
        void drawScroll(ALLEGRO_EVENT &ev);

        /// Public Method Protypes: Checking if a Button was Clicked
        bool checkClicked(ALLEGRO_EVENT &ev);
        bool checkClickedCircle(ALLEGRO_EVENT &ev);
        bool checkClickedTriangle(ALLEGRO_EVENT &ev);

        /// Public Method Prototype: Setting Hover Colour
        static ALLEGRO_COLOR findHoverColour(ALLEGRO_COLOR c);

    private:

        /// Private Variables
        int length;     // length of the button
        int width;      // width of the button
        float radius;   // radius of circle button
        float base;     // base of triangle button
        float height;   // height of triangle button
        int direction;  // direction of triangle button (point facing up/down or right/left)

        float xPos;     // x position of button (float is rounded when drawn)
        float yPos;     // y position of button (float is rounded when drawn)

        string text;    // text written on button

        bool hover;     // determines whether button should respond to hovering
        bool lastHover; // corresponds to last drawing of button (if the user was hovering or not)
        bool selected;  // corresponds to whether or not a circle button is selected
        bool border;    // determins whether a border should be drawn on a button

        ALLEGRO_COLOR colour;       // colour of button
        ALLEGRO_COLOR textColour;   // colout of text on button
        ALLEGRO_COLOR hoverColour;  // colour of button when user is hovering over it
};

#endif // BUTTON_H
