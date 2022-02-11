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
#define cursor_set_right_side() printf("\033[1;20H")
#define cursor_hide() printf("\033[?25l")
#define cursor_show() printf("\033[?25h")
#define block_c "■" // ■ or ⬛

const int h = 20, l = 12, distance = 70,
          area = h * l; //  the real number of rows of our field is h - 4 - 1 (default 20, so 15);
                        //  as for the colums, we have l - 2 colums (default 12, so 10);
                        //  default distance between fields is 70;
                        //  the function to print our field(s) has been dinamically
                        //  coded in relationship with the data given here, so that
                        //  we may vary our field's dimensions without worrying about
                        //  spaces and ratios (as long as we can play in those dimensions);

typedef struct tetramino {
  int *data;
  int size;
} tetramino_t;

int T1[9] = {0, 1, 0, 1, 1, 1, 0, 0, 0};                       // T
int T2[16] = {0, 0, 2, 0, 0, 0, 2, 0, 0, 0, 2, 0, 0, 0, 2, 0}; // I
int T3[9] = {0, 3, 3, 3, 3, 0, 0, 0, 0};                       // S
int T4[9] = {4, 4, 0, 0, 4, 4, 0, 0, 0};                       // Z
int T5[9] = {5, 5, 5, 5, 0, 0, 0, 0, 0};                       // L
int T6[9] = {6, 6, 6, 0, 0, 6, 0, 0, 0};                       // J
int T7[4] = {7, 7, 7, 7};                                      // O

tetramino_t Ts[7] = {{T1, 9}, {T2, 16}, {T3, 9}, {T4, 9},
                     {T5, 9}, {T6, 9},  {T7, 4}};

char shapes[] = "TISZLJO";

struct termios original;

int msleep(unsigned int tms) { return usleep(tms * 1000); }

void raw_disable() { tcsetattr(STDIN_FILENO, TCSAFLUSH, &original); }

void raw_enable() {
  tcgetattr(STDIN_FILENO, &original);
  atexit(raw_disable);

  struct termios raw = original;
  raw.c_lflag &= ~(ECHO | ICANON);

  tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

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
      t.data[j + i * e] = temp.data[j * e + e - i - 1];
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

void f_print(int *field, int mp) {
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
    if (mp)
      printf("\033[%d;%dH", i + 1, distance);
    if (k < 10)
      printf(" ");
    printf("  %d ║ ", k);
    for (j = 1; j < l - 1; j++)
      printf("\033[0;%dm%s \033[0m", 30 + field[j + i * l], block_c);
    printf("║");
    if (!mp)
      printf("\n");
  }
  if (mp)
    printf("\033[%d;%dH", h, distance);
  printf("     ╚");
  for (i = 0; i < 2 * l - 3; i++) {
    if (mp)
      printf("\033[%d;%dH", h, distance + 6 + i);
    printf("═");
  }
  if (mp)
    printf("\033[%d;%dH", h, distance + 2 * l + 3);
  printf("╝\n      ");
  if (mp)
    printf("\033[%d;%dH", h + 1, distance + 6);
  for (i = 1; i < l - 1; i++)
    printf(" %d", i);
  printf("\n\n");
  if (!mp)
    printf("\033[%d;%dHGiocatore 1", h + 3, l - 1);
  else if (mp == 1)
    printf("\033[%d;%dHGiocatore 2", h + 3, distance + l - 1);
  else
    printf("\033[%d;%dHCPU", h + 3, distance + l - 1);
}

