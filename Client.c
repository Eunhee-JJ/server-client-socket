#include <stdio.h>
#include <string.h> // memset
#include <stdlib.h> // atoi
#include <unistd.h> // sockaddr_in, read, write
#include <arpa/inet.h> // htnol, htons, INADDR_ANV, sockaddr_in
#include <sys/socket.h>
#include <sys/types.h>

#define BUFSIZE 20

int error_handling(char *message);

int main(int argc, char* arvg[])
{
    char buf[BUFSIZE+1];
    int len;
    char msg[] = "Hello Server\n";
    int msg_len;

    int server_socket;
    int client_socket;
    int retval;

    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;
    socklen_t serv_addr_size;

    //socekt()
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if(-1 == client_socket){
        error_handling("socket error");
    }

    //connect()
    memset(&client_addr, 0, sizeof(client_addr));
    client_addr.sin_family = AF_INET;
    client_addr.sin_addr.s_addr = inet_addr(arvg[1]); // 입력값의 첫번째 인자를 클라이언트 IP 주소로 설정
    client_addr.sin_port = htons(atoi(arvg[2])); // 입력값의 두번째 인자를 클라이언트 포트로 설정
    retval = connect(client_socket, (struct sockaddr*)&server_addr, sizeof(server_addr));
    if(retval == -1)
        error_handling("connect error");

    //서버와 데이터 교환
    while(1){
        //send()
        retval = send(server_socket, msg, retval, 0);
        if(retval == SO_ERROR){
            error_handling("send error");
            break;
        }

        //recv()
        retval = recv(server_socket, buf, BUFSIZE, 0);
        if(retval == SO_ERROR){
            error_handling("recv error");
        }
        else if(retval == 0)
            break;

        buf[retval] = '\0';
        printf("%s\n", buf);
            
    }

    //closesocket()
    close(server_socket);
    close(client_socket);
}

int error_handling(char *message){
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}