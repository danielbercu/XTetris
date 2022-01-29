#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define array_size(t) (int)sizeof(t)/sizeof(int)	// Only works where arrays are declared as such, rather than pointers (e.g. in the main function)
#define pipo(t) (int)sqrt(array_size(t))			// As stated above 
#define edge(t) (int)sqrt(t.size)					// General way to get the size of the edge of a square matrix, with the further declaration of tetramino_t

typedef struct tetramino{
	int* data;
	int size;
} tetramino_t;

int T1[9] = {0, 0, 0, 
			 0, 1, 0, 
			 1, 1, 1}; // T
int T2[16] = {0, 0, 0, 0, 
			  0, 0, 0, 0,
			  0, 0, 0, 0,
			  2, 2, 2, 2}; // I
int T3[9] = {0, 0, 0,
 			 0, 3, 3,
 			 3, 3, 0}; // N1
int T4[9] = {0, 0, 0,
			 4, 4, 0,
			 0, 4, 4}; // N2
int T5[9] = {0, 0, 0,
			 5, 5, 5,
			 5, 0, 0}; // L
int T6[9] = {0, 0, 0,
			 6, 6, 6,
			 0, 0, 6}; // J
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

void t_place_aux_f(int* field, int l, tetramino_t t, int x, int y){	// statically
	int i, j, m = edge(t);
	for (i = 0; i < m; i++)
		for (j = 0; j < m; j++)
			field[x + j + i * l + y * l] = t.data[j + i * m];
}

//void t_place_main_f(int* field, int l, tetramino_t t, int x);
//	int i, j, m = edge(t);
//	for (i = 0; i < m; i++)
//		for (j = 0; j < m; j++)
//			field[x + j + i * l] = t.data[j + i * m];

//void move_in_fields(int* field, int* aux_field, int l, tetramino_t t, int x, int y){
//	char input;
//	x = y = 0;
//	scanf("%c", &input);
//	do{
//		switch (input){
//			case /*right*/:
//				x++;
//				t_place_aux_f(aux_field, l, t, x, y);
//				f_print(aux_field, h, l);
//				break;
//			case /*left*/:
//				x--;
//				t_place_aux_f(aux_field, l, t, x, y);
//				f_print(aux_field, h, l);
//				break;
//			case /*up*/:
//				y++;
//				t_place_aux_f(aux_field, l, t, x, y);
//				f_print(aux_field, h, l);
//				break;
//			case /*down*/:
//				y--;
//				t_place_aux_f(aux_field, l, t, x, y);
//				f_print(aux_field, h, l);
//				break;
//			case /*enter*/:
//				t_place_main_f(field, l, t, x);
//				break;
//			default:
//				scanf("%c", &input);
//				break;
//		}
//	} while (input != /*right, left, up, down or Enter key is pressed*/);
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
	return T;
}

void f_print(int* field, int h, int l){
	int i, j;
	printf("╔");
	for (i = 0; i < 2 * l + 1; i++) printf("═");
	printf("╗\n");
	for (i = 0; i < h; i++){
		printf("║ ");
		for(j = 0; j < l; j++)
			printf("%d ", field[j + i * l]);
		printf("║\n");
	}
	printf("╚");
	for (i = 0; i < 2 * l + 1; i++) printf("═");
	printf("╝\n ");
	for (i = 1; i < l + 1; i++) printf(" %d", i);
	printf("\n");
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

int main(){
	int *field, *aux_field;
	tetramino_t T;
	int h = 15, l = 10, h_aux = 4;
	aux_field = f_create(4, l);
	field = f_create(h, l);
	f_clear(aux_field, h_aux, l);
	f_clear(field, h, l);
	f_print(aux_field, h_aux, l);
	f_print(field, h, l);

	T = t_select();
	t_print(T);
	t_rotate(T);
	t_print(T);
	t_place_aux_f(aux_field, l, T, 0, 0);
	f_print(aux_field, h_aux, l);

	return 0;
}