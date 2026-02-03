#include <stdio.h>

int main(void){
    char c;

    // getchar() 기본 내장 함수 
    printf("문자 입력 : ");
    c = getchar();

    printf(" 문자 : %c",c);
    printf(" 아스키 :%d",c); 
}