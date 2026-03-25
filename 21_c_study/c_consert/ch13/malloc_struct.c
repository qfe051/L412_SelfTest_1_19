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
Student *create_student(const char *name, int subject_count,
                        const int *input_scores) {
  // new_student 라는 이름의 구조체 선언 
  Student *new_student = (Student *)malloc(sizeof(Student));
  if (new_student == NULL) {
    printf("new_student malloc failed \n");
    return NULL;
  }

  // 내부 코드 동작
  // 함수 입력받은 name, 구조체 내부에 넣기 - 'new_student->name'
  // -1 하는 이유는 name[19] 써서?
  strncpy(new_student->name, name, sizeof(new_student->name) - 1);
  // 마지막에 NULL 추가 
  new_student->name[sizeof(new_student->name)-1] = '\0';

  // 함수 입력받은 subject_count, 구조체에 넣기
  new_student->subject_count = subject_count;
  // 다음주소 NULL 선언
  new_student->next = NULL;

  // 점수배열 동적 할당 -> 과목 수 만큼 배열 크기를 늘려준다 생각하기 
  new_student->scores = malloc(sizeof(int) * subject_count);
  if (new_student->scores == NULL) {
    printf("score malloc failed \n");
    free(new_student);
    return NULL;
  }

  // subject_count 만큼 scores[i] 입력 
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
  // head 비어있으면 head에 넣기 
  if (*head == NULL) {
    *head = new_student;
    return;
  }

  // 하나씩 땡겨오는 동작?  - cur에 next를 저장하면, next가 다음에 cur이 되는 느낌?
  Student *cur = *head;
  while (cur->next != NULL) {
    cur = cur -> next;
  }

  cur -> next = new_student;
}

void print_students(const Student *head) {
  // cur이 append 말고 여기서 선언?
  const Student *cur = head;

  while (cur != NULL) {
    printf("student name : %s \n", cur->name);
    printf("subject count : %d \n",cur->subject_count);
    printf("score : ");

    for (int i=0; i< cur-> subject_count; i++) {
      printf("%d ,",cur->scores[i]);
    }
    printf("\n");

    // 평균 계산 -> 구조체 내부에 저장 안하고 print할 때 연산 함
    int sum = 0;

    for (int i=0; i< cur-> subject_count; i++) {
      sum += cur->scores[i];
    }

    double avg = 0.0;
    if (cur->subject_count > 0) {
      //(double) 자료형 명시 필요한지 확인 
      avg = sum / cur->subject_count;
    }

    printf("avg : %.2f \n", avg);
    printf("----------------------- \n");

    cur = cur ->next;
  }
}

// 순차적인 free 동작
void free_students(Student **head) {
  // head 부터 진행
  Student *cur = *head;

  while (cur !=NULL) {
    Student *next = cur->next;

    // score 구조체 노드 먼저 해제 (내부 먼저)

    free(cur->scores);

    // 전체 노드 해제
    free(cur);

    cur = next;
  }

  *head = NULL;
}

// 함수 한번 더 체크하고 동작 확인하기 
int main(void) {



  return 0;
}
