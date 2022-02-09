#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

#define edge(t)                                                                \
  (int)sqrt(t.size) // General way to get the size of the edge of a square
                    // matrix, with the further declaration of tetramino_t
#define clear()                                                                \
  printf("\033[1;1H") // printf("\033[1;1H") or printf("\033[1;1H\033[2J")
#define cursor_hide() printf("\033[?25l")
#define cursor_show() printf("\033[?25h")
#define block_c "■" // ■ or ⬛

const int h = 20, l = 12,
          area = h * l; //  4 rows for initial placement, 15 rows for the main
                        //  field and 1 for the ground

typedef struct tetramino {
  int *data;
  int size;
} tetramino_t;

int T1[9] = {0, 1, 0,
             1, 1, 1,
             0, 0, 0};      // T
int T2[16] = {0, 0, 2, 0,
              0, 0, 2, 0, 
              0, 0, 2, 0, 
              0, 0, 2, 0};  // I
int T3[9] = {0, 3, 3, 
             3, 3, 0,
             0, 0, 0};      // S
int T4[9] = {4, 4, 0, 
             0, 4, 4,
             0, 0, 0};      // Z
int T5[9] = {5, 5, 5,
             5, 0, 0,
             0, 0, 0};      // L
int T6[9] = {6, 6, 6,
             0, 0, 6,
             0, 0, 0};      // J
int T7[4] = {7, 7,
             7, 7};         // O

tetramino_t Ts[] = {{T1, 9}, {T2, 16}, {T3, 9}, {T4, 9},
                    {T5, 9}, {T6, 9},  {T7, 4}};

char shapes[] = "TISZLJO";

struct termios canon;

int msleep(unsigned int tms) { return usleep(tms * 1000); }

void raw_enable() {
  struct termios raw;

  tcgetattr(0, &canon);
  raw = canon;
  raw.c_lflag &= ~ECHO;
  raw.c_lflag &= ~ICANON;
  raw.c_lflag &= ~ISIG;
  raw.c_cc[VMIN] = 0;
  raw.c_cc[VTIME] = 0;

  tcsetattr(0, TCSANOW, &raw);
}

void raw_disable() { tcsetattr(0, TCSANOW, &canon); }

tetramino_t t_create(int *src, int size) {
  tetramino_t t;
  int i;
  t.size = size;
  t.data = malloc(sizeof(int) * size);
  for (i = 0; i < t.size; i++)
    t.data[i] = src[i];
  return t;
}

void stats_print(int points, int* pieces){
  int i;
  printf("\n  Punti: %d\n  Tetramini rimasti:\n\n     Numero tetramino:     ", points);

    for (i = 0; i < 7; i++)
      printf("\033[0;%dm%d\033[0m   ", 31 + i, i + 1);
    printf("\n     Forma tetramino:      ");
    for (i = 0; i < 7; i++)
      printf("\033[1;%dm%c\033[0m   ", 31 + i, shapes[i]);
    printf("\n     Pezzi rimasti:       ");
    for (i = 0; i < 7; i++)
      printf("\033[4;%dm%d\033[0m  ", 31 + i, pieces[i]);
    printf("\n\n");
}

void t_rotate(tetramino_t t) {
  int i, j, e = edge(t);
  tetramino_t temp = t_create(t.data, t.size);
  for (i = 0; i < e; i++)
    for (j = 0; j < e; j++)
      t.data[j + i * e] = temp.data[j * e + e - i - 1];
  free(temp.data);
}

void t_print(tetramino_t t) {
  int i, j, e = edge(t);
  for (i = 0; i < e; i++) {
    for (j = 0; j < e; j++)
      printf("%d ", t.data[j + i * e]);
    printf("\n");
  }
  printf("\n");
}

void t_place(int *field, tetramino_t t, int x, int y) { // statically
  int i, j, e = edge(t);
  for (i = 0; i < e; i++)
    for (j = 0; j < e; j++) {
      if (field[x + j + (y + i) * l] != 0 && t.data[j + i * e] == 0)
        continue;
      if (field[x + j + (y + i) * l] == 0)
        field[x + j + (y + i) * l] = t.data[j + i * e];
    }
}

