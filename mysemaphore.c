/*File name: mysemaphore.c
 *Type: C source file
 *Date: 2017/04/26 
 */
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <sys/sem.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>
#include "mysemaphore.h"

static int sem_id;

union semun
{
    int val;//The value to intial the semaphore.
    struct semid_ds *buf;
    unsigned short *arry;
};

/*
 *Creat the semaphore when it doesn't exist.
 *
 **/
int creat_semaphore(int semaphore_fd)
{
    sem_id = semget((key_t)semaphore_fd, 1, 0666|IPC_CREAT);
    if(sem_id == -1)
    {
        printf("PID:%d, Creat the semaphore %d (%d) failed!\n", getpid(), sem_id, semaphore_fd);
        exit(EXIT_FAILURE);
    }
    return sem_id;
}

/*
 *Set the value for initial semaphore before we use it.
 *
 * */
void set_semvalue(int sem_id)
{
    union semun sem_union;
    sem_union.val = 1;
    if(semctl(sem_id, 0, SETVAL, sem_union) == -1)
    {
        printf("PID:%d, Init the semaphore value %d failed!\n", getpid(), sem_id);
        exit(EXIT_FAILURE);
    }
}

/*
 *Delte the semaphore
 *
 * */
void del_semvalue(int sem_id)
{
    union semun sem_union;
    if(semctl(sem_id, 0, SETVAL, sem_union) == -1)
    {
        printf("PID:%d, Init the semaphore value %d failed!\n", getpid(), sem_id);
        exit(EXIT_FAILURE);
    }
}

/*
 *Operate the semaphore by minus 1, P(sv);
 *
 * */
void semaphore_p(int sem_id)
{
    struct sembuf sem_b;
    sem_b.sem_num = 0;
    sem_b.sem_op = -1; //p(sv)
    sem_b.sem_flg = SEM_UNDO;
    if(semop(sem_id, &sem_b, 1) == -1)
    {
        printf("PID:%d, Occupyed the resource and the semaphore %d minus 1 failed!\n", getpid(), sem_id);
        exit(EXIT_FAILURE);
    }
}

/*
 *Operate the semaphore by add 1, V(sv);
 *
 * */
void semaphore_v(int sem_id)
{
    struct sembuf sem_b;
    sem_b.sem_num = 0;
    sem_b.sem_op = 1; //V(sv)
    sem_b.sem_flg = SEM_UNDO;
    if(semop(sem_id, &sem_b, 1) == -1)
    {
        printf("PID:%d, Release the resource and the semaphore %d add 1 failed!\n", getpid(), sem_id);
        exit(EXIT_FAILURE);
    }
}

/*
 *This is the test process for semaphore
 *
 * */
void test_semaphore(int semaphore_fd)
{
    int i;
    printf("PID:%d, Creat the semaphore\n", getpid());
    sem_id = creat_semaphore(semaphore_fd);
    printf("PID:%d, initial the semaphore:%d\n", getpid(), sem_id);
    set_semvalue(sem_id);

    for(i = 0; i <=10; i++)
    {
        //Enter the critical, occupy the resource
        semaphore_p(sem_id);
        printf("PID:%d, the semaphore %d is process semaphore_p\n", getpid(), sem_id);
        sleep(rand()%3);//sleep as a rang value
        //Exit the critical, Release the resource
        semaphore_v(sem_id);
        printf("PID:%d, the semaphore %d is process semaphore_v\n", getpid(), sem_id);
        sleep(rand()%3);//sleep as a rang value
    }
    printf("PID:%d, Delete the semaphore %d.\n", getpid(), sem_id);
    del_semvalue(sem_id);
}
