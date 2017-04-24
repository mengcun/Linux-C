/*File name: mysignal.c
 *Type: C source file
 *Date: 2017/04/20 
 */
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>

static int alarm_fired = 0;
/*
 *The handler for SIGINT.
 *The function shortould be like this
 */
void SIGINT_handler(int signal_fd)
{
    printf("\nI got a signal %d, and I will make the signal \"%d\" back to normal\n", signal_fd, SIGINT);
    (void) signal(SIGINT, SIG_DFL);
}

/*
 *The handler for SIGALRM.
 *The function shortould be like this
 */
void SIGALRM_handler(int signal_fd)
{
    printf("\nI got a signal %d, and the signal is \"%d\"\n", signal_fd, SIGALRM);
    alarm_fired = 1;
}

/*
 *signal test: 
 *The type of signal to catch :SIGINT
 *The signal_handler: signal_handler
 */
void signal_test()
{
    (void) signal(SIGINT, SIGINT_handler);
    while(1)
    {
        printf("Please type \"Ctrl+C\" to test the signal_test\n");
        sleep(1);
    }
}

/*
 *sigaction_test: int sigaction(int sig, const struct sigaction *act, struct sigaction *oact);
 *struct sigaction: 
 *                 void(*)(int) sa_handler;
 *                 sigset_t sa_mask;
 *                 int sa_flags;
 */
void sigaction_test()
{
    struct sigaction act;
    act.sa_handler = SIGINT_handler; //Point to the signal handler
    sigemptyset(&act.sa_mask); //Set the signal mask as empty means no signal will be masked
    act.sa_flags = SA_RESETHAND; //Set the sigaction function back to normal
    
    sigaction(SIGINT, &act, 0);

    while(1)
    {
        printf("Please type \"Ctrl+C\" to test the sigaction_test\n");
        sleep(1);
    }
}

/*
 *kill test: 
 *int kill(pid_t pid, int signal_fd)
 */

void kill_test()
{
    pid_t pid;
    pid = fork();
    switch(pid)
    {
        case -1:
            printf("Creat process failed by fork\n");
            perror("fork error");
            exit(EXIT_FAILURE);
            break;
        case 0:
            //This is the child process
            sleep(5);
            //Send the signal to parent process by kill
            kill(getppid(),SIGALRM);
            printf("The child process %d has sent the signal to parent process %d\n", getpid(), getppid());
            exit(EXIT_SUCCESS);
            break;
        default:
            break;
    }
    //Set the signal_handler
    signal(SIGALRM, SIGALRM_handler);
    while(!alarm_fired)
    {
        printf("The parent process %d didn't catch the signal \"%d\"\n", getppid(), SIGALRM);
        sleep(1);
    }
    if(alarm_fired)
        printf("The parent process %d catch the signal \"%d\"\n", getppid(), SIGALRM);
}

/*
 *alarm and pause test: 
 *unsigned int alarm(unsigned int seconds):The signal SIGALRM will be send to the current process after seconds
 *int pause(void): make the current process sleep until interrupt by signal.
 */
void alarm_pause_test()
{
    signal(SIGALRM, SIGALRM_handler);
    printf("call the alarm() and after 5s send the %d to current process %d\n", SIGALRM, getpid());
    alarm(5);
    printf("Make the current process %d sleep until catch the signal %d\n", getpid(), SIGALRM);
    pause();
    if(alarm_fired == 1)
        printf("The current process %d catch the signal %d\n", getpid(), SIGALRM);
}
