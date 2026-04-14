/*
 * Chapter 1. 자료구조와 알고리즘 기초
 *
 * [학습 목표]
 * 1. 시간복잡도가 무엇인지 이해한다.
 * 2. 차수(가장 큰 영향 항)의 의미를 이해한다.
 * 3. 순차탐색과 이진탐색의 차이를 코드로 확인한다.
 *
 * [핵심 정리]
 * - 시간복잡도는 입력 크기 n이 증가할 때 연산량이 어떻게 증가하는지 나타낸다.
 * - 복잡도 식에서는 가장 큰 차수의 항이 전체 성능을 결정한다.
 * - 순차탐색은 O(n), 이진탐색은 O(log n)이다.
 * - 단, 이진탐색은 "정렬된 데이터"에서만 사용할 수 있다.
 */

#include <stdio.h>
#include <stdbool.h>

// 순차 탐색. 정렬되지 않은 데이터에서 사용 가능한 이유는?

int linear_search(const int arr[], int size, int target, int *compare_count) {
  *compare_count = 0;
  for (int i =0; i  <size; i++) {
    (*compare_count)++;

    if (arr[i] == target) {
      return i;
    }
  }
  return -1;
}

// 이진탐색. 정렬된 배열에서 사용 이유는?
int binary_search(const int arr[], int size, int target, int *compare_count) {
  int left = 0;
  int right = size - 1;
  *compare_count = 0;

  while (left <= right) {
    //left~right 간격의 절반만큼만 이동
    int mid = left + (right - left) / 2;
    (*compare_count)++;

    if (arr[mid] == target) {
        return mid;
    }// 중앙값이 target 보다 작으면 left쪽 다 버림
    else if (arr[mid]<target) {
        left = mid +1;
    }// 중앙값이 target 보다 작으면 right쪽 다 버림
    else {
        right = mid+1;
    }
  }
  return -1;
}