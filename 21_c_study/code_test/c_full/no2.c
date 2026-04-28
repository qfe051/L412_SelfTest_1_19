/*
[문제 2] 가변형 2차원 데이터 '페이지 관리자'
문제: 행(Row)과 열(Column)의 크기가 제각각인 2차원 데이터를
메모리 낭비 없이 관리하는 시스템을 만드세요.
1. 중첩 할당: 전체 페이지 수(M)를 입력받고,
각 페이지마다 저장할 데이터 개수(N_i)를
각각 다르게 입력받아 메모리를 할당하세요.
2. 데이터 조작: 특정 페이지의 특정 인덱스 값을
실시간으로 수정하고 조회하는 기능을 구현하세요.
3. 무한 루프: 사용자가 exit를 입력할 때까지
페이지 생성, 값 수정, 전체 구조 조회를 반복하세요.
4. 제약: 2차원 포인터(int **)를 필수 사용하며,
메모리 해제 시 반드시 가장 깊은 곳부터 역순으로 해제해야 함.

*/

#include <stdio.h>
#include <stdlib.h>

// 배열의 가로 크기 지정하는 이유는?
// 정적메모리 2차원배열 예시 작성하기
void _sta_print_2D_Array(int arr[][5], int col, int row) {
  for (int i = 0; i < row; i++) {
    for (int j = 0; j < col; j++) {
      printf("%d ", arr[i][j]);
    }
    printf("\n");
  }
}

int main() {
  int page;
  // int data_size[] ;
  int row, col ;

  printf("set page number : \n");
  scanf("%d", &page);

  int data_size[page] ;
  
  // row, col 세팅
  for (int i =0; i<page; i++) {
    printf("set row number : \n");
    scanf("%d", &row);

    printf("set col number : \n");
    scanf("%d", &col);

    data_size[i] = row * col;

    int *data_i = malloc(data_size[i] * sizeof(int));
  }




  


  // 4. free 필수
  free();

  return 0;
}
