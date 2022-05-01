
// This indicates that we want the POSIX-2001 (and before) functions as
// well.
#define _POSIX_C_SOURCE 200112L

#include "screen.h"
#include "game_object.h"
#include "game.h"
#include "util.h"
#include "paddle_object.h"
#include "ball_object.h"
#include "wall_object.h"
#include "block_object.h"

#include <assert.h>
#include <time.h> //  nanosleep, POSIX.1-2008
#include <sys/time.h> // gettimeofday
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define NANOSEC_PER_SEC ((uint64_t) 1000000000ull)
#define MICROSEC_PER_SEC ((uint64_t) 1000000ull)
#define TIMER_TICK_PER_SEC 25

#ifndef MIN
#define MIN(a,b) ((a) < (b) ? (a) : (b))
#endif

// COMPLETE ME
// COMPLETE ME
}

void game_destroy(game_data_t * data)
{
// COMPLETE ME
}

// Adds the game objects to the index
// Also check the the screen is the large enough (you can assume the screen
// won't change in size during the game)
static void create_level(game_data_t * data)
{
// COMPLETE ME
}


// move paddle left or right
static void move_paddle(game_data_t * data, bool left)
{
// COMPLETE ME
}

// Returns the current system time in microseconds since the UNIX epoch
// (only use for relative comparisons)
static uint64_t get_time_in_us()
{
    struct timeval tv;
    if (gettimeofday(&tv, NULL) < 0)
    {
        perror("Problem getting time!");
        exit(1);
    }
    return ((uint64_t) tv.tv_sec * MICROSEC_PER_SEC) + tv.tv_usec;
}

// Suspend execution for the specified number of microseconds
static void sleep_for_us(uint64_t us)
{
    struct timespec t;
    t.tv_sec = us / MICROSEC_PER_SEC;
    t.tv_nsec = (NANOSEC_PER_SEC/MICROSEC_PER_SEC)*us;
    nanosleep(&t, NULL);
}

// Cause all objects to draw themselves on the screen.
static void draw_objects(game_data_t * data)
{
// COMPLETE ME
}


// Let all game objects know that there was a timer tick
static void do_timer_tick(game_data_t * data)
{
// COMPLETE ME
}

// Remove all the objects from data->to_remove from the spatial index.
// Don't forget to empty data->to_remove when done.
static void possibly_remove_objects(game_data_t * data)
{
// COMPLETE ME
}

// Do what is needed to let the spatial index know that the objects
// in to_update could have been moved.
// Clear to_update when done.
static void possibly_update_objects(game_data_t * data)
{
// COMPLETE ME
}

int main(int argc, char ** args)
{
    // Initialize game data
    game_data_t data;

    game_init(&data);

    // Optional: add a level system,
    // generating a different (more difficult?) board for every level.

    create_level(&data);

    // Number of microseconds since unix epoch.
    uint64_t next_timer_tick = get_time_in_us();

    bool stop = false;
    while (!data.level_done && !stop)
    {
       // Check if a key was pressed
        const int key = getch();
        switch (key)
        {
            case 'q':
                stop = true;
            case KEY_LEFT:
            case KEY_RIGHT:
                move_paddle(&data, key == KEY_LEFT);
                break;
        }

        // Check if we need to issue a timer tick to the objects.
        if (get_time_in_us() > next_timer_tick)
        {
            next_timer_tick += (MICROSEC_PER_SEC/TIMER_TICK_PER_SEC);

            // Tell all the objects some time has passed
            // They can respond by moving (and possibly colliding!),
            // updating their internal parameters, ...
            //
            // NOTE: See the discussion in the assigment regarding modifying
            // the spatial index while it is being enumerated.
            do_timer_tick(&data);

            // Check if we need to remove or move any game objects
            possibly_remove_objects(&data);
            possibly_update_objects(&data);

            // redraw the game objects
            draw_objects(&data);
        }


        // sleep 1ms (or until the next timer tick)
        // (to avoid spending 100% CPU polling the keyboard)
        // NOTE that if we somehow fell behind (next_timer_tick <
        // get_time_in_us) we won't sleep but process the pending timer ticks
        // until we're back caught up.
        if (next_timer_tick > get_time_in_us())
            sleep_for_us(MIN(1000, next_timer_tick - get_time_in_us()));
    }

    game_destroy(&data);

    return EXIT_SUCCESS;
}
