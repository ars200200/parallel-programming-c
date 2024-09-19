#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>
#include <time.h>
#define N 256 // Размер сетки
#define MAX_ITER 10000 // Максимальное количество итераций
#define TOL 1e-6 // Допустимая погрешность

void initialize(double grid[N][N]) {
double dx = 1.0 / (N - 1);
double dy = 1.0 / (N - 1);

for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
    double x = i * dx;
    double y = j * dy;

        if (j == 0) {
            grid[i][j] = sin(M_PI * x); // u(x, 0) = sin(pi * x)
        } else if (j == N-1) {
            grid[i][j] = exp(-x) * sin(M_PI * x); // u(x, 1) = e^(-x) * sin(pi * x)
        } else if (i == 0 || i == N-1) {
            grid[i][j] = 0.0; // u(0, y) = u(1, y) = 0
        } else {
            grid[i][j] = 0.0;
            // Внутренние точки инициализируем нулями
        }
    }
}

}

void write_grid_to_file(double grid[N][N], const char* filename) {
FILE *file = fopen(filename, "w");
if (file == NULL) {
    perror("Error opening file");
    exit(EXIT_FAILURE);
}

for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
        fprintf(file, "%.2f ", grid[i][j]);
        }
    fprintf(file, "\n");
}

fclose(file);
}

int main() {
double grid[N][N], new_grid[N][N];
initialize(grid);

int iter = 0;
double max_diff = 0.0;
double t0 = omp_get_wtime();
while (iter < MAX_ITER) {
max_diff = 0.0;

#pragma omp parallel for reduction(max:max_diff) 
for (int i = 1; i < N-1; i++) {

    for (int j = 1; j < N-1; j++) {
        new_grid[i][j] = 0.25 * (grid[i-1][j] + grid[i+1][j] + grid[i][j-1] + grid[i][j+1]);
        double diff = fabs(new_grid[i][j] - grid[i][j]);
        if (diff > max_diff) {
        max_diff = diff;
        }
    }
}

#pragma omp parallel for
for (int i = 1; i < N-1; i++) {

    for (int j = 1; j < N-1; j++) {
        grid[i][j] = new_grid[i][j];
        }
}

iter++;
if (max_diff <= TOL) break;
}

printf("Converged after %d iterations with max difference %f\n", iter, max_diff);

write_grid_to_file(grid, "solution.txt");
double t1 = omp_get_wtime();
printf("%f", (t1 -t0));
return 0;
}