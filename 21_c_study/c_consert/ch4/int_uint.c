#include <stdio.h>

int main(void){

    
    u_int8_t a = 200;
    u_int8_t b = 100;

    u_int8_t c = a + b;
    int8_t d = a + b;
    u_int16_t c_16 = a + b;
    int16_t d_16 = a + b;

    printf("c : %d \r\n",c);
    printf("d : %d \r\n",d);

    printf("c_16 : %d \r\n",c_16);
    printf("d_16 : %d \r\n",d_16);
    
}