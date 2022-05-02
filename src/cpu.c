#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "cpu.h"
#include "field.h"
#include "game.h"
#include "structures.h"
#include "tetramino.h"

void cpu_move(int *field, int *pieces, int *points, settings_t *s) {
  int old_points = *points;

  cpu_hit(field, pieces, &old_points, s);

  if (old_points != *points)
    return;
  else
    cpu_guess(field, pieces, s);
}

void cpu_hit(int *field, int *pieces, int *old_points, settings_t *s) {
  int i, j, k;
  int new_points = *old_points;
  int count = 0;
  int h = s->h, l = s->l;
  int *temp = f_create(h, l);
  tetramino_t T;

  for (i = 0; i < 7; i++) {
    if (pieces[i]) {
      T = t_create(s->p.Ts[i].data, s->p.Ts[i].size);
      for (j = 0; j < 4; j++) {
        for (k = -1; k < l - 1; k++) {
          memcpy(temp, field, sizeof(int) * h * l);
          t_gravity(temp, T, k, 0, 0, s, 0);
          count_points(temp, &new_points, &count, h, l);
          if (new_points != *old_points) {
            *old_points = new_points;
            pieces[i]--;
            print_remaining_pieces(pieces, h);
            t_gravity(field, T, k, 0, 100, s, 1);
            free(T.data);
            free(temp);
            return;
          }
          count = 0;
        }
        t_rotate(T);
      }
      free(T.data);
    }
  }
  free(temp);
}

void cpu_rand_move(int *field, int *pieces, settings_t *s, int d_left, int d_right) {
  int h = s->h, l = s->l;
  int rand_x;
  int random_default = s->random;
  tetramino_t T;
  int fourpcs_tet = pieces[1]; /*An additional check we make to make sure that *
                                *the four piece tetramino gets placed in the   *
                                *right position */

  s->random = 1;
  T = t_select(pieces, s);
  s->random = random_default;
  rand_x = d_left + rand() % d_right;
  /* Some checks for unbugged placement */
  if (pieces[1] != fourpcs_tet)
    rand_x -= 2;
  while (t_collision(field, T, rand_x, 0, 'r', l) && rand_x > 1 && rand_x < 8)
    rand_x--;
  while (t_collision(field, T, rand_x, 0, 'l', l) && rand_x > 1 && rand_x < 8)
    rand_x++;
  while (t_collision(field, T, rand_x, 0, 'd', l) && rand_x < 4)
    rand_x++;
  while (t_collision(field, T, rand_x, 0, 'd', l) && rand_x > 8)
    rand_x--;
  print_remaining_pieces(pieces, h);
  t_gravity(field, T, rand_x, 0, 100, s, 1);
  free(T.data);
}

int lowest_empty_row(int *field, int h, int l) {
  int i, j;
  for (i = h - 2; i > 2; i--) {
    for (j = 1; j < l - 1; j++) {
      if (field[i * l + j] != 0)
        break;
      if (j == l - 2)
        return i;
    }
  }
  return 0;
}

int consec_spaces_under_lowest(int *field, int l, int ler, int *d_left,
                               int *d_right) {
  int i = 0, j = 0, max = 0, count = 0;
  int temp_left = 0, temp_right = 0;
  for (i = 1; i < l - 1; i++) {
    j = i;
    while (field[(ler + 1) * l + j] == 0) {
      if (temp_left == 0)
        temp_left = j;
      count++;
      if (field[(ler + 1) * l + j + 1] != 0)
        temp_right = j;
      j++;
    }
    if (count > max) {
      *d_left = temp_left;
      *d_right = temp_right;
      max = count;
    }
    temp_left = temp_right = count = 0;
  }
  return max;
}

