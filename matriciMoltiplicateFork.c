#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <math.h>
#include <time.h>

int main(int argc, char *argv[]) {
	srand(time(NULL));
	double time_spent = 0.0;
	
	const char name1[] = "matrixA.txt";
	const char name2[] = "matrixB.txt";
	
	FILE *fptr;
	int dim;
	
	printf("enter size of matrices: ");
	scanf("%d", &dim);
	
	clock_t begin = clock();
	
	int matrixA[dim][dim], matrixB[dim][dim], matrixC[dim][dim];
	
	fptr = fopen(name1, "r");
	for(int i = 0; i < dim; i++) 
		for(int j = 0; j < dim; j++) 
			fscanf(fptr, "%d", &matrixA[i][j]);
	fclose(fptr);
	
	fptr = fopen(name2, "r");
	for(int i = 0; i < dim; i++) 
		for(int j = 0; j < dim; j++) 
			fscanf(fptr, "%d", &matrixB[i][j]);
	fclose(fptr);
	
	/*
	for(int i = 0; i < dim; i++) {
		for(int j = 0; j < dim; j++) {
			printf("a(%d, %d) = %d\n", i, j, matrixA[i][j]);
			printf("b(%d, %d) = %d\n", i, j, matrixB[i][j]);
		}
	}
	*/
	
	int pids[dim][dim];
	int pipes_for_process = 4;
	int n_pipes = pipes_for_process * dim * dim;
	int pipes[n_pipes][2];
	
	for(int i = 0; i < n_pipes; i++) {
		pipe(pipes[i]);
	}
	
	int acc = 0;
	for(int i = 0; i < dim; i++) {
		for(int j = 0; j < dim; j++) {
			pids[i][j] = fork();
			if(pids[i][j] == 0) {
				//child
				
				//int index = i*dim+j;
				int indexA_r = i*dim+j;
				int indexB_r = j*dim+i+dim*dim;
				
				int indexA_w = indexA_r - 1;
				if(j==0) {
					indexA_w = indexA_r+(dim-1);
				}
				
				int indexB_w = indexB_r - 1;
				if(i==0) {
					indexB_w = indexB_r + (dim-1);
				}
				
				for(int k = 0; k < n_pipes; k++) {
					if(k != indexA_r && k != indexB_r) {
						close(pipes[k][0]);
					}
					
					if(k != indexA_w && k != indexB_w) {
						close(pipes[k][1]);
					}
				}
				
				int a, b, ind_a_j, ind_b_i;
				
				if(i+j < dim) 
					ind_a_j = i+j;
				else
					ind_a_j = i+j-dim;
				
				if(i+j < dim)
					ind_b_i = i+j;
				else
					ind_b_i = i+j-dim;
					
				a = matrixA[i][ind_a_j];
				b = matrixB[ind_b_i][j];
				
				for(int k = 0; k < dim; k++) {

					acc = acc + a*b;				
					
					write(pipes[indexA_w][1], &a, sizeof(int));
					write(pipes[indexB_w][1], &b, sizeof(int));
					
					read(pipes[indexA_r][0], &a, sizeof(int));
					read(pipes[indexB_r][0], &b, sizeof(int));
					
				}
				
				printf("c(%d, %d) = %d\n", i, j, acc);
				
				return 0;
			}
		}
	}
	
	for(int i = 0; i < dim*dim; i++)
		wait(NULL);
		
	clock_t end = clock();
	time_spent += (double)(end - begin) / CLOCKS_PER_SEC;
	time_spent = time_spent * 1000;
	printf("Tempo impiegato = %f ms\n", time_spent);
	
	return 0;
}
