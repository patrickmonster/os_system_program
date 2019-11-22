// 멀티스레드 실험
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

#define SIZE 5

static volatile int counter = 0;

struct _data {
    int t,out;
    char c;
};
void *mythread(void *arg){
    int i;
    struct _data *d = (struct _data *)arg;// 구조체 데이터 변환
    for (i =0; i <= 1000000; i++){
        counter ++;// 글로벌 변수
        printf("[%c] %d usec\n", d->c,d->t);
        usleep(d->t);
    }
    d-> out = counter;
}

int main(int argc, char *argv[]){
    pthread_t pt[SIZE];// 스레드 포인터 배열
    int i;
    struct _data dd[SIZE] = { 
        { .t = 100, .c = 'A', .out = 0},
        { .t = 50, .c = 'B', .out = 0},
        { .t = 30, .c = 'C', .out = 0},
        { .t = 30, .c = 'D', .out = 0},
        { .t = 10, .c = 'E', .out = 0},
    };// 스레드 구조체 정의

    for (i = 0; i < SIZE; i++)
        pthread_create(&pt[i],NULL,mythread,(void *)&dd[i]);// 스레드 호출(실행)

    for (i = 0;i < SIZE; i++)pthread_join(pt[i], NULL);//각 스레드가 중지되는것을 기다림

    for (i = 0;i < SIZE; i++)printf("counter 값: %d\n", dd[i].out);//각 스레드가 중지되는것을 기다림

    printf("\n테스트 종료.\n");
    
    return 0;
}
