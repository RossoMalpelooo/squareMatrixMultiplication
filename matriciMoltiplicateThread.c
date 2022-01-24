#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <string.h>

#define MAX_DIM_MATRIX 256

int matrixA[MAX_DIM_MATRIX][MAX_DIM_MATRIX];
int matrixB[MAX_DIM_MATRIX][MAX_DIM_MATRIX];
int matrixC[MAX_DIM_MATRIX][MAX_DIM_MATRIX];

struct Indexes {
	int i, j, dim;
};

int get_dim() {
	int n;
	printf("enter size of matrices: ");
	scanf("%d", &n);
	return n;
}

void read_matrix(const char name[], int dim) {
	FILE *fptr;
	
	fptr = fopen(name, "r");
	for(int i = 0; i < dim; i++) {
		for(int j = 0; j < dim; j++) {
			if(strcmp(name, "matrixA.txt") == 0)
				fscanf(fptr, "%d", &matrixA[i][j]);	
			if(strcmp(name, "matrixB.txt") == 0) 
				fscanf(fptr, "%d", &matrixB[i][j]);
		}
	}
	
	fclose(fptr);
}

void* routine(void* args) {
	struct Indexes* indexes;
	indexes = (struct Indexes*)args;
	int i = indexes->i;
	int j = indexes->j;
	int dim = indexes->dim;
	
	for(int k = 0; k < dim; k++) 
		matrixC[i][j] += matrixA[i][k]*matrixB[k][j];
	
	return (void**)args;
}

int main(int argc, void* argv[]) {
	srand(time(NULL));
	double time_spent = 0.0;
	
	int dim = get_dim();
	
	const char nameA[] = "matrixA.txt";
	const char nameB[] = "matrixB.txt";
	
	read_matrix(nameA, dim);
	read_matrix(nameB, dim);
	
	clock_t begin = clock();
	
	pthread_t threads[dim][dim];
	
	struct Indexes* indexes;
	for(int i = 0; i < dim; i++) {
		for(int j = 0; j < dim; j++) {
			indexes = malloc(sizeof(struct Indexes));
			indexes->i = i;
			indexes->j = j;
			indexes->dim = dim;
			pthread_create(&threads[i][j], NULL, &routine, (void*)indexes);
		}
	}
	
	printf("Matrix C = \n");
	struct Indexes* res;
	for(int i = 0; i < dim; i++) {
		for(int j = 0; j < dim; j++) {
			pthread_join(threads[i][j], (void*)&res);
			free(res);
			printf("%d ", matrixC[i][j]);
		}
		printf("\n");
	}
	
	
	clock_t end = clock();
	time_spent += (double)(end - begin) / CLOCKS_PER_SEC;
	time_spent = time_spent * 1000;
	printf("time spent = %f ms\n", time_spent);
}
