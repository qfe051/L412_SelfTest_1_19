#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

int main(void) {

  int list[10] ;

  for (int i = 0; i < 10; i++) {
    list[i] = rand() % 100;
  }
  system("cls");

  for (int i=0; i<10; i++) {
    for (int j = 0; j < list[i]/10; j++) {
      printf("*");
    }
    printf("\n");
  }
  system("cls");

  // 중첩하여 비교
  for (int k=0; k<9; k++) {
    for (int i = 0; i < 9; i++) {
      if (list[i] > list[i + 1]) {
        int tmp = list[i];
        list[i] = list[i + 1];
        list[i + 1] = tmp;
      }
    }
  }

  for (int i = 0; i < 10; i++) {
    for (int j = 0; j < list[i] / 10; j++) {
      printf("*");
    }
    printf("\n");
  }
  return 0;
}