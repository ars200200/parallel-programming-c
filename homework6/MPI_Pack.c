#include <stdio.h>
#include <mpi.h>
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
    MPI_Request req;


    MPI_Status status;
    char buffer[N * M / size * sizeof(int)];
    int position;

    if (rank == 0) {
        
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < M; j++) {
                matrix[i][j] = i * M + j;
            }
        }

        position = 0;
        int counter = 0;
        for (int i=0; i<(int) sqrt(size); i++){
            for (int j=0; j<(int) sqrt(size); j++){
                position = 0;
                for (int k=0; k<horizontal_side; k++)
                {
                    for (int l=0; l<vertical_side; l++)
                    {
                        MPI_Pack(&matrix[i * horizontal_side + k][j * vertical_side + l], 1, MPI_INT, buffer, N * M / size * sizeof(int), &position, MPI_COMM_WORLD);
                    }
                }
                
                MPI_Send(buffer, position, MPI_PACKED, counter, 1, MPI_COMM_WORLD);
                counter += 1;
            }
            
        }

    } 



    position = 0;
    int mtr[horizontal_side][vertical_side];
    MPI_Recv(buffer, N * M / size * sizeof(int), MPI_PACKED, 0, 1, MPI_COMM_WORLD, &status);
    MPI_Unpack(buffer, N * M / size * sizeof(int), &position, mtr, N * M / size, MPI_INT, MPI_COMM_WORLD);
        
    printf("Rank %d received block:\n", rank);
    for (int i = 0; i < horizontal_side; i++) {
        for (int j = 0; j < vertical_side; j++) {
            printf("%d ", mtr[i][j]);
        }
        printf("\n");
    }


    MPI_Finalize();
    return 0;
}