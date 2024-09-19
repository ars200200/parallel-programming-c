#include <stdio.h>
#include "mpi.h"
#include <stdlib.h>


int main(int argc, char* argv[]){

    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    double start, finish;
    start  = MPI_Wtime();
    int arraysize = 200;
    int *matrix = calloc(arraysize * arraysize, sizeof(int));
    int *array = calloc(arraysize, sizeof(int));
    int *recv_matrix = calloc(arraysize * arraysize / size, sizeof(int));
    int *result = calloc(arraysize, sizeof(int));
    int *result_each_proc = calloc(arraysize / size, sizeof(int));


    if (rank == 0){
        for (int i=0; i<arraysize; i++){
            array[i] = i * i;
        }

        for (int i=0; i<arraysize * arraysize; i++){
            matrix[i] = i + 1;
        }
    }


    int size_of_matrix_in_each_proc = arraysize * arraysize / size;
    MPI_Scatter(matrix, size_of_matrix_in_each_proc, MPI_INT,
                recv_matrix, size_of_matrix_in_each_proc, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(array, arraysize, MPI_INT, 0, MPI_COMM_WORLD);


    for (int i=0; i<arraysize / size; i++){

        int sum = 0;
        for (int j=0; j<arraysize; j++){
            sum += recv_matrix[i * arraysize + j] * array[j];
        }
        result_each_proc[i] = sum;
    }


    MPI_Gather(result_each_proc, arraysize / size, MPI_INT,
                result, arraysize / size, MPI_INT, 0, MPI_COMM_WORLD);

    finish = MPI_Wtime();
    if (rank == 0){
        for (int i=0; i<arraysize; i++){
            //printf("%d\n", result[i]);
        }

        printf("______________________________\n");
        printf("%f\n", finish - start);
    }

    
    return 0;
}
