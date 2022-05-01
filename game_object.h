#pragma once

#include "util.h"


// Forward declaration for game_data_t
// (otherwise there would be a circular dependency between game_object.h and
//  paddle_object.h)
struct game_data_t;
typedef struct game_data_t game_data_t;

struct ball_object_t;
typedef struct ball_object_t ball_object_t;

// Interface for all game objects.
// Game objects are visible on the screen.
//
// Feel free to add more methods to the interface, as you see fit.
//
// For a complete example of this technique, see the examples directory.
//

typedef struct game_object_t game_object_t;

typedef struct game_object_vtable_t game_object_vtable_t;
struct game_object_vtable_t
{
    // Methods for the game object
    //   You definitely want a method to:
    //      - draw the object on the screen
    //      - tell the object another object collided with it
    //          (best to pass the other object as well to the method)
    //      - A 'timer tick', i.e. a method which gets called
    //        every X milliseconds, so that object can perform time
    //        based actions (such as a block temporarily becoming
    //        invisible), move across the playing field, etc.
    //      - A method to allow inserting these objects in a spatial index
    //      (i.e. intersect, bounding box)
    //
    //    and of course you need a method to destroy the object as well.
// COMPLETE ME
// COMPLETE ME
