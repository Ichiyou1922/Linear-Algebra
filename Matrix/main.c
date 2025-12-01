#include <stdio.h>
#include <stdlib.h>

typedef struct {
  int rows;
  int cols;
  double **data; //m[i][j]と書ける．*dataだとdata[i * cols +j].
} Matrix;

void free_matrix(Matrix *m) {
  for (int i = 0; i < m->rows; i++) {
    free(m->data[i]);
  }
  free(m->data);
  free(m);
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
  new_matrix->data = (double **)malloc(rows * sizeof(double*));
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

int main() {
  Matrix *A = create_matrix(2, 2);
  A->data[0][0] = 1;
  A->data[0][1] = 2;
  A->data[1][0] = 3;
  A->data[1][1] = 4;

  Matrix *B = create_matrix(2, 2);
  B->data[0][0] = 5;
  B->data[0][1] = 6;
  B->data[1][0] = 7;
  B->data[1][1] = 8;

  Matrix *C = mat_mult(A, B);
  print_matrix(C);

  free_matrix(A);
  free_matrix(B);
  free_matrix(C);
  
  return 0;
}
