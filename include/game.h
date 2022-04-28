#ifndef GAME_H
#define GAME_H

#include "structures.h"

/**
 * The singleplayer gamemode, which provides an array
 * of 20 pieces per tetramino for the player to place
 * in a single field.
 */

void singleplayer(settings_t *s);

/**
 * The multiplayer mode, which provides an array of
 * 40 pieces per tetramino for two players to place
 * in two separate fields.
 */

void multiplayer(settings_t *s);

/**
 * The CPU mode, which works the same way as the
 * multiplayer function but is fixed accordingly to
 * the second player being the CPU rather than a
 * real person.
 */

void cpu(settings_t *s);

/**
 * A function that analyses two aspects of the current
 * game: it checks if there are any pieces left for
 * the players to use or if a player placed a piece
 * over the 15th row.
 * When none of these conditions are true it returns 0,
 * otherwise it returns 1 or 2 if the game is singleplayer
 * or multplayer.
 */

int is_game_over(int *field, int *pieces, int mp, int l);

/**
 * Counts and updates the points made at a given round,
 * checking if one or more rows have been completely
 * filled.
 */

void count_points(int *field, int *points, int *count, int h, int l);

/**
 * This function provides a nice add to the standard Tetris:
 * if a player filled 4 or more rows in the same round,
 * the same number of rows on the opposite field starting from
 * the bottom get filled in the positions in which they are
 * empty and viceversa. It uses the same 'count' variable
 * of the above function, so that's when it knows it has
 * to operate.
 */

void reverse_lines(int *field, int *count, int h, int l);

/**
 * This one also does what is says. Displayes under the
 * field(s) a list of the available pieces left.
 */

void print_remaining_pieces(int *pieces, int h);

#endif
