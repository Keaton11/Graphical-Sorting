/*  Name: Keaton Lee
 *  Date: June 18, 2021
 *  Title: Graphical Sorting
 *
 *  Description: Holds the implementations of the methods associated with the
 *  Button class
*/

// includes header file
#include "Button.h"

/// Constructor
// parameterized constructor
Button::Button(string t, int l, int w, float x, float y, ALLEGRO_COLOR c, ALLEGRO_COLOR tc) {
    // variables are set based on parameter values
    text = t;
    length = l;
    width = w;
    xPos = x;
    yPos = y;
    colour = c;
    textColour = tc;

    hoverColour = findHoverColour(c);   // static method is called to set hover colour based on button colour
    hover = true;   // hover is set to true by default
    border = false; // border is set to false by default
}

/// Setters (Mutators)
// recieves a string (t) as parameters
// sets text (message that will appear on button)
void Button::setText(string t) {
    text = t;
}
// recieves a boolean (h) as parameters
// sets hover boolean (which determines whether button should respond to hovering)
void Button::setHover(bool h) {
    hover = h;
}
// recieves an allegro colour (c) as parameters
// sets colour of button
void Button::setColour(ALLEGRO_COLOR c) {
    colour = c;
    hoverColour = findHoverColour(c);
}
// recieves a boolean (s) as parameters
// sets whether button is selected
void Button::setSelected(bool s) {
    selected = s;
}
// recieves a boolean (b) as parameters
// sets whether button is selected
void Button::setBorder(bool b) {
    border = b;
}
// recieves a float (x) as parameters
// sets x position of button
void Button::setXPos(float x) {
    xPos = x;
}
// recieves a float (y) as parameters
// sets y position of button
void Button::setYPos(float y) {
    yPos = y;
}
// recieves circle variables as parameters
// set radius, x and y coordinates, and colour for circle button
void Button::setCircle(float r, float x, float y, ALLEGRO_COLOR c) {
    radius = r;
    xPos = x;
    yPos = y;
    colour = c;

    selected = false;   // selected is set to false by default
}
// recieves triangle variables as parameters
// sets base and height, direction, x and y coordinates, and colour for triangle button
void Button::setTriangle(float b, float h, int d, float x, float y, ALLEGRO_COLOR c) {
    base = b;
    height = h;
    direction = d;

    xPos = x;
    yPos = y;
    colour = c;
    hoverColour = findHoverColour(c);   // static method is called to set hover colour based on button colour
    hover = true;   // hover is set to true by default
}

/// Getters (Accessors)
// recieves no parameters
// returns text as a string
string Button::getText() {
    return text;
}
// recieves no parameters
// returns lastHover (used to determine whether user was hovering over button) as a boolean
bool Button::getLastHover() {
    return lastHover;
}
// recieves no parameters
// returns colour as an ALLEGRO_COLOR
ALLEGRO_COLOR Button::getColour() {
    return colour;
}

