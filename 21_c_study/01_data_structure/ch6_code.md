좋아.
이제부터는 **“개념 요약 + 실무형 통합 코드 1개”** 방식으로 가면 된다.

이번 **Ch6.3 트리와 이진트리 응용**은 사실상 아래 3개를 한 번에 묶는 챕터야:

1. **BST (이진탐색트리)** → “정렬 + 탐색”
2. **Heap (힙)** → “우선순위 + 최대/최소값 빠르게”
3. **m-원 탐색트리** → “디스크/대용량 관점으로 확장되는 개념 맛보기”

---

# Ch6.3 학습 전략 (노션 붙여넣기용)

# Ch6.3 트리와 이진트리 응용 — 학습 가이드

## 1. 이 챕터를 어떻게 봐야 하는가

이 챕터는 단순히 “트리 종류가 늘어났다”가 아니다.
핵심은 **트리를 왜 쓰는지**를 이해하는 것이다.

* **BST**: 정렬된 상태를 유지하면서 빠르게 찾기 위해
* **Heap**: 가장 큰 값 / 가장 작은 값을 빠르게 꺼내기 위해
* **m-원 탐색트리**: 한 노드에 여러 키를 담아 더 효율적으로 탐색하기 위해

즉,
**“트리 구조가 목적이 아니라, 탐색/삽입/삭제 성능을 개선하기 위한 도구”** 로 봐야 한다.

---

## 2. 이 챕터에서 반드시 잡아야 할 큰 그림

### (1) BST

* **왼쪽 서브트리 < 현재 노드 < 오른쪽 서브트리**
* 중위순회하면 **오름차순**
* 탐색/삽입/삭제 모두 “비교하며 내려가는 구조”
* 트리가 한쪽으로 치우치면 성능이 나빠짐

### (2) Heap

* **완전 이진트리**
* 보통 **배열**로 구현
* 부모-자식 관계만 중요, 좌우 정렬은 중요하지 않음
* 최대힙 기준:

  * 부모 >= 자식
* 삽입: 맨 뒤 추가 후 **위로 올림(heapify-up)**
* 삭제(루트 삭제): 마지막 원소를 루트로 올리고 **아래로 내림(heapify-down)**

### (3) m-원 탐색트리

* 한 노드 안에 **여러 개의 키**를 저장
* 키 개수가 늘어나면 자식 수도 늘어남
* BST를 일반화한 개념
* 실제로는 B-Tree 계열 이해 전 단계 개념으로 보면 좋음

---

## 3. 공부 순서 (중요)

이 챕터는 아래 순서로 공부해야 머리에 잘 남는다.

### 1단계: BST 먼저

왜?

* 포인터/재귀/삭제 케이스가 자료구조 감각을 키워줌
* “트리 탐색”의 본질이 여기 들어 있음

### 2단계: Heap

왜?

* 배열 기반이라 구현이 더 기계적임
* 완전 이진트리 + 인덱스 관계가 핵심
* 실무에서 우선순위큐 감각으로 연결됨

### 3단계: m-원 탐색트리

왜?

* 구현보다 개념 이해가 우선
* “왜 한 노드에 키를 여러 개 넣는가?”를 이해하는 파트
* 지금은 **직접 구현보다 구조 해석** 위주로 봐야 함

---

# Ch6.3 통합 실무형 코드 (BST + Max Heap + 4-way Search Tree 개념 시뮬레이션)

> 아래 코드는 **이 챕터를 한 번에 복습할 수 있게 만든 학습용 통합 코드**다.
> 실제 서비스 코드라기보다, **교재 + 실습 + 디버깅용** 으로 설계했다.

