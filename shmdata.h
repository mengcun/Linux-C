/*File name: shmdata.h
 *Type: C header file
 *Date: 2017/04/21
 */

#ifndef SHMDATA_H
#define SHMDATA_H


#ifdef __cplusplus
extern "C"{
#endif

#define SHMEM_SZ 2048
struct shared_memory_struct
{
    char text[SHMEM_SZ]; //record the text
    int RW_flag; //0:Write, !0:Read
};

#ifdef __cplusplus
}
#endif
#endif
