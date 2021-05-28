#include "serverFunc.h"

void errorHandler(char* error_type, int asock) {
    char header[BUFSIZE];
    char error_message[100];
    char* error_text = "에러가 발생하였습니다.";
    strcpy(error_message, error_type);
    strcat(error_message, error_text);
    printf("%s\n", error_message);
    fill_header(header, atoi(error_type), sizeof(error_type), "text/html"); //에러타입 보내기
    write(asock, header, strlen(header));
    write(asock, error_type, sizeof(error_type));
}