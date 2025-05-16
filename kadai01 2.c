#include <stdio.h>

#define MAX 100

// 関数宣言
void readMatrix(FILE *fp, int A[MAX][MAX], int *rows, int *cols, int *a);
void printTranspose(FILE *fp, int A[MAX][MAX], int rows, int cols);
void printProduct(FILE *fp, int A[MAX][MAX], int rows, int cols);
void printDivision(FILE *fp, int A[MAX][MAX], int rows, int cols, int a);


int main() {
    FILE *infile = fopen("input1.txt", "r");
    FILE *outfile = fopen("output1.txt", "w");//「w」で上書きを許可

    int A[MAX][MAX], rows, cols, a;
    readMatrix(infile, A, &rows, &cols, &a);

    // 結果出力
    fprintf(outfile, "(A^T):\n");
    printTranspose(outfile, A, rows, cols);

    fprintf(outfile, "(A x A^T):\n");
    printProduct(outfile, A, rows, cols);

    fprintf(outfile, "(A / a):\n");
    printDivision(outfile, A, rows, cols, a);

    fclose(infile);
    fclose(outfile);
    return 0;
}

// 行列と除数の読み込み
void readMatrix(FILE *fp, int A[MAX][MAX], int *rows, int *cols, int *a) {
    fscanf(fp, "%d %d", rows, cols);
    for (int i = 0; i < *rows; i++)
        for (int j = 0; j < *cols; j++)
            fscanf(fp, "%d", &A[i][j]);
    fscanf(fp, "%d", a);
}

// 転置の出力
void printTranspose(FILE *fp, int A[MAX][MAX], int rows, int cols) {
    for (int j = 0; j < cols; j++) {
        for (int i = 0; i < rows; i++)
            fprintf(fp, "%d ", A[i][j]);
        fprintf(fp, "\n");
    }
}

// 積 A x A^T の出力（[rows x rows] 行列）
void printProduct(FILE *fp, int A[MAX][MAX], int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < rows; j++) {
            int sum = 0;
            for (int k = 0; k < cols; k++)
                sum += A[i][k] * A[j][k];
            fprintf(fp, "%d ", sum);
        }
        fprintf(fp, "\n");
    }
}

// 除算 A / a の出力
void printDivision(FILE *fp, int A[MAX][MAX], int rows, int cols, int a) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++)
            fprintf(fp, "%.2f ", (float)A[i][j] / a);
        fprintf(fp, "\n");
    }
}
