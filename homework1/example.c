#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[]) {
    int rank, size;
    char * data;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size < 2) {
        fprintf(stderr, "Этот пример требует как минимум 2 процесса\n");
        MPI_Finalize();
        return 1;
    }

    if (rank == 0) {
        data = "hello from 0";
        MPI_Sendrecv_replace(&data, 12, MPI_CHAR, 1, 0, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Процесс %d получил значение: %s\n", rank, data);
    } else if (rank == 1) {
        data = "hello from 1";
        MPI_Sendrecv_replace(&data, 12, MPI_CHAR, 0, 0, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Процесс %d получил значение: %s\n", rank, data);
    }

    MPI_Finalize();
    return 0;
}
