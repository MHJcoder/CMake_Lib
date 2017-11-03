#include "FileTransfer.h"
// #include <iostream>




#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <ifaddrs.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <netdb.h>

#include <errno.h>
#define FILEPORT 6201


//文件名和包大小主客户端一致
#define FileNameLen 1024
#define BufSize 1024
int server_fd;

static int Client_Num=0;



//struct in_addr addr = {0};
typedef struct {
    int id;
    int buf_size;
    
}PackInfo;
 struct Pack{
    PackInfo*head;
    char buffer[BufSize];
    
}data;
long  sender(uint8_t* data,long length){
    long nbytes=0;
    return nbytes;
}

/*
 *创建服务端Socket，等待客户端连接
 *返回socket标示符，ｉｎｔ
 *
 */
long  createServer(int Server_port){
    printf("start\n");
    server_fd=socket(AF_INET, SOCK_STREAM, 0);
    
    if(server_fd ==-1){
        perror("socket\n");
        exit(1);
    }
    int reused = 1;    //C中没有bool
    setsockopt(server_fd , SOL_SOCKET, SO_REUSEPORT ,(const void *)&reused , sizeof(reused));
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(INADDR_ANY);
    addr.sin_port = htons(Server_port);
    if(-1==bind(server_fd,( const struct sockaddr *)&addr, sizeof(addr))){
        perror("bind\n");
        exit(1);
    }
    /*listen*/
    if(-1==listen(server_fd, 10)){
        perror("listen\n");
        exit(2);
    }
    printf("Create Socket Successfully\n");
    
    int clientfd;
    struct sockaddr_in clientaddr;
    long addrlen=sizeof(clientaddr);
    while (1) {
        clientfd=accept(server_fd, (struct sockaddr *)&clientaddr, (socklen_t*)&addrlen);
        if(clientfd<0){
            perror("accept\n");
            exit(3);
        }
        //获取主机信息
        struct hostent *peerInfo;
        peerInfo=gethostbyaddr(&(clientaddr.sin_addr.s_addr), 4, AF_INET);
        
        
        char filename[FileNameLen];
        bzero(filename, sizeof(filename));
        
        //读取客户端请求的文件名
        long  flag =read(clientfd, filename, sizeof(filename));
        if (flag==-1) {
            perror("recv name fail");
            exit(4);
        }
        else if (flag==0){
            printf("未收到请求的文件名");
            continue;
        }
        else {
            //打印请求的文件名
            printf("%s",filename);
            
            FILE*fp=fopen(filename, "rb");
            //对应包ID
            long  len=fread(data.buffer, sizeof(char), BufSize, fp);
            if(len>BufSize){
                printf("读取文件出错\n");
                break;
            }
            else if(len==0){
                printf("到达文件尾部\n");
                break;
            }
            else{
                long len=write(clientfd, data.buffer, sizeof(data.buffer));
                if(len!=sizeof(data.buffer)){
                    perror("发送出错");
                    //重新发，待定
                }
                else{
                    
                }
                
            }
            
        }
        
        
    }
    
    return server_fd;
}















int connectServer(char* Server_IP,int Server_port){
    int client_fd=socket(AF_INET, SOCK_STREAM, 0);
    if(-1==client_fd){
        printf("socket\n");
        exit(1);
    }
    struct sockaddr_in addr;
    bzero(&addr, sizeof(addr));
    addr.sin_family=AF_INET;
    addr.sin_addr.s_addr=inet_addr(Server_IP);
    addr.sin_port=htons(Server_port);
    if(-1==bind(client_fd , (struct sockaddr*)(&addr), sizeof(addr))){
        perror("bind");
        exit(1);
    }
    if(-1==connect(client_fd, (struct sockaddr*)&addr, sizeof(addr))){
        perror("connect");
        exit(1);
    }
    printf("connnect successfully\n");
    return client_fd;
}



int main(){

    return 0;
}