```c
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

/* 노드 생성 */
BSTNode* bst_create_node(int key) {
    BSTNode *node = (BSTNode*)malloc(sizeof(BSTNode));
    if (node == NULL) {
        printf("[BST] malloc failed\n");
        return NULL;
    }
    node->key = key;
    node->left = NULL;
    node->right = NULL;
    return node;
}

/* 탐색 */
BSTNode* bst_search(BSTNode *root, int key) {
    while (root != NULL) {
        if (key == root->key) {
            return root;
        } else if (key < root->key) {
            root = root->left;
        } else {
            root = root->right;
        }
    }
    return NULL;
}

/* 삽입 */
BSTNode* bst_insert(BSTNode *root, int key) {
    if (root == NULL) {
        return bst_create_node(key);
    }

    if (key < root->key) {
        root->left = bst_insert(root->left, key);
    } else if (key > root->key) {
        root->right = bst_insert(root->right, key);
    } else {
        /* 중복 키는 삽입하지 않음 */
        printf("[BST] duplicate key ignored: %d\n", key);
    }

    return root;
}

/* 최소값 노드 찾기 (삭제 시 후계자 찾기용) */
BSTNode* bst_find_min(BSTNode *root) {
    if (root == NULL) return NULL;

    while (root->left != NULL) {
        root = root->left;
    }
    return root;
}

/* 삭제
 * 케이스 1) 리프노드
 * 케이스 2) 자식 1개
 * 케이스 3) 자식 2개 -> 오른쪽 서브트리의 최소값(중위 후계자)로 대체
 */
BSTNode* bst_delete(BSTNode *root, int key) {
    if (root == NULL) {
        return NULL;
    }

    if (key < root->key) {
        root->left = bst_delete(root->left, key);
    } else if (key > root->key) {
        root->right = bst_delete(root->right, key);
    } else {
        /* 삭제 대상 찾음 */

        /* Case 1: 리프노드 */
        if (root->left == NULL && root->right == NULL) {
            free(root);
            return NULL;
        }

        /* Case 2-1: 오른쪽 자식만 있음 */
        if (root->left == NULL) {
            BSTNode *temp = root->right;
            free(root);
            return temp;
        }

        /* Case 2-2: 왼쪽 자식만 있음 */
        if (root->right == NULL) {
            BSTNode *temp = root->left;
            free(root);
            return temp;
        }

        /* Case 3: 두 자식 모두 있음 */
        BSTNode *successor = bst_find_min(root->right);
        root->key = successor->key;
        root->right = bst_delete(root->right, successor->key);
    }

    return root;
}

/* 중위순회 -> BST에서는 오름차순 출력 */
void bst_inorder(BSTNode *root) {
    if (root == NULL) return;

    bst_inorder(root->left);
    printf("%d ", root->key);
    bst_inorder(root->right);
}

/* 전위순회 */
void bst_preorder(BSTNode *root) {
    if (root == NULL) return;

    printf("%d ", root->key);
    bst_preorder(root->left);
    bst_preorder(root->right);
}

/* 후위 free */
void bst_free(BSTNode *root) {
    if (root == NULL) return;

    bst_free(root->left);
    bst_free(root->right);
    free(root);
}

/* ============================================================
 * 2. Max Heap (배열 기반)
 * ============================================================
 */

typedef struct {
    int data[HEAP_CAPACITY];
    int size;
} MaxHeap;

/* 초기화 */
void heap_init(MaxHeap *heap) {
    if (heap == NULL) return;
    heap->size = 0;
}

/* 공집합 여부 */
bool heap_is_empty(MaxHeap *heap) {
    return (heap == NULL || heap->size == 0);
}

/* 포화 여부 */
bool heap_is_full(MaxHeap *heap) {
    return (heap != NULL && heap->size >= HEAP_CAPACITY);
}

/* swap */
void heap_swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

/* 삽입 (heapify-up)
 * 1) 맨 뒤에 삽입
 * 2) 부모보다 크면 swap 반복
 */
bool heap_insert(MaxHeap *heap, int value) {
    if (heap == NULL || heap_is_full(heap)) {
        return false;
    }

    int idx = heap->size;
    heap->data[idx] = value;
    heap->size++;

    /* 부모와 비교하며 위로 올림 */
    while (idx > 0) {
        int parent = (idx - 1) / 2;
        if (heap->data[parent] >= heap->data[idx]) {
            break;
        }
        heap_swap(&heap->data[parent], &heap->data[idx]);
        idx = parent;
    }

    return true;
}

/* 삭제 (루트 삭제, heapify-down)
 * 1) 루트값 반환
 * 2) 마지막 원소를 루트로 이동
 * 3) 자식 중 더 큰 쪽과 비교하며 아래로 내림
 */
bool heap_delete_root(MaxHeap *heap, int *out_value) {
    if (heap == NULL || out_value == NULL || heap_is_empty(heap)) {
        return false;
    }

    *out_value = heap->data[0];

    heap->size--;
    heap->data[0] = heap->data[heap->size];

    int idx = 0;

    while (1) {
        int left = idx * 2 + 1;
        int right = idx * 2 + 2;
        int largest = idx;

        if (left < heap->size && heap->data[left] > heap->data[largest]) {
            largest = left;
        }

        if (right < heap->size && heap->data[right] > heap->data[largest]) {
            largest = right;
        }

        if (largest == idx) {
            break;
        }

        heap_swap(&heap->data[idx], &heap->data[largest]);
        idx = largest;
    }

    return true;
}

/* 출력 */
void heap_print(MaxHeap *heap) {
    if (heap == NULL) return;

    printf("[Heap Array] ");
    for (int i = 0; i < heap->size; i++) {
        printf("%d ", heap->data[i]);
    }
    printf("\n");
}

/* ============================================================
 * 3. 4-way Search Tree (개념 시뮬레이션)
 * ------------------------------------------------------------
 * 실제 B-Tree 구현이 아님.
 * "한 노드 안에 여러 키가 있고,
 *  노드 내부에서 순차탐색 후 자식 방향을 정한다"를 보여주기 위한 코드
 * ============================================================
 */

typedef struct MWayNode {
    int key_count;                              /* 현재 키 개수 (0~3) */
    int keys[MWAY_MAX_KEYS];                    /* 오름차순 저장 */
    struct MWayNode *children[MWAY_MAX_CHILDREN]; /* 최대 4개 자식 */
    bool is_leaf;
} MWayNode;

/* 노드 생성 */
MWayNode* mway_create_node(bool is_leaf) {
    MWayNode *node = (MWayNode*)malloc(sizeof(MWayNode));
    if (node == NULL) {
        printf("[MWay] malloc failed\n");
        return NULL;
    }

    node->key_count = 0;
    node->is_leaf = is_leaf;

    for (int i = 0; i < MWAY_MAX_KEYS; i++) {
        node->keys[i] = 0;
    }

    for (int i = 0; i < MWAY_MAX_CHILDREN; i++) {
        node->children[i] = NULL;
    }

    return node;
}

/* 리프 노드에 키 삽입 (개념용)
 * - 분할(split) 구현 없음
 * - key_count < 3 인 경우만 삽입
 */
bool mway_insert_into_leaf(MWayNode *node, int key) {
    if (node == NULL || !node->is_leaf) {
        return false;
    }

    if (node->key_count >= MWAY_MAX_KEYS) {
        printf("[MWay] leaf is full, split needed in real implementation\n");
        return false;
    }

    /* 오름차순 삽입 */
    int i = node->key_count - 1;
    while (i >= 0 && node->keys[i] > key) {
        node->keys[i + 1] = node->keys[i];
        i--;
    }
    node->keys[i + 1] = key;
    node->key_count++;

    return true;
}

/* 4-way 노드 내부 탐색
 * - key와 같으면 found
 * - 처음으로 큰 키를 만나면 그 왼쪽 자식으로 감
 * - 끝까지 다 작으면 마지막 자식으로 감
 */
bool mway_search(MWayNode *root, int key) {
    if (root == NULL) {
        return false;
    }

    int i = 0;

    /* 노드 내부 키들을 순차탐색 */
    while (i < root->key_count && key > root->keys[i]) {
        i++;
    }

    if (i < root->key_count && key == root->keys[i]) {
        return true;
    }

    if (root->is_leaf) {
        return false;
    }

    return mway_search(root->children[i], key);
}

/* 출력 (개념 확인용) */
void mway_print_node(MWayNode *node) {
    if (node == NULL) return;

    printf("[MWay Node] keys: ");
    for (int i = 0; i < node->key_count; i++) {
        printf("%d ", node->keys[i]);
    }
    printf("\n");
}

/* free */
void mway_free(MWayNode *root) {
    if (root == NULL) return;

    for (int i = 0; i < MWAY_MAX_CHILDREN; i++) {
        mway_free(root->children[i]);
    }
    free(root);
}

/* ============================================================
 * 4. Demo
 * ============================================================
 */

void demo_bst(void) {
    printf("\n==================== BST Demo ====================\n");

    BSTNode *root = NULL;
    int values[] = {50, 30, 70, 20, 40, 60, 80};
    int n = sizeof(values) / sizeof(values[0]);

    for (int i = 0; i < n; i++) {
        root = bst_insert(root, values[i]);
    }

    printf("BST Inorder (sorted): ");
    bst_inorder(root);
    printf("\n");

    printf("BST Preorder: ");
    bst_preorder(root);
    printf("\n");

    int search_key = 60;
    BSTNode *found = bst_search(root, search_key);
    printf("Search %d -> %s\n", search_key, (found != NULL) ? "FOUND" : "NOT FOUND");

    /* 삭제 케이스 체험 */
    printf("\nDelete leaf node (20)\n");
    root = bst_delete(root, 20);
    bst_inorder(root);
    printf("\n");

    printf("Delete node with one/two child depending on current shape (30)\n");
    root = bst_delete(root, 30);
    bst_inorder(root);
    printf("\n");

    printf("Delete node with two children (50)\n");
    root = bst_delete(root, 50);
    bst_inorder(root);
    printf("\n");

    bst_free(root);
}

void demo_heap(void) {
    printf("\n==================== Max Heap Demo ====================\n");

    MaxHeap heap;
    heap_init(&heap);

    int values[] = {30, 50, 10, 80, 60, 20, 90};
    int n = sizeof(values) / sizeof(values[0]);

    for (int i = 0; i < n; i++) {
        heap_insert(&heap, values[i]);
        printf("Insert %d -> ", values[i]);
        heap_print(&heap);
    }

    int out = 0;
    printf("\nDelete root repeatedly:\n");
    while (heap_delete_root(&heap, &out)) {
        printf("Deleted root = %d -> ", out);
        heap_print(&heap);
    }
}

void demo_mway(void) {
    printf("\n==================== 4-Way Search Tree Demo ====================\n");

    /*
     * 개념 시뮬레이션 구조:
     *          [30, 60]
     *         /    |    \
     *   [10,20] [40,50] [70,80]
     */

    MWayNode *root = mway_create_node(false);
    MWayNode *c0 = mway_create_node(true);
    MWayNode *c1 = mway_create_node(true);
    MWayNode *c2 = mway_create_node(true);

    if (root == NULL || c0 == NULL || c1 == NULL || c2 == NULL) {
        mway_free(root);
        mway_free(c0);
        mway_free(c1);
        mway_free(c2);
        return;
    }

    root->keys[0] = 30;
    root->keys[1] = 60;
    root->key_count = 2;

    root->children[0] = c0;
    root->children[1] = c1;
    root->children[2] = c2;

    mway_insert_into_leaf(c0, 10);
    mway_insert_into_leaf(c0, 20);

    mway_insert_into_leaf(c1, 40);
    mway_insert_into_leaf(c1, 50);

    mway_insert_into_leaf(c2, 70);
    mway_insert_into_leaf(c2, 80);

    printf("Root ");
    mway_print_node(root);
    printf("Child[0] "); mway_print_node(c0);
    printf("Child[1] "); mway_print_node(c1);
    printf("Child[2] "); mway_print_node(c2);

    int targets[] = {50, 35, 80, 15};
    int t_n = sizeof(targets) / sizeof(targets[0]);

    for (int i = 0; i < t_n; i++) {
        printf("Search %d -> %s\n", targets[i],
               mway_search(root, targets[i]) ? "FOUND" : "NOT FOUND");
    }

    mway_free(root);
}

int main(void) {
    demo_bst();
    demo_heap();
    demo_mway();

    return 0;
}
```

