/*File name: message.h
 *Type: C header file
 *Date: 2017/04/24
 */

#ifndef MESSAGE_H
#define MESSAGE_H

#ifdef __cplusplus
extern "C"{
#endif

void msgreceive(int msginfo_fd);
void msgsend(int msginfo_fd);

#ifdef __cplusplus
}
#endif
#endif
