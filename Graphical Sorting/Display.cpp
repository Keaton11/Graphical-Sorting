/*  Name: Keaton Lee
 *  Date: June 18, 2021
 *  Title: Graphical Sorting
 *
 *  Description: Holds the implementations of the methods associated with the
 *  Display class
*/

// includes header file
#include "Display.h"

/// Constructor
// recieves no parameters
Display::Display() {
    exit = false;   // exit is set to false by default
}

/// Method Implementations: User Input - Number
// recieves minimum and maximum number as parameters (these serve as bounds for user input)
// recieves user input from keyboard
// returns true if next button was clicked and false if cancel button was clicked
// returns false and sets exit to true if allegro display was closed
bool Display::inputNumber(int minNum, int maxNum) {
    // string for input number is reset
    inputNum = "";
    char input;     // variable is declared to recieve keyboard input using Allegro

    //semi-transparent rectangle is drawn over the entire display to give the effect of the background fading
    al_draw_filled_rectangle(0, 0, 1280, 720, FADE);

    // buttons are declared
    Button next("Next", 100, 30, 700, 412.5, IRIS, LAVENDER);
    Button cancel("Cancel", 100, 30, 480, 412.5, IRIS, LAVENDER);

    ALLEGRO_EVENT ev;
    // timer is registered to the event queue (used to draw a flashing vertical line to give the effect of typing)
    al_register_event_source(event_queue, al_get_timer_event_source(timer));

    while (true) {
        // input box is added to the allegro display (surrounded by an iris border)
        al_draw_filled_rounded_rectangle(460, 257.5, 820, 462.5, 3, 3, IRIS);
        al_draw_filled_rounded_rectangle(470, 267.5, 810, 452.5, 3, 3, WHITE);

        // message is added to the allegro display (along with bonds for user input)
        al_draw_text(basic_font, IRIS, 640, 277.5, ALLEGRO_ALIGN_CENTRE, "Enter a Number Between");
        al_draw_textf(basic_font, IRIS, 640, 307.5, ALLEGRO_ALIGN_CENTRE, "%d and %d:", minNum, maxNum);
        al_draw_filled_rectangle(500, 347.5, 780, 397.5, LAVENDER);

        // inputNum (number that the user has typed) is added to the allegro display
        al_draw_text(basic_font, IRIS, 640, 357.5, ALLEGRO_ALIGN_CENTRE, (inputNum).c_str());

        if (inputNum.size() == 0) {
            // if no number has been inputted, the next button is set to grey
            next.setColour(GREY);
            next.setHover(false);
        } else if (stoi(inputNum) >= minNum && stoi(inputNum) <= maxNum) {
            // if the inputted number is outside the bounds, the next button is set to grey
            next.setColour(IRIS);
            next.setHover(true);
        } else {
            // if the inputted number is valid, the next button is set to iris
            next.setColour(GREY);
            next.setHover(false);
        }

        if (ev.type == ALLEGRO_EVENT_TIMER) {
            if (!next.getLastHover() && !cancel.getLastHover()) {
                if ((al_get_timer_count(timer) / 10) % 2 == 0) {
                    // "|" is used as a flashing vertical line to give the effect of typing
                    // "|" is added to the display 50% of the time so it appears flashing
                    if (inputNum.size() > 0) {
                        // "|" is drawn after the number that has been typed
                        al_draw_text(basic_font, IRIS, 643 + al_get_text_width(basic_font, (inputNum).c_str()) / 2, 357.5, ALLEGRO_ALIGN_CENTRE, "|");
                    } else {
                        // if no numbers have been typed, "|" is centered
                        al_draw_text(basic_font, IRIS, 640, 357.5, ALLEGRO_ALIGN_CENTRE, "|");
                    }
                }
            }
            // buttons are drawn (based on whether user was hovering over them)
            next.drawLast();
            cancel.drawLast();
        } else {
            // buttons are drawn
            next.draw(ev);
            cancel.draw(ev);
        }

        al_flip_display();

        if (ev.type == ALLEGRO_EVENT_KEY_CHAR) {
            // method is called to recieve keyboard input
            input = getInputNumber(ev);
            if (input != '\0') {
                // '\0' corresponds to an invalid input, so no updates are made if the input is '\0'
                if (input == '\b') {
                    // '\b' corresponds to a backspace, so inputNum is decreased by 1, starting at the right end
                    if (inputNum.size() > 0) {
                        inputNum = inputNum.substr(0, inputNum.size() - 1);
                    }
                } else if (inputNum.size() < 20) {
                    // if user input was a number, it is added to inputNum
                    inputNum += getInputNumber(ev);
                }
            }
        }

        if (next.checkClicked(ev)) {
            // if next is clicked, choice number is updated, and true is returned
            // next cannot be clicked if no number has been entered or if the inputted number is outside the bounds
            choiceNum = stoi(inputNum);
            return true;
        } else if (cancel.checkClicked(ev)) {
            // if the cancel button was clicked, false is returned
            return false;
        } else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            // if allegro display was closed, exit is set to true, and false is returned
            exit = true;
            return false;
        }
        al_wait_for_event(event_queue, &ev);
    }
}
// recieves an allegro event as parameters
// returns a character corresponding to user input
char Display::getInputNumber(ALLEGRO_EVENT &ev) {
    if (ev.keyboard.keycode >= ALLEGRO_KEY_0 && ev.keyboard.keycode <= ALLEGRO_KEY_9) {
        // if a valid character (a number betwen 0 and 9) was entered, that number is returned
        return char('0' + (ev.keyboard.keycode - ALLEGRO_KEY_0));
    } else if (ev.keyboard.keycode == ALLEGRO_KEY_BACKSPACE) {
        // if backspace was clicked, '\b' is returned
        return '\b';
    }
    // if an invalid character was clicked, '\0' is returned
    return '\0';
}

