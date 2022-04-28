#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>

#include "cpu.h"
#include "field.h"
#include "game.h"
#include "structures.h"
#include "tetramino.h"
#include "utilities.h"

int is_game_over(int *field, int *pieces, int mp, int l) {
  int i;
  for (i = 1; i < l - 1; i++)
    if (field[3 * l + i])
      return 1;
  for (i = 0; i < 7; i++) {
    if (pieces[i])
      break;
    if (i == 6) {
      if (mp)
        return 2;
      return 1;
    }
  }
  return 0;
}

void count_points(int *field, int *points, int *count, int h, int l) {
  int i, j;
  for (i = 4; i < h - 1; i++) {
    for (j = 1; j < l - 1; j++) {
      if (!field[j + i * l])
        break;
      if (j == l - 2)
        (*count)++;
    }
  }
  switch (*count) {
  case 0:
    break;
  case 1:
    (*points)++;
    break;
  case 2:
    *points += 3;
    break;
  case 3:
    *points += 6;
    break;
  case 4:
    *points += 12;
    break;
  default:
    *points += 12;
    break;
  }
}

void reverse_lines(int *field, int *count, int h, int l) {
  int i, j;
  if (*count < 4)
    return;
  for (i = h - 2; i > h - 2 - *count; i--) {
    for (j = 1; j < l - 1; j++) {
      if (field[j + i * l] == 0)
        field[j + i * l] = 7;
      else
        field[j + i * l] = 0;
    }
  }
}

