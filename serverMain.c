#define MAXTHREAD 10
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "serverFunc.h"

int clientnum = 0;
int getFD = -1;
int id = 0;
sem_t consume;
Queue queue;

int bindSock(int lsock, int port){
    struct sockaddr_in sin; //sa_family (address family) 및 IP addr + Portnum

    sin.sin_family = AF_INET; //뭘로 통신할지
    sin.sin_addr.s_addr = htonl(INADDR_ANY); //호스트 아이피 
    sin.sin_port = htons(port); //포트
    return bind(lsock, (struct sockaddr *)&sin, sizeof(sin));
}


void *sckThread(void* sckArg) //쓰레드 설정
{
    int tid = id++; // 쓰레드 ID
    int asock = -1;

    while(1){
        sem_wait(&consume); //consume 세마포어 1 줄임
        asock = getFD; //global에서 socketfd 받아오고
        toQueue(&queue,asock); //큐에 asock 넣어주고
        getFD = -1; //global 변수 초기화 시키고
        
        printf("Queue size = %d\n", queue.count);
        

        
        popQueue(&queue);
        clientnum--;
        close(asock); //소캣 꺼주고
    }

    return NULL;   

}

int main(int argc, char **argv){
    int port, pid;
    int lsock, asock;
    int i; //세마포어 카운터
    pthread_t sckPth[MAXTHREAD]; //쓰레드 선언
    struct sockaddr_in remote_sin;
    socklen_t remote_sin_len;
    initQueue(&queue, MAXTHREAD); //큐 초기화

    if (argc<2){
        printf("Useage : \n");
        printf("\t%s {port} : runs mini HTTP server \n", argv[0]);
    }
    port = atoi(argv[1]);
    printf("Start at port numver : %d. \n", port);

    lsock = socket(AF_INET, SOCK_STREAM, 0);
    if (lsock <0){
        perror("Failed to create lsock \n");
        exit(1);
    }
    if (bindSock(lsock,port) < 0 ){
        perror("Failed to bind lsock \n");
        exit(1);
    }
    if (listen(lsock, 10) < 0) {
        perror("Failed to listen lsock.\n");
         exit(1); 
    }

    printf("socket is waiting..\n");

    sem_init(&consume,0,0); //세마포어 0으로 초기화
    for(i=0; i<MAXTHREAD; i++){
        pthread_create(&sckPth[i], NULL, sckThread, (void *)&i);
        printf("Process is created : %d\n",i);
    }
    //Accept는 MainThread에서 (왜?)
    while(1){
        while((asock = accept(lsock, (struct sockaddr *)&remote_sin, &remote_sin_len))>=0){ //accept

        clientnum++; //accept시 clientnum 늘리기
        printf("현재 %d / %d명 수용중 \n", clientnum, MAXTHREAD);
        if(clientnum > MAXTHREAD){
            //클라이언트가 최대보다 많으면 이거 보내기
            clientnum--;
        }
        else{
            getFD = asock; //쓰레드에 전역변수로 보냄
            sem_post(&consume); //세마포어 1 늘리기
        }

        }        
    }
    close(lsock);
    return 0;
}