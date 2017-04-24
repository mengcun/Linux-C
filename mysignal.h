/*File name: mysignal.h
 *Type: C header file
 *Date: 2017/04/20
 */

#ifndef MYSIGNAL_H
#define MYSIGNAL_H

#ifdef __cplusplus
extern "C"{
#endif
void signal_test();
void SIGINT_handler(int signal_fd);
void SIGALRM_handler(int signal_fd);
void sigaction_test();
void kill_test();
void alarm_pause_test();
#ifdef __cplusplus
}
#endif
#endif
