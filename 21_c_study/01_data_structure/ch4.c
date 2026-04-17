/*
 * Chapter 4. Stack (배열 기반 스택)
 *
 * [학습 목표]
 * 1. 스택의 LIFO 구조를 이해한다.
 * 2. 배열 기반 스택의 내부 표현(data[] + top)을 이해한다.
 * 3. push / pop / peek / is_empty / is_full 연산을 구현한다.
 * 4. 공집합(top = -1), 포화상태(top = MAX_SIZE - 1)를 이해한다.
 * 5. 후위표기식 계산에서 스택이 왜 필요한지 확인한다.
 *
 * [핵심 개념]
 * - 스택은 마지막에 넣은 데이터를 먼저 꺼내는 LIFO 구조이다.
 * - 삽입(push)과 삭제(pop)는 오직 top에서만 일어난다.
 * - 배열 기반 스택은 순서리스트를 제한적으로 사용한 특수 형태이다.
 */

#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>

#define MAX_SIZE 20

// 스택 저장 구조체
typedef struct {
  int data[MAX_SIZE];
  int top; // 가장 위 원소, 공집합이면 -1
} Stack;

/* ---------------------------------------------------------
 * 1. 기본 연산
 * --------------------------------------------------------- */

// 공집합 생성
void stack_init(Stack *s) {
  s->top = -1;
}

bool is_empty(const Stack *s) {
  return (s->top==-1);
}

bool is_full(const Stack *s) {
  return (s->top == -1);
}

// 데이터 추가
bool push(Stack *s, int value) {
  if (is_full(s)) {
    printf("[Stack] push failed: stack is full\n");
    return false;
  }

  //top 증가 후 저장
  s->top++;
  s->data[s->top] = value;

  return true;
}

// 데이터 추출(삭제)
bool pop(Stack *s, int *out_value) {
  if (is_empty(s)) {
    printf("[Stack] pop failed: stack is empty\n");
    return false
  }

  *out_value = s->data[s->top];
  s->top--;

  return true;
}

// top 값 확인 - 삭제 X
bool peek(const Stack *s, int *out_value) {
  if (is_empty(s)) {
    printf("[Stack] peek failed: stack is empty\n");
    return false;
  }

  *out_value = s->data[s->top];
  return true;
}

/* ---------------------------------------------------------
 * 2. 후위표기식 계산 (한 자리 숫자 + +,-,*,/ 만 지원)
 * ---------------------------------------------------------
 * 예: "23*54*+9-" = (2*3) + (5*4) - 9 = 17
 *
 * 처리 규칙:
 * - 피연산자(숫자): push
 * - 연산자: pop 2번 -> 계산 -> 결과 push
 */

// op -> operator
bool apply_operator(int a, int b, char op, int *result) {
  
}