#include <stdio.h>


int main(void){

    int x = 0;
    int y = -1;

    if ( x && y ) {
        printf("A\n");
    } else {
        printf("B\n");
    }

    if ( y ) {
        printf("-1도 참 \n");
    } else {
    }

}