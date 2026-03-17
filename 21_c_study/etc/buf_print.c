#include <stdio.h>
#include <string.h>

uint8_t rxData;

uint8_t buf0[100];
uint8_t buf1[100];

uint8_t active_buf_flag = 0;   // 현재 저장 버퍼: 0 -> buf0, 1 -> buf1
uint8_t buf_index = 0;        // 현재 저장 위치
uint8_t buf0_print_flag = 0;
uint8_t buf1_print_flag = 0;    

void callback(void){
    if (active_buf_flag == 0){
        buf0[buf_index] = rxData ;
        buf_index++;
        if (rxData=='\n' | buf_index>=99){
           buf0[buf_index-1] = '\0';
           active_buf_flag = (active_buf_flag+1) % 2 ;
            buf_index = 0;
            buf0_print_flag = 1; 
        }
        
    }
    else{
        buf1[buf_index] = rxData ;
        buf_index++;
        if (rxData=='\n' | buf_index>=99){
           buf1[buf_index-1] = '\0';
           active_buf_flag = (active_buf_flag+1) % 2 ;
            buf_index = 0;
            buf1_print_flag = 1;
    }
    
}

int main(void){
    while (1)
    {
        if (buf0_print_flag){
            printf("[] buf0 : %s \r\n",buf0);
            buf0_print_flag = 0;
        }
        else if (buf1_print_flag){
            printf("[] buf0 : %s \r\n",buf0);
            buf1_print_flag = 0;
        }
        
    }
    
}