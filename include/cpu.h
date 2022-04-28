#ifndef CPU_H
#define CPU_H

#include "structures.h"

/**
 * Wrapper for cpu_hit and cpu_guess; if cpu_hit succeded
 * move on with the next round, otherwise call cpu_guess to try
 * to make a semi-valid move.
 */

void cpu_move(int *field, int *pieces, int *points, settings_t *s);

/**
 * This function's goal is to make a point by trying every possible
 * combination of tetraminos, rotations and positions.
 * If a point has been scored, move on to the next round.
 */

void cpu_hit(int *field, int *pieces, int *old_points, settings_t *s);

/**
 * If cpu_hit failed in its attempt to make a point, we should try
 * to make it easier for it to make a point the next move: we are
 * doing so by analysing some chunks of the field, and trying to
 * understand how many places we have left on the row under the lowest
 * empty row, and placing a semi-random chosen tetramino in that position.
 */

void cpu_guess(int *field, int *pieces, settings_t *s);

/**
 * The function that gets called when both cpu_hit and cpu_guess fail.
 * By calculating the left and right bounds of the longest chunk
 * of free places under the lowest row, place a random tetramino in that
 * chunk.
 */

void cpu_rand_move(int *field, int *pieces, settings_t *s, int d_left,
                   int d_right);

/**
 * Calculates the lowest empty row.
 */

int lowest_empty_row(int *field, int h, int l);

/**
 * Calculates the number of the most consecutive spaces under the
 * lowest row.
 */

int consec_spaces_under_lowest(int *field, int l, int ler, int *d_left,
                              int *d_right);

#endif
