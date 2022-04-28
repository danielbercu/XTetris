#ifndef FIELD_H
#define FIELD_H

#include "structures.h"

/**
 * Creates a new field of height h and length l.
 * The field consists of an array of h * l integers,
 * dynamically allocated.
 * These integers aren't initialized as soon as they are
 * created.
 */

int *f_create(int h, int l);

/**
 * This is the function that assigns a value to the
 * elements of the field.
 * The empty places are set equal to 0, and the left,
 * right and bottom sides are set equal to 9 as they
 * represent the limit of the field, over which the
 * tetraminos can't go.
 */

void f_clear(int *field, int h, int l);

/**
 * Prints one field for call, depending on the int 'mp'
 * which tells it to print the left one or the second one.
 * Basing on this int, this function prints at a given
 * distance one of the two fields, overwriting the borders
 * (the left, right and bottom sides, of value 9) with
 * ASCII characters that resemble lines, and printing
 * the tetraminos on the field with a special character
 * that resembles a block of a certain color (which depends
 * on the number of tetramino, 1 to 7).
 */

void f_print(int *field, int mp, settings_t *s);

/**
 * Starting from the bottom of the field, it checks
 * if a row is filled from non-zero values; if so, it
 * shifts the field down and calls itself back to check
 * for upper rows.
 */

void f_lower(int *field, int h, int l);

#endif