void cpu_guess(int *field, int *pieces, settings_t *s) {
  int i;
  int h = s->h, l = s->l;
  int d_left = 1, d_right = 8;
  int ler = lowest_empty_row(field, h, l);
  int csul = consec_spaces_under_lowest(field, l, ler, &d_left, &d_right);
  int val = -1;
  int valid = 1;
  tetramino_t T;

  switch (csul) {
  case 1:
    for (i = 0; i < 4; i++) {
      valid &= field[(ler + 1 + i) * l + d_left] == 0;
    }
    if (valid) {
      if (pieces[1]) {
        T = t_create(s->p.Ts[1].data, s->p.Ts[1].size);
        pieces[1]--;
        d_left -= 2;
        break;
      }
    }
    if (field[ler * l + d_left + 1] == 0) {
      if (field[ler * l + d_left + 2] == 0) {
        if (pieces[4]) {
          T = t_create(s->p.Ts[4].data, s->p.Ts[4].size);
          pieces[4]--;
          break;
        }
      } else {
        if (pieces[0]) {
          T = t_create(s->p.Ts[0].data, s->p.Ts[0].size);
          pieces[0]--;
          for (i = 0; i < 3; i++)
            t_rotate(T);
          d_left--;
          break;
        }
      }
    }
    if (field[ler * l + d_left - 1] == 0) {
      if (field[ler * l + d_left - 2] == 0) {
        if (pieces[5]) {
          T = t_create(s->p.Ts[5].data, s->p.Ts[5].size);
          pieces[5]--;
          d_left -= 2;
          break;
        }
      } else {
        if (pieces[0]) {
          T = t_create(s->p.Ts[0].data, s->p.Ts[0].size);
          pieces[0]--;
          d_left--;
          t_rotate(T);
          break;
        }
      }
    }
    cpu_rand_move(field, pieces, s, d_left, d_right);
    return;
  case 2:
    if (field[(ler + 2) * l + d_left] == 0 &&
        field[(ler + 2) * l + d_left + 1] == 0) {
      if (pieces[6]) {
        T = t_create(s->p.Ts[6].data, s->p.Ts[6].size);
        pieces[6]--;
        break;
      }
    }
    if (field[ler * l + d_left + 2] == 0 &&
        field[(ler + 1) * l + d_left + 2] != 0) {
      if (pieces[2]) {
        T = t_create(s->p.Ts[2].data, s->p.Ts[2].size);
        pieces[2]--;
        break;
      }
    }
    if (field[ler * l + d_left - 1] == 0 &&
        field[(ler + 1) * l + d_left - 1] != 0) {
      if (pieces[3]) {
        T = t_create(s->p.Ts[3].data, s->p.Ts[2].size);
        pieces[3]--;
        d_left--;
        break;
      }
    }
    if (pieces[2] || pieces[3]) {
      val = 2 + rand() % 2;
      while (!pieces[val])
        val = 2 + rand() % 2;
      T = t_create(s->p.Ts[val].data, s->p.Ts[val].size);
      pieces[val]--;
      break;
    }
    cpu_rand_move(field, pieces, s, d_left, d_right);
    return;
  case 3:
    if (field[(ler + 2) * l + d_left] != 0 &&
        field[(ler + 2) * l + d_left + 2] != 0 &&
        field[(ler + 2) * l + d_left + 1] == 0) {
      if (pieces[0]) {
        T = t_create(s->p.Ts[0].data, s->p.Ts[0].size);
        pieces[0]--;
        t_rotate(T);
        t_rotate(T);
        break;
      }
    } else {
      if (pieces[4] || pieces[5]) {
        val = 4 + rand() % 2;
        while (!pieces[val])
          val = 4 + rand() % 2;
        T = t_create(s->p.Ts[val].data, s->p.Ts[val].size);
        t_rotate(T);
        t_rotate(T);
        pieces[val]--;
        break;
      }
    }
    cpu_rand_move(field, pieces, s, d_left, d_right);
    return;
  /* We are going to use the four shaped tetramino placed
     horizontally only when we have 4 or 5 consecutive empty
     spaces, so that our CPU will avoid using sudden piece
     too much */
  case 4:
    if (pieces[1]) {
      T = t_create(s->p.Ts[1].data, s->p.Ts[1].size);
      t_rotate(T);
      pieces[1]--;
      break;
    }
    cpu_rand_move(field, pieces, s, d_left, d_right);
    return;
  case 5:
    if (pieces[1]) {
      T = t_create(s->p.Ts[1].data, s->p.Ts[1].size);
      t_rotate(T);
      pieces[1]--;
      break;
    }
    cpu_rand_move(field, pieces, s, d_left, d_right);
    return;
  default:
    cpu_rand_move(field, pieces, s, d_left, d_right);
    return;
  }
  while (t_collision(field, T, d_left, 0, 'r', l))
    d_left--;
  while (t_collision(field, T, d_left, 0, 'l', l))
    d_left++;
  while (t_collision(field, T, d_left, 0, 'd', l) && d_left < 4)
    d_left++;
  while (t_collision(field, T, d_left, 0, 'd', l) && d_left > 8)
    d_left--;
  print_remaining_pieces(pieces, h);
  t_gravity(field, T, d_left, 0, 100, s, 1);
  free(T.data);
}
