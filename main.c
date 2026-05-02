#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// MATRIZ
double** allocMatrix(int n) {
  double** M = malloc(n * sizeof(double*));
    for (int i = 0; i < n; i++)
      M[i] = calloc(n, sizeof(double));
    return M;
}

void freeMatrix(double** M, int n) {
  for (int i = 0; i < n; i++) 
    free(M[i]);
  free(M);
}

void fillRandom(double** M, int n) {
  for (int i = 0; i < n; i++)
    for (int j = 0; j < n; j++)
      M[i][j] = (double)(rand() % 201) - 100.0;  // rango [-100, 100]
}

// MULTIPLICACIÓN 
void multiplyClassic(double** A, double** B, double** C, int n) {
  for (int i = 0; i < n; i++)
    for (int k = 0; k < n; k++) {
      double a = A[i][k];
        for (int j = 0; j < n; j++)
          C[i][j] += a * B[k][j];
    }
}

// TIEMPO (nanosegundos → microsegundos)
long long elapsed_us(struct timespec start, struct timespec end) {
  return ((long long)(end.tv_sec  - start.tv_sec)  * 1000000000LL +(long long)(end.tv_nsec - start.tv_nsec)) / 1000;
}

// MAIN
int main(void) {
  srand(42);  

  printf("Multiplicacion estandar O(n^3)\n");
  printf("n tiempo_us\n");

  int sizes[] = { 2, 4, 8, 16, 32, 48, 64, 96, 128, 192, 256, 384, 512 };
  int numSizes = (int)(sizeof(sizes) / sizeof(sizes[0]));

  for (int si = 0; si < numSizes; si++) {
    int n = sizes[si];

    double** A = allocMatrix(n);
    double** B = allocMatrix(n);
    double** C = allocMatrix(n);

    fillRandom(A, n);
    fillRandom(B, n);

    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);
    multiplyClassic(A, B, C, n);
    clock_gettime(CLOCK_MONOTONIC, &end);

    printf("%d %lld\n", n, elapsed_us(start, end));
    fflush(stdout);

    freeMatrix(A, n); freeMatrix(B, n); freeMatrix(C, n);
  }

  return 0;
}
