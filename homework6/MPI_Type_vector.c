#include <stdio.h>
#include "mpi.h"
#include <math.h>


#define N 4
#define M 4

int main(int argc, char *argv[]) {
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int matrix[N][M];
    int horizontal_side = (int) N / sqrt(size);
    int vertical_side = (int) M / sqrt(size);
    int sqrt_size = (int) sqrt(size);

    MPI_Datatype blockType;
    MPI_Type_vector(horizontal_side, vertical_side, M, MPI_INT, &blockType);
    MPI_Type_commit(&blockType);

    if (rank == 0) {
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < M; j++) {
                matrix[i][j] = i * M + j;    
            }
        }

        for (int i=0; i<sqrt_size; i++){
            for (int j=0; j<sqrt_size; j++){
                MPI_Send(&matrix[horizontal_side * i][vertical_side * j], 1, blockType, i * sqrt_size + j, 0, MPI_COMM_WORLD);
            }
        }
    }



        int mtr[horizontal_side][vertical_side];
        MPI_Status status;
        MPI_Recv(mtr, N * M / size, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
        printf("Rank %d received block:\n", rank);
        for (int i = 0; i < horizontal_side; i++) {
            for (int j=0; j< vertical_side; j++){
                printf("%d ", mtr[i][j]);
            }
            printf("\n");
        }
    

    MPI_Type_free(&blockType);
    MPI_Finalize();
    return 0;
}
