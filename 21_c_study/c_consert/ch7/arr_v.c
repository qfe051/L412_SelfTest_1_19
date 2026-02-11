#include <stdio.h>
#include <stdlib.h> 

int main(void) {

  int arr[5] = {0};
  int sum = 0;
  int avg = 0;
  int v = 0;

  for (int i = 0; i < 5; i++) {
    arr[i] = rand() % 10;
    printf("arr[%d] : %d ", i, arr[i]);
    sum += arr[i];
  }
  printf("\n");
  avg = sum / 5;

  printf("sum : %d \n", sum);
  printf("avg : %d \n", avg);

  for (int i = 0; i < 5; i++) {
    v = (arr[i] - avg) * (arr[i] - avg);
  }
  v = v / 5;

  printf("v : %d \n", v);

  printf("\n");

  return 0;
}