/// Method Implementations: User Input - Colours
// recieves no parameters
// recieves user input based on the colours the user has selected
// returns true if next button was clicked and false if cancel button was clicked
// returns false and sets exit to true if allegro display was closed
bool Display::inputColours() {

    // varaible used to keep track of selected colours
    int selected[3] = {-1, -1, -1};

    // semi-transparent rectangle is drawn over the entire display to give the effect of the background fading
    al_draw_filled_rectangle(0, 0, 1280, 720, FADE);

    // buttons are declared
    Button next("Next", 100, 30, 755, 467.5, GREY, LAVENDER);
    Button cancel("Cancel", 100, 30, 425, 467.5, IRIS, LAVENDER);

    // by default, hover for next button is set to false
    next.setHover(false);

    // buttons are declared
    Button colourButton[3][9];
    for (int i = 0; i < 3; ++i) {
        // three sets of nine buttons (of different colours) are declared
        colourButton[i][0].setCircle(10, 430 + 170 * i, 327.5, RED);
        colourButton[i][1].setCircle(10, 470 + 170 * i, 327.5, ORANGE);
        colourButton[i][2].setCircle(10, 510 + 170 * i, 327.5, YELLOW);
        colourButton[i][3].setCircle(10, 430 + 170 * i, 367.5, DARK_BLUE);
        colourButton[i][4].setCircle(10, 470 + 170 * i, 367.5, BLUE);
        colourButton[i][5].setCircle(10, 510 + 170 * i, 367.5, CYAN);
        colourButton[i][6].setCircle(10, 430 + 170 * i, 407.5, IRIS);
        colourButton[i][7].setCircle(10, 470 + 170 * i, 407.5, PURPLE);
        colourButton[i][8].setCircle(10, 510 + 170 * i, 407.5, MAGENTA);
    }

    ALLEGRO_EVENT ev;
    al_unregister_event_source(event_queue, al_get_timer_event_source(timer));

    while (true) {

        if (selected[0] != -1 && selected[1] != -1 && selected[2] != -1 ) {
            // all three colours need to be selected for the user to click next button
            // once they have been selected, the button colour is set to iris
            next.setColour(IRIS);
            next.setHover(true);
        }

        // input box is added to the allegro display (surrounded by an iris border)
        al_draw_filled_rounded_rectangle(375, 202.5, 905, 517.5, 3, 3, IRIS);
        al_draw_filled_rounded_rectangle(385, 212.5, 895, 507.5, 3, 3, WHITE);

        // title is added to the allegro display
        al_draw_text(basic_font, IRIS, 640, 222.5, ALLEGRO_ALIGN_CENTRE, "Select Three Colours");

        // next and cancel buttons are drawn
        next.draw(ev);
        cancel.draw(ev);

        for (int i = 0; i < 3; i++) {
            // sub titles are added to the allegro display
            al_draw_filled_rectangle(395 + 170 * i, 262.5, 545 + 170 * i, 452.5, LAVENDER);
            al_draw_text(basic_font, IRIS, 470 + 170 * i, 267.5, ALLEGRO_ALIGN_CENTRE, ("Colour " + to_string(i + 1)).c_str());
        }

        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 9; ++j) {
                // colour buttons are drawn
                colourButton[i][j].drawCircle(ev);
            }
        }

        al_flip_display();

        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 9; ++j) {
                if (colourButton[i][j].checkClickedCircle(ev)) {
                    // if a colour button is selected, the button is set to selected
                    selected[i] = j;
                    for (int k = 0; k < 9; ++k) {
                        if (k != j)
                            // the rest of the buttons are "unselected" (by setting selected to false)
                            colourButton[i][k].setSelected(false);
                    }
                }
            }
        }

        if (next.checkClicked(ev)) {
            // if next is clicked, chosen colours are updated, and true is returned
            choiceColour[0] = colourButton[0][selected[0]].getColour();
            choiceColour[1] = colourButton[1][selected[1]].getColour();
            choiceColour[2] = colourButton[2][selected[2]].getColour();
            return true;
        } else if (cancel.checkClicked(ev)) {
            // if the cancel button was clicked, false is returned
            return false;
        } else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            // if allegro display was closed, exit is set to true, and false is returned
            exit = true;
            return false;
        }
        al_wait_for_event(event_queue, &ev);
    }
}

