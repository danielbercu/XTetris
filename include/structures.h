#ifndef STRUCTURES_H
#define STRUCTURES_H

#include <termios.h>

/**
 * User defined type that we can easily use to represent
 * tetraminos.
 */

typedef struct tetramino {

  /**
   * An array of values between 0 and 7. Due to the array
   * being easily representable as a square, the numbers
   * which compose the tetramino itself form its shape
   * inside the square matrix, while the empty spaces
   * are codified with the number 0.
   */

  int *data;

  /**
   * Tetramino's size. Must be a perfect square.
   */

  int size;

} tetramino_t;

/**
 * This structure contains 7 arrays, one for
 * each different-shaped tetramino, with their
 * size being the size of the array of the
 * tetramino they represent.
 * This structure also contains an array of type
 * tetramino_t of size 7, one position for each
 * tetramino.
 * Coding our tetraminos this way leads to a
 * very easy method to create a tetramino every
 * time we need one.
 */

typedef struct pieces {

  /**
   * T shape.
   */

  int T1[9];

  /**
   * I shape.
   */

  int T2[16];

  /**
   * S shape.
   */

  int T3[9];

  /**
   * Z shape.
   */

  int T4[9];

  /**
   * L shape.
   */

  int T5[9];

  /**
   * J shape.
   */

  int T6[9];

  /**
   * O shape.
   */

  int T7[4];

  /**
   * The array containing one tetramino for every shape.
   */

  tetramino_t Ts[7];

} pieces_t;

/**
 * A big container of various objects.
 */

typedef struct settings {

  /**
   * Field's height.
   */

  int h;

  /**
   * Field's length.
   */

  int l;

  /**
   * Distance between two fields.
   */

  int distance;

  /**
   * Control variable that manages gamemode selection.
   */

  int gamemode;

  /**
   * Control variable that toggles the restart of the game.
   */

  int replay;

  /**
   * Control variable that manages the tetramino
   * selection.
   */

  int random;

  /**
   * A struct pieces which we carry through all the code
   * to easily create tetraminos in dynamic memory.
   */

  pieces_t p;

  /**
   * Control variable that remembers the last time
   * a piece has dropped by 1 row.
   */

  long last_drop;

  /**
   * A struct termios we need to toggle on and off
   * the Raw Mode.
   */

  struct termios original;

} settings_t;

#endif
