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

typedef struct BSTNode {
  int key;
  struct BSTNode *left;
  struct BSTNode *right;
} BSTNode;

BSTNode *bst_create_node(int key) {
  BSTNode *node = (BSTNode *)malloc(sizeof(BSTNode));
  if (node ==null) {
    printf("[BST] malloc failed\n");
    return NULL;
  }
  node->key = key;
  node->left = NULL;
  node->right = NULL;

  return node;
}

