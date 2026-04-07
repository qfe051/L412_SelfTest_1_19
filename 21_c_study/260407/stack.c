#include <stdio.h>
#include <stdbool.h>

// last in , first out



typedef struct stack {
  uint8_t buffer[10];
  uint8_t front;    // 데이터 꺼내기
  uint8_t rear;     // 데이터 넣기
} stack;

uint8_t pop_data = 0;

stack s;

void init_stack(stack *s) {
  s->front = 0;
  s->rear = 9;
}

bool push(stack *s, uint8_t data) {
    if (data == '\0') {
        return false;
    }
  
  s->buffer[s->rear] = data;
  if (s->rear > 0) {
    s->rear --;
  }
  else {
    printf("rear is 0 \n");
  }

  return true;
}

bool pop(stack *s, uint8_t *output) {
  if (s->buffer[s->front]=='\0'  ) {
    return false;
  }
  *output = s->buffer[s->front];
  if (s->front <9) {
    s->front ++;
  }
  else {
    printf("front is 9 \n")
  }

  return true;
}





int main() {}








#define STACK_SIZE  10
typedef struct _stack_ {
  uint8_t buffer[STACK_SIZE];
  int8_t data_index;
} STACK_T;


void stack_init(STACK_T *pStack) {
  if (pStack == NULL)
    return;
    pStack->data_index = -1;
}

bool stack_isFull(STACK_T *pStack) {
  if (pStack == NULL)
    return true;

  return (pStack->data_index == STACK_SIZE) ? true : false;
    
}

bool stack_isEmpty(STACK_T *pStack) {
  if (pStack == NULL)
    return true;

  return (pStack->data_index == -1) ? true : false;
}

bool stack_Push(STACK_T *pStatck, uint8_t data) {}

bool stack_pop(STACK_T *pStatck, uint8_t *data) { }


























