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
    char buf[BUFSIZE+1];
    int len;

    int server_socket;
    int client_socket;
    int retval;

    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;
    socklen_t clnt_addr_size;

    //socekt()
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if(server_socket == SO_ERROR){
        error_handling("socket error");
    }

    //bind()
    memset(&server_addr, 0, sizeof(server_addr)); 
    server_addr.sin_family  = AF_INET;
    server_addr.sin_port = htons(3024);
    server_addr.sin_addr.s_addr = inet_addr(INADDR_ANV); // 컴퓨터에 존재하는 랜카드 중 사용 가능한 랜카드의 IP주소를 사용
    printf("SERVER IP ADDRESS: %d", server_addr.sin_addr.s_addr); // 클라이언트에서 접근할 수 있도록 서버 IP를 출력

    retval = bind(server_socket, (struct sockaddr_in*)&server_addr, sizeof(server_addr))
    if(retval == SO_ERROR)
        printf("bind error");

    //listen()
    retval = listen(server_socket, 5)
    if(retval ==SO_ERROR)
        error_handling("listen error");

    //accept()
    clnt_addr_size=sizeof(client_addr);
    client_socket == accept(server_socket, (struct sockaddr_in*)&client_addr, &clnt_addr_size));
    if(client_socket == SO_ERROR)
        error_handling("accept error");

    //recv()
    while(1){

        // 클라이언트와 데이터 통신
        while(1){
            retval = recv(client_socket, buf, BUFSIZE, 0)
            if(retval == SO_ERROR)
                error_handling("recv error");
                break;
            else if(retval) == 0)
                break;
            
            // 받은 데이터 출력
            buf[retval] = '\0';
            printf("[TCP/%s:%d] %s\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port), buf);

            //send()
            char msg[] = "Hello Client\n";
            retval = send(client_socket, msg, retval, 0);
            if(retval == SO_ERROR)
                error_handling("send error");
                break;
        }

    }
    
    //closesocket()
    closesocket(client_socket);
    closesocket(server_socket);
    
}

int error_handling(char *message){
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}