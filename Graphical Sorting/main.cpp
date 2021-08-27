/*******************************************************************************
 *  Name: Keaton Lee                                            June 18, 2021  *
 *  Title: Graphical Sorting                                                   *
 *                                                                             *
 *  Description: This program sorts a list of random integers using several    *
 *  recursive sorting algorithms (ex. merge sort and quick sort), as well as   *
 *  non recursive sorting algorithms (ex. either bubble sort, insertion sort,  *
 *  and selection sort). These algorithms are graphically displayed using      *
 *  Allegro. The program then compares the algorithms by keeping track of the  *
 *  number of comparisons, the number of “moves” that the algorithms require,  *
 *  and the amount of system time in microseconds that each algorithm takes.   *
 *  The program allows users to select several options including the type of   *
 *  randomization, the size of the data array (the number of bars in the       *
 *  display), the colour gradient, the speed of the algorithms, and which      *
 *  algorithms they would like to run. Additionally, users can view the array  *
 *  (sorted / unsorted), as well as their history.                             *
 *******************************************************************************/

// imports header files
#include "header.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <stdlib.h>
#include <time.h>

using namespace std;

// global allegro variables
ALLEGRO_EVENT_QUEUE *event_queue = nullptr;
ALLEGRO_DISPLAY *display = nullptr;
ALLEGRO_FONT *basic_font = nullptr;
ALLEGRO_FONT *title_font = nullptr;
ALLEGRO_TIMER *timer = nullptr;

int main() {

    // seeds rand with a starting value to allow for a random generation of numbers
    srand(time(0));

    // calls function to initialize allegro
    if (!initializeAllegro()) {
        // if allegro fails to initialize, 1 is returned
        return 1;
    }

    // calls play function
    // "Graphical Sorting Assignment" is the title
    // 1165 is the starting position (x) of the speed bar
    // data size is set to 0 (updated to default size during the play function)
    // page number is set to 1 (to display first set of sorting algorithms)
    play("Graphical Sorting Assignment", 1165, 0, 1);

    // calls function to close allegro
    closeAllegro();

    return 0;
}
