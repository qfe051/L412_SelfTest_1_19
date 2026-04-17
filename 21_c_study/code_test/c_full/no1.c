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

#define MAX_NUM_S 100

// Student 나타내는 순차리스트
typedef struct Student{
  char *name;
  char *major;
  int GPA;
  struct Student *next;
} Student;

// 반복 입력, 임시 저장용 구조체
typedef struct tmp_data{
    int type_input;
    char temp_name[20];
    char temp_major[20];
    int temp_GPA;
}tmp_data;

//tmp_data 구조체 선언
tmp_data t;

//  성적 순 정렬용 구조체 : Student 구조체, 점수
typedef struct gpa_list {
  Student *s_addr;
  int GPA;
} gpa_list;

//  구조체를 배열로 선언하여 MAX_NUM_S 만큼 받음
gpa_list s_list[MAX_NUM_S];

// status 구조체 : 저장된 학생 수 count 및 process 저장
typedef struct status {
  int count;
  int now_process;
  int process_cnt;
} status;
// status 구조체 선언
status sta;

// 타이핑으로 얻을 3가지 Process
enum S_Process{
  ADD_STUDENT =1,
  PRINT_STUDENT_GPA,
  EXIT
};

//초기에는 head 값 없는 상태 선언
Student *head = NULL;

// 동적메모리를 사용하여 Student 구조체 선언 | 입력 : tmp_data 구조체, 출력: Student 구조체
Student *set_student(tmp_data *t){
  Student *s =malloc(sizeof(Student));
  if (s==NULL){
    printf("Student malloc failed\n");
    return NULL;
  }

  // 이름 넣기
  s->name = malloc(strlen(t->temp_name)+1);
  if (s->name==NULL){
    printf("s->name malloc failed\n");
    free(s);
    return NULL;
  }

  // 전공 넣기
  s->major = malloc(strlen(t->temp_major)+1);
  if (s->major==NULL){
    printf("s->major malloc failed\n");
    free(s);
    free(s->name);
    return NULL;
  }

  strcpy(s->name,t->temp_name);
  strcpy(s->major,t->temp_major);
  s->GPA = t->temp_GPA;
  s->next = NULL;

  return s;
}

// 학생 추가 함수 | 입력 : Student 구조체 head 의 주소, student 구조체 s 출력 : T/F
bool append_student(Student **head, Student *s){
  if (s == NULL) {
    printf("set_student is NULL \n");
    return false;
  }
  //*head가 NULL일 때 동작 확인
  if (*head == NULL) {
    printf("*head is NULL \n");
    *head=s;
    return true;
  }

  Student *cur = *head;
  // cur이 1칸씩 next로 이동
  while (cur->next !=NULL){
    cur = cur ->next;
  }
  //새로운 구조체 추가 -> next에 저장된 구조체를 새로운 구조체로
  cur->next = s;
  return true;
}
// 학생 출력 함수(순서리스트 순서(append 순서)) | 입력 : 구조체 head, 출력 T/F
bool print_student(const Student *head){
  if (head == NULL)
  {
    printf("head is NULL \n");
    return false;
  }
  

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

  return true;
}

// Student 구조체. list 배열 형태로 정리(구조체 주소,GPA) | 입력 : Student 구조체 head, gpa_list 구조체 list[],status 구조체
// 출력 T/F
bool make_gpa_list(Student *head, gpa_list list[],status *sta) {
  if (list ==NULL)
  {
    printf(">>>>>>>>>>>>>>>>>> ");
    printf("No list - make_gpa_list \n");

    return false;
  }

  sta->count=0;

  Student *cur = head;
  

  // 순차리스트 출력방식 사용
  while (cur != NULL) {
    list[sta->count].s_addr = cur;
    list[sta->count].GPA = cur->GPA;
    sta->count++;

    cur = cur->next;
  }

  // 저장 개수
  return true;
}

// gpa_list 구조체 list[] 내부 정렬 및 출력 모두. 순차리스트와 별개의 인덱스 사용
// 입력 gpa_list 구조체 list[], status 구조체, 출력 T/F
bool print_gpa_list(gpa_list list[], status *sta) {
  if (list ==NULL || sta->count==0)
  {
    printf(">>>>>>>>>>>>>>>>>> ");
    printf("No data - print_gpa_list \n");

    return false;
  }
  
  for (int j = 0; j < sta->count - 1; j++) {
    for (int i = 0; i < sta->count - 1; i++) {
      if (list[i].GPA < list[i + 1].GPA) {
        gpa_list tmp = list[i];
        list[i] = list[i + 1];
        list[i + 1] = tmp;
      }
    }
  }

  for (int j = 0; j < sta->count; j++) {
    printf("---------------------------------------------- \n");
    printf("index : %d \n", j);
    printf("name : %s \n", list[j].s_addr->name);
    printf("major : %s \n", list[j].s_addr->major);
    printf("GPA : %d \n", list[j].s_addr->GPA);
  }

  return true;
}

//Exit시 전체 free
// 입력 : Student 구조체 head의 주소
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

//전체 시스템 진행 함수
// 입력:tmp_data 구조체,status 구조체 . 출력 : T/F
bool process_system(tmp_data *t,status *sta){
  if (t==NULL || sta ==NULL){
    printf(">>>>>>>>>>>>>>>>>> ");
    printf("No data - process_system \n");
  }
  

  // while (!(t->type_input==1||t->type_input==2||t->type_input==3)){}
    t->type_input = 0;
    printf("type (1) : add student \n type (2) : print student \n type (3) : "
            "exit \n");
    scanf("%d", &t->type_input);
  
  
  sta->now_process=t->type_input;

  switch (sta->now_process)
  {
  case ADD_STUDENT:
    printf("type name : ");
    scanf("%s", t->temp_name);

    printf("type major : ");
    scanf("%s", t->temp_major);

    printf("type GPA : ");
    scanf("%d", &t->temp_GPA);

    Student *s_1 = set_student(t);

    
    // append 함수 내에 정렬까지 추가하기
    if (append_student(&head, s_1)){
    printf("append_student is done \n ");
 
    if(print_student(head)){
      printf("print_student is done \n ");
    }
    }
    
    return true;
    break;
  case PRINT_STUDENT_GPA:
    //if문으로 함수 true일 때 실행
    if (make_gpa_list(head, s_list,sta))
      {
        printf("make_gpa_list is done \n\n ");
        if (print_gpa_list(s_list, sta))
        {
          printf("print_gpa_list is done \n\n ");
        }
      }
      return true;
      break;
  case EXIT:
    printf("process is done \n\n ");
    free_all(&head);

    return true;
    break;
  default:
    printf("type again \n ");
    return false;
    break;
  }

}

int main() {
  
while(1){
  sta.process_cnt++;
  printf("------------------------\n");
  printf("process_cnt : %d \n",sta.process_cnt);
  printf("------------------------\n");


    //false 반환하면 재반복
      if (process_system(&t,&sta))
      {
        sta.process_cnt=0;
      }
      else{
        
      }

    
    if (sta.now_process==EXIT)
    {
      printf("process is done ,.......... \n\n ");

      return 0;
    }
    

  }
}
