#include <stdio.h>

typedef enum num_cmd {
    SET = 1 ,
    GET = 2 ,
    RESET = 3,
} command;

void handle_cmd(command cmd){
    switch (cmd)
    {
    case SET:
        printf("Set \r\n");
        // break;
    case GET:
        printf("Get \r\n");
        // break;
    case RESET:
        printf("ReSet \r\n");
        // break;    
    default:
        printf("ACK 전송 \r\n");
        break;
    }
}

int main(void){

    handle_cmd(GET);
    
}