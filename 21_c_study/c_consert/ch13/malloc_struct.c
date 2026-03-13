#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct Student {
  char name[20];
  int subject_count;
  int *scores; // 점수 -> 학생마다 점수 개수가 다른 경우
  struct Student *next;
}Student;

// 구조체에서 함수 선언하는 개념 알아보기
Student* create_student(const char *name, int subject_count,
                        const int *input_scores) {
  Student *new_student = (Student *)malloc(sizeof(Student));
  if (new_student == NULL) {
    printf("new_student malloc failed \n");
    return NULL;
  }

  // 내부 코드 동작
  strncpy(new_student->name, name, sizeof(new_student->name) - 1);
  // 마지막에 NULL 추가 
  new_student->name[sizeof(new_student->name)-1] = '\0';

  new_student->subject_count = subject_count;
  new_student->next = NULL;

  // 점수배열 동적 할당
  new_student->scores = malloc(sizeof(int) * subject_count);
  if (new_student->scores == NULL) {
    printf("score malloc failed \n");
    free(new_student);
    return NULL;
  }

  for (int i = 0; i<subject_count; i++) {
    new_student->scores[i] = input_scores[i];
  }

  return new_student;
}

// 리스트 맨 뒤에 학생 추가, 앞서 선언해둔 구조체 멤버의 구조체 '*next' 활용하기
void append_student(Student **head, Student *new_student) {
  // 비어있는 경우, 다음꺼 추가 하지 않도록 함
  if (new_student == NULL) {
    return;
  }
  if (*head == NULL) {
    *head = new_student;
    return;
  }

  // 하나씩 땡겨오는 동작? 
  Student *cur = *head;
  while (cur->next != NULL) {
    cur = cur -> next;
  }

  cur -> next = new_student;
}


int main(void) {



  return 0;
}
