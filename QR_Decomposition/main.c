#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct {
  int dim;
  double *value;
} Vector;

typedef struct {
  int rows;
  int cols;
  double *value;
} Matrix;

void free_vector(Vector *v) {
  if (v->value != NULL) {
    free(v->value);
  }
  free(v);
}

void free_matrix(Matrix *m) {
  if (m->value != NULL) {
    free(m->value);
  }
  free(m);
}

Vector *create_vector(int dim) {
  Vector *v = (Vector *)malloc(sizeof(Vector));
  if (v == NULL) {
    printf("Failed to allocate Vector memory\n");
    exit(1);
  }
  v->value = (double *)calloc(dim, sizeof(double));
  if (v->value == NULL) {
    printf("Failed to allocate Vector value memory\n");
    free_vector(v);
    exit(1);
  } 
  return v;
}

Matrix *create_matrix(int rows, int cols) {
  Matrix *m = (Matrix *)malloc(sizeof(Matrix));
  if (m == NULL) {
    printf("Failed to allocate Matrix memory\n");
    exit(1);
  }
  m->value = (double *)calloc(rows * cols, sizeof(double));
  if (m->value == NULL) {
    printf("Failed to allocate Matrix value memory\n");
    free_matrix(m);
    exit(1);
  } 
  return m;
}

void sme(Matrix *m, int row, int col, double value) {
  if (row < 0 || row > m->rows || col < 0 || col > m->cols) {
    printf("Value Error(sme)\n");
    exit(1);
  }
  int k = row * m->cols + col;
  m->value[k] = value;
}

// get matrix element col
void gmec_over(Vector *u, Matrix *A, int col) {
  for (int i = 0; i < A->rows; i++) {
    int k = i * A->cols + col;
    u->value[i] = A->value[k];
  }
}

void qr_decomposition(Matrix *A, Matrix *Q, Matrix *R) {
  for (int k = 0; k < A->cols; k++) {

  }
}
