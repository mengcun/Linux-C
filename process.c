/*File name: process.c
 *Type: C source file
 *Date: 2017/04/19 
 */
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <math.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <string.h>

/*
 *pid = fork():1.don't share the data bss; 2.the order to execute is not clear
 *creat pipe between the parent and child process
 */
void CreatProcess_fork(int wait_child)
{
    pid_t pid, pr;
    int pipe_fd[2];
    char buf_r[100];
    static char buf_w[100] = {"This is the test for pipe_write."};
    int r_num;
    int w_num;

    //initial the buf_r
    memset(buf_r, 0, sizeof(buf_r));
    //creat the pipe between parent and child.
    if(pipe(pipe_fd) < 0)
    {
        perror("creat pipe error");
        printf("creat pipe error\n");
        exit(EXIT_FAILURE);
    }
    //creat the child process
    pid = fork();
    if(pid < 0)
    {
        perror("creat process error"); //display the error information to stdout.
        printf("Error occured when creat the process (ID %d): %s\n", getpid(), strerror(errno));
        exit(EXIT_FAILURE);
    }
    else if(pid ==0)
    {
        printf("The ID %d is a child process\n", getpid());
        
        close(pipe_fd[1]);//close the write point in child process
        sleep(wait_child);//child process sleep so that parent process can write data firstly.
        
        if((r_num = read(pipe_fd[0],buf_r,50)) >= 0)
        printf("%d numbers read from the pipe is \"%s\"\n", r_num, buf_r);
        else
        {
            perror("read the pipe error");
            exit(EXIT_FAILURE);
        }
        close(pipe_fd[0]);//close the pipe read point.
        //exit(EXIT_SUCCESS);
    }
    else//This is parent process
    {
        printf("The ID %d is a parent process\n", getpid());
       
        close(pipe_fd[0]);//close the read point in parent process
        
        if((w_num = write(pipe_fd[1],buf_w, 50)) >= 0)
        printf("%d numbers write into the pipe is \"%s\"\n", w_num, buf_w);
        else
        {
            perror("read the pipe error");
            exit(EXIT_FAILURE);
        }
        close(pipe_fd[1]);//close the write point

        pr = waitpid(pid, NULL, 0);  //wait for the child process done, and don't care the status when it done.
        if(pr < 0)
        {
            perror("wait child process close error"); //display the error information to stdout.
            exit(EXIT_FAILURE);
        }
        else
        printf("Wait the child process %d finished! and Write data into pipe \"%s\" success!\n", pr, buf_w);
        
        // exit(EXIT_SUCCESS);
    }
}

/*
 *pid = vfork():1.share the data bss; 2.the child process will run firstly.
 */
void CreatProcess_vfork(int wait_child)
{
    pid_t pid, pr;
    pid = vfork();
    if(pid < 0)
    {
        perror("execp error"); //display the error information to stdout.
        printf("Error occured when creat the process (ID %d): %s\n", getpid(), strerror(errno));
        exit(EXIT_FAILURE);
    }
    else if(pid ==0)
    {
        printf("The ID %d is a child process\n", getpid());
        sleep(wait_child);
    }
    else
    {
        pr = wait(NULL);  //wait for the child process done, and don't care the status when it done.
        if(pr < 0)
        perror("execp error"); //display the error information to stdout.
        else
        printf("The ID %d is a parent process and wait the child process %d finished!\n", getpid(), pr);
    }
}
