#include <stdio.h>
#include <stdint.h>

//구조체 선언
//내부에 어떤 데이터 있는지만 적기! 
struct Point
{
    int x;
    int y;
};

struct Data
{
    int a;
    int b;
};

struct Data g_data = {1,2}; //전역 

struct S1
{
    uint8_t a;
    uint32_t b;
    uint16_t c;
};

struct S2
{
    uint32_t b;
    uint16_t c;
    uint8_t a;
};
struct S
{
    int a;
};

void by_value(struct S s){
    s.a = 100;
}

void by_ptr(struct S *s){
    s->a = 200;
}

union U {
    uint32_t u32;
    uint8_t u8[4];
};


int main(void){
    printf("struct study \r\n");

    union U u;
    u.u32 = 0x11223344;

    //hex, bin 값들 출력
    printf("u32 = 0x%08X\n",u.u32);
    //u8[4] 에 데이터 넣는 과정 
    printf("bytes = %02X %02X %02X %02X \n", u.u8[0],u.u8[1],u.u8[2],u.u8[3]);

    //시프트 연산 
    uint8_t x = 0xB0;
    printf("x >> 4 = 0x%02X\n", x >> 4);

    // struct S x = {10};

    // // 구조체를 포인터로 넘기는 이유?
    // // x가 어느 자리에서 연산으로 쓰이는지?
    // by_value(x);
    // printf("by_value : %d \n", x.a);

    // by_ptr(&x);
    // printf("by_ptr : %d \n", x.a);



    // printf("sizeof(S1) = %zu \n",sizeof(struct S1));
    // printf("sizeof(S2) = %zu \n",sizeof(struct S2));

    // struct Data l_data = {3,4}; //지역 

    // // 주소값 위치 멀리있는 것만 확인하면 끝?
    // // (void*)&g_data -> 데이터 출력 방법
    // printf("g_data addr = %p \n",(void*)&g_data);
    // printf("l_data addr = %p \n",(void*)&l_data);

    // //typedef와 다른 형식으로 쓰이는지 확인 
    // struct Point p;

    // p.x = 10;
    // p.y = 20;

    // printf("p.x=%d \np.y=%d \n", p.x , p.y);

    return 0;
}