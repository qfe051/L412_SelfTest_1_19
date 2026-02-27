/*
3. 2차원 행렬의 전치(Transpose)
문제: M x N 2차원 동적 배열을 생성하고,
행과 열이 바뀐 N x M 전치 행렬을
새로 만들어 값을 옮기세요.

*/
#include <stdio.h>
#include <stdlib.h>
int main() {
    int r = 2, c = 3;
    // 1. 2x3 할당 및 값 채우기 / 2. 3x2 할당 및 값 이동
    int *matrix = malloc(r * c * sizeof(matrix));

    if (matrix ==NULL) {

      printf("ERROR : \n");
      free(matrix);

    }
    printf("Before Transpose : \n");

    for (int b = 0; b<r; b++) {
      for (int a = 0; a < c; a++) {
        matrix[0] = a+b;
        printf("[%d],[%d] = %d ",a,b, matrix[a][b]);
      }
      printf("\n");
    }

    int *matrix_t = malloc(r * c * sizeof(*matrix));
    // Transpose
    for (int b = 0; b<r; b++) {
      for (int a=0; a<c; a++) {
        matrix_t[a][b]= matrix[b][a];
      }

    }

    
    // [디버깅 틀] 3행 2열로 출력되는지 확인
    printf("Transpose Result : \n");
    // 3. 모든 메모리 free
    return 0;
}
