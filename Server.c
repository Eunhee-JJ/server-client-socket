#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>

//socekt()
int server_socket;
server_socket = socket(AF_INET, SOCK_STREAM, 0);
if(-1 == server_socket){
    printf("server socket 생성 실패");
    exit(1); 
}


//bind()
struct sockaddr_in server_addr;
memset(&server_addr, 0, sizeof(server_addr)); 
server_addr.sin_family  = AF_INET;
server_addr.sin_port = htons(4000);
server_addr.sin_addr.s_addr = 

//listen()

//accept()

//recv()

//send()

//closesocket()