#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/stat.h>

#define BUF_SIZE 1024
void error_handling(char *message);
int filesize(const char  *filename);

int main(int argc, char *argv[])
{
    FILE *fp = NULL;
    int sock, file_size, fname_len = 0, send_len = 0, send_cnt = 0;
    struct sockaddr_in serv_addr;
    char message[BUF_SIZE];
    char name_len[BUF_SIZE];
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
    // 연결 시작
    // 서버로 파일 이름의 크기를 먼저 전송한다.
    name_len[0] = (char)fname_len;
    write(sock, name_len, sizeof(char)+1);
    // 서버로 파일명을 전송한다.
    write(sock, argv[3], fname_len+1);
    // 파일 안에있는 내용의 크기를 확인하여 저장한다. 
    // 버퍼보다 큰 값을 보낼 경우를 대비하여 보낸 데이터의 크기를 비교하기 위해 사용한다.
    file_size = filesize(argv[3]);
    // 파일 안에 내용의 크기를 확인하는 디버깅용.
    // printf( "file size=%d\n", filesize(argv[3]));
    // while을 사용하여 파일안에 크기와 보낸 데이터 크기를 비교하여 
    // 보낸 데이터 크기가 파일안에 크기보다 작은동안 반복한다.
    // 보낸 파일 내용의 크기가 파일 크기보다 작을 수 있기 때문.
    while(send_len < file_size){
        fread(message, sizeof(char), sizeof(message)-1, fp);
        // printf("message : %s\n", message);
        send_cnt = write(sock, message, strlen(message)+1);
        // 값을 초기화 해준다.
        memset(&message, 0, sizeof(message));
        // printf("send_cnt = %d", send_cnt);
        // 보낸 데이터의 값을 추가해준다. 
        send_len = send_len + send_cnt;
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

int filesize(const char  *filename){

    struct stat file_info;
    int sz_file;

    if(0 > stat( filename, &file_info))
	    return -1;

    return file_info.st_size;
}