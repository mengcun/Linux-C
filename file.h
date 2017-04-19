/*File name: file.h
 *Type: C header file
 *Date: 2017/04/17
 */

#ifndef FILE_H
#define FILE_H

#ifdef __cplusplus
extern "C"{
#endif
FILE * open_file(char *filename);
void close_file(FILE *stream);
int CopyDataToFile(char *file_src, char *file_dst);
char *get_chars(char *filename, int numbers);
void put_chars(char *file_dst, char *src_chars, int numbers);
#ifdef __cplusplus
}
#endif
#endif
