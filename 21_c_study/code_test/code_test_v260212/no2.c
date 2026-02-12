#include <stdio.h>

float input[7] = {0.0};
float sum, avg, max = 0.0;

int main() {

  for (int i=0; i < 7; i++) {
    printf("day [%d] temp : ", i);
    scanf("%f", &input[i]);
    sum = sum + input[i];
  }

  avg = sum / 7.0;

  for (int i = 0; i < 7; i++) {
    if (max < input[i]) {
      max = input[i];
    }
  }
  printf("\n");
  printf("1)temp avg : %.2f \n", avg);
  printf("2)temp max : %.2f \n", max);
  printf("3)\n");

  for (int i = 0; i < 7; i++) {
    if (avg < input[i]) {
      max = input[i];
      printf("day [%d] over avg : %.2f  \n", i,input[i] );
    }
  }

  
}