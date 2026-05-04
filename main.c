#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define BASE_CASE 32

#define REPS 15

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

static void resetMatrix(double ** M, int n) {
  for (int i = 0; i < n; i++)
    memset(M[i], 0, n * sizeof(double));
}

void fillRandom(double** M, int n) {
  for (int i = 0; i < n; i++)
    for (int j = 0; j < n; j++)
      M[i][j] = (double)(rand() % 201) - 100.0;  // rango [-100, 100]
}

//MERGE SORT como ordenamiento
static void merge(long long *arr, long long *tmp, int left, int mid, int right) {
  int i = left, j = mid, k = left;
  while (i < mid && j < right)
    tmp[k++] = (arr[i] <= arr[j]) ? arr[i++] : arr[j++];
  while (i < mid)   tmp[k++] = arr[i++];
  while (j < right) tmp[k++] = arr[j++];
  memcpy(arr + left, tmp + left, (right - left) * sizeof(long long));
}

static void mergeSortRec(long long *arr, long long *tmp, int left, int right) {
  if (right - left <= 1) return;
  int mid = left + (right - left) / 2;
  mergeSortRec(arr, tmp, left, mid);
  mergeSortRec(arr, tmp, mid, right);
  merge(arr, tmp, left, mid, right);
}

static void mergeSort(long long *arr, int n) {
  long long *tmp = malloc(n * sizeof(long long));
  if (!tmp) { fputs("sin memoria\n", stderr); exit(1); }
  mergeSortRec(arr, tmp, 0, n);
  free(tmp);
}

static long long mediana(long long *t, int reps) {
  mergeSort(t, reps);
  return t[reps / 2];
}


// MULTIPLICACIÓN clásica 
void multiplyClassic(double** A, double** B, double** C, int n) {
  for (int i = 0; i < n; i++)
    for (int k = 0; k < n; k++) {
      double a = A[i][k]; 
      for (int j = 0; j < n; j++)
        C[i][j] += a * B[k][j];
    }
}

// STRASSEN

static void extractSub(double** dst, double** src, int n, int r, int c) {
  int h = n / 2;
  for (int i = 0; i < h; i++)
    for (int j = 0; j < h; j++)
      dst[i][j] = src[r+i][c+j];
}

static void insertSub(double** dst, double** src, int n, int r, int c) {
  int h = n / 2;
  for (int i = 0; i < h; i++)
    for (int j = 0; j < h; j++)
      dst[r+i][c+j] = src[i][j];
}

static void matAdd(double** C, double** A, double** B, int n) {
  for (int i = 0; i < n; i++)
    for (int j = 0; j < n; j++)
      C[i][j] = A[i][j] + B[i][j];
}

static void matSub(double** C, double** A, double** B, int n) {
  for (int i = 0; i < n; i++)
    for (int j = 0; j < n; j++)
      C[i][j] = A[i][j] - B[i][j];
}

