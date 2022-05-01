#ifndef TETRAMINO_H
#define TETRAMINO_H

#include "structures.h"

/**
 * Creates a new tetramino_t by allocating
 * in dynamic memory its array of integers.
 */

tetramino_t t_create(int *src, int size);

/**
 * Rotate by 90° counter-clockwise a given tetramino.
 */

void t_rotate(tetramino_t t);

/**
 * Prints the content of a tetramino_t through a square
 * matrix. Never used it in the code, but it's nice to
 * have. You never know!
 */

void t_print(tetramino_t t);

/**
 * Places a tetramino in a field at coordinates (x,y).
 * Note that the game interpets the coordinates
 * (x,y) == (0,0) as the top-left corner.
 */

void t_place(int *field, tetramino_t t, int x, int y, int l);

/**
 * Returns a tetramino_t depending from the type of selection
 * (random or manual) and the availability of pieces.
 * We do not care if there are no available pieces BEFORE
 * calling this function: when this happens we will have already
 * made a check and exited the main loop of the gamemode,
 * so we won't enter in this function again.
 */

tetramino_t t_select(int *pieces, settings_t *s);

/**
 * Checks for collisions on the left, on the right or under
 * a tetramino in a field. The tetramino does not need to be
 * placed in the field in order to check its collisions:
 * using its edge (the square root of its size) and some given
 * coordinates (x,y), we can estimate if there is a non-zero
 * element under or near the places where it would be placed.
 */

int t_collision(int *field, tetramino_t t, int x, int y, char dir, int l);

/**
 * This function both fulfills functional and esthetical aspects:
 * it is only used for the CPU aspect, with two different roles:
 * when 'ms' and 'print' are set to zero, it is used to (almost)
 * instantly check every possible combination of tetramino_t's,
 * rotations and positions searching for a precise move to make,
 * doing this without any time delay between a fall and another
 * and without printing the falling animation.
 * When 'ms' is set at a certain value (in milliseconds) and
 * 'print' is set to 1, we can see the falling animation of a
 * tetramino_t inside a field at a speed of 1 piece / 'ms'
 * milliseconds.
 */

void t_gravity(int *field, tetramino_t t, int x, int y, int ms, settings_t *s,
               int print);

/**
 * At its core, t_move might resemble t_gravity. The great difference
 * stands at the fact that while the piece is falling the player can
 * move the piece around the field and rotate it until it finds a
 * collision with the field's bounds or other pieces.
 * This is accomplished by calculating the rate of speed with a different
 * approach, taking into consideration the points of the player who
 * is currently moving the piece and using non-blocking input
 * functions.
 * Both this function and t_gravity make use of temporary fields, that
 * constantly get filled and wiped according to the speed and direction
 * of the pieces.
 */

void t_move(int *field, tetramino_t t, int x, int y, int mp, int *points,
            settings_t *s);

#endif /* TETRAMINO_H */
