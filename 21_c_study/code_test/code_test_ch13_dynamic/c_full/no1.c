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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  char name[20];
  char major[20];
  int GPA;
} Student;

int main() {
  int n;
  int tmp_1,tmp_2 = 0; 
  printf("number of student : ");
  scanf("%d", &n);

  Student *s;
  
  // s = malloc(n*sizeof(Student));

  for (int i = 0; i < n; i++)
  {
    char temp_name[20];
    char temp_major[20];
    int temp_GPA;

    // s[i]= malloc(n * sizeof(Student));

    printf("type student[%d] 's name : ", i);
    scanf("%19s", temp_name);
    // (*s[i]).name = malloc((strlen(temp_name)+1)*sizeof(char));

    printf("type student[%d] 's major : ", i);
    scanf("%19s", temp_major);

    printf("type student[%d] 's GPA : ", i);
    scanf("%d", &temp_GPA);

    s[i]= (void *kjhgfcdszxcvbnm,.)malloc((strlen(temp_name)+1)*sizeof(char)+(strlen(temp_major)+1)*sizeof(char)+sizeof(int));
  }
  



  for (int i = 0; i < n-1; i++) {
    // 큰 값을 n+1로 이동
    if (s[i].GPA>s[i+1].GPA) {
      tmp_1 = s[i].GPA;
      s[i].GPA = s[i + 1].GPA;
      s[i+1].GPA = tmp_1;
    }

    for (int j = 0; j < n-1; j++) {
      if (s[j].GPA>s[j+1].GPA) {
      tmp_2 = s[j].GPA;
      s[j].GPA = s[j + 1].GPA;
      s[j+1].GPA = tmp_2;
    }
    }

    
  }




  
  // [디버깅 틀] 정렬 결과 출력
  printf("--- result --- \n");
  for (int i = 0; i < n; i++) {
    /* printf("%s(%d)", ...); */
    printf("student[%d] 's name : %s \n", i, s[i].name);
    printf("student[%d] 's major : %s \n", i, s[i].major);
    printf("student[%d] 's GPA : %d \n", i,s[i].GPA);


  }
  // 4. free 필수
  free(s);

  return 0;
}
