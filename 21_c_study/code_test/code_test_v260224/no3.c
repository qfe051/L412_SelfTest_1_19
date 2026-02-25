/*
3. 직원 정보 업데이트 (Call by Reference)
문제: 직원의 이름, 나이, 연봉을 멤버로 하는 Employee 구조체를 정의하세요.
특정 직원의 연봉을 10% 인상시키는 void updateSalary(Employee *p) 함수를
작성하세요.
메인 함수에서 구조체 변수를 선언한 후, 함수에 주소값을 전달하여
연봉이 실제로 수정되는지 확인하세요.


*/

#include <stdio.h>
#include <string.h>
#include <math.h>





int main() {

  struct Employee {
    char name[20];
    int age;
    float salary;
  };



  void updateSalary(struct Employee *p) {
    p->salary = p->salary * 1.1; 
  }


  struct Employee e[10];
  e[1].salary = 1000;


  updateSalary(&e[1]);

  printf("updateSalary : %f ",e[1].salary);



  return 0;
}