#include <stdio.h>


int main(void){

    int err_sensor = 0;
    int err_motor  = 1;
    int err_comm   = 0;

    if ( err_comm | err_motor | err_sensor ) {
        printf("에러 발생\n");
    }
}