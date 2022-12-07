#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>

#define BUF_SIZE 100
#define MAX_CLNT 256

void *handle_clnt(void * arg);
void *send_msg(char * msg, int len);
void error_handling(char *message);

int clnt_cnt = 0;           // 데이터 영역 공유 변수
int clnt_socks[MAX_CLNT];   // 데이터 영역 공유 변수
pthread_mutex_t mutx;       // mutex

int main(int argc, char *argv[]){
    int serv_sock, clnt_sock;                       // 서버는 서버 소켓, 클라이언트 소켓
    struct sockaddr_in serv_adr, clnt_adr;          //        서버 주소, 클라이언트 주소를 가져야 한다.
    int clnt_adr_sz;                                // 클라이언트 주소 사이즈
    pthread_t t_id;                                 // thread를 사용하기 위한 thread id 선언

    if(argc != 2){
        printf("Usage : %s <port>\n", argv[0]);
        exit(1);
    }

    pthread_mutex_init(&mutx, NULL);                // mutex 초기화
    serv_sock = socket(PF_INET, SOCK_STREAM, 0);    // TCP 블로킹 소켓 선언

    memset(&serv_adr, 0, sizeof(serv_adr));     
    serv_adr.sin_family = AF_INET;       
    serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_adr.sin_port = htons(atoi(argv[1]));

    if(bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)
        error_handling("bind() error");

    if(listen(serv_sock, 5) == -1)
        error_handling("listen() error");

    while(1){
        clnt_adr_sz = sizeof(clnt_adr);
        clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_adr, &clnt_adr_sz);   // 클라이언트 접속 대기

        pthread_mutex_lock(&mutx);                  // mutex lock
        clnt_socks[clnt_cnt++] = clnt_sock;         // critical section 
        pthread_mutex_unlock(&mutx);                // mutex unlock

        pthread_create(&t_id, NULL, handle_clnt, (void*)&clnt_sock);                // 실제 thread 실행 함수
        pthread_detach(t_id);                       // thread 리소스 정리
        printf("Connected client IP: %s \n", inet_ntoa(clnt_adr.sin_addr));         // client IP를 출력
    }
    close(serv_sock);
    return 0;
}

void *handle_clnt(void * arg){
    int clnt_sock = *((int*)arg);
    int str_len = 0, i;
    char msg[BUF_SIZE];

    while((str_len = read(clnt_sock, msg, sizeof(msg))) != 0) // read 함수가 0을 리턴하는 경우 : 파일의 끝을 만나면 0
        send_msg(msg, str_len);

    pthread_mutex_lock(&mutx);
    for(i = 0; i < clnt_cnt; i++){
        if(clnt_sock==clnt_socks[i]){
            while(i++ < clnt_socks[i])
                clnt_socks[i]=clnt_socks[i+1];
            break;
        }
    }
    clnt_cnt--;
    pthread_mutex_unlock(&mutx);

    close(clnt_sock);
    return NULL;
}
void *send_msg(char * msg, int len){
    int i;
    
    pthread_mutex_lock(&mutx);
    for(i = 0; i < clnt_cnt; i++)
        write(clnt_socks[i], msg, len);
    pthread_mutex_unlock(&mutx);
}
void error_handling(char* message){
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}
