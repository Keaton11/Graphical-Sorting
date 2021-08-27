/*  Name: Keaton Lee
 *  Date: June 18, 2021
 *  Title: Graphical Sorting
 *
 *  Description: Contains function prototypes
*/

// checks if header file was defined to avoid double inclusions
#ifndef HEADER_H
#define HEADER_H

// includes header files
#include "Sorting.h"
#include "Display.h"
#include "Button.h"
#include "colours.h"

/// Function Prototypes

// prototypes for functions in allegroFunctions.cpp
bool initializeAllegro();
void closeAllegro();

// prototype for function in playFunctions.cpp
void play(string title, float s, int dataSize, int page);

#endif // HEADER_H
