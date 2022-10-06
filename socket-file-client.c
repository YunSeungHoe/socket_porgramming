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
    int sock;
    struct sockaddr_in serv_addr;
    char message[BUF_SIZE];
    int str_len;
    FILE* fp = NULL;
    // char filename[BUF_SIZE];
    char *filename;

    if(argc != 4){
        printf("Usage : %s <IP> <port> <filename>\n", argv[0]);
        exit(1);
    }
    
    sock = socket(PF_INET, SOCK_STREAM, 0);
    if(sock == -1)
        error_handling("socket() error!");

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;       
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));

    if(connect(sock, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) == -1)
        error_handling("connect() error!");
    // 연결 성공

    
    filename = argv[3];
    fp = fopen(filename, "r");
    if(fp == NULL){
        error_handling("file open error!");
    }
    else 
    {
        write(sock, filename, BUF_SIZE);
        printf("file open!!!!\n");
    }

    // str_len = read(sock, message, sizeof(message)-1);
    // if(str_len == -1)
    //     error_handling("read() error!");
    
    // printf("Message from server : %s \n", message);
    while(1){
        ;
    }
    // 연결 해제
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
