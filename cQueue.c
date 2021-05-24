// C에는 Queue가 없나? 없으면 만든다.
// Array, LinkedList중 Array로 구현 (원형 큐) / rear 다음 front면 꽉 찬것으로 취급
//int형만 가능
#include <stdio.h>
#include <stdlib.h>
#include "serverFunc.h"

void initQueue(Queue *queue, int qsize){
    queue->buf = (int *)malloc(sizeof(int)*qsize); //qsize만큼 buf 공간 할당 (사실 int만큼임)
    queue->qsize = qsize; //qsize 할당한 만큼 만들기
    queue->front = queue->rear = 0; //앞 뒤를 0으로 설정
    queue->count = 0; //보관 개수 0개
}
int isFull(Queue *queue){
    return queue->count == queue->qsize; //개수가 qsize만큼 있으면 꽉 찬거지
}

int isEmpty(Queue *queue){
    return queue->count == 0; //0개면 없는거지
}

void toQueue(Queue *queue, int data){
    if(isFull(queue)) //큐가 꽉 찼을 때
        return ;
    queue->buf[queue->rear] = data; //맨 뒤 rear에 데이터 보관
    queue->rear = NEXT(queue->rear,queue->qsize); //index++ 하고 rear 다음 위치
    queue->count++;
}
int deQueue(Queue *queue){
    int res = 0;
    if(isEmpty(queue)) //큐가 없으면
        return 1;
    res = queue->buf[queue->front]; //front값을 res로 빼고
    queue->front = NEXT(queue->front, queue->qsize); //front 인덱스 변경
    queue->count--;
    return res;
}
void delQueue(Queue *queue){
    free(queue->buf);
}
void popQueue(Queue *queue){
    if(isEmpty(queue))
        return;
    queue->front = NEXT(queue->front, queue->qsize); //front 인덱스 변경
    queue->count--;
}