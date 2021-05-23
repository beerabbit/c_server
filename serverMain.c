#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <arpa/inet.h>


int bindSock(int lsock, int port){
    struct sockaddr_in sin; //sa_family (address family) 및 IP addr + Portnum

    sin.sin_family = AF_INET; //뭘로 통신할지
    sin.sin_addr.s_addr = htonl(INADDR_ANY); //호스트 아이피 
    sin.sin_port = htons(port); //포트
    return bind(lsock, (struct sockaddr *)&sin, sizeof(sin));
}

typedef struct sockThreadArg{
    //쓰레드에 보낼 것

} sockThreadArg, *PsockThreadArg;
void *sckThread(void* sckArg) //쓰레드설정
{

}

int main(int argc, char **argv){
    int port, pid;
    int lsock, asock;

    struct sockaddr_in remote_sin;
    socklen_t remote_sin_len;

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
    pthread_t sckPth;
    int sckth;
    /*if(sckth = pthread_create(&sckPth, NULL, sckThread, (void *)&sckArg)){
        printf("Thread Err \n");
    }
    pthread_join(sckPth, NULL);*/ //쓰레드 설정해야함
    int num = 0;
    while((asock = accept( lsock, (struct sockaddr *)&remote_sin, &remote_sin_len) >=0 )){
    printf(" %d 명 들어왔네요 \n", num);
    num++;
        if (asock < 0){      
            perror("Failed to accept \n");
            exit(0);
        }
    }

    
}