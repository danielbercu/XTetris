#include <stdio.h>
#include <stdlib.h>
#include <termios.h>

#include "field.h"
#include "structures.h"
#include "utilities.h"

int *f_create(int h, int l) {
  int *field = malloc(sizeof(field) * h * l);
  return field;
}

void f_clear(int *field, int h, int l) {
  int i, j;
  for (i = 0; i < h - 1; i++)
    for (j = 0; j < l; j++)
      field[j + i * l] = 0;
  for (i = 0; i < l; i++)
    field[(h - 1) * l + i] = 9;
  for (i = 0; i < h; i++)
    field[i * l] = field[i * l + (l - 1)] = 9;
}

void f_print(int *field, int mp, settings_t *s) {
  int h = s->h, l = s->l, distance = s->distance;
  int i, j, k = h - 5;
  for (i = 0; i < 4; i++) {
    if (mp)
      printf("\033[%d;%dH", i + 1, distance);
    printf("       ");
    for (j = 1; j < l - 1; j++) {
      printf("\033[0;%dm%s \033[0m", 30 + field[j + i * l], block_c);
    }
    printf("\n");
  }
  for (i = 4; i < h - 1; i++, k--) {
    if (mp) {
      printf("\033[%d;%dH", i + 1, distance);
    }
    if (k < 10) {
      printf(" ");
    }
    printf("  %d ║ ", k);
    for (j = 1; j < l - 1; j++) {
      printf("\033[0;%dm%s \033[0m", 30 + field[j + i * l], block_c);
    }
    printf("║");
    if (!mp) {
      printf("\n");
    }
  }
  if (mp) {
    printf("\033[%d;%dH", h, distance);
  }
  printf("     ╚");
  for (i = 0; i < 2 * l - 3; i++) {
    if (mp) {
      printf("\033[%d;%dH", h, distance + 6 + i);
    }
    printf("═");
  }
  if (mp) {
    printf("\033[%d;%dH", h, distance + 2 * l + 3);
  }
  printf("╝\n      ");
  if (mp) {
    printf("\033[%d;%dH", h + 1, distance + 6);
  }
  for (i = 1; i < l - 1; i++) {
    printf(" %d", i);
  }
  printf("\n\n");
  switch (mp) {
  case 0:
    printf("\033[%d;%dHGiocatore 1", h + 3, l - 1);
    break;
  case 1:
    printf("\033[%d;%dHGiocatore 2", h + 3, distance + l - 1);
    break;
  case 2:
    printf("\033[%d;%dHCPU", h + 3, distance + l + 3);
    break;
  }
}

void f_lower(int *field, int h, int l) {
  int i, j, k, q;
  for (i = h - 2; i > 2; i--) {
    for (j = 1; j < l - 1; j++) {
      if (!field[j + i * l])
        break;
      if (j == l - 2) {
        for (k = 1; k < l - 1; k++)
          for (q = i; q > 2; q--)
            field[k + q * l] = field[k + (q - 1) * l];
        f_lower(field, h, l);
      }
    }
  }
}
