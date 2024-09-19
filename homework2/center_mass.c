#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h> 
#include <time.h>

int main(int argc, char* argv[]){

    int size, rank;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    MPI_Status status;
    
    int N_COUNT = 10;
    double x_avg, y_avg, sum_ro, SUM_RO;
    double x_array[N_COUNT], y_array[N_COUNT];
    double x_sum, y_sum;

    srand(time(NULL) + rank);

    for (int i=0; i<N_COUNT; i++){

        x_array[i] = (rand() * 1.0) / (RAND_MAX * 1.0) * 100;
        y_array[i] = (rand() * 1.0) / (RAND_MAX * 1.0)* 100;  
        x_sum += x_array[i];
        y_sum += y_array[i];
    }

    x_avg = 1.0 * x_sum / (N_COUNT * 1.0);
    y_avg = 1.0 * y_sum / (N_COUNT * 1.0);

    double avg[2] = {x_avg, y_avg};
    double AVG[2], SUM[2];

    MPI_Reduce(avg, SUM, 2, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0){

        AVG[0] = SUM[0] / (size * 1.0);
        AVG[1] = SUM[1] / (size * 1.0);

    }

    MPI_Bcast(AVG, 2, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    double ro[N_COUNT];
    for (int i=0; i<N_COUNT; i++){
        ro[i] = sqrt( pow(x_array[i] - AVG[0], 2) + pow(y_array[i] - AVG[1], 2));
    }

    if (rank != 0){
        MPI_Send(ro, N_COUNT, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
    }

    if (rank == 0){
        double ro_array[N_COUNT * size];
        for (int i=0; i<N_COUNT; i++){
            ro_array[i] = ro[i];
        }
        
        for (int i=1; i<size; i++){
            MPI_Recv(&ro, N_COUNT, MPI_DOUBLE, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
            for (int j=0; j<N_COUNT; j++){
                ro_array[i * N_COUNT + j] = ro[j];
                
            }
        }
    
        for (int i=0; i<N_COUNT * size; i++){
            printf("%f\n", ro_array[i]);
        }
    }

    MPI_Finalize();
    return 0;
}
