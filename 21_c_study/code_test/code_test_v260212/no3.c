#include <stdio.h>
// #include <math.h>

int is_prime(int n);
int k = 0;
int i,odd = 0;
int multiple = 1;



int main() {
  printf("type number :");
  scanf("%d",&k);

  if (is_prime(k)) {
    printf("prime number : O \n");
  }
  else {
    printf("prime number : X \n");
  }
  
  
}

int is_prime(int n) {
  // if문 내에 소수인 경우
  for (i = 2; i < n; i++) {
    odd = n % i;
    multiple = multiple * odd;
  }

  if (multiple) {
    return 1;
  }
  return 0;
}