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
#include <semaphore.h>
#include "mypthread.h"


pthread_mutex_t mutex;
sem_t sem;
sem_t sem_add;
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
 *This is the function that will be called for test the mutex
 * */

void *thread_mutex_func(void *arg)
{
    int i = 0;
    char *ptr = arg;
    sleep(1); //make the thread sleep so that the other thread have the chance to run.
    // lock the mutex, so that only this thread will be run
    pthread_mutex_lock(&mutex);
    printf("PID:%d, The mutex has been locked by thread_mutex_func.\n", getpid());

    for(i = 0; ptr[i] != '\0'; ++i)
        if(ptr[i] >= 'a' && ptr[i] <= 'z')
        {
            ptr[i] -= 'a' - 'A';
        }
    printf("PID:%d, Get %d characters\n", getpid(), i-1);
    printf("OID:%d, Uppercase the characters: %s\n", getpid(), ptr);
    //unlock the mutex so that other thread can access into arg
    printf("PID:%d, The mutex has been unlocked by thread_mutex_func.\n", getpid());
    pthread_mutex_unlock(&mutex);
    sleep(1); //This is for other thread can have chance to run.
    //Then lock the mutex again
    printf("PID:%d, The mutex will be locked again by thread_mutex_func.\n", getpid());
    pthread_mutex_lock(&mutex);
    printf("PID:%d, The mutex has been locked again by thread_mutex_func.\n", getpid());
    
    //unlock the mutex after finished.
    pthread_mutex_unlock(&mutex);
    //exit the thread_mutex_func
    printf("PID:%d, the Uppercase finished and the pthread_mutex will exit\n", getpid());
    return (void *)3;
    pthread_exit(NULL);
}

/*
 *This is the function that will be called for test the mutex
 * */

void *thread_sem_func(void *msg)
{
    int i = 0;
    char *ptr = msg;
    sleep(1); //make the thread sleep so that the other thread have the chance to run.
    printf("PID:%d, I get the sem.\n", getpid());
    // minus the sem and occupyed by thread_sem_func
    sem_wait(&sem);
    printf("PID:%d, The sem -1 and occupyed by thread_sem_func .\n", getpid());

    for(i = 0; ptr[i] != '\0'; ++i)
        if(ptr[i] >= 'a' && ptr[i] <= 'z')
        {
            ptr[i] -= 'a' - 'A';
        }
    printf("PID:%d, Get %d characters\n", getpid(), i-1);
    printf("OID:%d, Uppercase the characters: %s\n", getpid(), ptr);
    // add the sem +1 that means this thread_sem_func finished.
    printf("PID:%d, The sem_add +1 and Uppercase finished.\n", getpid());
    sem_post(&sem_add);

    printf("PID:%d, This is in thread_sem_func after first sem_wait(&sem)\n", getpid());
    sleep(1); //This is for other thread can have chance to run.
   
    printf("PID:%d, I get the sem again.\n", getpid());
    // minus the sem again and occupyed by thread_sem_func
    sem_wait(&sem);
    printf("PID:%d, The sem -1 again and occupyed by thread_sem_func again.\n", getpid());
    sem_post(&sem_add);
    printf("PID:%d, The sem_add +1 and thread finished and will exit.\n", getpid());
    return (void *)4;
    pthread_exit(NULL);
}

/*
 *This is the test for pthread! 
 * */