tetramino_t t_select(int *pieces) {
  int t;
  printf("\033[%d;4H                       \r    Seleziona un tetramino: ", h + 18);
  scanf("%d", &t);
  printf("\033[%d;4H                               \r", h + 18);
  if (t > 0 && t < 8 && pieces[t - 1]) {
    pieces[t - 1]--;
    return t_create(Ts[t - 1].data, Ts[t - 1].size);
  }
  while (t < 1 || t > 7 || !pieces[t - 1]) {
    printf(
        "\033[%d;4H                       \r    Inserisci un numero valido: ", h + 18);
    scanf("%d", &t);
    printf("\033[%d;4H                                        \r", h + 18);
    if (t > 0 && t < 8 && pieces[t - 1]) {
      pieces[t - 1]--;
      return t_create(Ts[t - 1].data, Ts[t - 1].size);
    }
    clear();
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

void t_gravity(int *field, tetramino_t t, int x, int y, int lvl, int mp) {
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
    f_print(temp_1, mp);
    printf("\n");
    memcpy(temp_2, temp_1, sizeof(int) * area);
    memcpy(temp_1, field, sizeof(int) * area);
    msleep(tms);
    clear();
  }
  memcpy(field, temp_2, sizeof(int) * area);
  f_print(field, mp);
  cursor_show();
  free(temp_1);
  free(temp_2);
}

void t_move(int *field, tetramino_t t, int x, int y, int mp) {
  int i;
  char dir;
  char k;
  int *temp_1 = f_create(h, l);
  int *temp_2 = f_create(h, l);
  memcpy(temp_1, field, sizeof(int) * area);
  memcpy(temp_2, field, sizeof(int) * area);
  cursor_hide();
  raw_enable();
  do {
    scanf("%c", &k);
    switch (k) {
    case 'w':
      t_rotate(t);
      if (t_collision(temp_1, t, x, y, dir))
        for (i = 0; i < 3; i++)
          t_rotate(t);
      break;
    case 's':
      dir = 'd';
      if (!t_collision(temp_1, t, x, y + 1, dir)) { // or y + 2
        y++;
        break;
      } else {
        clear();
        t_place(field, t, x, y); // or y + 1
        f_print(field, mp);
        raw_disable();
        cursor_show();
        free(temp_1);
        free(temp_2);
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
    f_print(temp_1, mp);
    printf("\n");
    memcpy(temp_2, temp_1, sizeof(int) * area);
    memcpy(temp_1, field, sizeof(int) * area);
    if (k == 'e')
      t_gravity(field, t, x, y, 99, mp);
    clear();
  } while (k != 'e');
  raw_disable();
  clear();
  f_print(field, mp);
  cursor_show();
  free(temp_1);
  free(temp_2);
}

int is_game_over(int *field, int *pieces, int mp) {
  int i;
  for (i = 3 * l + 1; i < 4 * l - 1; i++)
    if (field[i])
      return 1;
  for (i = 0; i < 7; i++) {
    if (pieces[i])
      break;
    if ((i == 6) && !pieces[i]) {
      if (mp)
        return 2;
      return 1;
    }
  }
  return 0;
}

void count_points(int *field, int *points) {
  int i, j, count = 0;
  for (i = 4; i < h - 1; i++) {
    for (j = 1; j < l - 1; j++) {
      if (!field[j + i * l])
        break;
      if ((j == l - 2) && field[j + i * l])
        count++;
    }
  }
  switch (count) {
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

// void reverse_lines(int* field){     //mp
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

void print_remaining_pieces(int *pieces) {
  int i, j;

  int tetraminos[] = {0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 2, 0, 0, 3, 3,
                      0, 4, 4, 0, 0, 5, 5, 5, 0, 6, 6, 6, 0, 7, 7, 0, 0, 1,
                      1, 1, 0, 2, 0, 3, 3, 0, 0, 0, 4, 4, 0, 5, 0, 0, 0, 0,
                      0, 6, 0, 7, 7, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0,
                      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

  printf("\033[%d;5HNumero del tetramino:", h + 9);
  for (i = 0; i < 7; i++)
    printf("   \033[0;%dm%d   \033[0m", 31 + i, i + 1);
  printf("\n\n");

  for (i = 0; i < 4; i++) {
    printf("                        ");
    for (j = 0; j < 26; j++)
      printf("\033[0;%dm%s \033[0m", 30 + tetraminos[j + i * 26], block_c);
    printf("\n");
  }
  printf("\033[%d;5HPezzi rimanenti:    ", h + 16);
  for (i = 0; i < 7; i++) {
    if (i == 2)
      printf("\b");
    if (i == 3)
      printf(" ");
    if (i == 5)
      printf("  ");
    printf("   \033[0;%dm%d  \033[0m", 31 + i, pieces[i]);
  }
}

void check_lines(int *field) {
  int i, j, k, q;
  for (i = h - 2; i > 2; i--) {
    for (j = 1; j < l - 1; j++) {
      if (!field[j + i * l])
        break;
      if ((j == l - 2) && field[j + i * l]) {
        for (k = 1; k < l - 1; k++)
          for (q = i; q > 2; q--)
            field[k + q * l] = field[k + (q - 1) * l];
        check_lines(field);
      }
    }
  }
}

void singleplayer() {
  int *field;
  tetramino_t T;
  int x = 4, y = 0; //  starting position for our tetraminos
  int i, j;
  int points = 0;
  int game_over = 0;
  int pieces[7] = {20, 20, 20, 20, 20, 20, 20};
  int mp = 0;

  system("clear");

  field = f_create(h, l);
  f_clear(field);
  f_print(field, mp);
  printf("\033[%d;%dHPunti: %d", h + 4, l + 1, points);

  while (!game_over) {
    clear();
    cursor_hide();
    print_remaining_pieces(pieces);
    cursor_show();
    T = t_select(pieces);
    cursor_hide();
    print_remaining_pieces(pieces);
    t_move(field, T, x, y, mp);
    free(T.data);
    count_points(field, &points);
    printf("\033[%d;%dHPunti: %d", h + 4, l + 1, points);
    check_lines(field);
    clear();
    f_print(field, mp);
    game_over = is_game_over(field, pieces, mp);
  }

  printf("\033[%d;%dH\033[0;31mGame Over!\033[0m\n", h + 5, distance / 2);
  free(field);
}

void multiplayer() {
  int *field1, *field2;
  tetramino_t T;
  int x = 4, y = 0; //  starting position for our tetraminos
  int i, j;
  int points1 = 0, points2 = 0;
  int game_over = 0;
  int pieces[7] = {40, 40, 40, 40, 40, 40, 40};
  int mp = 1;

  system("clear");

  field1 = f_create(h, l);
  field2 = f_create(h, l);
  f_clear(field1);
  f_clear(field2);
  f_print(field1, 0);
  f_print(field2, 1);
  printf("\033[%d;%dHPunti: %d", h + 4, l + 1, points1);
  printf("\033[%d;%dHPunti: %d", h + 4, distance + l, points2);

  while (!game_over) {
    clear();
    cursor_hide();
    printf("\033[%d;%dHTocca al giocatore 1!", h + 5, distance / 2 + 5);
    print_remaining_pieces(pieces);
    cursor_show();
    T = t_select(pieces);
    cursor_hide();
    print_remaining_pieces(pieces);
    t_move(field1, T, x, y, 0);
    free(T.data);
    count_points(field1, &points1);
    printf("\033[%d;%dHPunti: %d", h + 4, l + 1, points1);
    check_lines(field1);
    clear();
    f_print(field1, 0);
    game_over = is_game_over(field1, pieces, mp);
    if (game_over) {
      printf("\033[%d;%dH\033[0;31mGame Over! Vince il giocatore 2!\033[0m\n", h + 5, distance / 2);
      break;
    }
    printf("\033[%d;%dHTocca al giocatore 2!\n\n\n", h + 5, distance / 2 + 5);
    cursor_show();
    T = t_select(pieces);
    print_remaining_pieces(pieces);
    cursor_hide();
    t_move(field2, T, x, y, 1);
    free(T.data);
    count_points(field2, &points2);
    printf("\033[%d;%dHPunti: %d", h + 4, distance + l, points2);
    check_lines(field2);
    clear();
    f_print(field2, 1);
    print_remaining_pieces(pieces);
    game_over = is_game_over(field2, pieces, mp);
    if (game_over == 1)
      printf("\033[%d;%dH\033[0;31mGame Over! Vince il giocatore 1!\033[0m\n", h + 5, distance / 2);
    else if (game_over == 2){
      int highest = points1 > points2 ? 1 : 2;
      printf("\033[%d;%dH\033[0;31mGame Over! Vince il giocatore %d!\033[0m\n", h + 5, distance / 2, highest);
    }
  }

  cursor_show();
  free(field1);
  free(field2);
}

void cpu() {
  system("clear");
  printf("\n\n\n\n\n\n\n\n                            Coming soon :)");
}

int main() {

  int gamemode, replay;

  system("clear");

  printf("\n\n     .----------------.  .----------------.  .----------------.  "
         ".----------------.  .----------------.  .----------------.  "
         ".----------------.\n");
  printf("    | .--------------. || .--------------. || .--------------. || "
         ".--------------. || .--------------. || .--------------. || "
         ".--------------. |\n");
  printf("    | |  ____  ____  | || |  _________   | || |  _________   | || |  "
         "_________   | || |  _______     | || |     _____    | || |    "
         "_______   | |\n");
  printf("    | | |_  _||_  _| | || | |  _   _  |  | || | |_   ___  |  | || | "
         "|  _   _  |  | || | |_   __ \\    | || |    |_   _|   | || |   /  "
         "___  |  | |\n");
  printf("    | |   \\ \\  / /   | || | |_/ | | \\_|  | || |   | |_  \\_|  | "
         "|| | |_/ | | \\_|  | || |   | |__) |   | || |      | |     | || |  | "
         " (__ \\_|  | |\n");
  printf("    | |    > `' <    | || |     | |      | || |   |  _|  _   | || |  "
         "   | |      | || |   |  __ /    | || |      | |     | || |   "
         "'.___`-.   | |\n");
  printf("    | |  _/ /'`\\ \\_  | || |    _| |_     | || |  _| |___/ |  | || "
         "|    _| |_     | || |  _| |  \\ \\_  | || |     _| |_    | || |  "
         "|`\\____) |  | |\n");
  printf("    | | |____||____| | || |   |_____|    | || | |_________|  | || |  "
         " |_____|    | || | |____| |___| | || |    |_____|   | || |  "
         "|_______.'  | |\n");
  printf("    | |              | || |              | || |              | || |  "
         "            | || |              | || |              | || |           "
         "   | |\n");
  printf("    | '--------------' || '--------------' || '--------------' || "
         "'--------------' || '--------------' || '--------------' || "
         "'--------------' |\n");
  printf("    '----------------'  '----------------'  '----------------'  "
         "'----------------'  '----------------'  '----------------'  "
         "'----------------' \n\n\n");

  sleep(1);

  printf("            [0] Giocatore singolo\n            [1] Multigiocatore\n  "
         "          [2] Player vs CPU\n            [3] Tutorial\n            "
         "[9] Esci\n\n");
  printf("            Seleziona la modalità: ");

  while (gamemode != 0 && gamemode != 1 && gamemode != 2) {
    scanf("%d", &gamemode);
    switch (gamemode) {
    case 0:
      singleplayer();
      break;
    case 1:
      multiplayer();
      break;
    case 2:
      cpu();
      break;
    case 3:
      printf(
          "\n            Seleziona un tetramino con i numeri 1 - 7. Usa [W] "
          "per ruotare il tetramino, [A] per spostarlo a sinistra,\n           "
          " [S] per spostarlo verso il basso, [D] per spostarlo verso destra e "
          "[E] per confermare il la posizione del pezzo.\033[22;36H");
      printf("\033[22;13H                              \r            Seleziona la modalità: ");
      break;
    case 9:
      return 0;
    default:
      printf("\033[22;13HInserisci un numero valido: ");
      break;
    }
  }

  sleep(2);
  printf("\033[%d;5HVuoi rigiocare?\n    [1] Sì\n    [0] No\n\033[%d;22H", 2 * h - 1, 2 * h - 1);
  while (replay != 0 && replay != 1) {
    scanf("%d", &replay);
    switch (replay) {
    case 0:
      return 0;
    case 1:
      main();
      break;
    default:
      printf("\033[43;5HInserisci un numero valido: ");
      break;
    }
  }
}