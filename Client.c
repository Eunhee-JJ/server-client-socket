#include <stdio.h>
#include <string.h> // memset
#include <stdlib.h> // atoi
#include <unistd.h> // sockaddr_in, read, write
#include <arpa/inet.h> // htnol, htons, INADDR_ANV, sockaddr_in
#include <sys/socket.h>
#include <sys/types.h>
#include <errno.h>

#define BUFSIZE 100

int error_handling(char *message);

int main(int argc, char* argv[])
{
    char buf[BUFSIZE+1];
    char msg[] = "Hello Server";

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
    //while(1){
        //send()
        strcpy(buf, "Hello Server");
        retval = send(client_socket, buf, strlen(buf), 0);
        if(retval == -1){
            error_handling("send error");
    //        break;
        }
    //    printf("send: %s, %d\n", buf, retval);

        //recv()
        recv_len = recv(client_socket, &buf, BUFSIZE, 0);
        if(recv_len == -1){
            printf("%d", errno);
            error_handling("recv error");
    //        break;
        }
    //    else if(recv_len == 0) break;
        printf("From server: %s\n", buf);
            
//    }

    close(client_socket);
}

int error_handling(char *message){
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}