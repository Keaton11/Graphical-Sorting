/*  Name: Keaton Lee
 *  Date: June 18, 2021
 *  Title: Graphical Sorting
 *
 *  Description: Contains the main play function which allows users to select
 *  which algorithm they would like to run
*/

// imports header files
#include "header.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

extern ALLEGRO_EVENT_QUEUE *event_queue;
extern ALLEGRO_DISPLAY *display;
extern ALLEGRO_FONT *basic_font;
extern ALLEGRO_FONT *title_font;
extern ALLEGRO_TIMER *timer;

void play(string title, float s, int dataSize, int page) {
// recieves a string corresponding to the title, a float corresponding to the position of the speed bar,
// an integer corresponding to the data size, and the page number as input
// function responds to user input (calls appropriate functions when buttons are clicked)
// code not included in main function as play function calls itself before drawing algorithms to update the title

    /// Declarations - Objects
    Sorting sortingObject;
    Display displayObject;

    /// Declarations - Variables
    // variables are declared based on parameters
    int pageNum = page;
    float speedX = s;
    bool updateSpeed = false;
    vector<string> history;

    /// Declarations - Buttons
    // speed button is declared
    Button speedButton;
    speedButton.setTriangle(30, -25.9808, 1, speedX, 160.9808, PURPLE);

    // sort buttons are declared
    Button sortButton[4][6];
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 6; ++j) {
            Button temp("", 180, 50, 1075, 230 + 70 * j, IRIS, LAVENDER);
            sortButton[i][j] = temp;
        }
    }
    // text is set for each sort button (each button corresponds to a different sorting algorithm)
    sortButton[0][0].setText("Bubble Sort");
    sortButton[0][1].setText("Selection Sort");
    sortButton[0][2].setText("Insertion Sort");
    sortButton[0][3].setText("Quick Sort");
    sortButton[0][4].setText("Merge Sort");
    sortButton[0][5].setText("Binary Search");
    sortButton[1][0].setText("Cocktail Sort");
    sortButton[1][1].setText("Comb Sort");
    sortButton[1][2].setText("Gnome Sort");
    sortButton[1][3].setText("Odd-Even Sort");
    sortButton[1][4].setText("Heap Sort");
    sortButton[1][5].setText("Cycle Sort");
    sortButton[2][0].setText("Radix Sort");
    sortButton[2][1].setText("Shell Sort");
    sortButton[2][2].setText("Bitonic Sort");
    sortButton[2][3].setText("Stooge Sort");
    sortButton[2][4].setText("Bogo Sort");
    sortButton[2][5].setText("std::sort");
    sortButton[3][0].setText("K Sort 1");
    sortButton[3][1].setText("K Sort 2");
    sortButton[3][2].setText("K Sort 3");
    sortButton[3][3].setText("K Sort 4");
    sortButton[3][4].setText("K Sort 5");
    sortButton[3][5].setText("K Sort 6");

    // left and right triangle buttons are declared
    Button leftButton;
    leftButton.setTriangle(30, 25.9808, 0, 1075, 675, PURPLE);
    Button rightButton;
    rightButton.setTriangle(30, -25.9808, 0, 1255, 675, PURPLE);

    // subtract and add buttons are declared
    Button subtractOne("-1", 30, 30, 25, 402, PURPLE, LAVENDER);
    Button addOne("+1", 30, 30, 175, 402, PURPLE, LAVENDER);
    subtractOne.setBorder(true);
    addOne.setBorder(true);

    // rand buttons are declared
    vector<Button> randButton;
    for (int i = 0; i < 3; ++i) {
        Button temp("", 180, 50, 25, 140 + 70 * i, IRIS, LAVENDER);
        randButton.push_back(temp);
    }
    // text is set for each rand button (each button corresponds to a different sorting algorithm)
    randButton[0].setText("Pseudo-Random");
    randButton[1].setText("Uniform");
    randButton[2].setText("Sequence");

    // options buttons are declared
    Button colourButton("Change Colours", 180, 50, 25, 505, IRIS, LAVENDER);
    Button moreOptions("More Options", 180, 50, 25, 575, IRIS, LAVENDER);
    Button exit("Exit", 180, 50, 25, 645, IRIS, LAVENDER);

    ALLEGRO_EVENT ev;

    while (true) {
        // timer is unregistered from event queue
        al_unregister_event_source(event_queue, al_get_timer_event_source(timer));

        /// Drawing - Titles
        // background colour and title are added to the allegro display
        al_clear_to_color(BLACK);
        al_draw_text(title_font, IRIS, 640, 18, ALLEGRO_ALIGN_CENTRE, title.c_str());

        //subtitles are added to the allegro display
        al_draw_text(basic_font, IRIS, 1165, 185, ALLEGRO_ALIGN_CENTRE, "Sorting Algorithms");
        al_draw_text(basic_font, IRIS, 1165, 95, ALLEGRO_ALIGN_CENTRE, "Speed");
        al_draw_text(basic_font, IRIS, 115, 95, ALLEGRO_ALIGN_CENTRE, "Randomization");
        al_draw_text(basic_font, IRIS, 115, 347, ALLEGRO_ALIGN_CENTRE, "Number of Bars");
        al_draw_text(basic_font, IRIS, 115, 460, ALLEGRO_ALIGN_CENTRE, "Settings");

        /// Drawing - Boxes
        // speed bar is drawn
        al_draw_filled_rounded_rectangle(1075, 145, 1255, 165, 3, 3, IRIS);
        al_draw_rounded_rectangle(1075, 145, 1255, 165, 3, 3, LAVENDER, 2);

        // number of bars box is drawn
        al_draw_filled_rectangle(65, 392, 165, 442, IRIS);
        al_draw_rectangle(65, 392, 165, 442, LAVENDER, 2);

        // page number box is drawn
        al_draw_filled_rectangle(1115, 655, 1215, 695, IRIS);
        al_draw_rectangle(1115, 655, 1215, 695, LAVENDER, 2);

        /// Drawing - Other Text

        // data size is added to the number of bars box
        if (dataSize == 0) {
            al_draw_text(basic_font, LAVENDER, 115, 401, ALLEGRO_ALIGN_CENTRE, (to_string(sortingObject.getDataSize())).c_str());
        } else {
            al_draw_text(basic_font, LAVENDER, 115, 401, ALLEGRO_ALIGN_CENTRE, (to_string(dataSize)).c_str());
        }
        // page number is added to the page number box
        al_draw_text(basic_font, LAVENDER, 1165, 660, ALLEGRO_ALIGN_CENTRE, (to_string(pageNum) + "  /  4").c_str());

        /// Drawing - Buttons
        // if page number is 1, hover for the left button is set to false
        if (pageNum == 1) {
            leftButton.setHover(false);
        } else {
            leftButton.setHover(true);
        }
        // if page number is 4, hover for the right button is set to false
        if (pageNum == 4) {
            rightButton.setHover(false);
        } else {
            rightButton.setHover(true);
        }

        // speed button position is updated and it is drawn
        speedButton.setXPos(speedX);
        speedButton.drawTriangle(ev);

        // sort buttons are drawn
        for (int i = 0; i < 6; ++i) {
            sortButton[pageNum - 1][i].draw(ev);
        }

        // rand buttons are drawn
        for (int i = 0; i < 3; ++i) {
            // the rand button corresponding to the current random setting is drawn in a darker shade
            if (sortingObject.getRandomType() == i) {
                randButton[i].setColour(DARK_IRIS);
                randButton[i].setHover(false);
            } else {
                randButton[i].setColour(IRIS);
                randButton[i].setHover(true);
            }
            randButton[i].draw(ev);
        }

        // the rest of the buttons are drawn
        leftButton.drawTriangle(ev);
        rightButton.drawTriangle(ev);
        colourButton.draw(ev);
        subtractOne.draw(ev);
        addOne.draw(ev);
        moreOptions.draw(ev);
        exit.draw(ev);

        // the grahical display is drawn
        sortingObject.drawBars();

        /// User Input (responding to actions taken by the user)

        if (title != "Graphical Sorting Assignment") {
            // if the title is not "Graphical Sorting Assignment", the function returns once everything has been drawn
            // this is used to update the title before running algorithms
            return;
        }

        if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
            // if mouse button is down, updateSpeed is set to true so speed can be updated
            updateSpeed = true;
        } else if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP) {
            // if mouse button is up, updateSpeed is set to false so speed cannot be updated
            updateSpeed = false;

            // if the user has clicked the box containing the number of bars, they can enter input using their keyboard
            if (ev.mouse.x > 65 && ev.mouse.x < 165 && ev.mouse.y > 392 && ev.mouse.y < 442) {
                // users can input a number between 2 and 800 for the number of bars
                if (displayObject.inputNumber(2, 800)) {
                    // data size is updated (to the users selection)
                    sortingObject.callRandom(true, displayObject.getChoiceNum());
                    history.push_back("Set Array Size to " + to_string(displayObject.getChoiceNum()));
                }
            }
        }

        if (ev.type == ALLEGRO_EVENT_MOUSE_AXES) {
            if (updateSpeed) {
                if (ev.mouse.x > speedX - 15 && ev.mouse.x < speedX + 15 && ev.mouse.y > 135 && ev.mouse.y < 160.9808) {
                    // if the user is clicking the speed triangle, the triangle's x position updates to match the users
                    speedX = ev.mouse.x;
                    // the x value for speed must remain within the length of the speed bar (275 <= x, x <= 425)
                    if (speedX < 1090) {
                        speedX = 1090;
                    } else if (speedX > 1240) {
                        speedX = 1240;
                    }
                }
            }
        }

        for (int i = 0; i < 6; ++i) {
            // if a sort button has been clicked, the corresponing sorting method is called
            if (sortButton[pageNum - 1][i].checkClicked(ev)) {
                if (pageNum == 1 && i == 5) {
                    // for binary search, function waits for keyboard input (to preform search)
                    if (displayObject.inputNumber(1, 1000)) {
                        // play function is called to redraw title
                        play("Binary Search: " + to_string(displayObject.getChoiceNum()), speedX, sortingObject.getDataSize(), pageNum);
                        sortingObject.setTarget(displayObject.getChoiceNum());
                        sortingObject.callSort(i, (1240 - speedX) / 150);
                        history.push_back("Binary Search: " + to_string(displayObject.getChoiceNum()));
                    }
                } else if (pageNum == 3 && i == 2) {
                    // for bitonic sort, message is displayed if the data size is not a power of 2
                    // '&' is the bitwise AND operator
                    if ((sortingObject.getDataSize() & (sortingObject.getDataSize() - 1)) == 0) {
                        // play function is called to redraw title
                        play("Algorithm: " + sortButton[pageNum - 1][i].getText(), speedX, sortingObject.getDataSize(), pageNum);
                        sortingObject.callSort(i + 6 * 2, (1240 - speedX) / 150);
                        history.push_back("Algorithm: " + sortButton[pageNum - 1][i].getText());
                    } else {
                        displayObject.drawMessage("Data size must be a power of 2");
                    }
                } else {
                    // play function is called to redraw title
                    play("Algorithm: " + sortButton[pageNum - 1][i].getText(), speedX, sortingObject.getDataSize(), pageNum);
                    sortingObject.callSort(i + 6 * (pageNum - 1), (1240 - speedX) / 150);
                    history.push_back("Algorithm: " + sortButton[pageNum - 1][i].getText());
                }
                al_register_event_source(event_queue, al_get_timer_event_source(timer));
            }
        }
        for (int i = 0; i < 3; ++i) {
            // if a rand button is clicked, the random type is updated, and a method is called to reset the data
            if (randButton[i].checkClicked(ev)) {
                sortingObject.setRandomType(i);
                sortingObject.callRandom(true, sortingObject.getDataSize());
                al_register_event_source(event_queue, al_get_timer_event_source(timer));
                history.push_back("Set Random: " + randButton[i].getText());
            }
        }
        if (leftButton.checkClickedTriangle(ev) && pageNum != 1) {
            // if the left page number is clicked, the page is reduced by 1 (as long as the page number is greater than 1)
            pageNum--;
            al_register_event_source(event_queue, al_get_timer_event_source(timer));
        } else if (rightButton.checkClickedTriangle(ev) && pageNum != 4) {
            // if the right page number is clicked, the page is increased by 1 (as long as the page number is less than 4)
            pageNum++;
            al_register_event_source(event_queue, al_get_timer_event_source(timer));
        } else if (colourButton.checkClicked(ev)) {
            // if the colour button is clicked, method is called to allow users to select three colours
            if (displayObject.inputColours()) {
                // if three colours are selected, method is called to update the colours
                sortingObject.setGradientColours(displayObject.getChoiceColours());
                al_register_event_source(event_queue, al_get_timer_event_source(timer));
                history.push_back("Changed Colours");
            }
        } else if (subtractOne.checkClicked(ev)) {
            // if the "-1" button is clicked, a method is called to reduce the number of bars by 1
            sortingObject.removeBar();
            al_register_event_source(event_queue, al_get_timer_event_source(timer));
            history.push_back("Removed a Bar");
        } else if (addOne.checkClicked(ev)) {
            // if the "+1" button is clicked, a method is called to increase the number of bars by 1
            sortingObject.addBar();
            al_register_event_source(event_queue, al_get_timer_event_source(timer));
            history.push_back("Added a Bar");
        } else if (moreOptions.checkClicked(ev)) {
            // if the more options button is clicked, a method is called to display the other options
            int option = displayObject.inputButton("Reset", "View Full Array", "View History");
            if (option == 0) {
                // if option 0 ("Reset") is selected, the speed, random type, data size, and colours are reset
                speedX = 1165;
                sortingObject.setRandomType(0);
                sortingObject.callRandom(true, 100);
                sortingObject.resetColours();
                history.push_back("Reset Data");
            } else if (option == 1) {
                // if option 1 ("View Full Array") is selected, a method is called to display the array
                displayObject.drawDataArray(sortingObject.getRawData());
            } else if (option == 2) {
                // if option 2 ("View History") is selected, a method is called to display the history
                displayObject.drawHistoryArray(history);
            }
            al_register_event_source(event_queue, al_get_timer_event_source(timer));
        } else if (exit.checkClicked(ev) || ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE || displayObject.getExit()) {
            // function returns if the exit button is clicked or the allegro display is closed
            return;
        }
        al_wait_for_event(event_queue, &ev);
    }
}
