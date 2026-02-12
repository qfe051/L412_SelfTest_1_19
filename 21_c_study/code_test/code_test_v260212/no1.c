#include <stdio.h>

int main() {
  int input = 0;

  printf("input : ");
  scanf("%d",&input);

  printf(" \n ");
  for (int i =1; i <= input; i++) {
    for (int j=0; j <input - i ; j++) {
        printf(" ");
    }
    for (int k=0; k<(i-1)*2+1; k++) {
        printf("%d",i);
    }
    printf(" \n ");
  }
  
}