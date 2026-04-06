#include <stdio.h>
#include <stdbool.h>

// 버퍼의 최대 크기를 매크로로 정의 (컴파일 시 고정됨)
#define BUFFER_SIZE 5

// 정적 배열을 포함한 링버퍼 구조체 정의
typedef struct {
    int buffer[BUFFER_SIZE]; // 크기가 고정된 정적 배열
    int front;                // 데이터를 읽을 위치 (Read Pointer)
    int rear;                // 데이터를 쓸 위치 (Write Pointer)
    int size;                // 현재 저장된 데이터의 개수
} RingBuffer;

// 링버퍼 초기화 함수
void initRingBuffer(RingBuffer* rb) {
    rb->front = 0;
    rb->rear = 0;
    rb->size = 0;
}

// 버퍼가 비었는지 확인
bool isEmpty(RingBuffer* rb) {
    return rb->size == 0;
}

// 버퍼가 꽉 찼는지 확인
bool isFull(RingBuffer* rb) {
    return rb->size == BUFFER_SIZE;
}

// 데이터 추가 (Enqueue / Write)
void enqueue(RingBuffer* rb, int item) {
    if (isFull(rb)) {
        printf("buffer is full. overwrite oldest one : %d add \n", item);
        // 가득 찬 상태에서 덮어쓰려면 front 포인터도 한 칸 이동시켜야 합니다.
        rb->front = (rb->front + 1) % BUFFER_SIZE;
    } else {
        rb->size++;
        printf("add data: %d\n", item);
    }

    rb->buffer[rb->rear] = item;
    // rear 포인터를 다음 위치로 이동 (배열 끝에 도달하면 0으로 순환)
    rb->rear = (rb->rear + 1) % BUFFER_SIZE;
}

// 데이터 추출 (Dequeue / Read)
int dequeue(RingBuffer* rb, bool* success) {
    if (isEmpty(rb)) {
        printf("buffer is empty.\n");
        *success = false;
        return -1; // 에러를 의미하는 임의의 값 반환
    }

    int item = rb->buffer[rb->front];
    // front 포인터를 다음 위치로 이동 (배열 끝에 도달하면 0으로 순환)
    rb->front = (rb->front + 1) % BUFFER_SIZE;
    rb->size--;
    
    *success = true;
    return item;
}

// 현재 버퍼 상태 출력
void display(RingBuffer* rb) {
    printf("buffer (size: %d) | front: %d | rear: %d\n", rb->size, rb->front, rb->rear);
    printf("current data : ");
    
    if (isEmpty(rb)) {
        printf("empty\n");
    } else {
        int count = 0;
        int i = rb->front;
        while (count < rb->size) {
            printf("%d ", rb->buffer[i]);
            i = (i + 1) % BUFFER_SIZE; // 나머지 연산으로 순환하며 출력
            count++;
        }
        printf("\n");
    }
    printf("----------------------------------------\n");
}

// 메인 함수 (테스트)
int main() {
    RingBuffer rb; // 스택 메모리에 링버퍼 구조체 할당 (정적)
    
    // 포인터를 넘겨 초기화 진행
    initRingBuffer(&rb);

    enqueue(&rb, 10);
    enqueue(&rb, 20);
    enqueue(&rb, 30);
    display(&rb);

    bool success;
    int data = dequeue(&rb, &success);
    if (success) {
        printf("read data: %d\n", data);
    }
    display(&rb);

    enqueue(&rb, 40);
    enqueue(&rb, 50);
    enqueue(&rb, 60);
    display(&rb);

    // 버퍼가 가득 찬 상태에서 데이터 추가 (가장 오래된 20이 덮어써지고 70이 들어감)
    enqueue(&rb, 70);
    display(&rb);

    // 정적 할당이므로 free() 함수를 호출할 필요가 없습니다.

    return 0;
}
