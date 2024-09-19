#include <stdio.h>
#include "mpi.h"
#include <stdlib.h>
int main(int argc, char* argv[]){
	 int nproc, pid;
	 int x = 1;
	 
	 MPI_Init(&argc, &argv);
	 MPI_Comm_size(MPI_COMM_WORLD, &nproc);
	 MPI_Comm_rank(MPI_COMM_WORLD, &pid);
	 int len = 18;
	 if (pid == 0){
		char string[] = "hello world from 0";
		MPI_Send(string, len, MPI_CHAR, 1, 1010, MPI_COMM_WORLD);
	 }
	 if (pid == 1){
		char *y = calloc(len, sizeof(char));
		MPI_Status status;
		MPI_Recv(y, len, MPI_CHAR, 0, 1010, MPI_COMM_WORLD, &status);
		printf("1 gets ");
		printf(	y,"\n");
		
	 }
	

	 MPI_Finalize(); 

	 return 0;
}