/// Method Implementations: User Input - Button
// recieves three strings as parameters
// recieves user input based on the button the user clicks
// returns an integer corresponding to the button clicked
// -1 is returned if cancel was clicked or the allegro display was closed
int Display::inputButton(string b1, string b2, string b3) {

    // semi-transparent rectangle is drawn over the entire display to give the effect of the background fading
    al_draw_filled_rectangle(0, 0, 1280, 720, FADE);

    // buttons are declared (using the strings given in the parameters
    Button button1(b1, 180, 50, 550, 260, IRIS, LAVENDER);
    Button button2(b2, 180, 50, 550, 330, IRIS, LAVENDER);
    Button button3(b3, 180, 50, 550, 400, IRIS, LAVENDER);

    // cancel button is declared
    Button cancel("Cancel", 100, 30, 590, 475, IRIS, LAVENDER);

    ALLEGRO_EVENT ev;
    al_unregister_event_source(event_queue, al_get_timer_event_source(timer));

    while (true) {

        // input box is added to the allegro display (surrounded by an iris border)
        al_draw_filled_rounded_rectangle(480, 195, 800, 525, 3, 3, IRIS);
        al_draw_filled_rounded_rectangle(490, 205, 790, 515, 3, 3, WHITE);

        // title is added to the allegro display
        al_draw_text(basic_font, IRIS, 640, 215, ALLEGRO_ALIGN_CENTRE, "Select an Option");

        // buttons are drawn
        button1.draw(ev);
        button2.draw(ev);
        button3.draw(ev);
        cancel.draw(ev);

        al_flip_display();

        // integer corresponding to the button clicked is returned
        if (button1.checkClicked(ev)) {
            return 0;
        } else if (button2.checkClicked(ev)) {
            return 1;
        } else if (button3.checkClicked(ev)) {
            return 2;
        } else if (cancel.checkClicked(ev)) {
            // if cancel was clicked, -1 is returned
            return -1;
        } else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            // if the allegro display was closed, exit is set to true, and -1 is returned
            exit = true;
            return -1;
        }

        al_wait_for_event(event_queue, &ev);
    }
}

