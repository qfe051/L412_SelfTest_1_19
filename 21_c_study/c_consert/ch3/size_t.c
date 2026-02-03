#include <stdio.h>

int main(void){

    printf(" sizeof(char) : %zu \r\n",sizeof(char));
    printf(" sizeof(short) :%lu \r\n",sizeof(short));
    printf(" sizeof(int) : %zu \r\n",sizeof(int)); 
    // 컴파일러 마다 long 크기가 다른지 확인하기
    printf(" sizeof(long) : %zu \r\n",sizeof(long));
    printf(" sizeof(long long) : %zu \r\n",sizeof(long long));
    printf(" sizeof(float) : %zu \r\n",sizeof(float)); 
    printf(" sizeof(double) : %zu \r\n",sizeof(double)); 
    printf(" sizeof(long double) : %zu \r\n",sizeof(long double)); 
    
}