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
  p = malloc(2 * sizeof(struct Book));

  int *q;
  q = malloc(10*sizeof(int));

  int *r = malloc(10*sizeof(int);

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
