/*File name: msgdata.h
 *Type: C header file
 *Date: 2017/04/24
 */

#ifndef MSGDATA_H
#define MSGDATA_H


#ifdef __cplusplus
extern "C"{
#endif
//The struct of the message_info must contain the long int msg_type.
#define MSG_SZ 2048
struct message_info_struct
{
    long int msg_type;
    char text[MSG_SZ]; 
};

#ifdef __cplusplus
}
#endif
#endif
