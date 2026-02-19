/*
3. 최대값과 최소값 동시에 찾기 (다중 반환)
• 문제: void findMaxMin(int *arr, int size, int *max, int *min)
함수를 만드세요.
배열을 입력받아 가장 큰 값과 가장 작은 값을 찾은 뒤,
포인터를 통해 메인 함수의 변수에 그 값을 직접 저장하세요.

*/
#include <stdio.h>

void findMaxMin(int *arr, int size, int *max, int *min) {

  // 최소값, 최대값 기준 설정
  *min = *arr;
  *max = *arr;
  for (int i = 0; i <size; i++) {
    if (*(arr + i) < *min) {
      *min = *(arr + i);
    }
    if (*(arr + i)>*max) {
      *max = *(arr + i);
    }
  }
  
}


int main() {
  

  int value_max = 0;
  int value_min = 0;
  // int value_size = 5;

  int arr[5] = {10, 20, 30, 40, 50};

  findMaxMin(arr, 5, &value_max, &value_min);


  printf("value_max = %d \n", value_max);
  printf("value_min = %d \n", value_min);


}
