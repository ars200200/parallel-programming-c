#include <stdio.h>
#include "mpi.h"


int main(int argc, char** argv){
    int size, rank;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    const int n = 4;
    int arr[4] = {0, 2, 4, 6};

    MPI_Group group_world, group1, group2_excl, group2_difference;

    MPI_Comm_group(MPI_COMM_WORLD, &group_world);

    MPI_Group_incl(group_world, n, arr, &group1);
    MPI_Group_excl(group_world, n, arr, &group2_excl);
    MPI_Group_difference(group_world, group1, &group2_difference);
    int result;
    MPI_Group_compare(group2_excl, group2_difference, &result);

    printf("%d\n", result == MPI_IDENT);
    MPI_Finalize();
    return 0;
}