#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>

uint8_t rxData;

uint8_t buf0[100];
uint8_t buf1[100];

uint8_t active_buf_flag = 0;   // 현재 저장 버퍼: 0 -> buf0, 1 -> buf1
uint8_t buf_index = 0;         // 현재 저장 위치
uint8_t buf0_print_flag = 0;
uint8_t buf1_print_flag = 0;

// ===== 네 기존 callback 구조 유지 =====
void callback(void)
{
    if (active_buf_flag == 0) {
        buf0[buf_index] = rxData;
        buf_index++;

        if (rxData == '\n' || buf_index >= 99) {
            buf0[buf_index - 1] = '\0';

            // 네 의도대로 flag 전환 (단, 실제 토글은 수정 필요)
            active_buf_flag = (active_buf_flag+1) /2 ;
            // active_buf_flag ^= 1;

            buf_index = 0;
            buf0_print_flag = 1;
        }
    }
    else {
        buf1[buf_index] = rxData;
        buf_index++;

        if (rxData == '\n' || buf_index >= 99) {
            buf1[buf_index - 1] = '\0';

            // 네 의도대로 flag 전환
            active_buf_flag = (active_buf_flag+1) % 2 ;
            // active_buf_flag ^= 1;

            buf_index = 0;
            buf1_print_flag = 1;
        }
    }
}

// ===== 내가 맡는 부분: 랜덤 데이터 생성 =====
uint8_t make_random_rxData(void)
{
    int r = rand() % 15;

    // 15번 중 1번은 '\n'
    if (r == 0) {
        return '\n';
    }

    // 나머지는 대문자 알파벳
    return 'A' + (rand() % 26);
}

int main(void)
{
    srand((unsigned int)time(NULL));

    // 초기화
    buf0[0] = '\0';
    buf1[0] = '\0';

    printf("=== random rxData test start ===\n\n");

    // ===== 내가 맡는 부분: 테스트용 데이터 생성 + callback 호출 =====
    for (int i = 0; i < 200; i++) {
        rxData = make_random_rxData();

        // // 디버깅용 입력 확인
        // if (rxData == '\n') {
        //     printf("[RX] \\n\n");
        // } else {
        //     printf("[RX] %c\n", rxData);
        // }

        callback();

        // ===== 네 기존 출력 방식 유지 =====
        if (buf0_print_flag) {
            printf("[] buf0 : %s\n", buf0);
            buf0_print_flag = 0;
        }
        else if (buf1_print_flag) {
            printf("[] buf1 : %s\n", buf1);
            buf1_print_flag = 0;
        }
    }

    // // 마지막에 남은 데이터 확인용 (선택)
    // if (buf_index > 0) {
    //     if (active_buf_flag == 0) {
    //         buf0[buf_index] = '\0';
    //         printf("[FINAL] buf0 : %s\n", buf0);
    //     } else {
    //         buf1[buf_index] = '\0';
    //         printf("[FINAL] buf1 : %s\n", buf1);
    //     }
    // }

    printf("\n=== test end ===\n");

    return 0;
}