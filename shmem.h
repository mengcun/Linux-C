/*File name: shmem.h
 *Type: C header file
 *Date: 2017/04/21
 */

#ifndef SHMEM_H
#define SHMEM_H

#ifdef __cplusplus
extern "C"{
#endif

void shmread(int shared_memory_fd);
void permission_handler(int signal_fd);
void shmwrite(int shared_memory_fd);

#ifdef __cplusplus
}
#endif
#endif
