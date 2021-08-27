/*  Name: Keaton Lee
 *  Date: June 18, 2021
 *  Title: Graphical Sorting
 *
 *  Description: Protoype for Display class (which is used to display messages / arrays and
 *  recieve user input via Allegro)
*/

// checks if header file was defined to avoid double inclusions
#ifndef DISPLAY_H
#define DISPLAY_H

// imports header files
#include "colours.h"
#include "Button.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <algorithm>
#include <iostream>
#include <string>

using namespace std;

extern ALLEGRO_EVENT_QUEUE *event_queue;
extern ALLEGRO_DISPLAY *display;
extern ALLEGRO_FONT *basic_font;
extern ALLEGRO_TIMER *timer;

class Display {
    public:
        /// Constructor
        Display();

        /// Getters (Accessors)
        // the getters of the Display class are small methods that are called fairly often
        // inline methods are used to speed up the code
        inline bool getExit() {
            // returns boolean for exit (used to whether user has closed the allegro display)
            return exit;
        };
        inline int getChoiceNum() {
            // returns choiceNum as an integer (corresponds to the number the user has selected)
            return choiceNum;
        };
        inline ALLEGRO_COLOR* getChoiceColours() {
            // returns choiceColour, which is an array of ALLEGRO_COLORs (corresponds to the colours the user has selected)
            return choiceColour;
        };

        /// Public Method Prototypes: Recieving User Input
        bool inputNumber(int minNum, int maxNum);
        bool inputColours();
        int inputButton(string b1, string b2, string b3);

        /// Public Method Prototypes: Drawing a Message / Array
        void drawDataArray(vector<int> unsortedData);
        void drawHistoryArray(vector<string> history);
        void drawMessage(string message);

    private:
        /// Private Variables
        bool exit;          // corresponds to whether user has closed the allegro display

        string inputNum;    // corresponds to number the user has inputted to display that number

        int choiceNum;                  // corresponds to the user's number selection
        ALLEGRO_COLOR choiceColour[3];  // corresponds to the user's colour selection

        /// Private Method Prototype
        char getInputNumber(ALLEGRO_EVENT &ev);
};

#endif // DISPLAY_H
