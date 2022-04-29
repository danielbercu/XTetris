#define _DEFAULT_SOURCE

#include <sys/time.h>
#include <termios.h>
#include <unistd.h>

#include "structures.h"
#include "utilities.h"

void init_pieces(pieces_t *p){
  p->T1[0] = 0; p->T1[1] = 1; p->T1[2] = 0;                     /* T */
  p->T1[3] = 1; p->T1[4] = 1; p->T1[5] = 1;
  p->T1[6] = 0; p->T1[7] = 0; p->T1[8] = 0;

  p->T2[0] = 0;  p->T2[1] = 0;  p->T2[2] = 2;  p->T2[3] = 0;    /* I */
  p->T2[4] = 0;  p->T2[5] = 0;  p->T2[6] = 2;  p->T2[7] = 0;
  p->T2[8] = 0;  p->T2[9] = 0;  p->T2[10] = 2; p->T2[11] = 0;
  p->T2[12] = 0; p->T2[13] = 0; p->T2[14] = 2; p->T2[15] = 0;

  p->T3[0] = 0; p->T3[1] = 3; p->T3[2] = 3;                     /* S */
  p->T3[3] = 3; p->T3[4] = 3; p->T3[5] = 0;
  p->T3[6] = 0; p->T3[7] = 0; p->T3[8] = 0;

  p->T4[0] = 4; p->T4[1] = 4; p->T4[2] = 0;                     /* Z */
  p->T4[3] = 0; p->T4[4] = 4; p->T4[5] = 4;
  p->T4[6] = 0; p->T4[7] = 0; p->T4[8] = 0;

  p->T5[0] = 5; p->T5[1] = 5; p->T5[2] = 5;                     /* L */
  p->T5[3] = 5; p->T5[4] = 0; p->T5[5] = 0;
  p->T5[6] = 0; p->T5[7] = 0; p->T5[8] = 0;

  p->T6[0] = 6; p->T6[1] = 6; p->T6[2] = 6;                     /* J */
  p->T6[3] = 0; p->T6[4] = 0; p->T6[5] = 6;
  p->T6[6] = 0; p->T6[7] = 0; p->T6[8] = 0;

  p->T7[0] = 7; p->T7[1] = 7;                                   /* O */
  p->T7[2] = 7; p->T7[3] = 7;

  p->Ts[0].data = p->T1; p->Ts[0].size = 9;
  p->Ts[1].data = p->T2; p->Ts[1].size = 16;
  p->Ts[2].data = p->T3; p->Ts[2].size = 9;
  p->Ts[3].data = p->T4; p->Ts[3].size = 9;
  p->Ts[4].data = p->T5; p->Ts[4].size = 9;
  p->Ts[5].data = p->T6; p->Ts[5].size = 9;
  p->Ts[6].data = p->T7; p->Ts[6].size = 4;
}

long current() {
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return 1000000 * tv.tv_sec + tv.tv_usec;
}

long points_to_ms(int points) {
  long res;
  if (points >= 0 && points < 8)
    res = 600000;
  if (points >= 8 && points < 16)
    res = 500000;
  if (points >= 16 && points < 24)
    res = 400000;
  if (points >= 24 && points < 32)
    res = 300000;
  if (points >= 32 && points < 40)
    res = 200000;
  if (points >= 40)
    res = 100000;
  return res;
}

int msleep(unsigned int tms) { return usleep(tms * 1000); }

void raw_disable(settings_t *s) { tcsetattr(0, TCSANOW, &s->original); }

void raw_enable(settings_t *s) {
  struct termios raw;

  tcgetattr(0, &s->original);
  raw = s->original;
  raw.c_lflag &= ~ICANON;
  raw.c_lflag &= ~ECHO;
  raw.c_lflag &= ~ISIG;
  raw.c_cc[VMIN] = 0;
  raw.c_cc[VTIME] = 0;
  tcsetattr(0, TCSANOW, &raw);
}

char select_char() {
  char buff;
  return read(STDIN_FILENO, &buff, 1) > 0 ? buff : 0;
}
