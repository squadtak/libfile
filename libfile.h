/*
 libfile Ver 1.0 2026-06-29
 (c)2026 squad
*/

#ifndef LIBFILE_H_INCLUDED
#define LIBFILE_H_INCLUDED

#ifdef _WIN32
#define LINETERMINATOR "\r\n"
#define LINETERMINATOR_LEN 2
#elif __linux__
#define LINETERMINATOR "\n"
#define LINETERMINATOR_LEN 1
#endif

#include <stdio.h>

int mkfile(char *path);
int fcopy(char *dst, char *src);
int isFile(char *path);
off_t isFileSizeP(char *path);
off_t isFileSizeH(FILE *fileHandle);

void *fileReadEndChars(FILE *fileHandle, void *readStr, size_t readStrLen, void *endStr, size_t endStrLen);

int fileCharsInsert(FILE *fileHandle, off_t insertPos, void *insertChars, size_t insertCharsLen);
int fileCharsDelete(FILE *fileHandle, off_t deletePos, size_t deleteCharsLen);

char *fileGetFileLineTerm(FILE *fileHandle, char *lineTerm, int *lineTermLen);


#endif // LIBFILE_H_INCLUDED