void test_pthread()
{
    int i;
    int errnum;
    
    pthread_t pthread_mutex_id;
    pthread_t pthread_sem_id;
    pthread_t pthread_id;
    pthread_t pthread1_id;
    pthread_t pthread2_id;

    static char arg[512] = {"this is the test for pthread_mutex\n"};
    static char msg[512] = {"this is the test for pthread_sem\n"};

    struct member *data;
    void *backfromfunc;
    data = (struct member *)malloc(sizeof(struct member));
    data -> a = 100;
    data -> s = "This is the test for pthread";

    //Initial the sem by defaut
    if((errnum = sem_init(&sem, 0, 0)) != 0)
    {
        printf("PID:%d, Initial the sem failed.\n", getpid());
        perror("Initial sem failed.\n");
        exit(EXIT_FAILURE);
    }

    //Initial the sem_add by 1
    if((errnum = sem_init(&sem_add, 0, 1)) != 0)
    {
        printf("PID:%d, Initial the sem_add failed.\n", getpid());
        perror("Initial sem_add failed.\n");
        exit(EXIT_FAILURE);
    }

    //Initial the mutex by defaut
    if((errnum = pthread_mutex_init(&mutex, NULL)) != 0)
    {
        printf("PID:%d, Initial the pthread_mutex failed.\n", getpid());
        perror("Initial pthread_mutex failed.\n");
        exit(EXIT_FAILURE);
    }

    //Create the pthread_sem_id and run the function for pthread
    if((errnum = pthread_create(&pthread_sem_id, NULL, thread_sem_func, msg)) != 0)
    {
        printf("PID:%d, Create pthread_sem_id failed:%d:%s\n", getpid(), errnum, strerror(errnum));
        exit(EXIT_FAILURE);
    }
    printf("PID:%d, Create pthread_sem_id(%d) success\n", getpid(), (int)pthread_sem_id);

    //minus sem_add -1 and the process will occupyed
    printf("PID:%d, sem_add will be -1 first time by test_pthread\n", getpid());
    sem_wait(&sem_add);
    printf("PID:%d, sem_add has -1 in process first time by test_pthread\n", getpid());
    printf("PID:%d, This is in process after first sem_wait(&sem_add)\n", getpid());
   
    //add sem +1 and the process will occupyed by thread_sem_func
    printf("PID:%d, sem will be +1 first time by test_pthread and will run thread_sem_func\n", getpid());
    sem_post(&sem);
    
    sleep(2);
    
    printf("PID:%d, sem has +1 in process first time by test_pthread\n", getpid());
    
    printf("PID:%d, sem will be +1 second time by test_pthread and will run thread_sem_func again\n", getpid());
    sem_post(&sem);
    printf("PID:%d, sem has +1 in process second time by test_pthread\n", getpid());
    
    sleep(1);
    
    printf("PID:%d, sem_add will  +1 in thread_sem_func\n", getpid());
    sem_wait(&sem_add);
    printf("PID:%d, sem has +1 in process first time by test_pthread\n", getpid());

    //Create the pthread_mutex_id and run the function for pthread
    if((errnum = pthread_create(&pthread_mutex_id, NULL, thread_mutex_func, arg)) != 0)
    {
        printf("PID:%d, Create pthread_mutex_id failed:%d:%s\n", getpid(), errnum, strerror(errnum));
        exit(EXIT_FAILURE);
    }
    printf("PID:%d, Create pthread_mutex_id(%d) success\n", getpid(), (int)pthread_mutex_id);
    
    //Lock the mutex so that only this thread can use the arg
    printf("PID:%d, Lock the mutex first time by test_pthread\n", getpid());
    pthread_mutex_lock(&mutex);

    printf("PID:%d, This is in process after first lock\n", getpid());

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

    //Unlock the mutex first time so that other thread can access into the arg
    printf("PID:%d, Unlock the mutex first time by test_pthread\n", getpid());
    pthread_mutex_unlock(&mutex);
    //Here the main thread will sleep so that other threads have chance to execute.
    sleep(2);
    //Lock the mutex second time so that only this thread can use the arg
    printf("PID:%d, Lock the mutex second time by test_pthread\n", getpid());
    pthread_mutex_lock(&mutex);
    
    printf("PID:%d, This is in process after second lock\n", getpid());

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

    //Unlock the mutex second time so that other thread can access into the arg
    printf("PID:%d, Unlock the mutex second time by test_pthread\n", getpid());
    pthread_mutex_unlock(&mutex);
    //Here the main thread will sleep so that other threads have chance to execute.
    sleep(2);
    
    printf("PID:%d, This is in process\n", getpid());

    //waiting the pthread_mutex_id finished. 
    printf("PID:%d, waiting for the pthread_mutex_id to exit\n", getpid());
    if((errnum = pthread_join(pthread_mutex_id, &backfromfunc)) != 0)
    {
        printf("PID:%d, Waiting the pthread_mutex_id failed:%d:%s\n", getpid(), errnum, strerror(errnum));
        exit(EXIT_FAILURE);
    }
    printf("PID:%d, received the code %d when pthread_mutex_id exit\n", getpid(), (int)backfromfunc);
    
    sleep(2);
    
    //waiting the pthread_sem_id finished. 
    printf("PID:%d, waiting for the pthread_sem_id to exit\n", getpid());
    if((errnum = pthread_join(pthread_sem_id, &backfromfunc)) != 0)
    {
        printf("PID:%d, Waiting the pthread_sem_id failed:%d:%s\n", getpid(), errnum, strerror(errnum));
        exit(EXIT_FAILURE);
    }
    printf("PID:%d, received the code %d when pthread_sem_id exit\n", getpid(), (int)backfromfunc);
    
    pthread_mutex_destroy(&mutex);
    sem_destroy(&sem);
    sem_destroy(&sem_add);
}