void f_print(int *field) {
  int i, j, k = 15;
  for (i = 0; i < 4; i++) {
    printf("       ");
    for (j = 1; j < l - 1; j++)
      printf("\033[0;%dm%s \033[0m", 30 + field[j + i * l], block_c);
    printf("\n");
  }
  for (i = 4; i < h - 1; i++, k--) {
    if (k < 10)
      printf(" ");
    printf("  %d ║ ", k);
    for (j = 1; j < l - 1; j++)
      printf("\033[0;%dm%s \033[0m", 30 + field[j + i * l], block_c);
    printf("║\n");
  }
  printf("     ╚");
  for (i = 0; i < 2 * l - 3; i++)
    printf("═");
  printf("╝\n      ");
  for (i = 1; i < l - 1; i++)
    printf(" %d", i);
  printf("\n\n");
}

tetramino_t t_select(int *field, int* pieces, int points) {
  int t;
  printf("  Seleziona un tetramino: ");
  scanf("%d", &t);
  system("clear");
  f_print(field);
  if (t > 0 && t < 8 && pieces[t - 1]){
    pieces[t - 1]--;
    stats_print(points, pieces);
    return t_create(Ts[t - 1].data, Ts[t - 1].size);
  }
  while (t < 1 || t > 7 || !pieces[t - 1]){
    system("clear");
    f_print(field);
    stats_print(points, pieces);
    printf("  Inserisci un numero valido: ");
    scanf("%d", &t);
    if (t > 0 && t < 8 && pieces[t - 1]){
      pieces[t - 1]--;
      system("clear");
      f_print(field);
      stats_print(points, pieces);
      return t_create(Ts[t - 1].data, Ts[t - 1].size);
    }
    clear();
    system("clear");
  }
}

void f_clear(int *field) {
  int i, j;
  for (i = 0; i < h - 1; i++)
    for (j = 0; j < l; j++)
      field[j + i * l] = 0;
  for (i = 0; i < l; i++)
    field[(h - 1) * l + i] = 9;
  for (i = 0; i < h; i++)
    field[i * l] = field[i * l + (l - 1)] = 9;
}

int *f_create(int h, int l) {
  int *field = malloc(sizeof(field) * area);
  return field;
}

int t_collision(int *field, tetramino_t t, int x, int y, char dir) {
  int i, j, found, e = edge(t);
  switch (dir) {
  case 'd': // down movement
    for (i = 0; i < e; i++) {
      found = 0;
      for (j = e - 1; j >= 0; j--) {
        found = t.data[i + j * e] != 0;
        if (found && field[x + i + (y + j) * l] != 0)
          return 1;
      }
    }
    return 0;
  case 'r': // right movement
    for (i = 0; i < e; i++) {
      found = 0;
      for (j = e - 1; j >= 0; j--) {
        found = t.data[j + i * e] != 0;
        if (found && field[x + j + (y + i) * l] != 0)
          return 1;
      }
    }
    return 0;
  case 'l': // left movement
    for (i = 0; i < e; i++) {
      found = 0;
      for (j = 0; j < e; j++) {
        found = t.data[j + i * e] != 0;
        if (found && field[x + j + (y + i) * l] != 0)
          return 1;
      }
    }
    return 0;
  }
}

void t_gravity(int *field, tetramino_t t, int x, int y, int lvl) {
  char dir = 'd';
  int tms;
  int *temp_1 = f_create(h, l);
  int *temp_2 = f_create(h, l);
  memcpy(temp_1, field, sizeof(int) * area);
  memcpy(temp_2, field, sizeof(int) * area);
  cursor_hide();
  switch (lvl) {
  case 1:
    tms = 1000;
    break;
  case 2:
    tms = 700;
    break;
  case 3:
    tms = 500;
    break;
  case 4:
    tms = 400;
    break;
  case 5:
    tms = 300;
    break;
  case 99:
    tms = 10;
    break;
  }
  while (!t_collision(field, t, x, y, dir)) {
    clear();
    t_place(temp_1, t, x, y++);
    f_print(temp_1);
    printf("\n");
    memcpy(temp_2, temp_1, sizeof(int) * area);
    memcpy(temp_1, field, sizeof(int) * area);
    msleep(tms);
    clear();
  }
  memcpy(field, temp_2, sizeof(int) * area);
  f_print(field);
  cursor_show();
  free(temp_1);
  free(temp_2);
}

