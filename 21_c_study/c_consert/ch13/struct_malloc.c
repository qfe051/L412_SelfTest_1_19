#include <stdio.h>
#include <stdlib.h>
#include <string.h>


 // Book 이라는 구조체 정의
struct Book{
  int number;
  char title[100];
};
  
int main(void) {

  struct Book *p;

  // 여기 해석 잘하기
  p = (struct Book *)malloc(2 * sizeof(struct Book));

  // c 캐스팅 없는 과정
  p = malloc(2 * sizeof(struct Book));

  if (p==NULL){
    printf("Memory ERROR \n");

    return 1;
  }
 
  p[0].number = 1;
  strcpy(p[0].title, "c proj");

  p[1].number = 2;
  strcpy(p[1].title, "data");

  free(p);
  
  return 0;
}
