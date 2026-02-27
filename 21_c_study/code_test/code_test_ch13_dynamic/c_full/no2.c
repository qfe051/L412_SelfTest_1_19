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

#include <stdio.h>
#include <stdlib.h>
typedef struct {
  char name[20];
  int age;
} Student;

int main() {
  int n;
  int tmp_1,tmp_2 = 0; 
  printf("number of student : ");
  scanf("%d", &n);
  // 1. n명만큼 malloc / 2. 정보 입력 / 3. 나이순 정렬
  Student *s1;

  s1= malloc(n * sizeof(Student));

  for (int i = 0; i < n; i++) {

    printf("type student[%d] 's name : ", i);
    scanf("%19s", s1[i].name);

    printf("type student[%d] 's age : ", i);
    scanf("%d", &s1[i].age);
  }

  for (int i = 0; i < n-1; i++) {
    // 큰 값을 n+1로 이동
    if (s1[i].age>s1[i+1].age) {
      tmp_1 = s1[i].age;
      s1[i].age = s1[i + 1].age;
      s1[i+1].age = tmp_1;
    }

    for (int j = 0; j < n-1; j++) {
      if (s1[j].age>s1[j+1].age) {
      tmp_2 = s1[j].age;
      s1[j].age = s1[j + 1].age;
      s1[j+1].age = tmp_2;
    }
    }


    // 작은 값을 n-1로 이동
    
  }




  
  // [디버깅 틀] 정렬 결과 출력
  printf("--- result --- \n");
  for (int i = 0; i < n; i++) {
    /* printf("%s(%d)", ...); */
    printf("student[%d] 's name : %s \n", i, s1[i].name);
    printf("student[%d] 's age : %d \n", i,s1[i].age);


  }
  // 4. free 필수
  free(s1);

  return 0;
}
