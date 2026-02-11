#include <stdio.h>
#include <stdlib.h> 

int main(void) {

  int arr[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  int book[10] = {0};
  int select =0;

  while (1) {
    printf("select seat :");
    scanf("%d", &select);

    book[select-1] = 1;

    // 10칸짜리 배열에 1~10까지 저장
    for (int i = 0; i < 10; i++) {
      printf("%d ", arr[i]);
    }
    printf("\n");

    // 10칸짜리 배열에 1~10까지 저장
    for (int i = 0; i < 10; i++) {
      printf("%d ", book[i]);
    }
  }

  printf("\n");

  return 0;
}