#ifndef FIELD_H
#define FIELD_H

#include "structures.h"

/**
 * Creates an array of h * l uninitialized values in
 * dynamic memory.
 */

int *f_create(int h, int l);

/**
 * Assigns values to the field. 0 stands for an empty
 * place in the field, 9 stands for its borders.
 */

void f_clear(int *field, int h, int l);

/**
 * If mp is set to 0, prints the left field (singleplayer/
 * player 1 field), otherwise prints the right field
 * (player 2 / CPU field) with every spaces occupied by
 * different tetraminos in different colors.
 */

void f_print(int *field, int mp, settings_t *s);

/**
 * Starting from the bottom of the field, it checks
 * if a row is filled from non-zero values; if so, it
 * shifts the field down and calls itself back to check
 * for upper rows.
 */

void f_lower(int *field, int h, int l);

#endif /* FIELD_H */
