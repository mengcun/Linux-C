/*File name: message.c
 *Type: C source file
 *Date: 2017/04/24
 */
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <sys/msg.h>
#include <string.h>
#include "msgdata.h"


void msgreceive(int msginfo_fd)
{
    int msgid = -1;
    struct message_info_struct msginfo;
    long int msgtype = 0;
    //Creat the message list
    msgid = msgget((key_t)msginfo_fd, 0666|IPC_CREAT);
    if(msgid == -1)
    {
        perror("msgget failed");
        printf("PID:%d, Failed to creat the message queue\n", getpid());
        exit(EXIT_FAILURE);
    }
    else
        printf("PID:%d, Success to Creat the message queue\n", getpid());

    while(1)
    {
        if(msgrcv(msgid, (void*)&msginfo, MSG_SZ, msgtype, 0) == -1)
        {
            perror("msgrcv failed");
            printf("PID:%d, Failed to receive the message from %d\n", getpid(), msgid);
            exit(EXIT_FAILURE);
        }
        else
            printf("PID:%d, Received the message from %d:%s\n", getpid(),msgid, msginfo.text);
        sleep(1);
    }

    if(msgctl(msgid, IPC_RMID, 0) == -1)
    {
        printf("PID:%d, Failed to delete the message queue %d\n", getpid(), msgid);
        exit(EXIT_FAILURE);   
    }
    else
        printf("PID:%d, Success to delete the message queue %d\n", getpid(),msgid);

}

void msgsend(int msginfo_fd)
{
    int msgid = -1;
    struct message_info_struct msginfo;
    msginfo.msg_type = 1;
    static char Data_WR[MSG_SZ] = {"This is test for message queue"};
    //Creat the message list
    msgid = msgget((key_t)msginfo_fd, 0666|IPC_CREAT);
    if(msgid == -1)
    {
        perror("msgget failed");
        printf("PID:%d, Failed to creat the message queue\n", getpid());
        exit(EXIT_FAILURE);
    }
    else
        printf("PID:%d, Success to Creat the message queue\n", getpid());

    while(1)
    {
        printf("PID:%d, Send the date:\n", getpid());
        
        strncpy(msginfo.text, Data_WR, MSG_SZ);
        printf("PID:%d, Send the data into msginfo.text:%s\n", getpid(), msginfo.text);
        if(msgsnd(msgid, (void*)&msginfo, MSG_SZ, 0) == -1)
        {
            perror("msgsnd failed");
            printf("PID:%d, Failed to send the message to %d\n", getpid(), msgid);
            exit(EXIT_FAILURE);
        }
        else
            printf("PID:%d, Success to send the message to %d:%s\n", getpid(),msgid, msginfo.text);
        sleep(1);
    }
    if(msgctl(msgid, IPC_RMID, 0) == -1)
    {
        printf("PID:%d, Failed to delete the message queue %d\n", getpid(), msgid);
        exit(EXIT_FAILURE);   
    }
    else
        printf("PID:%d, Success to delete the message queue %d\n", getpid(),msgid);

}