/// Method Implementations: Draw - Arrays
// recieves a vector containing integers as input
// draws the array on the allegro display
void Display::drawDataArray(vector<int> unsortedData) {

    // data is copied into another vector and then sorted
    // two vectors now exist (one containing the unsorted data and another containing the sorted data)
    vector<int> sortedData = unsortedData;
    std::sort(sortedData.begin(), sortedData.end());

    // percentOffset is calculated (used for the scroll bar)
    float percentOffset;
    if (unsortedData.size() > 10) {
        percentOffset = 255.0 / (unsortedData.size() - 10);
    } else {
        percentOffset = 255;
    }

    // variables are declared
    bool isSorted = false;
    float scrollY = 215;
    bool updateScroll = false;
    int dataInt;

    // buttons are declared
    Button cancel("Cancel", 100, 30, 500, 520, IRIS, LAVENDER);
    Button sorted("Sort", 100, 30, 680, 520, IRIS, LAVENDER);
    Button scroll("", 10, 30, 760, scrollY, PURPLE, LAVENDER);

    ALLEGRO_EVENT ev;

    while (true) {

        // timer is unregistered from the event queue
        al_unregister_event_source(event_queue, al_get_timer_event_source(timer));

        // rectangle and scroll bar are added to the allegro display (surrounded by iris borders)
        al_draw_filled_rounded_rectangle(480, 150, 800, 570, 3, 3, IRIS);
        al_draw_filled_rounded_rectangle(490, 160, 790, 560, 3, 3, WHITE);
        al_draw_filled_rounded_rectangle(500, 205, 780, 510, 3, 3, LAVENDER);
        al_draw_filled_rounded_rectangle(760, 215, 770, 500, 3, 3, IRIS);

        if (isSorted) {
            // if isSorted is true, the sorted vector is drawn
            al_draw_text(basic_font, IRIS, 640, 170, ALLEGRO_ALIGN_CENTRE, "Array Data (Sorted)");
            for (float i = ((scrollY - 215) / percentOffset); i < ((scrollY - 215) / percentOffset) + 10; ++i) {
                dataInt = (int)i;
                if (dataInt < (int)(((scrollY - 215) / percentOffset)) + 10 && dataInt < unsortedData.size()) {
                    // 10 numbers are drawn at a time (position is calculated using offset)
                    al_draw_text(basic_font, IRIS, 560, 177 + 30 * (i - ((scrollY - 215) / percentOffset) + 1), ALLEGRO_ALIGN_LEFT, (to_string(dataInt + 1) + ".").c_str());
                    al_draw_text(basic_font, IRIS, 720, 177 + 30 * (i - ((scrollY - 215) / percentOffset) + 1), ALLEGRO_ALIGN_RIGHT, (to_string(sortedData[dataInt])).c_str());
                }
            }
        } else {
            // if isSorted is false, the unsorted vector is drawn
            al_draw_text(basic_font, IRIS, 640, 170, ALLEGRO_ALIGN_CENTRE, "Array Data (Unsorted)");
            for (float i = ((scrollY - 215) / percentOffset); i < ((scrollY - 215) / percentOffset) + 10; ++i) {
                dataInt = (int)i;
                if (dataInt < (int)(((scrollY - 215) / percentOffset)) + 10 && dataInt < unsortedData.size()) {
                    // 10 numbers are drawn at a time (position is calculated using offset)
                    al_draw_text(basic_font, IRIS, 560, 177 + 30 * (i - ((scrollY - 215) / percentOffset) + 1), ALLEGRO_ALIGN_LEFT, (to_string(dataInt + 1) + ".").c_str());
                    al_draw_text(basic_font, IRIS, 720, 177 + 30 * (i - ((scrollY - 215) / percentOffset) + 1), ALLEGRO_ALIGN_RIGHT, (to_string(unsortedData[dataInt])).c_str());
                }
            }
        }

        // buttons are drawn
        cancel.draw(ev);
        sorted.draw(ev);

        // scroll bar position is updated then drawn
        scroll.setYPos(scrollY);
        scroll.drawScroll(ev);

        al_flip_display();

        if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
            // if mouse button is down, updateScroll is set to true so the user can move the scroll bar
            updateScroll = true;
        } else if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP) {
            // if mouse button is up, updateScroll is set to false so the user cannot move the scroll bar
            updateScroll = false;
        }

        if (ev.type == ALLEGRO_EVENT_MOUSE_AXES) {
            if (updateScroll) {
                if (ev.mouse.x > 760 && ev.mouse.x < 770 && ev.mouse.y > scrollY && ev.mouse.y < scrollY + 30) {
                    // if the user is clicking the scroll bar, the scroll bar's y position updates to match the users
                    scrollY = ev.mouse.y - 15;
                    // the scroll bar's y position must remain within the length of the scroll bar (y >= 215, y <=470)
                    if (scrollY < 215) {
                        scrollY = 215;
                    } else if (scrollY > 470) {
                        scrollY = 470;
                    }
                }
            }
        }

        if (sorted.checkClicked(ev)) {
            // if the sorted button is clicked, isSorted is set to true (to update the data)
            isSorted = !(isSorted);
            // message on sorted bar is updated accordingly
            if (isSorted) {
                sorted.setText("Unsort");
            } else {
                sorted.setText("Sort");
            }
            // timer event source is registered to quickly update the data
            al_register_event_source(event_queue, al_get_timer_event_source(timer));
        } else if (cancel.checkClicked(ev)) {
            // if the cancel button is clicked, function returns
            return;
        } else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            // if the allegro display is closed, exit is set to ture, and the function returns
            exit = true;
            return;
        }
        al_wait_for_event(event_queue, &ev);
    }
}
// recieves a vector containing strings as input
// draws the array on the allegro display
void Display::drawHistoryArray(vector<string> history) {

    // variables are declared
    float percentOffset;
    float scrollY = 215;
    bool updateScroll = false;
    int historyInt;

    // percentOffset is calculated (used for the scroll bar)
    if (history.size() > 10) {
        percentOffset = 255.0 / (history.size() - 10);
    } else {
        percentOffset = 255;
    }

    // buttons are declared
    Button cancel("Cancel", 100, 30, 590, 520, IRIS, LAVENDER);
    Button scroll("", 10, 30, 810, scrollY, PURPLE, LAVENDER);

    ALLEGRO_EVENT ev;

    while (true) {

        // timer is unregistered from the event queue
        al_unregister_event_source(event_queue, al_get_timer_event_source(timer));

        // rectangle and scroll bar are added to the allegro display (surrounded by iris borders)
        al_draw_filled_rounded_rectangle(430, 150, 850, 570, 3, 3, IRIS);
        al_draw_filled_rounded_rectangle(440, 160, 840, 560, 3, 3, WHITE);
        al_draw_filled_rounded_rectangle(450, 205, 830, 510, 3, 3, LAVENDER);
        al_draw_filled_rounded_rectangle(810, 215, 820, 500, 3, 3, IRIS);

        // title is added to the allegro display
        al_draw_text(basic_font, IRIS, 640, 170, ALLEGRO_ALIGN_CENTRE, "History");

        // history (moves the user has taken) is drawn
        // 10 strings are drawn at a time (position is calculated using offset)
        for (float i = ((scrollY - 215) / percentOffset); i < ((scrollY - 215) / percentOffset) + 10; ++i) {
            historyInt = (int)i;
            if (historyInt < (int)(((scrollY - 215) / percentOffset)) + 10 && historyInt < history.size()) {
                al_draw_text(basic_font, IRIS, 460, 177 + 30 * (i - ((scrollY - 215) / percentOffset) + 1), ALLEGRO_ALIGN_LEFT, (to_string(historyInt + 1) + ". " + history[historyInt]).c_str());
            }
        }

        // buttons are drawn
        cancel.draw(ev);

        // scroll bar position is updated then drawn
        scroll.setYPos(scrollY);
        scroll.drawScroll(ev);

        al_flip_display();

        if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
            // if mouse button is down, updateScroll is set to true so the user can move the scroll bar
            updateScroll = true;
        } else if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP) {
            // if mouse button is up, updateScroll is set to false so the user cannot move the scroll bar
            updateScroll = false;
        }

        if (ev.type == ALLEGRO_EVENT_MOUSE_AXES) {
            if (updateScroll) {
                if (ev.mouse.x > 810 && ev.mouse.x < 820 && ev.mouse.y > scrollY && ev.mouse.y < scrollY + 30) {
                    // if the user is clicking the scroll bar, the scroll bar's y position updates to match the users
                    scrollY = ev.mouse.y - 15;
                    // the scroll bar's y position must remain within the length of the scroll bar (y >= 215, y <=470)
                    if (scrollY < 215) {
                        scrollY = 215;
                    } else if (scrollY > 470) {
                        scrollY = 470;
                    }
                }
            }
        }

        if (cancel.checkClicked(ev)) {
            // if the cancel button is clicked, function returns
            return;
        } else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            // if the allegro display is closed, exit is set to ture, and the function returns
            exit = true;
            return;
        }
        al_wait_for_event(event_queue, &ev);
    }
}

