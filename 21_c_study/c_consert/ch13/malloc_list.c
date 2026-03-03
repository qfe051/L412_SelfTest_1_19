#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct NODE {
  int data;
  struct NODE *link; // 다음 Node의 주소 (ㄴ)
} NODE;

NODE *create_node(int data) {
  // 구조체 n 선언, NODE 1개 크기만큼 heap에 확보
  NODE *n = malloc(sizeof(NODE));

  if (n == NULL) {
    printf("ERROR 1 \n");

    return 0;
  }

  n->data = data;
  n ->link = NULL;
  
  return n;
}

bool push_back(NODE **pHead, int data) {
  // head 변수의 주소 변경 위해 ** 사용 (head 변수의 주소)
  NODE *newNode = create_node(data);
  if (newNode == NULL) {
    return false;
  }

  // head가 비어있는지 확인
  if (*pHead == NULL) {
    *pHead = newNode;
    return true;
  }

  // 마지막 node의 link 연결 , cur -> 현재값 나타내는지?
  NODE *cur = *pHead;
  while (cur -> link !=NULL) {
    cur = cur -> link;
  }
  cur -> link = newNode;
  return true;
}

// 리스트 순차적으로 출력
void print_list(const NODE *head) {
  printf("[LIST] ");
  const NODE *cur = head;
  while (cur != NULL) {
    // 현재 데이터값 출력
    printf("%d", cur->data);
    if (cur -> link != NULL) {
      printf(" -> ");
    }
    // cur의 주소에 'cur -> link' 에 적힌 주소 넣음
    cur = cur -> link;
  }
}

// 리스트 순차적으로 메모리 해제 
void free_list(NODE **pHead) {
  NODE *cur = *pHead;
  while (cur != NULL) {
    // 'cur -> link' 에 적힌 주소 => 다음 주소 저장
    NODE *next = cur->link;
    free(cur);
    cur = next ;
  }
  *pHead = NULL;
}

int main(void) {

  // head를 NULL로
  NODE *head = NULL;

  // 연결된 list 만들기, double pointer인 것 참고!
  push_back(&head, 1);
  push_back(&head, 2);
  push_back(&head, 3);

  print_list(head);
  printf("\n");

  free_list(&head);

  print_list(head);


  return 0;
}
