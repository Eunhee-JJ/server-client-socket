#include <stdio.h>
#include <string.h> // memset
#include <stdlib.h> // atoi
#include <unistd.h> // sockaddr_in, read, write
#include <arpa/inet.h> // htnol, htons, INADDR_ANV, sockaddr_in
#include <sys/socket.h>
#include <sys/types.h>
#include <errno.h>

#define BUFSIZE 1460

typedef struct Files {
    char name[255];
    unsigned long byte;
} Files;

int error_handling(char *message);
int recvn(int sock, char *buffer, int len, int flags);

int main(int argc, char* argv[])
{
    char buf[BUFSIZE+1];
    char msg[] = "Hello Server";
    unsigned int count;

    int client_socket;
    int retval;
    int recv_len;

    struct sockaddr_in server_addr;

    //socekt()
    client_socket = socket(PF_INET, SOCK_STREAM, 0);
    if(-1 == client_socket){
        error_handling("socket error");
    }

    //connect()
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(argv[1]); // 서버 IP주소 인자로 받은거 저장해주기
    server_addr.sin_port = htons(atoi(argv[2])); // 서버 포트번호 인자로 받은거 저장해주기
    retval = connect(client_socket, (struct sockaddr*)&server_addr, sizeof(server_addr));
    if(retval == -1)
        error_handling("connect error");

    //서버와 데이터 교환
    // Hello server 메시지 전송
    strcpy(buf,msg);
    retval = send(client_socket, buf, strlen(msg), 0);
    if(retval == -1){
        error_handling("send error");
    }
    //printf("send: %s, %d\n", buf, retval);
    buf[0]='\0';

    // 파일 기본 정보 받기
    FILE *fp = NULL;
    Files files;

    recv_len = recvn(client_socket, (char *)&files, sizeof(files), 0);
    if(recv_len == -1) error_handling("file open error");
    //else printf("file open");

    // 파일 중복 여부 확인
    fp = fopen(files.name, "rb");
    if(fp != NULL){
        error_handling("이미 같은 이름의 파일이 존재합니다. 다운로드를 종료합니다.");
    }

    // 파일 다운로드
    printf("전송받는 파일: %s, 파일 크기: %ld Byte\n", files.name, files.byte);
    printf("서버로부터 파일을 전송 받는 중입니다.\n");

    fp = fopen(files.name, "wb");

    unsigned int per;
    per = count = files.byte / BUFSIZE;

    while(count){
        retval = recvn(client_socket, buf, BUFSIZE, 0);
        if(retval == -1) error_handling("recv error");

        fwrite(buf, 1, BUFSIZE, fp);
        printf("\n진행도: %d %%", (per-count) * 100 / per);

        count--;
    }

    // 남은 파일 크기만큼 다운로드
    count = files.byte - ((files.byte / BUFSIZE) * BUFSIZE);

    retval = recvn(client_socket, buf, BUFSIZE, 0);
    if (retval == -1) error_handling("recv error");

    fwrite(buf, 1, count, fp);
    printf("\n진행도: %d %%", (per-count) * 100 / per);

    fclose(fp);
    printf("파일 다운로드 완료\n");

    close(client_socket);
}

int error_handling(char *message){
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}

int recvn(int sock, char *buffer, int len, int flags){
    int received;
    char *ptr = buffer;
    int left = len;

    while(left>0){
        received = recv(sock, ptr, left, flags);
        if(received == -1)
            return -1;
        else if(received == 0)
            break;
        left -= received;
        ptr += received;
    } 

    return (len-left);
}