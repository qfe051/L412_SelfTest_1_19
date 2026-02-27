/*
1. 동적 구조체 배열 정렬 (나이순)
문제: 학생 구조체(이름, 나이)를 정의하고,
입력받은 n명만큼 메모리를 할당하세요.
정보를 입력받은 후 **나이가 적은 순(오름차순)**으로
정렬하여 출력하세요.
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
