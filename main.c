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
    static char src_chars[100] = "This is a test of put_chars";
    char buf[100];
    char timenow[30];
    long int usec_start = 0, usec_end = 0;
    
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

    getcwd(buf,sizeof(buf));
    printf("The current path is:%s\n", buf);
    
    strcpy(timenow,current_time());
    printf("The current time is %s\n", timenow);

    usec_start = timeoftoday();
    printf("current time: %ld\n", usec_start);
   
    usec_end = timeoftoday();

    printf("current time: %ld\n", usec_end);
    
    printf("The process cost %d useconds\n", timemeasure(usec_start, usec_end));

    CreatProcess_fork(2);
    // CreatProcess_vfork();
    
    if(execlp("ls", "ls", "/etc", NULL) < 0)
        perror("execp error"); //display the error information to stdout.

    //system("ls -a /etc/passwd");

    exit(EXIT_SUCCESS);

}
