/*File name: main.c
 *Type: C source file
 *Date: 2017/04/17
 */
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include "file.h"
#include "mytime.h"
#include <string.h>
#include "process.h"
#include "myfifo.h"
#include "mysignal.h"
#include "shmdata.h"
#include "shmem.h"
#include "message.h"
#include "mysemaphore.h"
#include "mypthread.h"
/*ERROR INFORMATION ABOUT exit(i):
 *0 : Success
 *1 : Operation not permission
 *2 : No such file or directory
 *3 : No such process
 *4 : Interrupted system call
 *5 : Input/output error
 *6 : Device not configured
 *7 : Argument list too long
 *8 : Exec format error
 *9 : Bad file descriptor
 * */
int main(int argc, char *argv[])
{   
    //static char src_chars[100] = "This is a test of put_chars";
    //static char *fifo_name = "/home/cme/WorkspaceForTQ210/Study/LinuxApp/lesson1/fifotest";
    char buf[100];
    char timenow[30];
    long int usec_start = 0, usec_end = 0;
    
    getcwd(buf,sizeof(buf));
    printf("The current path is:%s\n", buf);
    
    strcpy(timenow,current_time());
    printf("The current time is %s\n", timenow);

    usec_start = timeoftoday();
    printf("current start time: %ld\n", usec_start);
   /*
    if(argc < 3)
    {
        printf("Please input the filename and try again\n");
        exit(EXIT_FAILURE);
    }
    
    CopyDataToFile(argv[1],argv[2]);         
    printf("%s\n",get_chars(argv[2],100));

    put_chars("testputchars",src_chars, 10);

    if(access("/etc/passwd",R_OK) == 0)
        printf("/etc/passwd can be read!\n");
    else
        printf("/etc/passwd can not be read!\n");

    if(access(argv[1],R_OK|W_OK|F_OK|X_OK) != 0)
        printf("%s doesn't have the all permission!\n", argv[1]);
    

    CreatProcess_fork(2);
    */
    // CreatProcess_vfork();
    
    
    //fifo_creat(fifo_name);
    //fifo_WRBLOCK(fifo_name, "file_src.log");
    
    /*
     * Here the process will be BLOCKED if you only uncomment the upper two lines.
     * beause there is no process to read the FIFO, the write fifo process is waiting.
    */
    //We can make another process so that to test:
    //1.Copy the lesson1 folder to lesson2: cp -R lesson1 lesson2
    //2.make the lesson1 as fifo_WRBLOCK: Just comment the fifo_RDBLOCK(fifo_name, "file_dst.log")
    //3.make the lesson2 as fifo_RDBLOCK: Just comment the fifo_WRBLOCK(fifo_name, "file_src.log")

    // fifo_RDBLOCK(fifo_name, "file_dst.log");


    //kill_test();
    //alarm_pause_test();
    //signal_test();
    //sigaction_test();
    //if(execlp("ls", "ls", "/etc", NULL) < 0)
        //perror("execp error"); //display the error information to stdout.

    //system("ls -a /etc/passwd");


    /*
     * Here we need to make some process to test the shared memory
    */
    //We can make another process so that to test:
    //1.Copy the lesson1 folder to lesson2: cp -R lesson1 lesson2
    //2.make the lesson1 as shmread.
    //3.make the lesson2 as shmwrite.

    //shmread(1234);
    //shmwrite(1234);

    /*
     * Here we need to make some process to test the message.
    */
    //We can make another process so that to test:
    //1.Copy the lesson1 folder to lesson2: cp -R lesson1 lesson2
    //2.make the lesson1 as msgsend.
    //3.make the lesson2 as msgreceive.

    //msgsend(5678);
    //msgreceive(5678);

    
    /*
     * Here we need to make some process to test the semaphore
    */
    //We can make another process so that to test:
    //1.Copy the lesson1 folder to lesson2: cp -R lesson1 lesson2
    //2.Run both of the APP.
    
    //test_semaphore();
    
    
    test_pthread();
    
    usec_end = timeoftoday();
    printf("current time: %ld\n", usec_end);
    printf("The process cost %d useconds\n", timemeasure(usec_start, usec_end));
    // exit(EXIT_SUCCESS);
}
