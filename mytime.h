/*File name: mytime.h
 *Type: C header file
 *Date: 2017/04/19
 */

#ifndef MYTIME_H
#define MYTIME_H

#ifdef __cplusplus
extern "C"{
#endif
char *current_time();
long int timeoftoday();
int timemeasure(long int start_time, long int end_time);
#ifdef __cplusplus
}
#endif
#endif
