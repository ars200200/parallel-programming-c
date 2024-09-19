#include <stdio.h>
#include "mpi.h"

int main(int argc, char* argv[]){
    int rank, size;
    double t1, t2;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Status status;


    int data[800000];
    if (rank == 0){
        for (int i=0; i<800000; i++){
            data[i] = i*i;
        }
    }
    t1 = MPI_Wtime();
    if (rank == 0){
        for (int i=1; i<size; i++){
            MPI_Send(data, 800000, MPI_INT, i, 1010, MPI_COMM_WORLD);
        }
    }
   
    if (rank != 0){
        MPI_Recv(data, 800000, MPI_INT, 0, 1010, MPI_COMM_WORLD, &status);
    }
    t2 = MPI_Wtime();
    if (rank == 0){printf("%f", t2 - t1);}

    MPI_Finalize();

    return 0;
}
