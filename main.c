#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define edge(t)                                                                \
  (int)sqrt(t.size) // General way to get the size of the edge of a square
                    // matrix, with the further declaration of tetramino_t
#define clear() printf("\033[1;1H")
#define hide_cursor() printf("\033[?25l")
#define show_cursor() printf("\033[?25h")
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
             0, 0, 0};  // T
int T2[16] = {0, 0, 2, 0,
              0, 0, 2, 0, 
              0, 0, 2, 0, 
              0, 0, 2, 0};  // I
int T3[9] = {0, 3, 3, 
             3, 3, 0,
             0, 0, 0};  // N1
int T4[9] = {4, 4, 0, 
             0, 4, 4,
             0, 0, 0};  // N2
int T5[9] = {5, 5, 5,
             5, 0, 0,
             0, 0, 0};  // L
int T6[9] = {6, 6, 6,
             0, 0, 6,
             0, 0, 0};  // J
int T7[4] = {7, 7,
             7, 7}; // O

tetramino_t Ts[] = {{T1, 9}, {T2, 16}, {T3, 9}, {T4, 9}, {T5, 9}, {T6, 9},  {T7, 4}};

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
  int i, j, l = edge(t);
  tetramino_t temp = t_create(t.data, t.size);
  for (i = 0; i < l; i++)
    for (j = 0; j < l; j++)
      t.data[j + i * l] = temp.data[j * l + l - i - 1];
  free(temp.data);
}

void t_print(tetramino_t t) {
  int i, j, l = edge(t);
  for (i = 0; i < l; i++) {
    for (j = 0; j < l; j++)
      printf("%d ", t.data[j + i * l]);
    printf("\n");
  }
  printf("\n");
}

void t_place(int *field, tetramino_t t, int x, int y) { // statically
  int i, j, m = edge(t);
  for (i = 0; i < m; i++)
    for (j = 0; j < m; j++) {
      if (field[x + j + (y + i) * l] != 0 && t.data[j + i * m] == 0)
        continue;
      if (field[x + j + (y + i) * l] == 0)
        field[x + j + (y + i) * l] = t.data[j + i * m];
    }
}

tetramino_t t_select() {
  int t;
  printf("Seleziona un tetramino: ");
  scanf("%d", &t);
  do {
    if (t > 0 && t < 8) {
      printf("\n");
      return t_create(Ts[t - 1].data, Ts[t - 1].size);
    } else {
      printf("Inserisci un numero valido: ");
      scanf("%d", &t);
    }
  } while (t < 1 || t > 7);
}

void f_print(int *field) {
  int i, j, k = 15;
  for (i = 0; i < 4; i++) {
    printf("     ");
    for (j = 1; j < l - 1; j++)
      printf("\033[0;%dm%s \033[0m", 30 + field[j + i * l], block_c);
    printf("\n");
  }
  for (i = 4; i < h - 1; i++, k--) {
    if (k < 10)
      printf(" ");
    printf("%d ║ ", k);
    for (j = 1; j < l - 1; j++)
      printf("\033[0;%dm%s \033[0m", 30 + field[j + i * l], block_c);
    printf("║\n");
  }
  printf("   ╚");
  for (i = 0; i < 2 * l - 3; i++)
    printf("═");
  printf("╝\n    ");
  for (i = 1; i < l - 1; i++)
    printf(" %d", i);
  printf("\n\n");
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

void t_move(int *field, tetramino_t t) {
  int x = 5, y = 0;
  if (getchar() == '\033') {
    getchar();
    switch (getchar()) {
    case 'A':
      t_rotate(t);
      t_place(field, t, x, y);
      f_print(field);
      break;
    case 'B':
      y++;
      t_place(field, t, x, y);
      f_print(field);
      break;
    case 'C':
      x++;
      t_place(field, t, x, y);
      f_print(field);
      break;
    case 'D':
      x--;
      t_place(field, t, x, y);
      f_print(field);
      break;
    }
  } else if (getchar() == '\n')
    t_place(field, t, x, 14);
  //	do{
  //		switch (input){
  //			case /*right*/:
  //				x++;
  //				t_place(field, l, t, x, y);
  //				f_print(field, h, l);
  //				break;
  //			case /*left*/:
  //				x--;
  //				t_place(field, l, t, x, y);
  //				f_print(field, h, l);
  //				break;
  //			case /*up*/:									//
  //actually this one shouldn't be working, as in tetris u cant move ur piece up
  // y--; 				t_place(field, l, t, x, y); 				f_print(field, h, l); 				break; 			case /*down*/:
  //				y++;
  //				t_place(field, l, t, x, y);
  //				f_print(field, h, l);
  //				break;
  //		//	case /*enter*/:
  //		//		t_place(field, l, t, x, y);
  //				break;
  //			case r:
  //				t_rotate(t);
  //				break;
  //			default:
  //				scanf("%c", &input);
  //				break;
  //		}
  //	} while (input != Enter*/);
}

// void movimento(int *field, int h, int l, tetramino_t t, int x, int y) {
//  clear();
//  t_place(temp_1, l, t, x, y);
//  f_print(temp_1, h, l);
//  printf("\n");
//  memcpy(temp_2, temp_1, sizeof(int) * area);
//  memcpy(temp_1, field, sizeof(int) * area);
//  sleep(1);
//  clear();
//}

int t_collision(int *field, tetramino_t t, int x, int y, char dir) {
  int i, j, found;
    switch (dir){
      case 'd': //down movement
        for (i = 0; i < edge(t); i++) {
          found = 0;
          for (j = edge(t) - 1; j >= 0; j--) {
            found = t.data[i + j * edge(t)] != 0;
            if (found && field[x + i + (y + j) * l] != 0)
              return 1;
          }
        }
        return 0;
      case 'r': //right movement
        for (i = 0; i < edge(t); i++) {
        found = 0;
        for (j = edge(t) - 1; j >= 0; j--){
          found = t.data[j + i * edge(t)] != 0;
          if (found && field[x + j + (y + i) * l] != 0)
            return 1;
        }
      }
        return 0;
      case 'l': //left movement
        for (i = 0; i < edge(t); i++) {
        found = 0;
        for (j = 0; j < edge(t); j++){
          found = t.data[j + i * edge(t)] != 0;
          if (found && field[x + j + (y + i) * l] != 0)
            return 1;
        }
      }
        return 0;
    }
}

void t_gravity(int *field, tetramino_t t, int x, int y) {
  int i;
  char dir = 'd';
  int *temp_1 = f_create(h, l);
  int *temp_2 = f_create(h, l);
  memcpy(temp_1, field, sizeof(int) * area);
  memcpy(temp_2, field, sizeof(int) * area);
  hide_cursor();
  while (1) {
    if (t_collision(field, t, x, y, dir))
      break;
    clear();
    t_place(temp_1, t, x, y++);
    f_print(temp_1);
    printf("\n");
    memcpy(temp_2, temp_1, sizeof(int) * area);
    memcpy(temp_1, field, sizeof(int) * area);
    sleep(1);
    clear();
  }
  memcpy(field, temp_2, sizeof(int) * area);
  f_print(field);
  show_cursor();
  free(temp_1);
  free(temp_2);
}

int main() {
  int *field;
  tetramino_t T;
  int x = 5, y = 0; //	starting position for our tetraminos

  system("clear");

  field = f_create(h, l);
  f_clear(field);
  f_print(field);

  T = t_select();
  t_gravity(field, T, x, y);
  return 0;
}