#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>



double logistic_func(double x, double r){
    int max_iteration = 500;
    double result = x;
    for (int i=0; i<max_iteration; i++){
        result = r * result * (1 - result);
    }
    return result;
}

void matrix_to_txt(int N, double A[], char *name){
    FILE * fp;
    char header[] = "output.txt";
    if (name != NULL)
        fp = fopen(name,"w");
    else
        fp = fopen(header,"w");

    for (int i = 0; i < N*N; i++)
    {
        fprintf(fp,"%.10f",A[i]);
        if ((i+1)%N == 0)
            fprintf(fp,"\n");
        else
            fprintf(fp,",");
    }
    fclose(fp);
}


int main(int argc, char * argv[]){

    int rank, size;
    double x_min, x_max, r_min, r_max, dx, dr;
    const int points = 2048;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    x_min = 0.1;
    x_max = 1.0;
    r_min = 0.0;
    r_max = 4.0;
    double *result, *r_array;
    double *x_array = calloc(points, sizeof(double));
    dx = (x_max - x_min) / points;
    dr = (r_max - r_min) / points;
    for (int i=0; i<points; i++){
        x_array[i] = x_min + dx * i;
    }

    if (rank == 0){
        r_array = calloc(points, sizeof(double));
        for (int i=0; i<points; i++){
            r_array[i] = r_min + i * dr;
        }
         result = calloc(points * points, sizeof(double));
    }


    const int Np = points / size;
    double *r_array_local = calloc(Np, sizeof(double));
    double *result_local = calloc(Np * points, sizeof(double));
    
    MPI_Scatter(r_array, Np, MPI_DOUBLE, r_array_local, Np, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    for (int i=0; i<Np; i++){
        for (int j=0; j<points; j++){
            result_local[i * points + j] = logistic_func(x_array[j], r_array_local[i]);
            
        }  
    }
 
    MPI_Gather(result_local, Np*points, MPI_DOUBLE, result, Np * points, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    if (rank == 0){
        matrix_to_txt(points, result, NULL);
    }


    MPI_Finalize();

    return 0;
}
