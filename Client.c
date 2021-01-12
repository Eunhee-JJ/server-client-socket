#include <stdio.h>
#include <string.h> // memset
#include <stdlib.h> // atoi
#include <unistd.h> // sockaddr_in, read, write
#include <arpa/inet.h> // htnol, htons, INADDR_ANV, sockaddr_in
#include <sys/socket.h>
#include <sys/types.h>

int error_handling(char *message);

int main(int argc, char* arvg[])
{
    int server_socket;
    int client_socket;

    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;
    socklen_t serv_addr_size;

    //socekt()
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if(-1 == client_socket){
        error_handling("socket error")
    }

    //connect()
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(argv[1]); // 입력값의 첫번째 인자를 서버 IP 주소로 설정
    server_addr.sin_port = htons(atoi(arvg[2]); // 입력값의 두번째 인자를 서버 포트로 설정
    if(connect(client_socket, (struct sockaddr_in*)&server_addr, sizeof(server_addr)) == -1)
        error_handling("connect error");

    //send()
    send(client_socket, buf)

    //recv()

    //closesocket()
}

int error_handling(char *message){
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}