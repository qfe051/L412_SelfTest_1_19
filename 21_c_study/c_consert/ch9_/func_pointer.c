#include <stdio.h>
#include <string.h>

int main(void) {

  FILE *fp;
  char str[100];

  fp = fopen("test.txt", "w");

  if (fp == NULL) {
    printf("wrong file \n");
    return 1;
  }

  do {
    gets_s(str, sizeof(str));
    fputs(str, fp);
  } while (strlen(str) != 0);

  fclose(fp);

  return 0;
}