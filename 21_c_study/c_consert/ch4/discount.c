#include <stdio.h>

int main(void){

    
    int a = 20000;
    float b = 10.0;

    float c = a - a*(b*(0.01));
    float d = a - a*(b*(1/100));

    printf("c : %.2f \r\n",c);
    printf("d : %.2f \r\n",d);
    
    
}