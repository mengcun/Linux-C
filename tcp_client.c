/*File name: tcp_client.c
 *Type: C source file
 *Date: 2017/04/27 
 */
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <string.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "tcp_client.h"

#define portnumber 3333

/*
 *The process about TCP_CLIENT:
 *1.Creat the socket by socket();
 *2.Config the IP address and portnumber which will connect to;
 *3.Connect to the server by connect();
 *4.Then, using send()/ write() and recv()/ read() to send/receive data;
 *5.Close the internet connect;
 * */

void tcp_client()
{
    int socket_fd;
    char buffer[1024];
    struct sockaddr_in server_addr;
    struct hostent *host;
    //Get the host name through gethostbyname
    if ((host = gethostbyname("cme")) == NULL)
    {
        printf("PID:%d, Get the hostname error\n", getpid());
        exit(EXIT_FAILURE);
    }
    //Creat the socket in client: AF_INET = internet, SOCK_STREAM = tcp
    if((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        printf("PID:%d, Creat the socket Error:%s\a\n", getpid(), strerror(errno));
        exit(EXIT_FAILURE);
    }
    
    //Config the IP address and portnumber
    bzero(&server_addr, sizeof(server_addr));//Initial as 0
    server_addr.sin_family = AF_INET;// This is IPV4
    server_addr.sin_port = htons(portnumber);//portnumber: this should change the short data in PC to the short data on internet.
    server_addr.sin_addr = *((struct in_addr *)host -> h_addr);//IP address

    //Connect to the server
    if(connect(socket_fd, (struct sockaddr *)(&server_addr), sizeof(struct sockaddr)) == -1)
    {
        printf("PID:%d, Connect to server Error:%s\a\n", getpid(), strerror(errno));
        exit(EXIT_FAILURE);
    }

    printf("PID:%d, Connect to server success, will send the data:\n", getpid());
    //get data from stdin and send to socket_fd
    fgets(buffer, 1024, stdin);
    write(socket_fd, buffer, strlen(buffer));

    //Close the internet
    close(socket_fd);
}
