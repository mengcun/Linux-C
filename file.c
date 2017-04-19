/*File name: file.c
 *Type: C source file
 *Date: 2017/04/17 
 */
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <strings.h>

#define BUFFER_SIZE 1024


FILE * open_file(char *filename)
{
    FILE *fd;
    //Open a R&W file as append mode
    if((fd = fopen(filename, "ab+")) == NULL)
    {
        printf("Open file %s failure!\n", filename);
        exit(EXIT_FAILURE);
    }
    return fd;
}

void close_file(FILE *stream)
{
    int flag_close;
    if((flag_close = fclose(stream)) != 0)
    {
        printf("Close file %i failure!\n", flag_close);
        exit(EXIT_FAILURE);
    }
}

char *get_chars(char *filename, int numbers)
{
    FILE *fd;
    long file_len;
    int bound = 0, i = 0;
    static char c[500] = {""};
    fd = open_file(filename);
    
    fseek(fd,0L,SEEK_END);
    file_len = ftell(fd);
    fseek(fd,0L,SEEK_SET);

    if(file_len >= numbers)
        bound = numbers;
    else
        bound = file_len;
    
    while(bound != 0)
    { 
        c[i] = fgetc(fd);
        printf("Get the number of %i from %s:%c\n", i, filename, c[i]);
        bound--;
        i++;
    }
    close_file(fd);
    return c;
}

void put_chars(char *file_dst, char *src_chars, int numbers)
{
    FILE *fd;
    int i;
    fd = open_file(file_dst);
    for(i = 0; i<numbers; i++)
    {
        fputc(src_chars[i], fd);
        printf("Write the number of %i (%c) into %s success!\n", i, src_chars[i], file_dst);
    }
    close_file(fd);
}

int CopyDataToFile(char *file_src, char *file_dst)
{
    long file_len = 0;
    FILE *fd_src, *fd_dst;
    char buffer[BUFFER_SIZE];

    fd_src = open_file(file_src);    
    fd_dst = open_file(file_dst);
    
    fseek(fd_src,0L,SEEK_END);
    file_len = ftell(fd_src);
    fseek(fd_src,0L,SEEK_SET);
    printf("The file size of src is %ld\n", file_len);
    while(!feof(fd_src)) //Read out the data and write into *fd_dst until the end of the *fd_src
    {
        fread(buffer,BUFFER_SIZE,1,fd_src);

        if(BUFFER_SIZE >= file_len)
        {
            fwrite(buffer,file_len,1,fd_dst);//if the buffer is big enough, then write the file_len.
        }
        else
        {
            fwrite(buffer,BUFFER_SIZE,1,fd_dst);
            file_len -= BUFFER_SIZE;
        }

        bzero(buffer,BUFFER_SIZE);//clear the date in this memory.
    }
    close_file(fd_dst);
    close_file(fd_src);
    return 1;
}


