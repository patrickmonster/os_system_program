// 멀티스레드 락 실험
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>

#define SIZE 3
#define MAX 10

static volatile char count = 0;
static volatile char end = 0;
static volatile char point = 0;
static volatile char *queue;

pthread_cond_t empty;
pthread_cond_t fill;
pthread_mutex_t pmt; // get mutex

struct _data {
    char start,end;
};

void put(char data){
    queue[end++] = data;
    end %= MAX;
    count++;
}

char get(){
    char data= queue[point++];
    point %= MAX;
    count--;
    return data;
}

void *producer(void *arg){
    struct _data *d = (struct _data *)arg;// 구조체 데이터 변환
    char i;
    for (i =d->start; i <= d->end; i++){
        pthread_mutex_lock(&pmt);
        while(count == MAX)
            pthread_cond_wait(&empty,&pmt); // gmt 에 시그널을 보냄
        if(i > 'Z' && i < 'a')i = 'a';
        put(i);
        pthread_cond_signal(&fill);// 시그널 전송
        pthread_mutex_unlock(&pmt);
    }
}

void *consumer(void *arg){
    char i,data;
    struct _data *d = (struct _data *)arg;// 구조체 데이터 변환
    for (i =d->start; i <= d->end; i++){
        pthread_mutex_lock(&pmt);//  뮤텍스 락
        while(count == 0)
            pthread_cond_wait(&fill,&pmt); // gmt 에 시그널을 보냄
        data = get();
        printf(data > 50?"%c":"%d",data);
        printf(i % 10?"\t":"\n");
        pthread_cond_signal(&empty);// 시그널 전송
        pthread_mutex_unlock(&pmt);// 뮤텍스 락 해제
    }
}

int main(int argc, char *argv[]){
    pthread_t pt[SIZE];// 스레드 포인터 배열
    char i;
    struct _data dd[SIZE] = { 
        { .start ='A',.end = 'z'},
        { .start = 1, .end = 50},
        { .start = 1, .end = 102}
    };// 스레드 구조체 정의
    
    queue = calloc(0,sizeof(char)* MAX);// 데이터 저장을 위한 배열
    
    pthread_create(&pt[0],NULL,producer,(void *)&dd[0]);// 스레드 호출(실행)
    pthread_create(&pt[1],NULL,producer,(void *)&dd[1]);// 스레드 호출(실행)
    pthread_create(&pt[2],NULL,consumer,(void *)&dd[2]);// 스레드 호출(실행)

    for (i = 0;i < SIZE; i++)pthread_join(pt[i], NULL);//각 스레드가 중지되는것을 기다림
    
    printf("\n테스트 종료.\n");
    
    free(queue);
    return 0;
}
