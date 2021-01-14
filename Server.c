#include <stdio.h>
#include <string.h> // memset
#include <stdlib.h> // atoi
#include <unistd.h> // sockaddr_in, read, write
#include <arpa/inet.h> // htnol, htons, INADDR_ANY, sockaddr_in
#include <sys/socket.h>
#include <errno.h>

#define BUFSIZE 100

int error_handling(char *message);

int main(int argc, char* argv[])
{
    char buf[BUFSIZE+1];
    char msg[] = "Hello Client";
    int recv_len;

    int server_socket;
    int client_socket;
    int retval;

    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;
    int clnt_addr_size;

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
    while(1){
        //accept()
        clnt_addr_size=sizeof(client_addr);
        client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &clnt_addr_size);
        if(client_socket == -1)
            error_handling("accept error");
        printf("[Client Info] IP주소 = %s, 포트번호 = %d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

        // 한 클라이언트에 대해 반복
        while(1){
            //recv()
            recv_len=recv(client_socket, &buf, BUFSIZE, 0);
            if(recv_len == -1){
            printf("erron: %d\n", errno);
            error_handling("recv error");
            
            break;
            }
            else if(recv_len == 0){
                break;
            } 
            else printf("From Client : %s\n", buf);

            //send()
            strcpy(buf, "Hello Client");
            retval = send(client_socket, &buf, strlen(buf), 0);
            if(retval == -1){
                error_handling("send error");
                break;
            }
                
            //printf("send: %d", retval);
            
        }
        close(client_socket);
    }

    close(server_socket);

    return 0;
}

int error_handling(char *message){
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}