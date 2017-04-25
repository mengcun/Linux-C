/*File name: semaphore.h
 *Type: C header file
 *Date: 2017/04/26
 */

#ifndef SEMAPHORE_H
#define SEMAPHORE_H

#ifdef __cplusplus
extern "C"{
#endif

int creat_semaphore(int semaphore_fd);
void set_semvalue(int sem_id);
void del_semvalue(int sem_id);
void semaphore_p(int sem_id);
void semaphore_v(int sem_id);
void test_semaphore();

#ifdef __cplusplus
}
#endif
#endif
