#include <stdio.h>
#include "mpi.h"

int main(int argc, char* argv[]){
    int rank, size;
    double t1, t2;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int data[800000];
    if (rank == 0){
    for (int i=0; i<800000; i++){
        data[i] = i;
    }
    }
    t1 = MPI_Wtime();
    MPI_Bcast(data, 800000, MPI_INT, 0, MPI_COMM_WORLD);
    t2 = MPI_Wtime();
    MPI_Finalize();
    if (rank == 0){printf("%f\n", t2 - t1);}
    
    return 0;
}
