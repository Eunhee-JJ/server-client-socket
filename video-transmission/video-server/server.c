#include <stdio.h>
#include <string.h> // memset
#include <stdlib.h> // atoi
#include <unistd.h> // sockaddr_in, read, write
#include <arpa/inet.h> // htnol, htons, INADDR_ANY, sockaddr_in
#include <sys/socket.h>
#include <errno.h>

#define BUFSIZE 1460

int error_handling(char *message);
int recvn(int sock, char *buffer, int len, int flags);

typedef struct Files{
    char name[255];
    unsigned long byte;
}Files;

int main(int argc, char* argv[])
{
    char buf[BUFSIZE+1];
    char msg[] = "Hello Client";
    int recv_len;
    int send_len;

    int server_socket;
    int client_socket;
    int retval;

    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;
    int clnt_addr_size;

    unsigned int count; //(파일크기 / 버퍼 사이즈) 계산한 값을 while문으로 돌리기 위한 변수

    // 파일 이름 및 크기 확인
    FILE *fp;
    Files files;

    strcpy(files.name, "test.mp4");
    fp = fopen(files.name, "rb");
    if(fp == NULL) error_handling("File Pointer error");

    fseek(fp, 0L, SEEK_END);
    files.byte = ftell(fp);
    fseek(fp, 0L, SEEK_SET);
    
    //socekt()
    server_socket = socket(PF_INET, SOCK_STREAM, 0);
    if(server_socket == -1){
        error_handling("socket error");
    }

    //bind()
    memset(&server_addr, 0, sizeof(server_addr)); 
    server_addr.sin_family  = AF_INET;
    server_addr.sin_port = htons(atoi(argv[1]));
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY); // 컴퓨터에 존재하는 랜카드 중 사용 가능한 랜카드의 IP주소를 사용
    //printf("SERVER IP ADDRESS: %d\n", INADDR_ANY); // 클라이언트에서 접근할 수 있도록 서버 IP를 출력

    retval = bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if(retval == -1)
        error_handling("bind error");

    //listen()
    retval = listen(server_socket, 5);
    if(retval ==-1)
        error_handling("listen error");

    // 클라이언트와 데이터 통신
//    while(1){
        //accept()
        clnt_addr_size=sizeof(client_addr);
        client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &clnt_addr_size);
        if(client_socket == -1)
            error_handling("accept error");
        printf("[Client Info] IP주소 = %s, 포트번호 = %d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));


        // Hello server 메시지 수신
        recv_len=recv(client_socket, buf, BUFSIZE, 0);
        if(recv_len == -1){
            printf("erron: %d\n", errno);
            error_handling("recv error");
            //break;
        }
        else {
            printf("From Client : %s\n", buf);
            }

        buf[0] = '\0';
        sleep(5);

        // 파일 기본 정보 전송
        retval = send(client_socket, (char *)&files, sizeof(files), 0);
        if(retval == -1) error_handling("send error");

        unsigned int per;
        per = count = files.byte/BUFSIZE;
        

        // 파일 전송
        while(count){
            system("clear");
            printf("전송 파일: %s, 파일 크기: %ld Byte\n", files.name, files.byte);
            fread(buf, 1, BUFSIZE, fp);

            retval = send(client_socket, buf, BUFSIZE, 0);
            if(retval == -1){
                error_handling("send error");
                break;
            }
            printf("\n진행도: %d %% ", (per-count) * 100 / per);

            count--;        
        }

        // 남은 파일 크기만큼 나머지 전송
        count = files.byte - (per*BUFSIZE);
        fread(buf, 1, count, fp);
        count--;

        retval = send(client_socket, buf, BUFSIZE, 0);
        if(retval == -1) error_handling("send error");

        printf("전송 파일: %s 파일 크기: %ld Byte\n", files.name, files.byte);
        printf("\n진행도: %d %%", (per-count) * 100 / per);
        printf("\n전송 완료\n");
        fclose(fp);
        close(client_socket);
//    }

    close(server_socket);

    
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