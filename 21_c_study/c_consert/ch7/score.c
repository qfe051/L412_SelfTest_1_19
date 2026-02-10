#include <stdio.h>
#include <stdlib.h> 

int main(void) {

  int score[5] ;
  int avg, sum = 0;
  int a=0;

  for (int i = 0; i < 5; i++) {
    score[i] = rand() % 100;
    sum += score[i];
    printf("score[%d] : %d \r\n", a =i+1, score[i]);
  }
  avg = sum / 5;
  printf("avg : %d \r\n", avg);
  
  return 0;
}