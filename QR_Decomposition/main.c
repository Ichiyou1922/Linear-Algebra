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

  v->dim = dim;
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

void sme(Matrix *m, int row, int col, double value) {
  if (row < 0 || row >= m->rows || col < 0 || col >= m->cols) {
    printf("Value Error(sme)\n");
    exit(1);
  }
  int k = row * m->cols + col;
  m->value[k] = value;
}

// set matrix col
void smc(Matrix *Q, int j, Vector *q) {
  if (Q->rows != q->dim) {
    printf("Failed to put q to Q\n");
    exit(1);
  }
  for (int i = 0; i < Q->rows; i++) {
    sme(Q, i, j, q->value[i]);
  }
}

// get matrix col
void gmec_over(Vector *u, Matrix *A, int col) {
  for (int i = 0; i < A->rows; i++) {
    int k = i * A->cols + col;
    u->value[i] = A->value[k];
  }
}

// get matrix element
double gme(Matrix *A, int row, int col) {
  double ans = 0;
  int k = row * A->cols + col;
  ans = A->value[k];
  return ans;
}  

double inner_product(Vector *q, Vector *a) {
  if (q->dim != a->dim) {
    printf("Different size of Vector\n");
    exit(1);
  }
  double ans = 0;
  for (int i = 0; i < q->dim; i++) {
    ans += q->value[i] * a->value[i];
  }
  return ans;
}

double norm(Vector *u) {
  double ans = 0;
  for (int i = 0; i < u->dim; i++) {
    ans += u->value[i] * u->value[i];
  }
  return sqrt(ans);
}

Matrix *mat_mult(Matrix *A, Matrix *B) {
  if (A->cols != B->rows) {
    printf("Different size of Matrix\n");
    exit(1);
  }
  Matrix *ans = create_matrix(A->rows, B->cols);
  for (int i = 0; i < ans->rows; i++) {
    for (int j = 0; j < ans->cols; j++) {
      for (int k = 0; k < A->cols; k++) {
        int a = i * A->cols + k;
        int b = k * B->cols + j;
        int c = i * ans->cols + j;

        ans->value[c] += A->value[a] * B->value[b];
      }
    }
  }
  return ans;
}

void qr_decomposition(Matrix *A, Matrix *Q, Matrix *R) {
  int n = A->rows;
  int m = A->cols;

  Vector *u = create_vector(n);
  Vector *q_col = create_vector(n);
  double r = 0;

  for (int k = 0; k < A->cols; k++) {
    gmec_over(u, A, k);
    
    for (int j = 0; j < k; j++) {
      gmec_over(q_col, Q, j);
      double r = inner_product(q_col, u);
      sme(R, j, k, r);

      for (int i = 0; i < n; i++) {
        u->value[i] -= r * q_col->value[i];
      }
    }
    double u_norm = norm(u);
    sme(R, k, k, u_norm);

    for (int i = 0; i < n; i++) {
      sme(Q, i, k, u->value[i] / u_norm);
    }
  }
  free_vector(u);
  free_vector(q_col);
}

void print_matrix(Matrix *A) {
  for (int i = 0; i < A->rows; i++) {
    for (int j = 0; j < A->cols; j++) {
      int k = i * A->cols + j;
      printf("%f  ", A->value[k]);
      if (j == A->cols - 1) {
        printf("\n");
      }
    }
  }
}

// get inversed Matrix
Matrix *transpose(Matrix *m) {
  Matrix *inv = create_matrix(m->rows, m->cols);
  for (int i = 0; i < m->rows; i++) {
    for (int j = 0; j < m->cols; j++) {
      int k = i * m->cols + j;
      int l = j * inv->cols + i;
      inv->value[l] = m->value[k];
    }
  }
  return inv;
}

int main() {
  Matrix *A = create_matrix(3, 3);
  sme(A, 0, 0, 1);
  sme(A, 0, 1, 1);
  sme(A, 0, 2, 0);
  sme(A, 1, 0, 1);
  sme(A, 1, 1, 0);
  sme(A, 1, 2, 1);
  sme(A, 2, 0, 0);
  sme(A, 2, 1, 1);
  sme(A, 2, 2, 1);
  
  Matrix *Q = create_matrix(3, 3);
  Matrix *R = create_matrix(3, 3);

  qr_decomposition(A, Q, R);
  printf("Q\n");
  print_matrix(Q);
  printf("R\n");
  print_matrix(R);

  Matrix *QR = mat_mult(Q, R);
  printf("QR\n");
  print_matrix(QR);
  
  Matrix *QT = transpose(Q);
  Matrix *QTQ = mat_mult(QT, Q);
  printf("QTQ\n");
  print_matrix(QTQ);
  
  free_matrix(A);
  free_matrix(Q);
  free_matrix(R);
  free_matrix(QR);
  free_matrix(QT);
  free_matrix(QTQ);
  return 0;
}