/// Method Implementations: Draw - Message
// recieves a string containing as message as input
// draws the message using the allegro display
void Display::drawMessage(string message) {

    // semi-transparent rectangle is drawn over the entire display to give the effect of the background fading
    al_draw_filled_rectangle(0, 0, 1280, 720, FADE);

    // button is declared
    Button continueButton("Continue", 100, 30, 590, 375, IRIS, LAVENDER);

    ALLEGRO_EVENT ev;
    // timer is unregistered from the event queue
    al_unregister_event_source(event_queue, al_get_timer_event_source(timer));

    while (true) {

        // rectangle is added to the allegro display (surrounded by iris borders)
        al_draw_filled_rounded_rectangle(450, 295, 830, 425, 3, 3, IRIS);
        al_draw_filled_rounded_rectangle(460, 305, 820, 415, 3, 3, WHITE);

        // message is added to the allegro display
        al_draw_text(basic_font, IRIS, 640, 315, ALLEGRO_ALIGN_CENTRE, message.c_str());

        // button is drawn
        continueButton.draw(ev);

        al_flip_display();

        if (continueButton.checkClicked(ev)) {
            // if the continue button is clicked, function returns
            return;
        } else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            // if the allegro display is closed, exit is set to ture, and the function returns
            exit = true;
            return;
        }
        al_wait_for_event(event_queue, &ev);
    }
}
