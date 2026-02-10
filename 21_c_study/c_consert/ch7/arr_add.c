#include <stdio.h>
#include <stdlib.h> 

int main(void) {

  int a[3][3] = {{1, 0, 0}, {0, 1, 0}, {0, 0, 1}};
  int b[3][3] = {{1, 0, 0}, {0, 1, 0}, {0, 0, 1}};
  int c[3][3] = {0};
  
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      c[i][j] = a[i][j] + b[i][j];
    }
  }

  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      printf("%d ", c[i][j]);
    }
    printf("\n");
  }
  
  return 0;
}