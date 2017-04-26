/*File name: mypthread.c
 *Type: C source file
 *Date: 2017/04/26
 */
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>
#include "mypthread.h"

/*
 *This is a struct used by function which will be called by thread.
 * */
struct member
{
    int a;
    char *s;
};

/*
 *This is the clean thread called by pthread_cleanup_push() and pthread_cleanup_pop().
 * */

void *clean(void *arg)
{
    printf("PID:%d, this is the clean function:%s\n", getpid(), (char *)arg);
    return (void*)0;
}

/*
 *This is the function that will be called by pthread1_id
 * */

void *thread1_func(void *arg_cntl)
{
    printf("PID:%d, thread1_id start...\n", getpid());
    printf("PID:%d, push the clean function into stack\n", getpid());
    
    pthread_cleanup_push((void *)clean, "This is the first handler to clean up the pthread1_id");
    pthread_cleanup_push((void *)clean, "This is the second handler to clean up the pthread1_id");
    
    printf("PID:%d, push the clean function into stack finished.\n", getpid());

    if(arg_cntl)
    {
        return ((void *)1);//The thread will stop at here and because of the return, the clean function will not be called.
    }

    pthread_cleanup_pop(0);
    printf("PID:%d, pop the second handler clean function out of stack finished.\n", getpid());
    pthread_cleanup_pop(0);
    printf("PID:%d, pop the first handler clean function out of stack finished.\n", getpid());
    
    printf("PID:%d, pop the clean function out of stack finished.\n", getpid());

    return (void *)1;
}

/*
 *This is the function that will be called by pthread2_id
 * */

void *thread2_func(void *arg_cntl)
{
    printf("PID:%d, thread2_id start...\n", getpid());
    printf("PID:%d, push the clean function into stack\n", getpid());
    
    pthread_cleanup_push((void *)clean, "This is the first handler to clean up the pthread2_id");
    pthread_cleanup_push((void *)clean, "This is the second handler to clean up the pthread2_id");
    
    printf("PID:%d, push the clean function into stack finished.\n", getpid());

    if(arg_cntl)
    {
        pthread_exit((void *)2);//The thread will stop at here and because of the pthread_exit, the clean function will be called.
    }

    pthread_cleanup_pop(0);
    printf("PID:%d, pop the second handler clean function out of stack finished.\n", getpid());
    pthread_cleanup_pop(0);
    printf("PID:%d, pop the first handler clean function out of stack finished.\n", getpid());
    
    printf("PID:%d, pop the clean function out of stack finished.\n", getpid());

    pthread_exit((void *)2);
}

/*
 *This is the function that will be called by threadID: pthread_id
 * */

void *creat_member(void *arg)
{
    int i;
    struct member *temp;
    temp = (struct member *)arg;
    printf("PID:%d, thread_id start...\n", getpid());
    printf("PID:%d, the member.a = %d\n", getpid(), temp -> a);
    printf("PID:%d, the member.s = %s\n", getpid(), temp -> s);
    for(i = 0; i < 10; i++)
    {
        sleep(1);
        printf("PID:%d, This is in thread_id after sleep(2)\n", getpid());
    }
    return (void *)0;
}


/*
 *This is the test for pthread! 
 * */

void test_pthread()
{
    int i;
    int errnum;
    pthread_t pthread_id;
    pthread_t pthread1_id;
    pthread_t pthread2_id;
    struct member *data;
    void *backfromfunc;
    data = (struct member *)malloc(sizeof(struct member));
    data -> a = 100;
    data -> s = "This is the test for pthread";

    //Create the pthread_id and run the function for pthread
    if((errnum = pthread_create(&pthread_id, NULL, creat_member, (void *)data)) != 0)
    {
        printf("PID:%d, Create pthread_id failed:%d:%s\n", getpid(), errnum, strerror(errnum));
        exit(EXIT_FAILURE);
    }
    printf("PID:%d, Create pthread_id(%d) success\n", getpid(), (int)pthread_id);

    //Create the pthread1_id and run the function for pthread
    if((errnum = pthread_create(&pthread1_id, NULL, thread1_func, (void *)1)) != 0)
    {
        printf("PID:%d, Create pthread1_id failed:%d:%s\n", getpid(), errnum, strerror(errnum));
        exit(EXIT_FAILURE);
    }
    printf("PID:%d, Create pthread1_id(%d) success\n", getpid(), (int)pthread1_id);

    //Create the pthread2_id and run the function for pthread
    if((errnum = pthread_create(&pthread2_id, NULL, thread2_func, (void *)1)) != 0)
    {
        printf("PID:%d, Create pthread2_id failed:%d:%s\n", getpid(), errnum, strerror(errnum));
        exit(EXIT_FAILURE);
    }
    printf("PID:%d, Create pthread2_id(%d) success\n", getpid(), (int)pthread2_id);
    
    //waiting the thread_id finished. 
    printf("PID:%d, waiting for the pthread_id to exit\n", getpid());
    if((errnum = pthread_join(pthread_id, &backfromfunc)) != 0)
    {
        printf("PID:%d, Waiting the pthread_id failed:%d:%s\n", getpid(), errnum, strerror(errnum));
        exit(EXIT_FAILURE);
    }
    printf("PID:%d, received the code %d when thread_id exit\n", getpid(),(int)backfromfunc);
    
    //waiting the thread1_id finished. 
    printf("PID:%d, waiting for the pthread1_id to exit\n", getpid());
    if((errnum = pthread_join(pthread1_id, &backfromfunc)) != 0)
    {
        printf("PID:%d, Waiting the pthread1_id failed:%d:%s\n", getpid(), errnum, strerror(errnum));
        exit(EXIT_FAILURE);
    }
    printf("PID:%d, received the code %d when thread1_id1 exit\n", getpid(), (int)backfromfunc);
   
    //waiting the thread2_id finished. 
    printf("PID:%d, waiting for the pthread2_id to exit\n", getpid());
    if((errnum = pthread_join(pthread2_id, &backfromfunc)) != 0)
    {
        printf("PID:%d, Waiting the pthread2_id failed:%d:%s\n", getpid(), errnum, strerror(errnum));
        exit(EXIT_FAILURE);
    }
    printf("PID:%d, received the code %d when thread_id2 exit\n", getpid(), (int)backfromfunc);
    
    for(i = 0; i < 10; i++)
    {
        sleep(2);
        printf("PID:%d, This is in process\n", getpid());
    }
}
