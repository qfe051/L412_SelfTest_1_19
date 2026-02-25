/*
1. 성적 관리 시스템 (구조체 배열과 포인터)
문제: 학생의 이름(문자열), 학번(정수), 학점(실수)을 멤버로 가지는 Student
구조체를 만드세요.
학생 3명의 정보를 입력받아 배열에 저장한 뒤,
학점이 가장 높은 학생의 모든 정보를 출력하는 프로그램을 작성하세요.
*/

#include <stdio.h>
#include <string.h>

 struct student {
    char name[20];
    int number;
    float score;
  };

int main() {

 

  float max_score = 0;
  int max_index = 0;

  struct student stu[3];

  for (int i = 0; i < 3; i++) {
    printf("type stu[%d]'s number : ",i);
    scanf("%d", &stu[i].number);
    printf("stu[%d]'s number : %d \n",i,stu[i].number);

    printf("type stu[%d]'s score : ",i);
    scanf("%f", &stu[i].score);
    printf("stu[%d]'s score : %.2f \n",i,stu[i].score);

    printf("type stu[%d] 's name : ", i);
    // fgets(stu[i].name, sizeof(stu[i].name), stdin);
    scanf("%19s",stu[i].name);
    printf("stu[%d]'s name : %s \n",i,stu[i].name);
    

    

    if (max_score < stu[i].score) {
      max_score = stu[i].score;
      max_index = i;
    }
  }

  printf("Highest Student Information \n");
  printf("std[%d] 's name : %s \n",max_index, stu[max_index].name);
  printf("std[%d] 's number : %d \n", max_index, stu[max_index].number);
  printf("std[%d] 's score : %.2f \n",max_index,stu[max_index].score);

  return 0;
}