/// Method Implementations: Drawing
// recieves an allegro event as parameters
// draws button (colour depends on whether user is hovering over it)
void Button::draw(ALLEGRO_EVENT &ev) {
    if (ev.mouse.x > xPos && ev.mouse.x < xPos + length && ev.mouse.y > yPos && ev.mouse.y < yPos + width && hover) {
        // if user is hovering over the button, the button is drawn in the hover colour
        // if hover is set to false, the button is not drawn in the hover colour
        al_draw_filled_rounded_rectangle(xPos - 2, yPos - 2, xPos + length + 2, yPos + width + 2, 2, 2, hoverColour);
        if (border) {
            // if border is set to true, button is drawn with a border
            al_draw_rounded_rectangle(xPos - 2, yPos - 2, xPos + length + 2, yPos + width + 2, 2, 2, LAVENDER, 2);
        }
        lastHover = true;   // updates lastHover
    } else {
        // if user is not hovering over the button, the button is drawn in the button's colour
        al_draw_filled_rounded_rectangle(xPos, yPos, xPos + length, yPos + width, 2, 2, colour);
        if (border) {
            // if border is set to true, button is drawn with a border
            al_draw_rounded_rectangle(xPos, yPos, xPos + length, yPos + width, 2, 2, LAVENDER, 2);
        }
        lastHover = false;  // updates lastHover
    }
    // text is drawn centered on button in the text colour
    al_draw_text(basic_font, textColour, xPos + length / 2, yPos + width / 2 - 16, ALLEGRO_ALIGN_CENTRE, text.c_str());
}
// recieves no parameters
// draws button (colour depends on whether user was hovering over it when it was last drawn)
void Button::drawLast() {
    if (lastHover && hover) {
        // if user is hovering over the button, the button is drawn in the hover colour
        // if hover is set to false, the button is not drawn in the hover colour
        al_draw_filled_rounded_rectangle(xPos - 2, yPos - 2, xPos + length + 2, yPos + width + 2, 2, 2, hoverColour);
    } else {
        // if user is not hovering over the button, the button is drawn in the button's colour
        al_draw_filled_rounded_rectangle(xPos, yPos, xPos + length, yPos + width, 2, 2, colour);
    }
    // text is drawn centered on button in the text colour
    al_draw_text(basic_font, textColour, xPos + length / 2, yPos + width / 2 - 16, ALLEGRO_ALIGN_CENTRE, text.c_str());
}
// recieves an allegro event as parameters
// draws circle button (colour depends on whether user is hovering over it or if it was selected)
void Button::drawCircle(ALLEGRO_EVENT &ev) {
    if (selected) {
        // if selected circle button is drawn with a green border
        al_draw_filled_circle(xPos, yPos, radius + 4, GREEN);
        al_draw_filled_circle(xPos, yPos, radius, colour);
    } else if (ev.mouse.x > xPos - radius && ev.mouse.x < xPos + radius && ev.mouse.y > yPos - radius && ev.mouse.y < yPos + radius) {
        // if user is hovering over the button, the button is drawn larger
        // button is drawn with a black border
        al_draw_filled_circle(xPos, yPos, radius + 3, colour);
        al_draw_circle(xPos, yPos, radius + 3, BLACK, 2);
    } else {
        // if user is not hovering over the button, the button is drawn in its regular size
        // button is drawn with a black border
        al_draw_filled_circle(xPos, yPos, radius, colour);
        al_draw_circle(xPos, yPos, radius, BLACK, 2);
    }
}
// recieves an allegro event as parameters
// draws triangle button (colour depends on whether user is hovering over it)
void Button::drawTriangle(ALLEGRO_EVENT &ev) {
    if (direction == 0) {
    // triangle button is drawn based on its direction
    // 0 corresponds to right / left, whereas 1 corresponds to up / down
    // coordinates are calculated accordingly
    // all triangle buttons are drawn with a lavender border
        if (!hover) {
            // if hover is set to false, button is drawn in grey
            al_draw_filled_triangle(xPos, yPos, xPos + height, yPos - (base / 2), xPos + height, yPos + (base / 2), GREY);
            al_draw_triangle(xPos, yPos, xPos + height, yPos - (base / 2), xPos + height, yPos + (base / 2), LAVENDER, 2);
        } else if (((ev.mouse.x > xPos && ev.mouse.x < xPos + height && height > 0) || (ev.mouse.x > xPos + height && ev.mouse.x < xPos && height < 0)) && ev.mouse.y > yPos - (base / 2) && ev.mouse.y < yPos + (base / 2)) {
            // if user is hovering over the button, the button is drawn in its hover colour
            al_draw_filled_triangle(xPos, yPos, xPos + height, yPos - (base / 2), xPos + height, yPos + (base / 2), hoverColour);
            al_draw_triangle(xPos, yPos, xPos + height, yPos - (base / 2), xPos + height, yPos + (base / 2), LAVENDER, 3);
        } else {
            // if user is not hovering over the button, the button is drawn in its default colour
            al_draw_filled_triangle(xPos, yPos, xPos + height, yPos - (base / 2), xPos + height, yPos + (base / 2), colour);
            al_draw_triangle(xPos, yPos, xPos + height, yPos - (base / 2), xPos + height, yPos + (base / 2), LAVENDER, 2);
        }
    } else {
        if (((ev.mouse.y > yPos && ev.mouse.y < yPos + height && height > 0) || (ev.mouse.y > yPos + height && ev.mouse.y < yPos && height < 0)) && ev.mouse.x > xPos - (base / 2) && ev.mouse.x < xPos + (base / 2)) {
            // if user is hovering over the button, the button is drawn in its hover colour
            al_draw_filled_triangle(xPos, yPos, xPos - (base / 2), yPos + height, xPos + (base / 2), yPos + height, hoverColour);
            al_draw_triangle(xPos, yPos, xPos - (base / 2), yPos + height, xPos + (base / 2), yPos + height, LAVENDER, 3);
        } else {
            // if user is not hovering over the button, the button is drawn in its default colour
            al_draw_filled_triangle(xPos, yPos, xPos - (base / 2), yPos + height, xPos + (base / 2), yPos + height, colour);
            al_draw_triangle(xPos, yPos, xPos - (base / 2), yPos + height, xPos + (base / 2), yPos + height, LAVENDER, 2);
        }
    }
}
// recieves an allegro event as parameters
// draws scroll button (colour depends on whether user is hovering over it)
void Button::drawScroll(ALLEGRO_EVENT &ev) {
    // similar to default draw function, but scroll button does not expand when the user is hovering over it
    if (ev.mouse.x > xPos && ev.mouse.x < xPos + length && ev.mouse.y > yPos && ev.mouse.y < yPos + width && hover) {
        // if user is hovering over the button, the button is drawn in its hover colour
        al_draw_filled_rounded_rectangle(xPos, yPos, xPos + length, yPos + width, 2, 2, hoverColour);
        al_draw_rounded_rectangle(xPos, yPos, xPos + length, yPos + width, 2, 2, BLACK, 2);
    } else {
        // if user is not hovering over the button, the button is drawn in its default colour
        al_draw_filled_rounded_rectangle(xPos, yPos, xPos + length, yPos + width, 2, 2, colour);
        al_draw_rounded_rectangle(xPos, yPos, xPos + length, yPos + width, 2, 2, BLACK, 2);
    }
}

