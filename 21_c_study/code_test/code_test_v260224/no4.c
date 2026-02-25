/*
4. 날짜 비교 프로그램 (구조체와 열거형)
문제: 년, 월, 일을 멤버로 하는 Date 구조체를 만드세요.
두 날짜를 입력받아 어느 날짜가 더 앞서는지 비교하는 프로그램을 작성하세요.
비교 결과(EARLIER, SAME, LATER)는
**열거형(enum)**으로 정의하여 가독성을 높이세요.



*/

#include <stdio.h>
#include <string.h>
#include <math.h>





int main() {

  struct Date {
    int year;
    int month;
    int day;
  };

  enum COMPARE_DATE {
    EARLIER = 1,
    SAME,
    LATER
  };

  // 날짜 비교항수
  void compare_date(struct Date date1, struct Date date2) {
    int value_date1 = 0;
    int value_date2 = 0;
    int state =0;

    value_date1 = date1.year * 10000 + date1.month * 100 + date1.day;
    value_date2 = date2.year * 10000 + date2.month * 100 + date2.day;

    if (value_date1 < value_date2) {
      state = EARLIER;
    } else if (value_date1 == value_date2) {
      state = SAME;
    } else {
      state = LATER;
    }

    switch (state) {
    case EARLIER:
      printf(" date1 is earlier than date2 \n");
      break;
    case SAME:
      printf(" date1 is same with date2 \n");
      break;
    case LATER:
      printf(" date1 is later than date2 \n");
      break;
    default:
      printf(" wrong \n");
      break;
    }
  }

  struct Date input_date[2];

  for (int i = 0; i < 2; i++) {
    printf("type input_date[%d]'s year : ",i);
    scanf("%d", &input_date[i].year);
    // printf("input_date[%d]'s year : %d \n", i, input_date[i].year);

    printf("type input_date[%d]'s month : ",i);
    scanf("%d", &input_date[i].month);
    // printf("input_date[%d]'s month : %d \n",i,input_date[i].month);


    printf("type input_date[%d]'s day : ",i);
    scanf("%d", &input_date[i].day);

    printf("input_date[%d]'s date : %d.%d.%d  \n",i,input_date[i].year,input_date[i].month,input_date[i].day);
  }

  
  compare_date(input_date[0],input_date[1]);


  return 0;
}