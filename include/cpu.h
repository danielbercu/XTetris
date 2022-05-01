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
 * Parse every possible combination of tetraminos, rotations and positions
 * to try to score at least a point.
 * If a point has been scored, move on to the next round.
 */

void cpu_hit(int *field, int *pieces, int *old_points, settings_t *s);

/**
 * If cpu_hit didn't make a point, this function places a semi-randomly
 * chosen tetramino between borders which contain the highest number
 * of free spaces in the row beneath the lowest empty row.
 */

void cpu_guess(int *field, int *pieces, settings_t *s);

/**
 * If both cpu_hit and cpu_guess fail, this function places a completely
 * random piece between the two borders calculated before.
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

#endif /* CPU_H */
