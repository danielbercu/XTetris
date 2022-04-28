#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

#include "game.h"
#include "structures.h"
#include "utilities.h"

int main() {

  settings_t s;

  pieces_t p;

  struct termios orig;

  init_pieces(&p);

  /**
   *  The actual number of rows of our field is h - 4 - 1 (default 20, so 15);
   *  As for the colums, we have l - 2 colums (default 12, so 10);
   *  Default distance between fields is 70;
   *  The function to print our field(s) has been dinamically
   *  coded in relationship with the data given here, so that
   *  we may vary our field's dimensions without worrying too much about
   *  spaces and ratios (as long as we can play in those dimensions).
   */

  s.h = 20;
  s.l = 12;
  s.distance = 70;
  s.gamemode = 10;
  s.replay = 10;
  s.random = 1;
  s.p = p;
  s.last_drop = current();
  s.original = orig;

  system("clear");

  printf("\n\n               _  ________     __       _     \n              | "
         "|/ /_  __/__  / /______(_)____\n"
         "              |   / / / / _ \\/ __/ ___/ / ___/\n             /   | "
         "/ / /  __/ /_/ /  / (__  ) \n"
         "            /_/|_|/_/  \\___/\\__/_/  /_/____/  \n                   "
         "                           \n");

  sleep(1);

  /*Main menu*/

  printf("\033[9;12H[0] Giocatore singolo\033[10;12H[1] Multigiocatore"
         "\033[11;12H[2] Player vs CPU\033[12;12H[3] Tutorial"
         "\033[13;12H[4] Impostazioni\033[14;12H[9] Esci"
         "\033[16;12HSeleziona la modalità: ");

  while (s.gamemode != 0 && s.gamemode != 1 && s.gamemode != 2) {
    scanf("%d", &s.gamemode);
    switch (s.gamemode) {
    case 0:
      singleplayer(&s);
      break;
    case 1:
      multiplayer(&s);
      break;
    case 2:
      cpu(&s);
      break;
    case 3:
      printf("\033[18;12H                                                                                                                "
                            "\033[18;12HUsa [W] per ruotare il tetramino, [A] per spostarlo a sinistra, [S] per spostarlo verso il basso, ");
      printf("\033[19;12H                                                                                                                "
                            "\033[19;12H[D] per spostarlo a destra e [Barra Spaziatrice] per confermare il la posizione del pezzo.");
      printf("\033[20;12H                                                                                                                "
                            "\033[20;12HNella modalità di inserimento pezzi manuale, dovrai selezionare un tetramino con i numeri 1 - 7.");
      printf("\033[21;12H                                                                                                                "
                            "\033[21;12HL'eliminazione di una riga vale 1 punto, di due righe 3 punti, di tre righe 6 punti e di quattro righe 12 punti.");
      printf("\033[22;12H                                                                                                                "
                            "\033[22;12HOgni 8 punti la velocità di discesa dei tetramini aumenta progressivamente fino a 40 punti!");
      printf("\033[16;12H                                  \033[16;12HSeleziona la modalità: ");
      break;
    case 4:
      printf("\033[18;12H                                                                                                                "
                            "\033[18;12HSeleziona la modalità di inserimento dei tetramini. Default: [1]");
      printf("\033[19;12H                                                                                                                "
                            "\033[19;12H[0] Manuale");
      printf("\033[20;12H                                                                                                                "
                            "\033[20;12H[1] Random");
      printf("\033[21;12H                                                                                                                ");
      printf("\033[22;12H                                                                                                                "
                            "\033[22;12HSeleziona la modalità di inserimento: ");
      scanf("%d", &s.random);
      while(s.random != 0 && s.random != 1){
        printf("\033[22;12H                                          \033[22;12HInserisci un numero valido: ");
        scanf("%d", &s.random);
      }
      printf("\033[18;12H                                                                                                                ");
      printf("\033[19;12H                                                                                                                ");
      printf("\033[20;12H                                                                                                                ");
      printf("\033[21;12H                                                                                                                ");
      printf("\033[22;12H                                          ");
      printf("\033[16;12H                                         \033[16;12HSeleziona la modalità: ");
      break;
    case 9:
      printf("\033[27;1H");
      return 0;
    default:
      printf("\033[18;12H                                                                                                                ");
      printf("\033[19;12H                                                                                                                ");
      printf("\033[20;12H                                                                                                                ");
      printf("\033[21;12H                                                                                                                ");
      printf("\033[22;12H                                                                                                                ");
      printf("\033[16;12H                                         \033[16;12HInserisci un numero valido: ");
      break;
    }
  }

  sleep(2);
  printf("\033[%d;5HVuoi rigiocare?\n    [1] Sì\n    [0] No\n\033[%d;22H",
         2 * s.h - 1, 2 * s.h - 1);
  while (s.replay != 0 && s.replay != 1) {
    scanf("%d", &s.replay);
    switch (s.replay) {
    case 0:
      printf("\033[%d;1H", 2 * s.h + 3);
      return 0;
    case 1:
      main();
      break;
    default:
      printf("\033[43;5HInserisci un numero valido: ");
      break;
    }
  }
  return 0;
}
