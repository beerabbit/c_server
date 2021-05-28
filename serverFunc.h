//헤-더
#define NEXT(index,QSIZE) ((index+1)%QSIZE) //큐 인덱스 변경
#define BUFSIZE 1000
#define HEADER_FMT "HTTP/1.1 %d %s\nContent-Length: %ld\nContent-Type: %s\n\n"
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <arpa/inet.h>
typedef struct Queue{

    int *buf; //저장하는 곳
    int qsize;
    int front; //꺼낼 위치
    int rear; //보관 위치
    int count; //저장한 개수

} Queue;

void fill_header(char *header, int status, long len, char *type); //헤더 채우기
void initQueue(Queue *queue, int qsize); //큐 초기화
int isFull(Queue *queue); // 큐 찼나
int isEmpty(Queue *queue); //큐 비었나
void toQueue(Queue *queue, int data); //큐 보관
int deQueue(Queue *queue); //큐에서 빼기
void delQueue(Queue *queue); // 큐 해제
void popQueue(Queue *queue); //큐 pop
void find_mime(char *ct_type, char *uri); //mime check
void errorHandler(char* error_type, int asock); //에러 핸들러