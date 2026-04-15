/*
 * Chapter 3. 순서리스트 / 단순 연결리스트 / 이중 연결리스트 비교 정리
 *
 * [학습 목표]
 * 1. 순서리스트(배열 기반)와 연결리스트(포인터 기반)를 구분한다.
 * 2. 공집합 상태가 각각 어떻게 표현되는지 이해한다.
 * 3. 삽입/삭제/탐색의 내부 동작 차이를 이해한다.
 * 4. 단순 연결리스트와 이중 연결리스트의 차이를 구분한다.
 *
 * [핵심 구분]
 * - 순서리스트      : 배열 + length
 * - 단순 연결리스트 : Node(data + next)
 * - 이중 연결리스트 : DNode(data + prev + next)
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_SIZE 10

/* =========================================================
 * 1) 순서리스트 (Sequential List)
 * =========================================================
 * - 배열 기반
 * - 공집합 상태: length == 0
 * - 삽입 시: 뒤에서 앞으로 밀기
 * - 삭제 시: 앞으로 당기기
 */

typedef struct {
    int data[MAX_SIZE];
    int length;
}SeqList;

void seq_init(SeqList *list){
    list->length =0;
}

bool seq_insert(SeqList *list,int pos,int value){
    if(list->length >= MAX_SIZE){
        printf("[Seq] insert failed : list is full \n");
        return false;
    }

    if (pos<0 || pos > list->length){
        printf("[Seq] insert failed : invalid position \n");
        return false;
    }

    // 뒤에서 앞으로 밀기
    // 앞의 인덱스를 뒤의 인덱스로! -> 무슨 작업인지?
    for(int i = list->length;i>pos;i--){
        list->data[i] = list->data[i-1];
    }

    list->data[pos]=value;
    list->length++;

    return true;
}

// target이 몇번 인덱스에 있는지
int seq_search(const SeqList *list, int target){
    for (int i = 0; i < list->length; i++){
        if (list->data[i]==target){
            return i;
        }
    }
    return -1;
    
}

// 그냥 출력
void seq_print(const SeqList *list) {
    printf("[SeqList] length=%d | ", list->length);

    for (int i = 0; i < list->length; i++) {
        printf("%d ", list->data[i]);
    }

    printf("\n");
}

/* =========================================================
 * 2) 단순 연결리스트 (Singly Linked List)
 * =========================================================
 * - 노드 + next
 * - 공집합 상태: head == NULL
 * - 동적 메모리 사용
 * - current == NULL 은 순회 종료 / 끝 도달 의미
 */

 typedef struct Node{
    int data;
    struct Node *next;
 } Node;
 

 Node *create_node(int value){
    Node *new_node = (Node *)malloc(sizeof(Node));
    //malloc 공간 없는 경우 
    if (new_node == NULL){
        printf("[SList] malloc failed\n");
        return NULL;
    }

    new_node->data=value;
    new_node->next = NULL;

    return new_node;
 }

 bool slist_append(Node **head, int value){
    // create_node에서 new_node 반환? -> 개념 확실히
    Node *new_node = create_node(value);
    if (new_node == NULL){
        
        return false;
    }
    if (*head == NULL){
        *head=new_node;

        return true;
    }
    // head에 이중포인터 쓰는 이유는?
    Node *current = *head;

    while (current->next != NULL){
        current = current->next;
    }
    current->next=new_node;
    
    return true;
 }

 Node *slist_search(Node *head, int target){
    Node *current = head;

    // next 넘어가면서 target 찾음
    while (current != NULL){
        if (current->data == target){
            return current;
        }
        current = current->next;
    }

    return NULL;
 }


 // 여기부터는 시간 나면 해보기
 bool slist_delete_first(Node **head, int target) {
    if (*head == NULL) {
        printf("[SList] delete failed: list is empty\n");
        return false;
    }

    Node *current = *head;
    Node *prev = NULL;

    while (current != NULL) {
        if (current->data == target) {
            /* 첫 노드 삭제 */
            if (prev == NULL) {
                *head = current->next;
            }
            else {
                prev->next = current->next;
            }

            free(current);
            return true;
        }

        prev = current;
        current = current->next;
    }

    printf("[SList] delete failed: target not found\n");
    return false;
}

void slist_print(Node *head) {
    Node *current = head;

    printf("[SList] ");

    while (current != NULL) {
        printf("%d -> ", current->data);
        current = current->next;
    }

    printf("NULL\n");
}

void slist_free_all(Node **head) {
    Node *current = *head;

    while (current != NULL) {
        Node *temp = current;
        current = current->next;
        free(temp);
    }

    *head = NULL;
}

/* =========================================================
 * 3) 이중 연결리스트 (Doubly Linked List)
 * =========================================================
 * - prev + next
 * - 양방향 순회 가능
 * - 맨 뒤 삽입 시 tail 관리가 편리
 */


typedef struct DNode{
    int data;
    struct DNode *prev;
    struct DNode *next;
}DNode;

DNode *dlist_create_node(int value){
    DNode *new_node = (DNode *)malloc(sizeof(DNode));
    if (new_node == NULL) {
        printf("[DList] malloc failed\n");
        return NULL;
    }

    new_node->data = value;
    new_node->prev=NULL;
    new_node->next=NULL;

    return new_node;
}