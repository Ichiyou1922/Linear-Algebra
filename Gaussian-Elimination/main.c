#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct {
  int rows;
  int cols;
  double **data; //m[i][j]と書ける．*dataだとdata[i * cols +j].
} Matrix;

typedef struct {
  int dim;
  double *value;
} Vector;

void free_matrix(Matrix *m) {
  for (int i = 0; i < m->rows; i++) {
    free(m->data[i]);
  }
  free(m->data);
  free(m);
}

void free_vector(Vector *v) {
  if (v->value != NULL) {
    free(v->value);
  }
  free(v);
}

Matrix *create_matrix(int rows, int cols) { //返す構造体は1つ-> *は1つ
  // Matrix * 型１つ分malloc
  Matrix *new_matrix = (Matrix *)malloc(sizeof(Matrix));
  if (new_matrix == NULL) {
    printf("Failed to allocate structure memory\n");
    exit(1);
  }
  
  new_matrix->rows = rows;
  new_matrix->cols = cols;

  // 行を作る．double * 型がrows個入る配列をmalloc(確保)
  new_matrix->data = (double **)calloc(rows, rows * sizeof(double*));
  if (new_matrix->data == NULL) {
    free_matrix(new_matrix);
    printf("Failed to allocate rows memory\n");
  }
  // 列を作る．double 型がcols個入る配列をcalloc(確保して初期化)
  for (int i = 0; i < rows; i++) {
    new_matrix->data[i] = (double *)calloc(cols, sizeof(double));
    if (new_matrix->data[i] == NULL) {
      printf("Failed to allocate cols memory\n");
      free_matrix(new_matrix);
    }
  }

  
  return new_matrix;
}

Vector *create_vector(int dim) {
  Vector *v = (Vector *)malloc(sizeof(Vector));
  if (v == NULL) {
    printf("Failed to allocate structure memory\n");
    exit(1);
  }
  
  v->dim = dim;
  v->value = (double *)calloc(dim, sizeof(double));
  if (v->value == NULL) {
    printf("Failed to allocate value memory\n");
    free_vector(v);
  }
  return v;
}

void print_matrix(Matrix *m) {
  for (int i = 0; i < m->rows; i++) {
    for (int j = 0; j < m->cols; j++) {
      printf("%.2f ", m->data[i][j]);
      if (j == m->cols - 1) {
        printf("\n");
      }
    }
  }
}

Matrix *mat_mult(Matrix *A, Matrix *B) {
  if (A->cols != B->rows) {
    printf("Different size of Matrixes");
    exit(1);
  }
  
  Matrix *C = create_matrix(A->rows, B->cols);
  for (int i = 0; i < C->rows; i++) {
    for (int j = 0; j < C->cols; j ++) {
      for (int k = 0; k < A->cols; k++) {
        C->data[i][j] += A->data[i][k] * B->data[k][j];
      }
    }
  }
  return C;
}

// ガウスの消去法(部分ピボット選択付き)
Vector *solve_system(Matrix *A, Vector *b) {
  int n = A->rows;
  Vector *x = create_vector(n);

  for (int k = 0; k < n; k++) {

    //ピボット選択: k行目以降で，k列目の絶対値が最大の行を探す．
    int max_row = k;
    double max_val = fabs(A->data[k][k]);

    for (int i = k + 1; i < n; i++) {
      if (fabs(A->data[i][k]) > max_val) {
        max_val = fabs(A->data[i][k]);
        max_row = i;
      }
    }

    if (max_val < 1e-10) {
      printf("Error: Singular matrix\n");
      exit(1);
    }

    if(max_row != k) {
      //Aの行(ポインタ)を交換
      double *temp_ptr = A->data[k];
      A->data[k] = A->data[max_row];
      A->data[max_row] = temp_ptr;

      double temp_val = b->value[k];
      b->value[k] = b->value[max_row];
      b->value[max_row] = temp_val;
    }

    for (int i = k + 1; i < n; i++) {
      //倍率を計算
      double factor = A->data[i][k] / A->data[k][k];

      for (int j = k; j < n; j++) {
        A->data[i][j] -= factor * A->data[k][j];
      }
      b->value[i] -= factor * b->value[k];
    }
  }
  for (int i = n - 1; i >= 0; i--) {
    double sum = 0.0;
    for (int j = i + 1; j < n; j++) {
      sum += A->data[i][j] * x->value[j];
    }
    x->value[i] = (b->value[i] - sum) / A->data[i][i];
  }
  return x;
}

int main() {
  Matrix *A = create_matrix(3, 3);
  A->data[0][0] = 3;
  A->data[0][1] = 2;
  A->data[0][2] = -1;
  A->data[1][0] = 2;
  A->data[1][1] = -2;
  A->data[1][2] = 4;
  A->data[2][0] = -1;
  A->data[2][1] = 0.5;
  A->data[2][2] = -1;
  
  Vector *b = create_vector(3);
  b->value[0] = 1;
  b->value[1] = -2;
  b->value[2] = 0;

  Vector *x = solve_system(A, b);
  for (int i = 0; i < 3; i++) {
    printf("%.2f\n", x->value[i]);
  }
  free_matrix(A);
  free_vector(b);
  free_vector(x);
  
  return 0;
}
