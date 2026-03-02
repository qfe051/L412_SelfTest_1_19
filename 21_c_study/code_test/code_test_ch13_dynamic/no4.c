/*
4. [머리 쓰기] 포인터 연산으로 배열 뒤집기
문제: 숫자 n개를 입력받아 동적 배열에 저장한 후,
**추가 배열 없이 포인터 두 개(시작, 끝)**만을 사용하여
배열을 역순으로 뒤집으세요.

포인터에 관해 
*/
#include <stdio.h>
#include <stdlib.h>
int main() {
  int n;
  printf("type number n :");
  scanf("%d", &n);
  int *arr = (int*)malloc(sizeof(int) * n);
  // 1. 숫자 입력 / 2. 포인터 두 개로 swap 하며 중앙으로 이동
  int temp = 0;
  int num =0;
  // num++로 입력 대체 가능 
  for(int i=0 ; i<n; i++){
    // printf("type arr[%d] :",i);
    // scanf("%d", &arr[i]);

    *(arr+i) = num++;
  }


  for(int i=0 ; i<(n/2); i++){


    temp = *(arr+i);
    *(arr+i)=*(arr+n-1-i);
    *(arr+n-1-i)=temp;

    // printf("temp : %d /n",temp);
    // printf(" *(arr+i) : %d /n", *(arr+i));
    // printf("*(arr+n-1-i) : %d /n",*(arr+n-1-i));

  }


  
  // [디버깅 틀] 결과 확인
  printf("뒤집힘: ");
  for(int i=0; i<n; i++) printf("%d ", arr[i]);
  free(arr);
  return 0;
}
