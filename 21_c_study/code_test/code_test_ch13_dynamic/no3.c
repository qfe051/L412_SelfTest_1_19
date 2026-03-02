/*
3. 2차원 행렬의 전치(Transpose)
문제: M x N 2차원 동적 배열을 생성하고,
행과 열이 바뀐 N x M 전치 행렬을
새로 만들어 값을 옮기세요.

*/
#include <stdio.h>
#include <stdlib.h>
int main() {
    // int r = 2, c = 3;
    // 1. 2x3 할당 및 값 채우기 / 2. 3x2 할당 및 값 이동
    int *matrix[2];
    int num = 0;

    // malloc 각 열마다 실행
    for ( int i = 0; i < 2; i++)
    {
      matrix[i]=malloc(3*sizeof(int));
    }
    

    printf("Before Transpose : \n");

    for (int a = 0; a<2; a++) {
      for (int b = 0; b < 3; b++) {
        num++;
        matrix[a][b] = num;
        printf("[%d],[%d] = %d ",a,b, matrix[a][b]);
      }
      printf("\n");
    }

    int *matrix_t[3];

    for ( int i = 0; i < 3; i++)
    {
      matrix_t[i]=malloc(2*sizeof(int));
    }


    // Transpose
    
    for (int a=0; a<2 ; a++) {
      for (int b = 0; b<3; b++) {
        matrix_t[b][a]= matrix[a][b];
        
      }
      printf("\n");
    }

    
    // [디버깅 틀] 3행 2열로 출력되는지 확인
    printf("Transpose Result : \n");

    for (int a = 0; a < 3; a++) {
      for (int b = 0; b<2; b++) {
        printf("[%d],[%d] = %d ",a,b, matrix_t[a][b]);
      }
      printf("\n");
    }

    // 3. 모든 메모리 free
    // matrix free
    for ( int i = 0; i < 2; i++){
      free(matrix[i]);
    }

    for ( int i = 0; i < 3; i++)
    {
      free(matrix_t[i]);
    }

    return 0;
}
