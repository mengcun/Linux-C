/*File name: shmem.c
 *Type: C source file
 *Date: 2017/04/21 
 */
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <sys/shm.h>
#include <string.h>
#include "shmdata.h"
#include "signal.h"

int shmid;
void *shmem_addr= NULL; //This is the start address of the shared memory
struct shared_memory_struct *shared_memory; //Creat the shared_memory

/*
 *void permission_handler(int signal_fd):Handle the RW_flag in struct.
 */

void permission_handler(int signal_fd)
{
    printf("\nPID:%d, I got a signal %d, and I will change the permission to WRITE only\n", getpid(), signal_fd);
    shared_memory -> RW_flag = !shared_memory->RW_flag;
}

/*
 *void shmread(int shared_memory_fd):Read the shared_memory_fd.
 */

void shmread(int shared_memory_fd)
{
    //Creat the shared_memory if it doesn't exits. all the process is R/W/X permission: 0666
    if((shmid = shmget((key_t)shared_memory_fd, sizeof(struct shared_memory_struct), 0666|IPC_CREAT)) == -1)
    {
        printf("PID:%d, Creat the shared memory failed in process.\n", getpid());
        exit(EXIT_FAILURE);
    }
    //Link the shared_memory to the current process
    shmem_addr = shmat(shmid, 0 ,0);
    if(shmem_addr == (void*)-1)
    {
        printf("PID:%d, There is an error when Link the shared_memory into current process.\n", getpid());
        exit(EXIT_FAILURE);
    }
    else
        printf("PID:%d, The shared_memory attached at %ld in process.\n",getpid(), (unsigned long int)shmem_addr);

    //Config the shared_memory
    shared_memory = (struct shared_memory_struct*)shmem_addr; 
    signal(SIGALRM, permission_handler);
   
    while(1) //Read the data from the shared_memory
    {

        if(shared_memory->RW_flag != 0) //No process Write data into shared_memory, then we can Read out
        {
            printf("PID:%d, Read from the shared_memory: %s\n",getpid(),shared_memory->text);
            //After read out the data
            sleep(rand() % 3);
            //Send the signal to set the shared_memory can be only write.
            if(kill(getpid(), SIGALRM) == -1)
            {
                printf("PID:%d, Send out the signal to change the permission to write only Failure\n", getpid());
                exit(EXIT_FAILURE);
            }
            else
                printf("PID:%d, Send out the signal to change the permission to write only Successful\n", getpid());
        }
        else//There is another process is writing the shared_memory.
            sleep(1);
            printf("PID:%d, Waiting the data wrting into shared_memory\n",getpid());

    }
    //Separate the shared_memory from the current process
    if(shmdt(shmem_addr) == -1)
    {
        printf("PID:%d, Separate the shared memory %d \"(%d)\" failed in process.\n",getpid(), shmid, shared_memory_fd);
        exit(EXIT_FAILURE);        
    }
    //Delete the shared_memory
    if(shmctl(shmid, IPC_RMID, 0) == -1)
    {
        printf("PID:%d, Delete the shared memory %d \"(%d)\" failed in process.\n",getpid(), shmid, shared_memory_fd);
        exit(EXIT_FAILURE);
    }

}

/*
 *void shmwrite(int shared_memory_fd):Write the shared_memory_fd.
 */

void shmwrite(int shared_memory_fd)
{
    static char Data_WR[SHMEM_SZ] = {"This is test for shared_memory"};
    //Creat the shared_memory if it doesn't exits. all the process is R/W/X permission: 0666
    if((shmid = shmget((key_t)shared_memory_fd, sizeof(struct shared_memory_struct), 0666|IPC_CREAT)) == -1)
    {
        printf("PID:%d, Creat the shared memory failed in process.\n", getpid());
        exit(EXIT_FAILURE);
    }
    //Link the shared_memory to the current process
    shmem_addr = shmat(shmid, 0 ,0);
    if(shmem_addr == (void*)-1)
    {
        printf("PID:%d, There is an error when Link the shared_memory into current process.\n", getpid());
        exit(EXIT_FAILURE);
    }
    else
        printf("PID:%d, The shared_memory attached at %ld in process.\n",getpid(), (unsigned long int)shmem_addr);

    //Config the shared_memory
    shared_memory = (struct shared_memory_struct*)shmem_addr; 
           
    signal(SIGALRM, permission_handler);

    while(1)
    {
        
        while(shared_memory->RW_flag != 0)//Waiting the data to be read
        {
            sleep(1);
            printf("PID:%d, Waiting for the data to be read\n", getpid());
        }
        printf("PID:%d, Write the date:\n", getpid());
        
        strncpy(shared_memory -> text, Data_WR, SHMEM_SZ);
        printf("PID:%d, Writing the data into shared_memory.text:%s\n", getpid(), shared_memory->text);
          
        if(kill(getpid(), SIGALRM) == -1)
         {
            printf("PID:%d, Send out the signal to change the permission to Read only Failure\n", getpid());
            exit(EXIT_FAILURE);
        }
        else
        {
            printf("PID:%d, Send out the signal to change the permission to Read only\n", getpid());
        }

    }
    
    //Separate the shared_memory from the current process
    if(shmdt(shmem_addr) == -1)
    {
        printf("PID:%d, Separate the shared memory %d \"(%d)\" failed in process.\n",getpid(), shmid, shared_memory_fd);
        exit(EXIT_FAILURE);        
    }
    //Delete the shared_memory
    if(shmctl(shmid, IPC_RMID, 0) == -1)
    {
        printf("PID:%d, Delete the shared memory %d \"(%d)\" failed in process.\n",getpid(), shmid, shared_memory_fd);
        exit(EXIT_FAILURE);
    }

}
