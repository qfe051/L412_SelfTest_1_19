/*
[3번] 단일 연결 리스트 기반 '실시간 성적 필터링 시스템'
문제 개요: 학생 정보를 단일 연결 리스트로 관리하되,
데이터가 항상 성적(GPA) 내림차순으로 정렬된 상태를 유지해야 합니다.
또한, 특정 조건에 맞는 데이터들을
한꺼번에 메모리 해제하는 '필터링' 기능이 핵심입니다.
1. 가변 데이터 구조체:  • Node 구조체는 이름(char*), 학번(int), 성적(double),
다음 노드 주소(next)를 가집니다.
이름은 입력된 길이에 딱 맞춰 malloc 하세요.
2. 명령어 및 기능 (무한 루프):
• add [이름] [학번] [성적]: 학생을 추가합니다.
이때, 단순히 맨 뒤에 붙이는 게 아니라
성적이 높은 순서대로 알맞은 위치를 찾아 삽입하여
항상 정렬된 리스트를 유지하세요.
• filter [기준성적]: 입력한 기준 성적 미만인 모든 학생을
리스트에서 삭제하고 메모리를 즉시 해제하세요.
(단일 연결 리스트에서 연속된 노드를 삭제하는 논리가 꽤 까다롭습니다!)
• print: 현재 리스트에 있는 모든 학생을 순서대로 출력하세요.
3. 강화된 실시간 디버깅:
• 매 명령어 실행 후, 현재 메모리에 할당된 총 학생 수와 이름 저장에 사용된
**순수 문자열 메모리 합계(바이트)**를 출력하세요.
4. 완벽한 메모리 해제:
• exit 시, 리스트의 head부터 NULL까지 모든 노드와
그 안의 이름 문자열을 하나씩 찾아가며 완벽하게 free 하세요.
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