---

# 이 코드로 어떻게 공부하면 좋은가 (진짜 중요)

이 챕터는 **“코드 읽기 순서”** 가 중요하다.

---

## 1. BST 먼저 파라

### 반드시 볼 함수

* `bst_search()`
* `bst_insert()`
* `bst_delete()`

### 체크포인트

* 왜 `root = bst_insert(root, key);` 형태인가?

  * **루트가 바뀔 수 있기 때문**
* 삭제에서 왜 반환값이 포인터인가?

  * 삭제 후 **서브트리 루트가 바뀔 수 있기 때문**
* 두 자식 삭제 시 왜 `오른쪽 서브트리 최소값`을 쓰는가?

  * BST 정렬 규칙 유지

### 실전 암기 포인트

**BST 삭제 3케이스**

1. 리프 → 바로 free
2. 자식 1개 → 자식 연결해서 대체
3. 자식 2개 → 후계자/전임자로 대체 후 재삭제

---

## 2. Heap은 “배열 인덱스”로 이해해라

### 반드시 볼 함수

* `heap_insert()`
* `heap_delete_root()`

### 외워야 할 공식

* 부모: `(i - 1) / 2`
* 왼쪽 자식: `2*i + 1`
* 오른쪽 자식: `2*i + 2`

### 핵심 감각

