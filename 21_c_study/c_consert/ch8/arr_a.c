#include <stdio.h>

void show_digit(int n);

int main(void) {

  int input=0 ;
  printf(" 정수 입력 : " );
  scanf("%d",&input);

  show_digit(input);



  
  return 0;
}

void show_digit(int n){
  int a = n % 10;
  int b = n / 10;
  printf("%d  ",a);
  if(b<10){
  return;
  }
  show_digit(b);

  
  

}