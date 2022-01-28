#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define array_size(t) (int)sizeof(t)/sizeof(int) // Only works where arrays are initialized as such, rather than pointers (e.g. in the main function)
#define pipo(t) (int)sqrt(t_size(t))			 // As stated above 
#define edge(t) (int)sqrt(t.size)				 // General way to get the size of the edge of a square matrix, with the further declaration of tetramino_t

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
}

void place_in_field(int* field, int l, tetramino_t t, int x, int y){
	int i, j, m = edge(t);
	for (i = 0; i < m; i++)
		for (j = 0; j < m; j++)
			field[x + j + i * l + y * l] = t.data[j + i * m];
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
	int *field, *temp_field;
	int h = 15, l = 10;
	temp_field = f_create(4, l);
	field = f_create(h, l);
	f_clear(temp_field, 4, l);
	f_clear(field, h, l);

	tetramino_t T = t_create(T1, array_size(T1));
	tetramino_t I = t_create(T2, array_size(T2));
	tetramino_t minchia = t_create(T3, array_size(T3));

	t_rotate(I);
	t_rotate(T);

	place_in_field(field, l, I, 0, 0);
	place_in_field(field, l, T, 0, 0);
	f_print(temp_field, 4, l);
	f_print(field, h, l);

	return 0;
}