void strassen(double** C, double** A, double** B, int n) {
  if (n <= BASE_CASE) { 
    multiplyClassic(A, B, C, n); 
    return; 
  }

  int h = n / 2;
  double** a11=allocMatrix(h); 
  double** a12=allocMatrix(h);
  double** a21=allocMatrix(h); 
  double** a22=allocMatrix(h);
  double** b11=allocMatrix(h); 
  double** b12=allocMatrix(h);
  double** b21=allocMatrix(h); 
  double** b22=allocMatrix(h);

  double** p1=allocMatrix(h);  
  double** p2=allocMatrix(h);
  double** p3=allocMatrix(h);  
  double** p4=allocMatrix(h);
  double** p5=allocMatrix(h);  
  double** p6=allocMatrix(h);
  double** p7=allocMatrix(h);
  
  double** t1=allocMatrix(h);  
  double** t2=allocMatrix(h);
  
  double** c11=allocMatrix(h); 
  double** c12=allocMatrix(h);
  double** c21=allocMatrix(h); 
  double** c22=allocMatrix(h);

  extractSub(a11,A,n,0,0); 
  extractSub(a12,A,n,0,h);
  extractSub(a21,A,n,h,0); 
  extractSub(a22,A,n,h,h);
  extractSub(b11,B,n,0,0); 
  extractSub(b12,B,n,0,h);
  extractSub(b21,B,n,h,0); 
  extractSub(b22,B,n,h,h);

  /* P1 = a11*(b12-b22) */
  matSub(t1,b12,b22,h); 
  strassen(p1,a11,t1,h);
  /* P2 = (a11+a12)*b22 */
  matAdd(t1,a11,a12,h); 
  strassen(p2,t1,b22,h);
  /* P3 = (a21+a22)*b11 */
  matAdd(t1,a21,a22,h); 
  strassen(p3,t1,b11,h);
  /* P4 = a22*(b21-b11) */
  matSub(t1,b21,b11,h); 
  strassen(p4,a22,t1,h);
  /* P5 = (a11+a22)*(b11+b22) */
  matAdd(t1,a11,a22,h); 
  matAdd(t2,b11,b22,h); 
  strassen(p5,t1,t2,h);
  /* P6 = (a12-a22)*(b21+b22) */
  matSub(t1,a12,a22,h); 
  matAdd(t2,b21,b22,h); 
  strassen(p6,t1,t2,h);
  /* P7 = (a11-a21)*(b11+b12) */
  matSub(t1,a11,a21,h); 
  matAdd(t2,b11,b12,h); 
  strassen(p7,t1,t2,h);
 
  /* c11 = P5+P4-P2+P6 */
  matAdd(c11,p5,p4,h); 
  matSub(c11,c11,p2,h); 
  matAdd(c11,c11,p6,h);
  /* c12 = P1+P2 */
  matAdd(c12,p1,p2,h);
  /* c21 = P3+P4 */
  matAdd(c21,p3,p4,h);
  /* c22 = P5+P1-P3-P7 */
  matAdd(c22,p5,p1,h); 
  matSub(c22,c22,p3,h); 
  matSub(c22,c22,p7,h);
 
  insertSub(C,c11,n,0,0); 
  insertSub(C,c12,n,0,h);
  insertSub(C,c21,n,h,0); 
  insertSub(C,c22,n,h,h);
  
  freeMatrix(a11,h); 
  freeMatrix(a12,h); 
  freeMatrix(a21,h); 
  freeMatrix(a22,h);
  
  freeMatrix(b11,h); 
  freeMatrix(b12,h); 
  freeMatrix(b21,h); 
  freeMatrix(b22,h);
  
  freeMatrix(p1,h);  
  freeMatrix(p2,h);  
  freeMatrix(p3,h);  
  freeMatrix(p4,h);
  freeMatrix(p5,h);  
  freeMatrix(p6,h);  
  freeMatrix(p7,h);
  
  freeMatrix(t1,h);  
  freeMatrix(t2,h);
  
  freeMatrix(c11,h); 
  freeMatrix(c12,h); 
  freeMatrix(c21,h); 
  freeMatrix(c22,h);
}

static int nextPow2(int n) {
  int p = 1; 
  while(p < n) 
    p<<=1; 
  return p; 
}

// TIEMPO (nanosegundos → microsegundos)
long long elapsed_us(struct timespec start, struct timespec end) {
  return ((long long)(end.tv_sec  - start.tv_sec)  * 1000000000LL +(long long)(end.tv_nsec - start.tv_nsec)) / 1000;
}


// MAIN
int main(void) {
  srand(42);  

  printf("Multiplicacion clasica vs Strassen\n");
  printf("n tiempo_us strassen_us\n");

  int sizes[] = {
    2, 4, 8,
    16, 20, 24, 28,        /* antes de 32              */
    32,                    /* umbral sugerido MIT       */
    36, 40, 44, 48,        /* justo despues de 32       */
    64, 96, 128, 192, 256, 384, 512
  };

  int numSizes = (int)(sizeof(sizes) / sizeof(sizes[0]));

  for (int si = 0; si < numSizes; si++) {
    int n = sizes[si];
    int np = nextPow2(n);
    
    long long tc[REPS], ts[REPS];

    //mismas matrices para las repeticiones
    double** A = allocMatrix(n);
    double** B = allocMatrix(n);

    fillRandom(A, n);
    fillRandom(B, n);

    double** C = allocMatrix(n);

    double** Ap = allocMatrix(np);
    double** Bp = allocMatrix(np);
    double** Cs = allocMatrix(np);
   

    for (int r = 0; r < REPS; r++){
      
      resetMatrix(C, n);
      resetMatrix(Cs, np);

      //clásico
      struct timespec start, end;
      clock_gettime(CLOCK_MONOTONIC, &start);
      multiplyClassic(A, B, C, n);
      clock_gettime(CLOCK_MONOTONIC, &end);
      tc[r] = elapsed_us(start, end);

      //padding a potencia de 2
      for (int i=0;i<n;i++)
        for (int j=0;j<n;j++) { 
          Ap[i][j] = A[i][j]; 
          Bp[i][j]=B[i][j]; 
        }

      //strassen      
      clock_gettime(CLOCK_MONOTONIC, &start);
      strassen(Cs, Ap, Bp, np);
      clock_gettime(CLOCK_MONOTONIC, &end);
      ts[r] = elapsed_us(start, end);
    }
    
    freeMatrix(Ap,np);
    freeMatrix(Bp,np);
    freeMatrix(Cs,np);

    freeMatrix(A, n);
    freeMatrix(B, n);
    freeMatrix(C, n);

    printf("%d %lld %lld\n", n, mediana(tc,REPS), mediana(ts,REPS));
  }

  return 0;
}
