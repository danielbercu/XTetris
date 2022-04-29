#ifndef UTILS_H
#define UTILS_H

#include "structures.h"

/**
 * A character we print for each block a tetramino covers
 * in the field. Its color depends on the shape of the
 * tetramino.
 */

#define block_c "■"

/**
 * Quick way to get the size of the edge of a tetramino
 * when we parse its data as a square matrix.
 */

#define edge(t) (int)sqrt(t.size)

/**
 * Hides the cursor.
 */

#define cursor_hide() printf("\033[?25l")

/**
 * Shows the cursor.
 */

#define cursor_show() printf("\033[?25h")

/**
 * "Clears" the screen by setting the cursor to row 1
 * and column 1 on the terminal, so that when it prints
 * the field the next time it will match with the
 * previously printed one. That's a solution to avoid
 * the screen flickering cause by the call of
 * "system(clear)"
 */

#define clear() printf("\033[1;1H")

/**
 * Due to the limitations of ANSI-C, we need to fill
 * our pieces struct integer per integer.
 * That's what this function does.
 */

void init_pieces(pieces_t *p);

/**
 * Returns the current time of the day in seconds
 * and microseconds. This result is then compared
 * to the 'last_drop' of a struct settings,
 * in order to decide when the next drop of the
 * tetramino is going to happen.
 */

long current();

/**
 * Converts the current points to an integer
 * value representing the milliseconds that occurr
 * between a drop and the next one.
 */

long points_to_ms(int points);

/**
 * Wrapper function for usleep;
 * it stops the flow for 'tms' milliseconds.
 */

int msleep(unsigned int tms);

/**
 * This function operates on a struct termios changing
 * some values so that the user's input is interpreted
 * as the movement of the tetramino in the field.
 */

void raw_enable(settings_t *s);

/**
 * It does what it says: restore the default
 * configuration of the terminal and disables Raw Mode.
 */

void raw_disable(settings_t *s);

/**
 * Non-blocking function we use to parse the keys pressed
 * without needing to wait for an Enter key.
 */

char select_char();

#endif
