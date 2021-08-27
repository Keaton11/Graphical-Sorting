/*  Name: Keaton Lee
 *  Date: June 18, 2021
 *  Title: Graphical Sorting
 *
 *  Description: Prototype for Sorting class
*/

// checks if header file was defined to avoid double inclusions
#ifndef SORTING_H
#define SORTING_H

// imports header files
#include "colours.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <algorithm>
#include <windows.h>
#include <iostream>
#include <stdlib.h>
#include <climits>
#include <random>
#include <vector>
#include <list>

using namespace std;

extern ALLEGRO_DISPLAY *display;
extern ALLEGRO_FONT *basic_font;
extern ALLEGRO_TIMER *timer;

class Sorting {
    public:
        /// Struct definitions
        // contains variables used to determine coordinate directions
        struct Directions {
            int x;
            int y;
        };
        // contains variables used for timing sorting algorithms
        struct systemTimer {
            LARGE_INTEGER start;
            LARGE_INTEGER stop;
        };

        /// Constructor
        Sorting();

        /// Setters (Mutators)
        // the setters of the Display class are small methods that are called fairly often
        // inline methods are used to speed up the code
        inline void setTarget(int t) {
            // recieves integer as parameters, sets target using this integer
            target = t;
        };
        inline void setGradientColours(ALLEGRO_COLOR c[]) {
            // recieves an ALLEGRO_COLOR array as parameters, sets gradientColour (array containing gradient colours) using this integer
            gradientColour[0] = c[0];
            gradientColour[1] = c[1];
            gradientColour[2] = c[2];
        };
        inline void setRandomType(int rT) {
            // recieves an integer as parameters, sets random type using this integer
            randomType = rT;
        };

        /// Getters (Accessors)
        // like the setters, the getters of the Display class are small methods that are called fairly often
        // inline methods are used to speed up the code
        inline int getDataSize() {
            // returns the size of the rawData vector as an integer
            return rawData.size();
        };
        inline int getRandomType() {
            // returns the randomType (current random settings) as an integer
            return randomType;
        };
        inline vector<int> getRawData() {
            // returns the data vector as a vector of integers
            return rawData;
        };

        /// Public Method Prototypes: Adjustements to Display
        void resetColours();
        void addBar();
        void removeBar();

        /// Public Method Prototypes: Drawing
        void drawBars();
        void drawMetrics();

        /// Public Method Prototypes: Calling Private Methods
        void callRandom(bool reset, int n);
        void callSort(int sT, float s);

    private:
        /// Private Variables
        vector<int> rawData;    // used to hold the "raw" (unsorted) data
        vector<int> data;       // sorting algorithms make changes to this vector which contains a copy of the raw data

        ALLEGRO_COLOR gradientColour[3];    // used to determine the colour of bars (colour is based on bar height)
        vector<int> colouredBars;           // used to colour certain important bars in another colour
        vector<bool> faded;                 // used to "fade" certain bars during the binary search algorithm

        int randomType;         // keeps track of the type of randomization
        int sortType;           // keeps track of the sorting algorithm

        Directions drawCorner;  // contains x and y coordinates of graphical display frame
        Directions drawSize;    // contains width and height of graphical display frame

        int comparisons;        // tracks the number of comparisons during a search
        int moves;              // tracks the number of moves during a search

        int target;             // holds value of target for binary search
        int targetIndex[2];     // holds value of target index (or closest range) after binary search

        bool sorted;            // boolean value is set depending on whether or not the data has been sorted

        LARGE_INTEGER Frequency;    // used to hold frequency (fixed at system boot)
        systemTimer tDraw;          // used to measure draw time (for draw delay)
        long long int drawDelay;    // used to hold draw delay (later subtracted from system time)
        long long int systemTime;   // used to hold system time

        float speed;            // used to delay algorithms (in order to slow down algorithms)
        string loading;         // used to display the loading text

        /// Private Method Prototypes: Reset
        void resetBars();
        void resetVariables();

        /// Private Method Prototypes: Randomization
        void randPseudoRandom(int n);
        void randUniformDistribution(bool reset, int n);
        void randSequenceShuffle(bool reset, int n);

        /// Private Method Prototypes: Sorting Algorithms
        void bubbleSort();
        void selectionSort();
        void insertionSort();
        void quickSort(vector<int> *dataVector, int offset);
        void mergeSort(int leftIndex, int rightIndex);
        void binarySearch(int leftIndex, int rightIndex);
        void cocktailSort();
        void combSort();
        void gnomeSort();
        void oddEvenSort();
        void heapSort();
        void heapSortHeapify(int heapSize, int node);
        void cycleSort();
        void radixSort();
        void shellSort();
        void bitonicSort();
        void stoogeSort(int left, int right);
        void bogoSort();
        void kSort1();
        void kSort2();
        void kSort3();
        void kSort4(int leftIndex, int rightIndex);
        void kSort5();
        void kSort6();

        /// Private Method Prototypes: Other
        void updateLoading();
        void swapBars(int index1, int index2);
        ALLEGRO_COLOR colourGradient(float percentage);
};

#endif // SORTING_H
