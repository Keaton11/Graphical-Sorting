/*  Name: Keaton Lee
 *  Date: June 18, 2021
 *  Title: Graphical Sorting
 *
 *  Description: Contains colour definitions
*/

// included colour definitions in a seperate class as they are used in both the play function and the class methods

// checks if header file was defined to avoid double inclusions
#ifndef COLOURS_H
#define COLOURS_H

/// Definitions

// shades of iris
#define LIGHT_IRIS  al_map_rgb(137, 107, 255)
#define IRIS        al_map_rgb(93, 63, 211)
#define DARK_IRIS   al_map_rgb(63, 33, 181)

// shades or purple
#define PURPLE	    al_map_rgb(145, 30, 180)
#define DARK_PURPLE	al_map_rgb(115, 0, 150)

// shades of blue
#define BLUE	    al_map_rgb(0, 130, 200)
#define DARK_BLUE   al_map_rgb(20, 20, 159)

// other colours
#define RED		    al_map_rgb(230, 25, 75)
#define CYAN	    al_map_rgb(30, 200, 200)
#define GREEN	    al_map_rgb(60, 180, 75)
#define YELLOW	    al_map_rgb(250, 220, 20)
#define ORANGE      al_map_rgb(245, 130, 48)
#define MAGENTA	    al_map_rgb(240, 50, 230)
#define LAVENDER    al_map_rgb(230, 190, 255)

// grayscale colours
#define WHITE	    al_map_rgb(255, 255, 255)
#define GREY	    al_map_rgb(128, 128, 128)
#define BLACK	    al_map_rgb(0, 0, 0)
#define FADE        al_map_rgba(0, 0, 0, 100)

#endif // COLOURS_H
