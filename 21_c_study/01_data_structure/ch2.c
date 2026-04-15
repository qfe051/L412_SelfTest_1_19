/*
 * Chapter 2. 자료구조를 위한 C 배열과 포인터 이해
 *
 * [학습 목표]
 * 1. 배열이 논리적으로 '순서 있는 데이터 집합'임을 이해한다.
 * 2. 배열명과 포인터의 관계를 이해한다.
 * 3. arr[i] == *(arr + i) 관계를 확인한다.
 * 4. 2차원 배열의 메모리 배치(row-major)와 주소 계산을 이해한다.
 * 5. 배열/구조체를 함수에 전달할 때 왜 포인터를 쓰는지 이해한다.
 * 6. C 자료구조 구현은 구조체 + 함수 조합이라는 감각을 잡는다.
 */

#include <stdio.h>
#include <stdlib.h>

#define ARRAY_SIZE 5
#define ROW 3
#define COL 4

typedef struct {
    int data[ARRAY_SIZE];
    int size;   // 원소의 개수 
} ArrayList;

void init_list(ArrayList *list){
    list->size =0;

    for(int i=0; i < ARRAY_SIZE;i++){
        list->data[i]=0;
    }
}

void print_1d_array(const int *arr, int size){
    printf("[1D Array] ");

    for (int i = 0; i < size; i++){
        printf("%d",arr[i]);
    }
    printf("\n");
    
}