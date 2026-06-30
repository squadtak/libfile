/*
 * \file		libfile.c
 * \brief		snippet library for file
 * \author		squad
 * \version		Ver 1.1
 * \date		2026-06-30
 * \copyright	Copyright 2026 squad
 * \license		This library is released under the MIT license.
 */

#include "libfile.h"

#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>

int mkfile(char *path)
{
	int ret = isFile(path);
	if(ret == 1) return 0;
	if(ret < 0) return -1;

	FILE *fp;

	if((fp = fopen(path, "w")) == NULL) return -2;
	if(fclose(fp) == EOF) return -3;

	return 0;
}

int fcopy(char *dst, char *src)
{
	int ret = -1;

	int rett = 0;
	if((rett = isFile(src)) < 0 || rett == 0) goto err0;

	FILE *srcHandler = NULL;
	FILE *dstHandler = NULL;

	if((srcHandler = fopen(src, "rb")) == NULL){
		ret = -2;
		goto err0;
	}

	if((dstHandler = fopen(dst, "wb")) == NULL){
		ret = -3;
		goto err1;
	}

	do{
		unsigned char readbuf[4096];
		int readcnt = 0;

		readcnt = fread(readbuf, 1, sizeof(readbuf), srcHandler);

		if(ferror(srcHandler)){
			ret = -4;
			goto err2;
		}

		if(fwrite(readbuf, 1, readcnt, dstHandler) < readcnt){
			ret = -5;
			goto err2;
		}

	}while(!feof(srcHandler));

	ret = 0;
err2:
	fclose(dstHandler);
err1:
	fclose(srcHandler);
err0:
	return ret;
}

int isFile(char *path)
{
	struct stat fileStat;
	if(stat(path, &fileStat) != 0) return -1;
	return S_ISREG(fileStat.st_mode);
}

off_t isFileSizeP(char *path)
{
	struct stat fileStat;
	if(stat(path, &fileStat) != 0) return -1;
	return fileStat.st_size;
}

off_t isFileSizeH(FILE *fileHandle)
{
	struct stat fileStat;
	if(fstat(fileno(fileHandle), &fileStat) != 0) return -1;
	return fileStat.st_size;
}

void *fileReadEndChars(FILE *fileHandle, void *readStr, size_t readStrLen, void *endStr, size_t endStrLen)
{
	if(!fileHandle || !readStr || readStrLen == 0) return NULL;

	unsigned char *readStrBuf = readStr;
	unsigned char *endStrBuf = endStr;

	unsigned char *findPos = NULL;

	unsigned char compBuf[endStrLen];

	while((*compBuf = *readStrBuf++ = getc(fileHandle)) != EOF && (readStrBuf - (unsigned char *)readStr) != readStrLen){

		if(endStr && *compBuf == *endStrBuf){
			endStrBuf++;
			if((endStrBuf - (unsigned char *)endStr) == endStrLen){
				*findPos = '\0';
				return findPos;
			}
			continue;
		}

		findPos = readStrBuf;
		endStrBuf = endStr;
	}

	if(ferror(fileHandle)) return NULL;
	return readStr;
}

int fileCharsInsert(FILE *fileHandle, off_t insertPos, void *insertChars, size_t insertCharsLen)
{
	if(!fileHandle || !insertChars) return -1;

	if(fflush(fileHandle) == EOF) return -2;

	//get filesize(fileStat.st_size)
	struct stat fileStat;
	if(fstat(fileno(fileHandle), &fileStat) != 0) return -3;

	size_t bufSize = fileStat.st_size - insertPos;
	if(fileStat.st_size < insertPos) bufSize = 0;

	unsigned char *buf = NULL;
	if(bufSize){
		buf = calloc(bufSize, 1);
		if(buf == NULL) return -4;
	}

	if(bufSize){
		if(fseeko(fileHandle, insertPos, SEEK_SET)) { free(buf); return -5; }
		if(fread(buf, 1, bufSize, fileHandle) < bufSize) { free(buf); return -6; }
	}

	if(fseeko(fileHandle, insertPos, SEEK_SET)) { free(buf); return -7; }
	if(fwrite(insertChars, 1, insertCharsLen, fileHandle) < insertCharsLen) { free(buf); return -8; }
	if(bufSize) if(fwrite(buf, 1, bufSize, fileHandle) < bufSize) { free(buf); return -9; }

	free(buf);
	return 0;
}

int fileCharsDelete(FILE *fileHandle, off_t deletePos, size_t deleteCharsLen)
{
	if(!fileHandle || !deleteCharsLen) return -1;
	if(fflush(fileHandle) == EOF) return -2;

	//get filesize(fileStat.st_size)
	struct stat fileStat;
	if(fstat(fileno(fileHandle), &fileStat) != 0) return -3;

	if(fileStat.st_size < deletePos) return 0;

	off_t bufSize = fileStat.st_size - deletePos - deleteCharsLen;

	unsigned char *buf;
	if((buf = calloc(bufSize, 1)) == NULL) return -4;

	if(fseeko(fileHandle, deletePos + deleteCharsLen, SEEK_SET)) { free(buf); return -5; }
	if(fread(buf, 1, bufSize, fileHandle) < bufSize) { free(buf); return -6; }
	if(fseeko(fileHandle, deletePos, SEEK_SET)) { free(buf); return -7; }
	if(fwrite(buf, 1, bufSize, fileHandle) < bufSize) { free(buf); return -8; }
	if(fflush(fileHandle) == EOF) return -9;
	if(ftruncate(fileno(fileHandle), deletePos + bufSize)) { free(buf); return -10; }

	return 0;
}

char *fileGetFileLineTerm(FILE *fileHandle, char *lineTerm, int *lineTermLen)
{
	if(!fileHandle) return NULL;

	lineTerm[0] = lineTerm[1] = 0;
	if(lineTermLen) *lineTermLen = 0;

	char *readBuf = lineTerm;

	while((*readBuf = getc(fileHandle)) != EOF){

		if(*readBuf == '\r' || *readBuf == '\n'){

			readBuf++;
			if(lineTermLen) (*lineTermLen)++;

		}else if(readBuf - lineTerm){

			if(readBuf[-1] == '\r' || readBuf[-1] == '\n'){
				readBuf[0] = 0;
				break;
			}
		}
	}

	if(ferror(fileHandle)) return NULL;
	if(feof(fileHandle)) *readBuf = 0;

	return lineTerm;
}
