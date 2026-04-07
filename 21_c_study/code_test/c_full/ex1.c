#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct Student {
  char name[20]; // 학생 이름
  int subject_count; // 과목 수
  int *scores; // 점수 배열 (동적 메모리) -> 과목 수에 따라 점수 개수가 달라짐
  struct Student *next; // 다음 학생 노드
} Student;

// 학생 1명 생성하는 함수
Student *create_student(const char *name, int subject_count,
                        const int *input_scores) {
  // 구조체 선언 -> 구조체 사이즈만큼 선언
  Student *new_student = (Student *)malloc(sizeof(Student));
  if (new_student == NULL) {
    printf("malloc failed \n");
    return NULL;
  }

  // 입력된 name을 구조체로 
  // strlen 안쓰고 sizeof 에서 -1 한 이유는?
  strncpy(new_student->name, name, sizeof(new_student->name) - 1);
  // 문자열 null 처리
  new_student->name[sizeof(new_student->name) - 1] = '\0';

  new_student->subject_count = subject_count;
  // next는 왜 NULL?

  // 점수배열 subject_count 크기에 따라 동적 할당
  new_student->scores = (int *)malloc(sizeof(int) * subject_count);
  if (new_student->scores == NULL) {
    printf("malloc failed \n");
    // 앞서 처리했던 malloc 해제 함
    free(new_student);
    return NULL;
  }

  // input_scores는 어디선가 배열로 선언된 상태?
  for (int i =0; i < subject_count; i++) {
    new_student->scores[i] = input_scores[i];
  }

  // 생성된 구조체를 return
  return new_student;
}

// c언어에서 head가 뜻하는 것은? 앞? 뒤?
void append_student(Student **head, Student *new_student) {
    // 하나 이상은 있어야
 if (new_student == NULL) {
    return;
 }
    // head 없으면 넣어줌
  if (*head == NULL) {
    *head = new_student;
    return;
  }

  // 왜 head를 cur에 넣음?
  Student *cur = *head;
  // next값을 계속 cur에 업데이트 -> 한 칸씩 next로 이동
  while (cur->next != NULL) {
    cur = cur->next;
  }

  // 마지막 cur->next에는 새로운 학생 값 받아줌
  cur->next = new_student;
}

// 전체 학생 출력 cur->next 와 같은 이동 구조 생성
void print_students(const Student *head) {
  //head를 cur에 ㄷ고 이동하면서 출력 진행
  const Student *cur = head;

  while (cur != NULL) {
    printf("Name : %s \n", cur->name);
    printf("subject_count : %s \n", cur->subject_count);
    printf("score : ");

    for (int i =0; i< cur->subject_count; i++) {
        printf("%d",cur->scores[i]);
    }
    printf("\n");

    cur = cur ->next;
  }
}