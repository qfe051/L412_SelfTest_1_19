/*
[보너스] 이중 연결 리스트 기반 '웹 브라우저 히스토리'
문제 개요: 웹 브라우저의 '뒤로 가기'와 '앞으로 가기' 기능을
시뮬레이션하는 프로그램을 만드세요.
모든 방문 기록(URL)은 이중 연결 리스트로 관리되어야 하며,
새로운 페이지를 방문할 때마다 메모리가 동적으로 할당되어야 합니다.
1. 이중 연결 리스트 구조체:
• Node 구조체는 URL(char[50]), 이전 노드 주소(prev),
다음 노드 주소(next)를 가져야 합니다.
2. 명령어 및 기능 (반복 실행):
• visit [URL]: 새로운 페이지를 방문합니다.
현재 위치에서 새로운 노드를 생성해 연결하고,
**기존의 '앞으로 가기' 내역은 모두 해제(free)**해야 합니다. (이게 핵심!)
• back: 현재 위치를 이전 노드로 옮깁니다.
(이전 노드가 없다면 "시작 페이지입니다" 출력)
• forward: 현재 위치를 다음 노드로 옮깁니다.
(다음 노드가 없다면 "마지막 페이지입니다" 출력)
• history: 지금까지의 모든 방문 기록을 순서대로 출력하고,
현재 내가 있는 위치를 표시하세요.
3. 강화된 실시간 디버깅:
• 매 명령어 실행 후, 현재 메모리에 할당된 총 방문 기록 노드 수와 현
재 위치의 URL을 출력하세요.
4. 완벽한 메모리 해제:
• exit 시, 리스트의 처음부터 끝까지
모든 노드를 추적하며 하나도 빠짐없이 free 하세요.
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