void t_move(int *field, tetramino_t t, int x, int y) {
  int i;
  char dir;
  char k;
  int *temp_1 = f_create(h, l);
  int *temp_2 = f_create(h, l);
  tetramino_t temp = t_create(t.data, t.size);
  memcpy(temp_1, field, sizeof(int) * area);
  memcpy(temp_2, field, sizeof(int) * area);
  cursor_hide();
  raw_enable();
  do {
    scanf("%c", &k);
    switch (k) {
    case 'w':
      t_rotate(temp);
      if (!t_collision(temp_1, temp, x, y, dir))
        t_rotate(t);
      else
        for (i = 0; i < 3; i++)
          t_rotate(temp);
      break;
    case 's':
      dir = 'd';
      if (!t_collision(temp_1, t, x, y + 1, dir)) { // or y + 2 
        y++;
        break;
      } else {
        clear();
        t_place(field, t, x, y); // or y + 1
        f_print(field);
        raw_disable();
        cursor_show();
        free(temp_1);
        free(temp_2);
        free(temp.data);
        return;
      }
    case 'd':
      dir = 'r';
      if (!t_collision(temp_1, t, x + 1, y, dir))
        x++;
      break;
    case 'a':
      dir = 'l';
      if (!t_collision(temp_1, t, x - 1, y, dir))
        x--;
      break;
    default:
      break;
    }
    clear();
    t_place(temp_1, t, x, y);
    f_print(temp_1);
    printf("\n");
    memcpy(temp_2, temp_1, sizeof(int) * area);
    memcpy(temp_1, field, sizeof(int) * area);
    if (k == 'e')
      t_gravity(field, t, x, y, 99);
    clear();
  } while (k != 'e');
  raw_disable();
  clear();
  f_print(field);
  cursor_show();
  free(temp_1);
  free(temp_2);
  free(temp.data);
}

int is_game_over(int* field, int* pieces){
  int i;
  for (i = 3 * l + 1; i < 4 * l - 1; i++)
      if (field[i])
        return 1;
  for (i = 0; i < sizeof(pieces)/sizeof(int); i++){
      if (pieces[i]) break;
      if ((i == sizeof(pieces)/sizeof(int) - 1) && !pieces[i])
        return 1;
    }
  return 0;
}

void count_points(int* field, int *points){
  int i, j, count = 0;
  for (i = 4; i < h - 1; i++){
    for (j = 1; j < l - 1; j++){
      if (!field[j + i * l])
        break;
      if ((j == l - 2) && field[j + i * l]) count++;
    }
  }
  switch (count){
        case 0:
          break;
        case 1:
          (*points)++;
          break;
        case 2:
          *points = *points + 3;
          break;
        case 3:
          *points = *points + 6;
          break;
        case 4:
          *points = *points + 12;
          break;
        default:
          *points = *points + 12;
          break;
      }
}

//void reverse_lines(int* field){     //mp
//  int i, j, row_is_1 = 1;
//  int* reversed = malloc(sizeof(int) * l - 2);
//  for (i = h - 2; i > 3; i--){
//    k = 0;
//    for (j = 1; j < l - 1; j++){
//      row_is_1 = field[j + i * l] != 0;
//      if (!row_is_1)
//        break;
//      if (field[j + i * l])
//        reversed[k++] = 7;
//    }
//  }
//}

void check_lines(int* field){
  int i, j, k, q;
  for (i = h - 2; i > 3; i--){
    for (j = 1; j < l - 1; j++){
      if (!field[j + i * l])
        break;
      if ((j == l - 2) && field[j + i * l]){
        for (k = 1; k < l - 1; k++)
          for (q = i; q > 3; q--)
            field[k + (q) * l] = field[k + (q - 1) * l];
          check_lines(field);
      }
    }
  }
}

int main() {
  int *field;
  tetramino_t T;
  int x = 4, y = 0; //	starting position for our tetraminos
  int i, j;
  int points = 0, count = 0;
  int game_over = 0;
  int pieces[7] = {20, 20, 20, 20, 20, 20, 20};
  
  system("clear");

  field = f_create(h, l);
  f_clear(field);
  f_print(field);
  stats_print(points, pieces);

  while (!game_over) {
    T = t_select(field, pieces, points);
    t_move(field, T, x, y);
    free(T.data);
    count_points(field, &points);
    check_lines(field);
    clear();
    f_print(field);
    stats_print(points, pieces);
    game_over = is_game_over(field, pieces);
  }

  printf("  \033[0;31mGame Over! \033[0m\n\n");

  free(field);

  return 0;
}