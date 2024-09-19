#include <stdio.h>
#include "mpi.h"


int main(int argc, char** argv){
    int size, rank;
    int number=1222;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    const int n = 4;
    int arr[4] = {0, 2, 4, 6};
    

    if (rank == 0){
        number = 0;
    }

    if (rank == 1){
        number = 1;
    }
    MPI_Group group_world, group0, group1;

    MPI_Comm_group(MPI_COMM_WORLD, &group_world);

    MPI_Group_incl(group_world, n, arr, &group0);
    MPI_Group_difference(group_world, group0, &group1);
    MPI_Comm MPI_COMM_GROUP0, MPI_COMM_GROUP1;
    MPI_Comm_create(MPI_COMM_WORLD, group1, &MPI_COMM_GROUP1);
    MPI_Comm_create(MPI_COMM_WORLD, group0, &MPI_COMM_GROUP0);
    

 
    if (MPI_COMM_GROUP1 != MPI_COMM_NULL){
        
        MPI_Bcast(&number, 1, MPI_INT, 0, MPI_COMM_GROUP1);
        
    }

    if (MPI_COMM_GROUP0 != MPI_COMM_NULL){
        
        MPI_Bcast(&number, 1, MPI_INT, 0, MPI_COMM_GROUP0);
    }

    
    printf("process %d received %d\n", rank, number);
    if (MPI_COMM_NULL != MPI_COMM_GROUP0) {
        MPI_Comm_free(&MPI_COMM_GROUP0);
    }

    if (MPI_COMM_NULL != MPI_COMM_GROUP1) {
        MPI_Comm_free(&MPI_COMM_GROUP1);
    }

    int rnk0, rnk1;
    MPI_Group_free(&group_world);
    MPI_Group_rank(group0, &rnk0);


    if (rnk0 != MPI_UNDEFINED){
        MPI_Group_free(&group0);
    }

    MPI_Group_rank(group1, &rnk1 );
    if (rnk1 != MPI_UNDEFINED){
        MPI_Group_free(&group1);
    }

    MPI_Finalize();
    return 0;
}