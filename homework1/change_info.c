#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int main(int argc, char* argv[]){
	int nproc, pid;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &nproc);
	MPI_Comm_rank(MPI_COMM_WORLD, &pid);
	int size = 12;
	MPI_Status status;
	char *str = calloc(size, sizeof(char));
	if (pid == 0){
		str = "Hello from 0";
	}
	if (pid == 1){
		str = "Hello from 1";
	}
	if (pid < 2){
		printf("%d",(pid+1) % 2);
		MPI_Sendrecv_replace(str, size, MPI_CHAR, 
					(pid+1) % 2, 1010, (pid+1) % 2, 1010, MPI_COMM_WORLD, &status);
	}


	if (pid == 0){
		printf("proc 0 gets", str);
	}

	if (pid == 1){
		printf("proc 1 gets", str);
	}
	MPI_Finalize();
	free(str);
	return 0;
}

