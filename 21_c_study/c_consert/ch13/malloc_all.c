#include <stdio.h>
#include <stdlib.h>
#include <string.h>


 // Book 이라는 구조체 정의
struct Book{
  int number;
  char title[100];
};
  
int main(void) {

  printf("0) \n");

  int *a, b, *c;
  printf("b is not pointer \n");

  // malloc은 주소를 반환
  printf("1)\n");
  int *q = NULL; // 포인터를 NULL로 선언
  q = malloc(10*sizeof(*q));

  if (q==NULL){
    printf("Memory ERROR \n");
    return 1;
  }
  else {
    printf("Memory is allocated \n");
  }
  printf("q(address) = %p \n", q);
  printf("&q[0]=%p , &q[1]=%p \n", &q[0], &q[1]);
  q[0] = 10;
  q[1] = 20;
  printf("q[0]=%d , q[1]=%d \n", q[0], q[1]);

  printf("4)\n");
  const char *s = "Hello";
  char *pstr = malloc(strlen(s) + 1);
  if (pstr == NULL) {
    printf("str malloc ERR \n");
    free(q);
    return 1;
  }
  // 주소로 문자열 복사
  strcpy(pstr, s);

  printf("s = %s (length = %zu)\n", s, strlen(s));
  printf("pstr = %s (length = %zu)\n", pstr, strlen(pstr));

  printf("5)\n");
  struct Book *pbook = malloc(2 * sizeof(*pbook));
  if (pbook == NULL) {
    printf("book malloc ERR\n");
    free(q);
    free(pstr);
    return 1;
  }
  else {
    printf("pbook Memory is allocated \n");
  }

  printf("pbook(address)  = %p \n", pbook);
  printf("&pbook[0]  = %p \n", &pbook[0]);
  printf("&pbook[1]  (move size of (Struct Book))= %p \n", &pbook[1]);
  printf("pbook[0].number = %d \n", pbook[0].number);

  pbook[0].number = 1;
  strcpy(pbook[0].title, "c proj");

  pbook[1].number = 2;
  strcpy(pbook[1].title, "data");

  printf("pbook[0].number = %d \n", pbook[0].number);
  printf("pbook[0].title = %s \n", pbook[0].title);

  printf("pbook[1].number = %d \n", pbook[1].number);
  printf("pbook[1].title = %s \n", pbook[1].title);

  printf("6)\n");
  int rows = 3, cols = 4;
  int *mat = malloc(rows * cols * sizeof(mat));
  if (mat == NULL) {
    printf("mat malloc ERR\n");
    free(q);
    free(pstr);
    free(pbook);
    return 1;
  }
  else {
    printf("mat Memory is allocated \n");
  }

  for (int i=0; i<rows; i++) {
    for (int j=0; j<cols; j++) {
      mat[i*cols + j] = i * 100 + j;
    }
  }
  printf("mat[2][3] = %d (mat[2*cols+3])\n", mat[2 * cols + 3]);

  printf("7) free () \n");
  free(q);
  q = NULL;
  free(pstr);
  pstr = NULL;
  free(pbook);
  pbook=NULL;
  free(mat);
  mat = NULL;

  printf("after free\n");
  printf("q=%p \n pstr = %p \n pbook= %p \n mat=%p \n", q, pstr, pbook, mat);

  return 0;
}