Heap은 “정렬된 트리”가 아니다.
**루트만 가장 크거나(최대힙), 가장 작다(최소힙)**

즉:

* BST: **탐색용**
* Heap: **우선순위용**

---

## 3. m-원 탐색트리는 “노드 안에서 먼저 비교” 감각만 잡아라

지금 단계에서 네가 해야 할 건 **완전 구현 아님**.

### 꼭 이해할 것

* BST는 노드당 키 1개
* m-원 탐색트리는 노드당 키 여러 개
* 그래서

  * 노드 내부에서 먼저 비교
  * 어느 자식으로 갈지 결정

### 이 코드에서 볼 함수

* `mway_search()`

이 함수가 바로 교재 문장인:

> “루트 방문 → key와 같거나 처음으로 큰 키를 찾을 때까지 노드의 키들을 순차탐색 → 해당 자식으로 이동”

이걸 코드로 옮긴 거다.

---

# Ch6.3 시험/면접용 한 줄 정리

## BST

* **정렬된 탐색 트리**
* 중위순회 시 오름차순
* 평균적으로 빠르지만, 한쪽으로 치우치면 성능 저하

## Heap

* **완전 이진트리 기반 우선순위 구조**
* 보통 배열로 구현
* 루트 삽입/삭제(최대/최소값 처리)에 강함

