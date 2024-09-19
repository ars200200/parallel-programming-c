#include <stdio.h>
#include "mpi.h"
#include <stdlib.h>
#include <math.h>

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
    int *result_each_proc = calloc(arraysize / sqrt(size), sizeof(int));
    int *result = calloc(arraysize/ sqrt(size), sizeof(int));
    int *result_on_left = calloc(arraysize / sqrt(size), sizeof(int));
    int dims[2] = {2, 2};
    int periods[2] = {0, 0};
    int coords[2];
    int reorder = 0;
    MPI_Comm cart_comm;
    int counter = 0;


    if (rank == 0){
        
        for (int i=0; i<sqrt(size); i++){ 
                for (int j=0; j< sqrt(size); j++){
                    for(int k=0; k<arraysize/sqrt(size); k++){
                        for (int l=0; l<arraysize/sqrt(size); l++){
                            matrix[counter] = (arraysize/((int)sqrt(size)) * i + k) * arraysize + arraysize/((int)sqrt(size)) * j + l + 1;
                            counter += 1;
                        }
                    }

                }
                
        }
        for (int i=0; i<arraysize; i++){
            array[i] = i * i;
        }

       /* for (int i=0; i<arraysize*arraysize; i++){
            
            if (i % arraysize == 0){
                printf("\n");
            }
            printf("%d ", matrix[i]);
    
        } */

    }
    
    
    MPI_Cart_create(MPI_COMM_WORLD, 2, dims, periods, reorder, &cart_comm);
    MPI_Cart_coords(cart_comm, rank, 2, coords);

    int size_of_matrix_in_each_proc = arraysize * arraysize / size;
    MPI_Bcast(array, arraysize, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Scatter(matrix, size_of_matrix_in_each_proc, MPI_INT,
                recv_matrix, size_of_matrix_in_each_proc, MPI_INT, 0, cart_comm);


    for (int i=0; i<arraysize / sqrt(size); i++){
        int sum = 0;
        
        for (int j=0; j<arraysize / sqrt(size); j++){
            sum += recv_matrix[i * arraysize /(int)sqrt(size) + j] * array[coords[1] * arraysize /(int)sqrt(size) + j];
        }
        result_each_proc[i] = sum;
    }
  
    if (coords[1] == 1){
        int local_rank_other_coord;
        int other_coord[2] = {coords[0], 0};
        MPI_Cart_rank(cart_comm, other_coord, &local_rank_other_coord); 
        MPI_Send(result_each_proc, arraysize / sqrt(size), MPI_INT, local_rank_other_coord, 0, cart_comm);
    }

    
    if (coords[1] == 0){
        MPI_Status status;
        int local_rank_other_coord;
        int other_coord[2] = {coords[0], 1};
        MPI_Cart_rank(cart_comm, other_coord, &local_rank_other_coord);
        MPI_Recv(result_on_left, arraysize / sqrt(size), MPI_INT, local_rank_other_coord, 0, cart_comm, &status);
        
        for (int i=0; i<arraysize / sqrt(size); i++){
            result_each_proc[i] += result_on_left[i];        
        }

        
        if (coords[0] == 1){
            MPI_Send(result_each_proc, arraysize / sqrt(size), MPI_INT, 0, 0, cart_comm);
        }
        if (coords[0] == 0){
            int rnk;
            int other_coord [2]= {1, 0};
            MPI_Status status2;
            MPI_Cart_rank(cart_comm, other_coord, &rnk);
            MPI_Recv(result, arraysize / sqrt(size), MPI_INT, rnk, 0, cart_comm, &status2);
        }
    }
    finish = MPI_Wtime();
    if (rank == 0){
        for (int i=0; i<arraysize/sqrt(size); i++){
        //printf("%d\n", result_each_proc[i]);
        }
        for (int i=0; i<arraysize/sqrt(size); i++){
        //printf("%d\n", result[i]);
        }
        printf("_________________________________\n");
        printf("%f\n", finish - start);
    }
    
    
    
    return 0;
}
