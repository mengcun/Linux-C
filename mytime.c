/*File name: mytime.c
 *Type: C source file
 *Date: 2017/04/19 
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "mytime.h"
#include <unistd.h>
#include <sys/time.h>
char *current_time()
{
    static char str_asctime[30] = {""};
    struct tm *ptr;
    time_t lt;
    lt = time(NULL);
    ptr = localtime(&lt);
    sprintf(str_asctime, "%s\n", asctime(ptr));
    return str_asctime;
}

long int timeoftoday()
{
    struct timeval tv;
    struct timezone tz;
    gettimeofday(&tv, &tz);
    return tv.tv_usec;
}

int timemeasure(long int start_time, long int end_time)
{
    return (end_time - start_time);
}
