#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct {
  int rows;
  int cols;
  double *value;
} Matrix;

typedef struct {
  int dim;
  double *value;
} Vector;

void free_matrix(Matrix *m) {
  if (m->value != NULL) {
    free(m->value);
  }
  free(m);
}

void free_vector(Vector *v) {
  if (v->value != NULL) {
    free(v->value);
  }
  free(v);
}

Matrix *create_matrix(int rows, int cols) {
  Matrix *m = (Matrix *)malloc(sizeof(Matrix));
  if (m == NULL) {
    printf("Failed to allocate Matrix memory\n");
    exit(1);
  }
  m->rows = rows;
  m->cols = cols;
  m->value = (double *)calloc(rows * cols, sizeof(double));
  if (m->value == NULL) {
    printf("Failed to allocate Matrix value memory\n");
    free_matrix(m);
    exit(1);
  }
  return m;
}

Vector *create_vector(int dim) {
  Vector *v = (Vector *)malloc(sizeof(Vector));
  if (v == NULL) {
    printf("Failed to allocate Vector memory\n");
    exit(1);
  }
  v->dim = dim;
  v->value = (double *)calloc(dim, sizeof(double));
  if (v->value == NULL) {
    printf("Failed to allocate Vector value memory\n");
    free_vector(v);
    exit(1);
  }
  return v;
}

// set matrix element
void sme(Matrix *m, int row, int col, double value) {
  if (row < 0 || row > m->rows || col < 0 || col > m->cols) {
    printf("Value Error(sme)");
    exit(1);
  }
  int k = row * m->cols + col;
  m->value[k] = value;
}

// set vector element
void sve(Vector *v, int index, double value) {
  if (index < 0 || index > v->dim) {
    printf("Value Error(sve)");
    exit(1);
  }
  v->value[index] = value;
}

// get matrix element
double gme(Matrix *m, int row, int col) {
  if (row < 0 || row > m->rows || col < 0 || col > m->cols) {
    printf("Value Error(gme)");
    exit(1);
  }
  int k = row * m->cols + col;
  double ans = m->value[k];
  return ans;
}

// get vector element
double gve(Vector *v, int index) {
  if (index < 0 || index > v->dim) {
    printf("Value Error(gve)");
    exit(1);
  }
  double ans = v->value[index];
  return ans;
}

// 行列ベクトル積 
Vector *mat_vec_mult(Matrix *A, Vector *v) {
  if (A->cols != v->dim) {
    printf("Different size(mat_vec)\n");
    exit(1);
  }
  Vector *ans = create_vector(v->dim);
  for (int i = 0; i < A->rows; i++) {
    for (int j = 0; i< A->cols; j++) {
      ans->value[i] += gme(A, i, j) * gve(v, j);
    }
  }
  return ans;
}

// 行列積
Matrix *mat_mult(Matrix *A, Matrix *B) {
  if (A->cols != B->rows) {
    printf("Different size of Matrixes");
    exit(1);
  }

  Matrix *C = create_matrix(A->rows, B->cols);
  for (int i = 0; i < C->rows; i++) {
    for (int j = 0; j < C->cols; j++) {
      double tmp_ans = 0;
      for (int k = 0; k < A->cols; k++) {
        tmp_ans += gme(A, i, k) * gme(B, k, j);
      }
      sme(C, i, j, tmp_ans);
    }
  }
  return C;
}
// ノルム計算
double norm(Vector *v) {
  double tmp_ans = 0;
  for (int i =  0; i < v->dim; i++) {
    tmp_ans += v->value[i] * v->value[i];
  }
  double ans = sqrt(tmp_ans);
  return ans;
}

// べき乗法power method
double power_method(Matrix *A, int max_iter, double epsilon) {
  Vector *v = create_vector(A->cols);
  for (int index = 0; index < v->dim; index++) {
    v->value[index] = 1;
  }

  Vector *w = create_vector(v->dim);
  double mu = 0;
  
  for (int k = 0; k < max_iter; k++) {
    
  }

}

