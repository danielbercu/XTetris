#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <string.h>

#define array_size(t) (int)sizeof(t)/sizeof(int)	// Only works where arrays are declared as such, rather than pointers
#define pipo(t) (int)sqrt(array_size(t))			// As stated above 
#define edge(t) (int)sqrt(t.size)					// General way to get the size of the edge of a square matrix, with the further declaration of tetramino_t
#define area h * l
#define clear() system("clear")

typedef struct tetramino{
	int* data;
	int size;
} tetramino_t;

int T1[9] = {0, 1, 0, 
			 1, 1, 1, 
			 0, 0, 0}; // T
int T2[16] = {0, 0, 2, 0, 
			  0, 0, 2, 0,
			  0, 0, 2, 0,
			  0, 0, 2, 0}; // I
int T3[9] = {0, 3, 3,
 			 3, 3, 0,
 			 0, 0, 0}; // N1
int T4[9] = {4, 4, 0,
			 0, 4, 4,
			 0, 0, 0}; // N2
int T5[9] = {5, 5, 5,
			 5, 0, 0,
			 0, 0, 0}; // L
int T6[9] = {6, 6, 6,
			 0, 0, 6,
			 0, 0, 0}; // J
int T7[4] = {7, 7, 
			 7, 7}; // O

tetramino_t t_create(int* src, int size){
	tetramino_t t;
	int i;
	t.size = size;
	t.data = malloc(sizeof(int)*size);
	for (i = 0; i < t.size; i++)
		t.data[i] = src[i];
	return t;
}

void t_rotate(tetramino_t t){
	int i, j, l = edge(t);
	tetramino_t temp = t_create(t.data, t.size);
	for (i = 0; i < l; i++)
		for (j = 0; j < l; j++)
			t.data[j + i * l] = temp.data[j * l + l - i - 1];
	free(temp.data);
}

void t_print(tetramino_t t){
	int i, j, l = edge(t);
	for (i = 0; i < l; i++){
		for (j = 0; j < l; j++)
			printf("%d ", t.data[j + i * l]);
		printf("\n");
	}
	printf("\n");
}

void t_place(int* field, int l, tetramino_t t, int x, int y){	// statically
	int i, j, m = edge(t);
	for (i = 0; i < m; i++)
		for (j = 0; j < m; j++){
			if (field[x + j + (y + i) * l] != 0 && t.data[j + i * m] != 0)
				break;
			if (field[x + j + (y + i) * l] != 0 && t.data[j + i * m] == 0)
				continue;
			if (field[x + j + (y + i) * l] == 0)
				field[x + j + (y + i) * l] = t.data[j + i * m];
		}
}

//void move_in_fields(int* field, int l, tetramino_t t, int x, int y){
//	char input;
//	x = y = 0;
//	scanf("%c", &input);
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
//			case /*up*/:									//	actually this one shouldn't be working, as in tetris u cant move ur piece up
//				y--;
//				t_place(field, l, t, x, y);
//				f_print(field, h, l);
//				break;
//			case /*down*/:
//				y++;
//				t_place(field, l, t, x, y);
//				f_print(field, h, l);
//				break;
//			case /*enter*/:
//				t_place(field, l, t, x, y);
//				break;
//			case r:
//				t_rotate(t);
//				break;
//			default:
//				scanf("%c", &input);
//				break;
//		}
//	} while (input != /*right, left, up, down or Enter*/);
//}


tetramino_t t_select(void){
	int t;
	tetramino_t T;
	printf("Seleziona un tetramino: ");
	scanf("%d", &t);
	do{
		switch (t){
			case 1:
				T = t_create(T1, array_size(T1));
				break;
			case 2:
				T = t_create(T2, array_size(T2));
				break;
			case 3:
				T = t_create(T3, array_size(T3));
				break;
			case 4:
				T = t_create(T4, array_size(T4));
				break;
			case 5:
				T = t_create(T5, array_size(T5));
				break;
			case 6:
				T = t_create(T6, array_size(T6));
				break;
			case 7:
				T = t_create(T7, array_size(T7));
				break;
			default:
				printf("Inserisci un numero valido: ");
				scanf("%d", &t);
				break;
		}
	} while (t < 1 || t > 7);
	printf("\n");
	return T;
}

void f_print(int* field, int h, int l){
	int i, j, k = 15;
	for (i = 0; i < 4; i++){
		printf("     ");
		for (j = 0; j < l; j++)
			printf("%d ", field[j + i * l]);
		printf("\n");
	}
	for (i = 4; i < h; i++, k--){
		if (k < 10) printf(" ");
		printf("%d ║ ", k);
		for(j = 0; j < l; j++)
			printf("%d ", field[j + i * l]);
		printf("║\n");
	}
	printf("   ╚");
	for (i = 0; i < 2 * l + 1; i++) printf("═");
	printf("╝\n    ");
	for (i = 1; i < l + 1; i++) printf(" %d", i);
	printf("\n\n");
}

void f_clear(int* field, int h, int l){
	int i, j;
	for (i = 0; i < h; i++)
		for (j = 0; j < l; j++)
			field[j + i * l] = 0;
}

int* f_create(int h, int l){
	int* field = malloc(sizeof(int) * h * l);
	return field;
}

void t_move(int* field, int h, int l, tetramino_t t, int x, int y){
	int i;
	//create two auxiliary copies of the main field
	int* temp_1 = f_create(h, l);
	int* temp_2 = f_create(h, l);
	memcpy(temp_1, field, sizeof(int) * area);
	memcpy(temp_2, field, sizeof(int) * area);

	while (y < 6){
		clear();
		t_place(temp_1, l, t, x, y++);
		f_print(temp_1, h, l);
		sleep(1);
		printf("\n");
		memcpy(temp_2, temp_1, sizeof(int) * area);
		memcpy(temp_1, field, sizeof(int) * area);
		clear();
	}
	memcpy(field, temp_2, sizeof(int) * area);
	f_print(field, h, l);
	free(temp_1);
	free(temp_2);
	return;
}

int main(){
	int *field;
	tetramino_t T;
	int h = 19, l = 10;	//	4 rows for initial placement, 15 rows for the main field
	int x = 3, y = 0;	//	starting position for our tetraminos

	field = f_create(h, l);
	f_clear(field, h, l);
	f_print(field, h, l);

	T = t_select();
//	t_rotate(T);
//	t_place(field, l, T, 3, 0);
//	f_print(field, h, l);
//	t_place(field, l, T, 3, 10);

	t_move(field, h, l, T, x, y);

	T = t_select();
	t_move(field, h, l, T, x, y);

	return 0;
}