/// Method Implementations: Checking if Clicked
// recieves an allegro event as parameters
// determines whether the button has been clicked
// returns true if button was clicked and false if not
bool Button::checkClicked(ALLEGRO_EVENT &ev) {
    if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP) {
        if (ev.mouse.x > xPos && ev.mouse.x < xPos + length && ev.mouse.y > yPos && ev.mouse.y < yPos + width && hover) {
            // returns true if button was clicked and hover is true
            return true;
        }
    }
    // returns false if button was not clicked or hover is false
    return false;
}
// recieves an allegro event as parameters
// determines whether a circle button has been clicked
// returns true if button was clicked and false if not
bool Button::checkClickedCircle(ALLEGRO_EVENT &ev) {
    if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP) {
        if (ev.mouse.x > xPos - radius && ev.mouse.x < xPos + radius && ev.mouse.y > yPos - radius && ev.mouse.y < yPos + radius) {
            // returns true if button was clicked
            selected = true;
            return true;
        }
    }
    // returns false if button was not clicked
    return false;
}
// recieves an allegro event as parameters
// determines whether a triangle button has been clicked
// returns true if button was clicked and false if not
bool Button::checkClickedTriangle(ALLEGRO_EVENT &ev) {
    if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP) {
        if (direction == 0) {
        // checks if triangle button was clicked based on its direction, base, height, and coordinates
            if (((ev.mouse.x > xPos && ev.mouse.x < xPos + height && height > 0) || (ev.mouse.x > xPos + height && ev.mouse.x < xPos && height < 0)) && ev.mouse.y > yPos - (base / 2) && ev.mouse.y < yPos + (base / 2)) {
                // returns true if button was clicked
                return true;
            }
        } else {
            if (((ev.mouse.y > yPos && ev.mouse.y < yPos + height && height > 0) || (ev.mouse.y > yPos + height && ev.mouse.y < yPos && height < 0)) && ev.mouse.x > xPos - (base / 2) && ev.mouse.x < xPos + (base / 2)) {
                // returns true if button was clicked
                return true;
            }
        }
    }
    // returns false if button was not clicked
    return false;
}

/// Method Implementations: Setting Hover Colour
// recieves an allegro colour as parameters
// method uses rgb values from colour to produce a darker version of such colour
// returns darker version of initial colour (used for a button's hover colour)
ALLEGRO_COLOR Button::findHoverColour(ALLEGRO_COLOR c) {
    // vector is declared (used to hold RGB values of an allegro colour)
    vector<unsigned char> hColourRBG(3);

    // the rgb values of the colour from the parameters are unmapped into the unsigned char vector
    al_unmap_rgb(c, &hColourRBG[0], &hColourRBG[1], &hColourRBG[2]);

    for (int i = 0; i < 3; i++) {
        // each rgb value of the colour is decreased by 30 (to darken the colour)
        hColourRBG[i] -= 30;
        if (hColourRBG[i] < 0) {
            // if an rgb value is decreased to below 0, it is set to 0;
            hColourRBG[i] = 0;
        }
    }

    // a darker version of the initial colour (used for a button's hover colour) is returned
    return al_map_rgb(hColourRBG[0], hColourRBG[1], hColourRBG[2]);
}
