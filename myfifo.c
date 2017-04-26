/*File name: myfifo.c
 *Type: C source file
 *Date: 2017/04/20 
 */
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <limits.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <fcntl.h>

/*
 *Creat the FIFO and all rights for FIFO to operate.
 */
void fifo_creat(char *fifo_name)
{
    int res = 0;
    if(access(fifo_name, F_OK) == -1)
    {
        //The FIFO doesn't exist and creat it!
       if((res = mkfifo(fifo_name, 0777)) != 0)
       {
           perror("fifo_creat error");
           printf("Could not creat fifo %s\n", fifo_name);
           exit(EXIT_FAILURE);
       }    
    }
}

/*
 *Open FIFO as WRITE_ONLY & BLOCKED
 *Then write the data into FIFO 
 */
void fifo_WRBLOCK(char *fifo_name, char *src_data)
{
    int fifo_fd = -1;
    int src_data_fd = -1;
    int bytes_sent = 0;
    char buffer[PIPE_BUF + 1];
    int bytes_read = 0;
    int bytes_write = 0;
    
    fifo_fd = open(fifo_name, O_WRONLY);
    if(fifo_fd == -1)
    {
        printf("Open the FIFO %s in process %d error\n", fifo_name, getpid());
        exit(EXIT_FAILURE);
    }
    else
    {
        //Open the src_data file as READONLY
        src_data_fd = open(src_data, O_RDONLY);
        if(src_data_fd == -1)
        {
            printf("Open the src_data %s in process %d error\n", src_data, getpid());
            exit(EXIT_FAILURE);
        }
        else
        {
            //Read out the date from src_data file
            //Then write into FIFO
            if((bytes_read = read(src_data_fd, buffer, PIPE_BUF)) >= 0 )
            {
                buffer[bytes_read] = '\0';
                //Write the data into FIFO
                while(bytes_read > 0)
                {
                    if((bytes_write = write(fifo_fd, buffer, bytes_read)) == -1)
                    {
                        perror("Write data into FIFO error");
                        printf("Write the src_data %s into %s in process %d error\n", src_data, fifo_name, getpid());
                        exit(EXIT_FAILURE);
                    }
                    else
                    {
                        bytes_sent += bytes_write; //add the bytes have been writen.
                        if((bytes_read = read(src_data_fd, buffer, PIPE_BUF)) == -1)
                        {
                            
                            perror("Read the data from src_data error");
                            printf("Read the src_data %s in process %d error\n", src_data, getpid());
                            exit(EXIT_FAILURE);
                        }
                        else
                        buffer[bytes_read] = '\0';
                    }
                }
            }
            else
            {
                perror("Read the data from src_data error");
                printf("Read the src_data %s in process %d error\n", src_data, getpid());
                exit(EXIT_FAILURE);
            }
        }
        close(src_data_fd);
    }
    close(fifo_fd);
    printf("Process %d finished to wtite size of data %d into FIFO %s from %s\n", getpid(),bytes_sent, fifo_name, src_data);
    //exit(EXIT_SUCCESS);
}

/*
 *Open FIFO as READ_ONLY & BLOCKED
 *Then read the data into dst_data 
 */

void fifo_RDBLOCK(char *fifo_name, char *dst_data)
{
    int fifo_fd = -1;
    int dst_data_fd = -1;
    int bytes_sent = 0;
    char buffer[PIPE_BUF + 1];
    int bytes_read;
    int bytes_write;

    memset(buffer, '\0', sizeof(buffer));
    if((fifo_fd = open(fifo_name, O_RDONLY)) == -1)
    {
        printf("Open the FIFO %s in process %d error\n", fifo_name, getpid());
        exit(EXIT_FAILURE);
    }
    else
    {
        
        //Creat and open the dst_data as WRITE_ONLY.

        if((dst_data_fd = open(dst_data, O_WRONLY|O_CREAT, 0644)) == -1)
        {
            printf("Creat/Open the dst_data %s in process %d error\n", dst_data, getpid());
            exit(EXIT_FAILURE);
        }
        else
        {
            do
            {
                if((bytes_read = read(fifo_fd, buffer, PIPE_BUF)) >= 0)
                {
                    if((bytes_write = write(dst_data_fd, buffer, bytes_read)) == -1)
                    {
                        perror("Write the data into dst_data file from FIFO error");
                        printf("Write the FIFO data \"%s\" from %s in process %d error\n",buffer, fifo_name, getpid());
                        exit(EXIT_FAILURE);
                    }
                    else
                    {    
                        bytes_sent += bytes_write;
                    }
                }
                else
                {
                    perror("Read the data from FIFO error");
                    printf("Read the FIFO data from %s in process %d error\n", fifo_name, getpid());
                    exit(EXIT_FAILURE);
                }
            }
            while(bytes_read > 0);
        }
        close(dst_data_fd);
    }
    close(fifo_fd);
    printf("Process %d finished to wtite size of data %d into dst_data file %s from %s\n", getpid(), bytes_sent, dst_data, fifo_name);
    //exit(EXIT_SUCCESS);
}
