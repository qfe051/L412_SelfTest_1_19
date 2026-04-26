/*
 * ============================================================
 * Ch6.3 트리와 이진트리 응용 - 통합 학습 코드
 * ------------------------------------------------------------
 * [포함 내용]
 * 1) BST (이진탐색트리)
 *    - 탐색 / 삽입 / 삭제(리프, 한 자식, 두 자식)
 *    - 중위순회(오름차순 확인)
 *
 * 2) Max Heap (배열 기반)
 *    - 삽입 (heapify-up)
 *    - 삭제 (루트 삭제, heapify-down)
 *    - 완전 이진트리의 순차표현
 *
 * 3) 4-way Search Tree 개념 시뮬레이션
 *    - 한 노드에 여러 키(최대 3개) 저장
 *    - "노드 내부에서 순차탐색 후 어느 자식으로 갈지 결정" 감각
 *    - 완전한 B-Tree 구현이 아니라 개념 이해용
 *
 * [학습 포인트]
 * - BST: "비교하면서 내려감"
 * - Heap: "배열 인덱스로 부모/자식 계산"
 * - m-way: "한 노드 안에서 여러 키 비교 후 자식 분기"
 * ============================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define HEAP_CAPACITY 100
#define MWAY_MAX_KEYS 3
#define MWAY_MAX_CHILDREN 4

/* ============================================================
 * 1. BST (Binary Search Tree)
 * ============================================================
 */

 // key와 left right의 주소로 이루어짐
 typedef struct BSTNode{
    int key;
    struct BSTNode *left;
    struct BSTNode *right;
 } BSTNode;

 // 노드 생성 - 주소 상관없이 생성함
 // key대입
 BSTNode* bst_create_node(int key){
    BSTNode *node = (BSTNode*)malloc(sizeof(BSTNode));
    if (node == NULL){
        printf("[BST] malloc failed\n");
        return NULL;
    }
    
    node->key = key;
    node->left= NULL;
    node->right= NULL;

    return node;
 }
 
 // 탐색 - root 주소(현재 탐색 시작 노드), 원하는 key 값 필요
 // 같음 -> 반환
 // 작음 -> 좌측 이동 root = root->left;
 // 큼 -> 우측 이동 root = root->right;
 BSTNode* bst_search(BSTNode *root,int key){
    while (root == NULL){
        if(key==root->key){
            return root;
        }
        // root 변경하면서 탐색
        else if(key < root->key){
            root = root->left;
        }else{
            root = root->right;
        }
    }
    return NULL;

 }

 //삽입 - root 주소와 key 값
 // 자식 노드를 갱신하는 방식으로 업데이트 됨 : root->left = bst_insert(root->left,key);
 BSTNode* bst_insert(BSTNode *root,int key){
    // root 없으면 생성모드로 진행하기 
    if (root ==NULL){
        return bst_create_node(key);
    }

    if (key < root->key){
        root->left = bst_insert(root->left,key);
    }else if (key > root->key){
        root->right = bst_insert(root->right,key);
    }else{
        // 중복된 키는 삽입 X
        printf("[BST] duplicate key ignored: %d\n", key);
    }
    return root;
 }

 // 최소값 노드 찾기 -> 삭제시 후계자 찾기.....?
 BSTNode* bst_find_min(BSTNode *root){
    if (root==NULL){
        return NULL;
    }
    // 이진트리의 기본 개념 적용하면 혼동할 일 적음 : 목표는 가장 작은 값 찾기 
    // left에 작은 값 있음. -> 계속 left 방향만 찾으면 됨
    while (root->left != NULL){
        root = root ->left;
    }
    return root;
 }

 /* 삭제 - case 별로 대응하기 
 * 케이스 1) 리프노드
 * 케이스 2) 자식 1개
 * 케이스 3) 자식 2개 -> 오른쪽 서브트리의 최소값(중위 후계자)로 대체
 * ### case3 개념 다시 보기 
 */

 BSTNode* bst_delete(BSTNode *root, int key){
    if (root == NULL){
        printf("root is NULL \n");
        return NULL;
    }

    // 삭제 대상 없어서 순회하는 과정
    if(key<root->key){
        root->left = bst_delete(root->left,key);
    }else if (key > root->key){
        root->right = bst_delete(root->right,key);
    }
    else{
        // 삭제 대상 찾아서 삭제 진행
        // 판단부터, 처리까지 모두 살펴보기 

        // case1 리프노드
        if (root->left == NULL && root->right ==NULL){
            //free 하면서 삭제
            free(root);
            return NULL;
        }

        // case 2-1 : 오른쪽 자식만
        if (root->left==NULL){
            BSTNode *temp = root->right;
            free(root);
            return temp;
        }
        
        // case 2-2 : 왼쪽 자식만
        if (root->right==NULL){
            BSTNode *temp = root->left;
            free(root);
            return temp;
        }

        // case 3 : 2개 자식 모두 있음
        // bst_find_min에 right 대입하는 이유는?
        BSTNode *successor = bst_find_min(root->right);
        root->key = successor->key;
        root->right = bst_delete(root->right, successor->key);
        
    }
    return root;
 }
 // 중위순회 -> 어떤걸 출력하려는지 목적 파악 잘하기 

 void bst_inorder(BSTNode *root){
    if(root == NULL){
        return;
    }

    bst_inorder(root->left);
    // left의 key 출력?, 
    printf("d",root->key);
    // right -> 우측에서도 같은 동작? 
    bst_inorder(root->right);
 }

 // 전위순회
 void bst_preorder(BSTNode *root){
    if(root == NULL){
        return;
    }

    printf("%d",root->key);
    bst_preorder(root->left);
    bst_preorder(root->right);
 }

 // 후위 free -> 자식 먼저 지우는게 안정적! 
 void bst_free(BSTNode *root){
    if(root == NULL){
        return;
    }

    bst_free(root->left);
    bst_free(root->right);
    free(root);
 }
