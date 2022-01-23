#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

#define MAX_DIM_MUTEX 1024
#define MAX_DIM_MATRIX 256

pthread_mutex_t mutex_A[MAX_DIM_MUTEX];
pthread_mutex_t mutex_B[MAX_DIM_MUTEX];

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

void read_matrix(int dim, const char name[], int matrix[][dim]) {
	FILE *fptr;
	
	fptr = fopen(name, "r");
	for(int i = 0; i < dim; i++) 
		for(int j = 0; j < dim; j++) 
			fscanf(fptr, "%d", &matrix[i][j]);
	fclose(fptr);
}

void* routine(void* args) {
	struct Indexes* indexes;
	indexes = (struct Indexes*)args;
	int i = indexes->i;
	int j = indexes->j;
	int dim = indexes->dim;
	
	for(int k = 0; k < dim; k++) {
		printf("i'm p(%d,%d) calculating a(%d,%d)*b(%d,%d)\n", i, j, i, k, k, j);
	}
	
	return (void**)args;
}

int main(int argc, void* argv[]) {
	srand(time(NULL));
	double time_spent = 0.0;
	
	int dim = get_dim();
	
	const char nameA[] = "matrixA.txt";
	const char nameB[] = "matrixB.txt";
	
	read_matrix(dim, nameA, matrixA);
	read_matrix(dim, nameB, matrixB);
	
	clock_t begin = clock();
	
	pthread_t threads[dim][dim];
	int n_mutex = dim*dim*2;
	
	for(int i = 0; i < n_mutex; i++) {
		pthread_mutex_init(&mutex_A[i], NULL);
		pthread_mutex_init(&mutex_B[i], NULL);
	}
	
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
	
	struct Indexes* res;
	for(int i = 0; i < dim; i++) {
		for(int j = 0; j < dim; j++) {
			pthread_join(threads[i][j], (void*)&res);
			free(res);
		}
	}
	
	
	clock_t end = clock();
	time_spent += (double)(end - begin) / CLOCKS_PER_SEC;
	time_spent = time_spent * 1000;
	printf("time spent = %f ms\n", time_spent);
}
