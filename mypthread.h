/*File name: mypthread.h
 *Type: C header file
 *Date: 2017/04/26
 */

#ifndef MYPTHREAD_H
#define MYPTHREAD_H

#ifdef __cplusplus
extern "C"{
#endif
void *clean(void *arg);
void *thread1_func(void *arg_cntl);
void *thread2_func(void *arg_cntl);
void *creat_member(void *arg);
void test_pthread();
#ifdef __cplusplus
}
#endif
#endif
