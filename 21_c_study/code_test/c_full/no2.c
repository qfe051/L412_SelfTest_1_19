/*
[문제 2] 가변형 2차원 데이터 '페이지 관리자'
문제: 행(Row)과 열(Column)의 크기가 제각각인 2차원 데이터를
메모리 낭비 없이 관리하는 시스템을 만드세요.
1. 중첩 할당: 전체 페이지 수(M)를 입력받고,
각 페이지마다 저장할 데이터 개수(N_i)를
각각 다르게 입력받아 메모리를 할당하세요.
2. 데이터 조작: 특정 페이지의 특정 인덱스 값을
실시간으로 수정하고 조회하는 기능을 구현하세요.
3. 무한 루프: 사용자가 exit를 입력할 때까지
페이지 생성, 값 수정, 전체 구조 조회를 반복하세요.
4. 제약: 2차원 포인터(int **)를 필수 사용하며,
메모리 해제 시 반드시 가장 깊은 곳부터 역순으로 해제해야 함.

*/

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// 구조체 만들기, page에 이중 포인터 사용 해보기
typedef struct {
  int **pages;    // 페이지의 시작 주소 -> 2차원배열 나타내기 위함
  int *page_size; // 페이지의 사이즈 나타냄 1페이지 길이 a, 2페이지 길이 b
  int page_cnt;   // 전체 페이지 수 총 페이지가 몇페이지 인지
} page_manager;

// 페이지 관리자 구조체 선언
page_manager pm;

// 초기화 함수
void init_page(page_manager *p) {
  p->pages = NULL;
  p->page_size = NULL;
  p->page_cnt = 0;
}

// malloc fail하는 예외처리 하기
bool first_setup_page(page_manager *p) {
  printf("[1] type page_cnt : ");
  scanf("%d", &p->page_cnt);
  printf(">>>page size : %d \n", p->page_cnt);

  // 바로 malloc으로 대입
  p->page_size = malloc(sizeof(int) * p->page_cnt);
  p->pages = malloc(sizeof(int *) * p->page_cnt);
  printf("---------------------------------------------\n");

  for (int i = 0; i < p->page_cnt; i++) {
    printf("[2] type page[%d]_size  : ", i);
    scanf("%d", &p->page_size[i]);
    printf("page[%d]_size : %d \n\n", i, p->page_size[i]);
    // p->page_size[i]d에 대한 malloc 대입
    p->pages[i] = malloc(sizeof(int) * p->page_size[i]);
  }
}

// realloc 고려 해보기
// page 1장 추가
bool add_page(page_manager *p) {
  if (p == NULL) {
  }
  int new_index = p->page_cnt; // 1개 뒤에 삽입
  int new_size;

  // 새로 추가하는 페이지의 크기
  printf("[ADD Page] type new page size : ");
  scanf("%d", &new_size);

  printf("[Inform] new page[%d] size : %d \n\n", new_index, new_size);

  // 카운트 증가
  p->page_cnt++;

  p->page_size = realloc(p->page_size, sizeof(int) * p->page_cnt);
  p->pages = realloc(p->pages, sizeof(int *) * p->page_cnt);

  // 값 저장 page_size, pages 세트로 가져가기
  p->page_size[new_index] = new_size;
  p->pages[new_index] = malloc(sizeof(int) * new_size);

  printf("[Create] new page[%d] size : %d \n\n", new_index, new_size);

  return true;
}
// 참고 코드 사용하여 동적메모리로 나타내기
void print_page() {}

// 2차원 포인터 사용하여, 가장 깊은 곳부터 역순으로 해제
bool free_page() {}

// 선택사항 입력하여 진행
// 1) print 2) 삭제 3)추가 4)수정
void process_loop() {}

bool delete_page() {}

bool revise_page() {}

// 배열의 가로 크기 지정하는 이유는? -> 저장핳 때는, malloc으로?
// 정적메모리 2차원배열 예시 작성하기
// page_size[]의 개수만큼 출력해보기
void _sta_print_2D_Array(int arr[][5], int col, int row) {
  // 단순 예시 코드이므로 동적메모리 출력으로 재작성 필요
  for (int i = 0; i < row; i++) {
    for (int j = 0; j < col; j++) {
      printf("%d ", arr[i][j]);
    }
    printf("\n");
  }
}

// malloc으로 입력 받은 값 생성 -> 초기 데이터 없는 경우에만 가능

int main() {

  first_setup_page(&pm);

  // 4. free 필수
  // free();

  return 0;
}
