/*

[문제 1] 동적 가변 구조체 배열과 주소 기반 정렬
문제: 학생 정보를 관리하는 프로그램을 만드세요.
단, 모든 데이터는 입력된 길이에 딱 맞춰 메모리를 할당해야 하며,
정렬 시 실제 데이터를 옮기지 않아야 합니다.
1. 동적 할당: 학생 수(n)를 입력받고,
각 학생의 이름과 전공은 입력받은 문자열 길이에
딱 맞는 크기로 각각 메모리를 할당하세요.
2. 포인터 기반 정렬: 성적(GPA) 순으로 정렬하되,
구조체 데이터를 복사/이동하지 말고 구조체를 가리키는
포인터들의 위치만 바꿔서 정렬을 완성하세요.
3. 반복 실행: 사용자가 종료를 선택할 때까지
학생 정보를 추가하거나 정렬된 결과를 조회할 수 있어야 합니다.
4. 제약: realloc 사용 금지. 모든 메모리는 free 시 누수가 없어야 함.

*/

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

// 배열 대신, 포인터로 선언하기
typedef struct Student{
  char *name;
  char *major;
  int GPA;
  struct Student *next;
} Student;

// 임시 저장용 구조체
typedef struct tmp_data{
    int type_input;
    char temp_name[20];
    char temp_major[20];
    int temp_GPA;
}tmp_data;

tmp_data t;

// 이 구조체를 배열로 선언
typedef struct gpa_list {
  Student *s_addr;
  int GPA;
} gpa_list;

gpa_list s_list[100];
int list_cnt;

Student *set_student(char *temp_name, char *temp_major, int temp_GPA){
  Student *s =malloc(sizeof(Student));
  if (s==NULL){
    printf("Student malloc failed\n");
    return NULL;
  }

  // 이름 넣기
  s->name = malloc(strlen(temp_name)+1);
  if (s->name==NULL){
    printf("s->name malloc failed\n");
    free(s);
    return NULL;
  }

  // 전공 넣기
  s->major = malloc(strlen(temp_major)+1);
  if (s->major==NULL){
    printf("s->major malloc failed\n");
    free(s);
    free(s->name);
    return NULL;
  }

  strcpy(s->name,temp_name);
  strcpy(s->major,temp_major);
  s->GPA = temp_GPA;
  s->next = NULL;

  return s;
}

// 오류 지점. 출력 후에 성적순 포인터 기반 정렬 하기 
// 현재 상태로 동작 멈춤 -> 동작 가능하도록 생각해보기
void append_student(Student **head, Student *s){
  if (s == NULL) {
    printf("set_student is NULL \n");
    return;
  }
  //*head가 NULL일 때 동작 확인
  if (*head == NULL) {
    printf("*head is NULL \n");
    *head=s;
    return;
  }

  Student *cur = *head;
  // cur이 1칸씩 next로 이동
  while (cur->next !=NULL){
    cur = cur ->next;
  }
  //새로운 구조체 추가 -> next에 저장된 구조체를 새로운 구조체로
  cur->next = s;

}

void print_student(const Student *head){
  //const 사용 이유
  const Student *cur = head;

  // cur=cur->next 기반으로 출력
  while (cur != NULL) {
    printf("---------------------------------------------- \n");
    printf("name: %s \n",cur->name);
    printf("major: %s \n",cur->major);
    printf("GPA: %d \n",cur->GPA);

    // next로 이동
    cur=cur->next;
  }
}

// list 배열 형태로 구조체 선언
int make_gpa_list(Student *head, gpa_list list[]) {
  Student *cur = head;
  int count = 0;

  // 순차리스트 출력방식 사용
  while (cur != NULL) {
    list[count].s_addr = cur;
    list[count].GPA = cur->GPA;
    count++;

    cur = cur->next;
  }

  // 저장 개수
  return count;
}

// 정렬 및 출력 모두. 순차리스트와 별개의 인덱스 사용
void print_gpa_list(gpa_list list[], int count) {
  for (int j = 0; j < count - 1; j++) {
    for (int i = 0; i < count - 1; i++) {
      if (list[i].GPA < list[i + 1].GPA) {
        gpa_list tmp = list[i];
        list[i] = list[i + 1];
        list[i + 1] = tmp;
      }
    }
  }

  for (int j = 0; j < count; j++) {
    printf("---------------------------------------------- \n");
    printf("index : %d \n", j);
    printf("name : %s \n", list[j].s_addr->name);
    printf("major : %s \n", list[j].s_addr->major);
    printf("GPA : %d \n", list[j].s_addr->GPA);
  }
}

//free 과정 다시보기 
void free_all(Student **head){
  Student *cur = *head;
  while (cur !=NULL){
    Student *next = cur->next;

    free(cur->name);
    free(cur->major);

    free(cur);

    cur = next;
  }
  *head = NULL;
}

int main() {
  //초기에는 head 값 없음
  Student *head = NULL;

  while (1) {
    t.type_input = 0;
    printf("type (1) : add student, type (2) : print student,type others : "
           "exit \n");
    scanf("%d", &t.type_input);

    if (t.type_input == 1) {
      printf("type name : ");
      scanf("%s", t.temp_name);

      printf("type major : ");
      scanf("%s", t.temp_major);

      printf("type GPA : ");
      scanf("%d", &t.temp_GPA);

      printf("name : %s , major : %s , GPA : %d \n", t.temp_name, t.temp_major,
             t.temp_GPA);

      printf("temp_GPA : %d \n", t.temp_GPA);

      Student *s_1 = set_student(t.temp_name, t.temp_major, t.temp_GPA);

      printf("append_student \n ");
      // append 함수 내에 정렬까지 추가하기
      append_student(&head, s_1);

      printf("print_student \n ");
      print_student(head);
    } else if (t.type_input == 2) {
      printf("make_gpa_list \n ");
      list_cnt = make_gpa_list(head, s_list);
      printf("print_gpa_list \n ");
      print_gpa_list(s_list, list_cnt);
    } else {
      free_all(&head);
      return 0;
    }
  }
}
