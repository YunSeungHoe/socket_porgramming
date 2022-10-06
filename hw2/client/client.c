#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024
void error_handling(char *message);

int main(int argc, char *argv[])
{
    FILE *fp = NULL;
    int sock, fname_len = 0;
    struct sockaddr_in serv_addr;
    // char message[] = "Hello World!";
    // char message[] = "H";
    
    if(argc != 4){
        printf("Usage : %s <IP> <port> <file name>\n", argv[0]);
        exit(1);
    }

    // argv[3]로 입력한 파일의 크기를 계산하기 위한 loop 
    for(int i = 0; argv[3][i] != '\0'; i++)
        fname_len++;
    // 파일을 읽기 모드로 열고, 없으면 error_handling 호출
    fp = fopen(argv[3], "r");
    if(fp == NULL)
        error_handling("fopen() error!");
    
    sock = socket(PF_INET, SOCK_STREAM, 0);
    if(sock == -1)
        error_handling("socket() error!");

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;       
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));

    if(connect(sock, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) == -1)
        error_handling("connect() error!");
    //연결
    printf("sizeof = %d\n", fname_len);
    // 서버로 파일명을 전송
    write(sock, argv[3], fname_len+1);
    while(1){
        // 파일의 내용을 읽어서 보내야한다.
        ;
    }
    // 해제
    close(sock);
    fclose(fp);
    return 0;
}

void error_handling(char* message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}
