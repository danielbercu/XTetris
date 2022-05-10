#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "field.h"
#include "structures.h"
#include "tetramino.h"
#include "utilities.h"

tetramino_t t_create(int *src, int size) {
  tetramino_t t;
  int i;
  t.size = size;
  t.data = malloc(sizeof(int) * size);
  for (i = 0; i < t.size; i++)
    t.data[i] = src[i];
  return t;
}

void t_rotate(tetramino_t t) {
  int i, j, e = edge(t);
  tetramino_t temp = t_create(t.data, t.size);
  for (i = 0; i < e; i++)
    for (j = 0; j < e; j++)
      t.data[j + i * e] = temp.data[(e - 1 - j) * e + i];     /*  Counter-clockwise: t.data[j + i * e] = temp.data[(j + 1) * e - i - 1];  */
                                                              /*  Clockwise:         t.data[j + i * e] = temp.data[(e - 1 - j) * e + i];  */
  free(temp.data);
}

void t_print(tetramino_t t) {
  int i, j, e = edge(t);
  for (i = 0; i < e; i++) {
    for (j = 0; j < e; j++)
      printf("\033[0;%dm%s \033[0m", 30 + t.data[j + i * e], block_c);
    printf("\n");
  }
}

void t_place(int *field, tetramino_t t, int x, int y, int l) { /* statically */
  int i, j, e = edge(t);
  for (i = 0; i < e; i++)
    for (j = 0; j < e; j++) {
      if (field[x + j + (y + i) * l] != 0 && t.data[j + i * e] == 0)
        continue;
      if (field[x + j + (y + i) * l] == 0)
        field[x + j + (y + i) * l] = t.data[j + i * e];
    }
}

tetramino_t t_select(int *pieces, settings_t *s) {
  int t, valid;
  int h = s->h;
  tetramino_t T;
  srand(time(0));
  if (s->random) {
    t = rand() % 7;
    if (!pieces[t]) {
      valid = 0;
      while (!valid) {
        t = rand() % 7;
        valid = (pieces[t] != 0);
      }
    }
    pieces[t]--;
    T = t_create(s->p.Ts[t].data, s->p.Ts[t].size);
  } else {
    printf("\033[%d;5H                       \033[%d;5HSeleziona un tetramino: ", h + 18, h + 18);
    scanf("%d", &t);
    printf("\033[%d;4H                               \r", h + 18);
    if (t >= 0 && t <= 6 && pieces[t]) {
      pieces[t - 1]--;
      T = t_create(s->p.Ts[t].data, s->p.Ts[t].size);
    }
    while (t < 0 || t > 6 || !pieces[t]) {
      printf("\033[%d;5H                       \033[%d;5HInserisci un numero valido: ", h + 18, h + 18);
      scanf("%d", &t);
      printf("\033[%d;5H                                        \r", h + 18);
      if (t >= 0 && t <= 6 && pieces[t]) {
        pieces[t]--;
        T = t_create(s->p.Ts[t].data, s->p.Ts[t].size);
      }
      clear();
    }
  }
  return T;
}

int t_collision(int *field, tetramino_t t, int x, int y, char dir, int l) {
  int i, j, found, e = edge(t);
  switch (dir) {
  case 'd': /* down movement */
    for (i = 0; i < e; i++) {
      found = 0;
      for (j = e - 1; j >= 0; j--) {
        found = t.data[i + j * e] != 0;
        if (found && field[x + i + (y + j) * l] != 0)
          return 1;
      }
    }
    break;
  case 'r': /* right movement */
    for (i = 0; i < e; i++) {
      found = 0;
      for (j = e - 1; j >= 0; j--) {
        found = t.data[j + i * e] != 0;
        if (found && field[x + j + (y + i) * l] != 0)
          return 1;
      }
    }
    break;
  case 'l': /* left movement */
    for (i = 0; i < e; i++) {
      found = 0;
      for (j = 0; j < e; j++) {
        found = t.data[j + i * e] != 0;
        if (found && field[x + j + (y + i) * l] != 0)
          return 1;
      }
    }
    break;
  }
  return 0;
}

void t_gravity(int *field, tetramino_t t, int x, int y, int ms, settings_t *s, int print) {
  int h = s->h, l = s->l;
  char dir = 'd';
  int *temp_1 = f_create(h, l);
  int *temp_2 = f_create(h, l);
  memcpy(temp_1, field, sizeof(int) * h * l);
  memcpy(temp_2, field, sizeof(int) * h * l);
  cursor_hide();
  while (!t_collision(field, t, x, y, dir, l)) {
    clear();
    t_place(temp_1, t, x, y++, l);
    if (print)
      f_print(temp_1, 2, s);
    msleep(ms);
    printf("\n");
    memcpy(temp_2, temp_1, sizeof(int) * h * l);
    memcpy(temp_1, field, sizeof(int) * h * l);
    clear();
  }
  memcpy(field, temp_2, sizeof(int) * h * l);
  if (print)
    f_print(field, 2, s);
  cursor_show();
  free(temp_1);
  free(temp_2);
}

void t_move(int *field, tetramino_t t, int x, int y, int mp, int *points, settings_t *s) {
  int h = s->h, l = s->l;
  int i, tbd;
  char dir, k;
  int *temp_1 = f_create(h, l);
  int *temp_2 = f_create(h, l);
  memcpy(temp_1, field, sizeof(int) * h * l);
  clear();
  t_place(temp_1, t, x, y, l);
  f_print(temp_1, mp, s);
  memcpy(temp_2, temp_1, sizeof(int) * h * l);
  memcpy(temp_1, field, sizeof(int) * h * l);
  cursor_hide();
  raw_enable(s);
  s->last_drop = current();
  while (1) {
    k = select_char();
    switch (k) {
      case 'w':
        t_rotate(t);
        if (t_collision(temp_1, t, x, y, dir, l))
          for (i = 0; i < 3; i++)
            t_rotate(t);
        break;
      case 's':
        dir = 'd';
        if (!t_collision(temp_1, t, x, y + 1, dir, l))
          y++;
        break;
      case 'd':
        dir = 'r';
        if (!t_collision(temp_1, t, x + 1, y, dir, l))
          x++;
        break;
      case 'a':
        dir = 'l';
        if (!t_collision(temp_1, t, x - 1, y, dir, l))
          x--;
        break;
      case ' ':
        while (1) {
          if (!t_collision(temp_1, t, x, y + 1, dir,l)) {
            y++;
          } else {
            clear();
            t_place(field, t, x, y, l);
            f_print(temp_1, mp, s);
            raw_disable(s);
            cursor_show();
            free(temp_1);
            free(temp_2);
            return;
          }
        }
        break;
      default:
        tbd = points_to_ms(*points);
        dir = 'd';
        if (current() - s->last_drop > tbd) {
          s->last_drop = current();
          if (!t_collision(temp_1, t, x, y + 1, dir,l)) {
            y++;
            break;
          } else {
            clear();
            t_place(field, t, x, y, l);
            f_print(temp_1, mp, s);
            raw_disable(s);
            cursor_show();
            free(temp_1);
            free(temp_2);
            return;
          }
        }
        break;
      }
    clear();
    t_place(temp_1, t, x, y, l);
    f_print(temp_1, mp, s);
    memcpy(temp_2, temp_1, sizeof(int) * h * l);
    memcpy(temp_1, field, sizeof(int) * h * l);
  }
  raw_disable(s);
  clear();
  f_print(temp_1, mp, s);
  cursor_show();
  free(temp_1);
  free(temp_2);
}