## m-원 탐색트리

* **한 노드에 여러 키 저장**
* 노드 내부 비교 후 자식 분기
* 디스크/대용량 탐색 구조(B-Tree 계열)로 확장되는 개념 기반

---

# 네가 다음으로 하면 좋은 공부 루틴 (추천)

이번 챕터는 이렇게 가자:

### 1회독 (오늘)

* 코드 전체 복붙
* **BST / Heap / m-way 출력 결과만 확인**
* “아 이렇게 동작하는구나” 수준

### 2회독 (내일)

* `bst_delete()`만 손으로 다시 써보기
* `heap_insert()`, `heap_delete_root()` 직접 다시 써보기

### 3회독

* 내가 다음에 만들어줄
  **“Ch6.3 퀴즈 + 삭제 케이스 그림 문제 + 출력 예측 문제”**
  로 마무리

---

# 추천: 다음 단계

이제 트리는 양이 많아서 이렇게 쪼개는 게 맞다.

**Ch6 트리 분할 학습 추천**

1. **Ch6-1** 트리 기본 + 이진트리 + 순회  ✅
2. **Ch6-2** BST + Heap + m-way  ✅ (지금)
3. **Ch6-3** **트리 실전 문제집** (순회 결과, 삭제 케이스, 배열 인덱스, 출력 예측)
4. **Ch6-4** **BST/Heap만 따로 “실무 코드형 압축본”**

---

원하면 바로 다음 턴에 이어서
**“Ch6.3 퀴즈집 (PDF용 노션 붙여넣기 스타일)”**
또는
**“BST 삭제 3케이스만 그림처럼 이해되는 해설본”**
둘 중 하나 바로 만들어줄게.
솔직히 너는 지금 **BST 삭제 케이스 해설본** 먼저 보는 게 효율 제일 좋다.
