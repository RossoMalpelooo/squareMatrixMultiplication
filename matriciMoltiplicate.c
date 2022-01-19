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
  printf("Inserire dimensione matrici; se A, b c M(nxn), n=");
  scanf("%d", &dim);
  return dim;
}

void matrixMultiplication(int n){
	FILE *fptr;
	int matrixA[n][n], matrixB[n][n], num, matrixC[n][n];
	
	//salvataggio matrice A
	fptr = fopen("matrixA.txt", "r");
	for(int i=0; i<n; i++){
	  for(int j=0; j<n; j++){
	    fscanf(fptr, "%d", &num);
	    matrixA[i][j] = num;
	  }
	}
	fclose(fptr);

	//salvataggio matrice B
	fptr = fopen("matrixB.txt", "r");
	for(int i=0 ; i<n; i++){
	  for(int j=0; j<n; j++){
	    fscanf(fptr, "%d", &num);
	    matrixB[i][j] = num;
	  }
	}
	fclose(fptr);

	//Moltiplicazione
	for(int i=0; i<n; i++){ //i = colonna(a,b) e riga(c)
	  for(int j=0; j<n; j++){ //j = riga(a,b) e colonna(c)
	    long long int sum = 0;
	    for(int k=0; k<n; k++){
	      sum = sum + matrixA[i][k] * matrixB[k][j];
	      matrixC[i][j] = sum;
	    }
	  }
	}

	print(n, matrixC);
}

int main(int argc, char *argv[]){
        srand(time(NULL));
	double time_spent = 0.0;
 
 	int n = get_dim();
	clock_t begin = clock();
	matrixMultiplication(n);
	clock_t end = clock();
	time_spent += (double)(end - begin) / CLOCKS_PER_SEC;
 	time_spent = time_spent*1000;
	printf("Tempo impiegato= %f ms\n", time_spent);
	return 0;
}
