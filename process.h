/*File name: process.h
 *Type: C header file
 *Date: 2017/04/19
 */

#ifndef PROCESS_H
#define PROCESS_H

#ifdef __cplusplus
extern "C"{
#endif
void CreatProcess_vfork(int wait_child);
void CreatProcess_fork(int wait_child);
#ifdef __cplusplus
}
#endif
#endif
