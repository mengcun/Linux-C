/*File name: myfifo.h
 *Type: C header file
 *Date: 2017/04/20
 */

#ifndef MYFIFO_H
#define MYFIFO_H

#ifdef __cplusplus
extern "C"{
#endif
void fifo_creat(char *fifo_name);
void fifo_WRBLOCK(char *fifo_name, char *src_data);
void fifo_RDBLOCK(char *fifo_name, char *dst_data);
#ifdef __cplusplus
}
#endif
#endif
