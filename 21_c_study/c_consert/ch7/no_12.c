#include <stdio.h>
#include <stdlib.h> 

int main(void) {

  int arr[3] = {1,2,3} ;

  for (int i = 0; i < 3; i++) {
    printf("%d ", arr[i]);
  }
  printf("\n");

  // arr[1] 삭제
  printf("\n arr[1] 삭제 후 \n");
  // arr[3]의 메모리가 사라지지는 않음 -> 무시되는 영역
  for (int i = 1; i < 2; i++) {
    arr[i] = arr[i + 1];
  }
  for (int i = 0; i < 2; i++) {
    printf("%d ", arr[i]);
  }
  printf("\n");
  return 0;
}