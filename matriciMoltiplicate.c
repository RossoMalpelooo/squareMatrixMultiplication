#include<stdio.h>
#include<stdlib.h>
#include<time.h>

void print(int n, int matrix[][n]) {
  for(int i = 0; i < n; i++){
    for(int j = 0; j < n; j++){
      printf("%d ", matrix[i][j]);
    }
     printf("\n");
  }
}

int get_dim(){
  int dim;
  printf("enter size of matrices: ");
  scanf("%d", &dim);
  return dim;
}

void read_matrix(int dim, const char name[], int matrix[][dim]) {
	FILE *fptr;
	
	fptr = fopen(name, "r");
	for(int i = 0; i < dim; i++) 
		for(int j = 0; j < dim; j++) 
			fscanf(fptr, "%d", &matrix[i][j]);
	fclose(fptr);
}

void matrixMultiplication(int dim, int matrixA[][dim], int matrixB[][dim], int matrixC[][dim]){
	FILE *fptr;

	for(int i=0; i<dim; i++){ //i = colonna(a,b) e riga(c)
	  for(int j=0; j<dim; j++){ //j = riga(a,b) e colonna(c)
	    long long int sum = 0;
	    for(int k=0; k<dim; k++){
	      sum = sum + matrixA[i][k] * matrixB[k][j];
	      matrixC[i][j] = sum;
	    }
	  }
	}

	print(dim, matrixC);
}

int main(int argc, char *argv[]){
        srand(time(NULL));
	double time_spent = 0.0;
	
	int n = get_dim();
	int matrixA[n][n], matrixB[n][n], matrixC[n][n];
	const char nameA[] = "matrixA.txt";
	const char nameB[] = "matrixB.txt";
	
	read_matrix(n, nameA, matrixA);
	read_matrix(n, nameB, matrixB);
 
	clock_t begin = clock();
	matrixMultiplication(n, matrixA, matrixB, matrixC);
	clock_t end = clock();
	time_spent += (double)(end - begin) / CLOCKS_PER_SEC;
 	time_spent = time_spent*1000;
	printf("time spent = %f ms\n", time_spent);
	return 0;
}
