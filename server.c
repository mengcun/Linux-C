/*File name: server.c
 *Type: C source file
 *Date: 2017/05/02 
 */
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <string.h>
#include <netdb.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include "client.h"

#define portnumber 3333

/*
 *The process about TCP_CLIENT:
 *1.Creat the socket by socket();
 *2.Bind the IP address, portnumber to socket by function bind().
 *3.Connect the max connections by function listen();
 *4.Accept the connections from client by function accept();
 *5.Recrive/Send the data by send()/recv() or read()/write();
 *6.Close the connections;
 * */

void tcp_server()
{
   int socket_fd, connect_fd;
   struct sockaddr_in server_addr;
   struct sockaddr_in client_addr;
   int sin_size;
   int nbytes;
   char buffer[1024];

   //Creat the socket
   if((socket_fd = socket(AF_INET, SOCK_STREAM, 0))== -1)//AF_INET:IPV4; SOCK_STREAM:TCP
   {
        printf("PID:%d, Creat the socket Error:%s\a\n", getpid(), strerror(errno));
        exit(EXIT_FAILURE);
   }
   //Initial the struct of sockaddr
   bzero(&server_addr, sizeof(struct sockaddr_in));//Initial to 0
   server_addr.sin_family = AF_INET;
   server_addr.sin_addr.s_addr = htonl(INADDR_ANY);//INADDR_ANY:The server can be any IP address; hton1:Change the long data on the merchine to long data in Internet.
   //We can bind to a fixed ip address: inet_addr:used for change the number IP to int type
   //server_addr.sin_addr.s_addr = inet_addr("192.168.1.1");
   server_addr.sin_port = htons(portnumber);//Config the portnumber. htons: Change the data to short.

   //Bind the socket_fd to IP address
   if(bind(socket_fd, (struct sockaddr *)(&server_addr), sizeof(struct sockaddr)) == -1 )
   {
        printf("PID:%d, Bind the socket Error:%s\a\n", getpid(), strerror(errno));
        exit(EXIT_FAILURE);
   }

   //Config the max connections to 5
   if(listen(socket_fd, 5) == -1)
   {
        printf("PID:%d, Listen the socket Error:%s\a\n", getpid(), strerror(errno));
        exit(EXIT_FAILURE);
   }

   //Start to communicate
   while(1)
   {
       //The server will be blocked untill the client is setup the connection.
       sin_size = sizeof(struct sockaddr_in);
       if((connect_fd = accept(socket_fd, (struct sockaddr *)(&client_addr), &sin_size)) == -1)
       {
            printf("PID:%d, Accept the socket Error:%s\a\n", getpid(), strerror(errno));
            exit(EXIT_FAILURE);
       }

       //Change the IP address to Char
       printf("PID:%d, Server get connection from %s\n", getpid(), inet_ntoa(client_addr.sin_addr));
       if((nbytes = read(connect_fd, buffer, 1024)) == -1)
       {
            printf("PID:%d, Read the socket Error:%s\a\n", getpid(), strerror(errno));
            exit(EXIT_FAILURE);
       }
       buffer[nbytes] = '\0';
       printf("PID:%d, Server received: %s\n", getpid(), buffer);
       // Close this connections and will be next
       close(connect_fd);
   }
   //Close the communicate.
   close(socket_fd);
}

/*
 *The process about UDP_SERVER:
 *1.Creat the socket by socket();
 *2.Bind the IP address, portnumber to socket by bind();
 *3.Then, using recefrom() to reveive data;
 *4.Close the internet connect;
 * */

void udp_server()
{
    static int socket_fd, client_len;
    static char buffer[1024];
    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;
    
    //Creat the socket in client: AF_INET = internet, SOCK_STREAM = tcp
    if((socket_fd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
    {
        printf("PID:%d, Creat the socket Error:%s\a\n", getpid(), strerror(errno));
        exit(EXIT_FAILURE);
    }
    
    //Config the Server IP address and portnumber
    bzero(&server_addr, sizeof(server_addr));//Initial as 0
    server_addr.sin_family = AF_INET;// This is IPV4
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(portnumber);//portnumber: this should change the short data in PC to the short data on internet.
    
    //Bind the socket_fd
    if(bind(socket_fd, (struct sockaddr *)&server_addr, sizeof(struct sockaddr_in)) < 0)
    {
        printf("PID:%d, Binding the socket Error:%s\a\n", getpid(), strerror(errno));
        exit(EXIT_FAILURE);
    }
    while(1)
    {
        bzero(buffer, 1024);//Initial the buffer.
        //Receive the data responed from server. We didn't care the source where data come from.
        recvfrom(socket_fd, buffer, sizeof(buffer), 0, (struct sockaddr *)&client_addr, &client_len);
        printf("PID:%d, Get data from client:%s\n", getpid(), buffer);
        if(fork() < 0)
        {
            perror("creat process error"); //display the error information to stdout.
            printf("Error occured when creat the process (ID %d): %s\n", getpid(), strerror(errno));
            exit(EXIT_FAILURE);
        }
        else
        {
            //use the child-process to handle the data.
            //Send out the data after the handle.
            sleep(5);
            sendto(socket_fd, buffer, sizeof(buffer), 0, (struct sockaddr *)&client_addr, sizeof(struct sockaddr_in));
            printf("PID:%d, sendout the data after handle:%s\n", getpid(), buffer);
            exit(EXIT_SUCCESS);
        }
    }
    //Close the internet
    close(socket_fd);
}
