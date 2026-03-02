/*
5. [머리 쓰기] 짝수만 골라내는 '동적 필터'
문제: 배열에서 짝수만 골라
딱 맞는 크기의 새로운 동적 배열에 담아
반환하는 함수를 만드세요.

*/
#include <stdio.h>
#include <stdlib.h>
int* filterEvens(int *arr, int size, int *newSize) {
    // 1. 짝수 개수 세기 -> malloc -> 짝수 복사 -> 주소 반환
    int even_cnt=0;

    for (int i = 0; i < size; i++)
    {
      if((arr[i]%2)==0){
        (*newSize)++;
      }
    }
    printf("(*newSize) : %d \n",(*newSize));

    int* even_arr = malloc((*newSize)*sizeof(int));




    for (int i = 0; i < size; i++)
    {
      if((arr[i]%2)==0){
        
        even_arr[even_cnt] = arr[i];
        printf("even_arr[%d] : %d \n",even_cnt,even_arr[even_cnt]);
        even_cnt++;
        // printf("arr[%d] : %d \n",i,arr[i]);
        
      }
    }

    printf("even_arr : %zu \n",even_arr);
    
  return even_arr;
}
int main() {
  int data[] = {1, 2, 3, 4, 5, 6};
  int count = 0;
  int *evens = filterEvens(data, 6, &count);
  // [디버깅 틀] 확인
  printf("개수: %d, 데이터: ", count);
  
  for(int i=0; i<count; i++) printf("%d ", evens[i]);

  printf("evens : %zu \n",evens);
  if (evens) {
    free(evens);
  }

  return 0;
}