void print_remaining_pieces(int *pieces, int h) {
  int i, j;

  int tetraminos[] = {0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                      0, 0, 1, 0, 0, 0, 2, 0, 0, 0, 3, 3, 0, 4, 4, 0, 0, 5, 5, 5, 0, 6, 6, 6, 0, 0, 7, 7, 0,
                      0, 1, 1, 1, 0, 0, 2, 0, 0, 3, 3, 0, 0, 0, 4, 4, 0, 5, 0, 0, 0, 0, 0, 6, 0, 0, 7, 7, 0,
                      0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

  printf("\033[%d;5HNumero del tetramino:", h + 9); /*First row of the function: display the number of each tetramino*/
  for (i = 0; i < 7; i++)
    printf("\033[%d;%dH\033[0;%dm%d\033[0m", h + 9, 29 + 8 * i, 31 + i, i + 1);
  printf("\n\n");

  for (i = 0; i < 4; i++) {                       /*Second row of the function: display tetraminos' shapes  */
    printf("                        ");
    for (j = 0; j < 29; j++)
      printf("\033[0;%dm%s \033[0m", 30 + tetraminos[j + i * 29], block_c);
    printf("\n");
  }

  printf("\033[%d;1H", h + 16);           /*Clear the third row; useful when we have <10 tetraminos remaining*/
  for (i = 0; i < 100; i++) {
    printf(" ");
  }

  printf("\033[%d;5HPezzi rimanenti:", h + 16); /*Third row of the function: display our remaining tetraminos*/
  for (i = 0; i < 7; i++) {
    printf("\033[%d;%dH\033[0;%dm%d\033[0m", h + 16, 28 + 8 * i + 1, 31 + i,
           pieces[i]);
  }
}

void singleplayer(settings_t *s) {
  int h = s->h, l = s->l, distance = s->distance;
  int *field = f_create(h, l);
  tetramino_t T;
  int x = 4, y = 0; /*  Starting position for our tetraminos  */
  int points = 0;
  int game_over = 0;
  int pieces[7] = {20, 20, 20, 20, 20, 20, 20};
  int mp = 0;
  int count = 0;

  system("clear");

  f_clear(field, h, l);
  f_print(field, mp, s);
  printf("\033[%d;%dHPunti: %d", h + 4, l + 1, points);

  while (!game_over) {
    cursor_hide();
    print_remaining_pieces(pieces, h);
    cursor_show();
    T = t_select(pieces, s);
    cursor_hide();
    print_remaining_pieces(pieces, h);
    t_move(field, T, x, y, mp, &points, s);
    free(T.data);
    count_points(field, &points, &count, h, l);
    printf("\033[%d;%dHPunti: %d", h + 4, l + 1, points);
    f_lower(field, h, l);
    count = 0;
    clear();
    f_print(field, mp, s);
    game_over = is_game_over(field, pieces, mp, l);
  }
  printf("\033[%d;%dH\033[0;31mGame Over!\033[0m\n", h + 5, distance / 2);
  free(field);
}

void multiplayer(settings_t *s) {
  int h = s->h, l = s->l, distance = s->distance;
  int *field1 = f_create(h, l), *field2 = f_create(h, l);
  tetramino_t T;
  int x = 4, y = 0; /*  Starting position for our tetraminos  */
  int points1 = 0, points2 = 0;
  int game_over = 0;
  int pieces[7] = {40, 40, 40, 40, 40, 40, 40};
  int mp = 1;
  int count = 0;

  system("clear");

  f_clear(field1, h, l);
  f_clear(field2, h, l);
  f_print(field1, 0, s);
  f_print(field2, 1, s);
  printf("\033[%d;%dHPunti: %d", h + 4, l + 1, points1);
  printf("\033[%d;%dHPunti: %d", h + 4, distance + l, points2);

  while (!game_over) {
    cursor_hide();
    printf("\033[%d;%dHTocca al giocatore 1!", h + 5, distance / 2 + 5);
    print_remaining_pieces(pieces, h);
    cursor_show();
    T = t_select(pieces, s);
    cursor_hide();
    print_remaining_pieces(pieces, h);
    t_move(field1, T, x, y, 0, &points1, s);
    free(T.data);
    count_points(field1, &points1, &count, h, l);
    reverse_lines(field2, &count, h, l);
    printf("\033[%d;%dHPunti: %d", h + 4, l + 1, points1);
    f_lower(field1, h, l);
    clear();
    f_print(field1, 0, s);
    count = 0;
    game_over = is_game_over(field1, pieces, mp, l);
    if (game_over) {
      /*If the game ends by this point with the default number of pieces is
       *that the first player passed the upper limit of the field, so
       *we can say for sure that player 2 won*/
      printf("\033[%d;%dH\033[0;31mGame Over! Vince il giocatore 2!\033[0m\n", h + 5, distance / 2);
      break;
    }
    printf("\033[%d;%dHTocca al giocatore 2!\n\n\n", h + 5, distance / 2 + 5);
    cursor_show();
    T = t_select(pieces, s);
    print_remaining_pieces(pieces, h);
    cursor_hide();
    t_move(field2, T, x, y, 1, &points2, s);
    free(T.data);
    count_points(field2, &points2, &count, h, l);
    reverse_lines(field1, &count, h, l);
    printf("\033[%d;%dHPunti: %d", h + 4, distance + l, points2);
    f_lower(field2, h, l);
    clear();
    f_print(field2, 1, s);
    count = 0;
    print_remaining_pieces(pieces, h);
    game_over = is_game_over(field2, pieces, mp, l);
    if (game_over == 1)
      printf("\033[%d;%dH\033[0;31mGame Over! Vince il giocatore 1!\033[0m\n", h + 5, distance / 2);
    else if (game_over == 2) {
      if (points1 > points2)
        printf("\033[%d;%dH\033[0;31mGame Over! Vince il giocatore 1!\033[0m\n", h + 5, distance / 2);
      if (points1 < points2)
        printf("\033[%d;%dH\033[0;31mGame Over! Vince il giocatore 2!\033[0m\n", h + 5, distance / 2);
      if (points1 == points2)
        printf("\033[%d;%dH\033[0;31mGame Over! Pareggio!\033[0m\n", h + 5, (distance / 2) + 5);
    }
  }
  cursor_show();
  free(field1);
  free(field2);
}

void cpu(settings_t *s) {
  int h = s->h, l = s->l, distance = s->distance;
  int *field1 = f_create(h, l), *field2 = f_create(h, l);
  tetramino_t T;
  int x = 4, y = 0; /*  Starting position for our tetraminos  */
  int points1 = 0, points2 = 0;
  int game_over = 0;
  int pieces[7] = {40, 40, 40, 40, 40, 40, 40};
  int mp = 2;
  int count = 0;

  system("clear");

  f_clear(field1, h, l);
  f_clear(field2, h, l);
  f_print(field1, 0, s);
  f_print(field2, 2, s);
  printf("\033[%d;%dHPunti: %d", h + 4, l + 1, points1);
  printf("\033[%d;%dHPunti: %d", h + 4, distance + l, points2);

  while (!game_over) {
    cursor_hide();
    printf("\033[%d;%dHTocca al giocatore!", h + 5, distance / 2 + 5);
    print_remaining_pieces(pieces, h);
    cursor_show();
    T = t_select(pieces, s);
    cursor_hide();
    print_remaining_pieces(pieces, h);
    t_move(field1, T, x, y, 0, &points1, s);
    free(T.data);
    count_points(field1, &points1, &count, h, l);
    reverse_lines(field2, &count, h, l);
    printf("\033[%d;%dHPunti: %d", h + 4, l + 1, points1);
    f_lower(field1, h, l);
    clear();
    f_print(field1, 0, s);
    count = 0;
    game_over = is_game_over(field1, pieces, mp, l);
    if (game_over) {
      /*If the game ends by this point with the default number of pieces is
       *that the first player passed the upper limit of the field, so
       *we can say for sure that the CPU won*/
      printf("\033[%d;%dH\033[0;31mGame Over! Vince la CPU!\033[0m\n", h + 5, distance / 2);
      break;
    }
    printf("\033[%d;%dH   Tocca alla CPU!   \n\n\n", h + 5, distance / 2 + 5);
    raw_enable(s);
    cpu_move(field2, pieces, &points2, s);
    raw_disable(s);
    count_points(field2, &points2, &count, h, l);
    reverse_lines(field1, &count, h, l);
    printf("\033[%d;%dHPunti: %d", h + 4, distance + l, points2);
    f_lower(field2, h, l);
    clear();
    f_print(field2, 2, s);
    count = 0;
    game_over = is_game_over(field2, pieces, mp, l);
    if (game_over == 1)
      printf("\033[%d;%dH\033[0;31mGame Over! Vince il giocatore!\033[0m\n", h + 5, distance / 2);
    else if (game_over == 2) {
      if (points1 > points2)
        printf("\033[%d;%dH\033[0;31mGame Over! Vince il giocatore!\033[0m\n", h + 5, distance / 2);
      if (points1 < points2)
        printf("\033[%d;%dH\033[0;31mGame Over! Vince la CPU!\033[0m\n", h + 5, (distance / 2) + 3);
      if (points1 == points2)
        printf("\033[%d;%dH\033[0;31mGame Over! Pareggio!\033[0m\n", h + 5, (distance / 2) + 5);
    }
  }
  cursor_show();
  free(field1);
  free(field2);
}
