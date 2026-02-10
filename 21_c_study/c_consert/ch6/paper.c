#include <stdio.h>

int main(void) {

  // 에베레스트에만 const 키워드 쓰는 이유는?
  double p_height = 0.001; // 종이 단위
  const double e_height = 8848; // 에베레스트 산 높이
  int count = 0;

  while (p_height < e_height) {
    p_height *= 2;
    count++;
  }
  printf("종이 접은 횟수 : %d", count);

  return 0;
}