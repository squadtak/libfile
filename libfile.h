/**
 * \file		libfile.h
 * \brief		snippet library for file
 * \author		squad
 * \version		Ver 1.1
 * \date		2026-06-30
 * \copyright	Copyright 2026 squad  This library is released under the MIT license.
 */

/**
 * @mainpage libfile
 * snippet library for file
 *
 * \copyright	Copyright 2026 squad
 * \license		This library is released under the MIT license.
 *
 * Headers
 * =======
 * including:
 * @code
 * #include "libfile.h"
 * @endcode
 *
 * Functions
 * =========
 * \sa mkfile() make file
 * \sa fcopy() copy file
 * \sa isFile() is file?
 * \sa isFileSizeP() is file size? (path)
 * \sa isFileSizeH() is file size? (FILE pointer)
 * \sa fileReadEndChars() read the file until the end characters appears.
 * \sa fileCharsInsert() insert characters in specified location.
 * \sa fileCharsDelete() delete characters in specified location.
 * \sa fileGetFileLineTerm() get file line termination
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


/**
 * \brief	make file
 *
 * \param 	[in]		*path	file path
 *
 * \return	0				success or already exist
 * \return	-1 to -3		error
 */
int mkfile(char *path);

/**
 * \brief	copy file
 *
 * \param 	[in]		*dst	destination file path
 * \param 	[in]		*src	source file path
 *
 * \return	0			success
 * \return	-1 to -5	error
 */
int fcopy(char *dst, char *src);

/**
 * \brief	is file?
 *
 * \param 	[in]		*path		file path
 *
 * \return	0			success
 * \return	-1			error
 */
int isFile(char *path);
/**
 * \brief	is file size? (path)
 *
 * \param 	[in]		*path		file path
 *
 * \return	>0			file size
 * \return	-1			error
 */
off_t isFileSizeP(char *path);

/**
 * \brief	is file size? (FILE pointer)
 *
 * \param 	[in]		fileHandle	file pointer (from fopen())
 *
 * \return	>0			file size
 * \return	-1	 		error
 */
off_t isFileSizeH(FILE *fileHandle);

/**
 * \brief	read the file until the end characters appears.
 *
 * \param 	[in]		fileHandle	file pointer (from fopen())
 * \param 	[out]		*readStr	read string (must be allocated)
 * \param 	[in]		readStrLen	max readStr size
 * \param 	[in]		*endStr		end characters
 * \param 	[in]		*endStrLen	end characters length
 *
 * \return	*readStr		success
 * \return	NULL		error
 */
void *fileReadEndChars(FILE *fileHandle, void *readStr, size_t readStrLen, void *endStr, size_t endStrLen);

/**
 * \brief	insert characters in specified location.
 *
 * \param 	[in]		fileHandle		file pointer (from fopen())
 * \param 	[in]		insertPos		insert position from header of file
 * \param 	[in]		insertChars		insert characters
 * \param 	[in]		insertCharsLen	insert characters length
 *
 * \return	0			success
 * \return	-1 to -9	error
 */
int fileCharsInsert(FILE *fileHandle, off_t insertPos, void *insertChars, size_t insertCharsLen);

/**
 * \brief	delete characters in specified location.
 *
 * \param 	[in]		fileHandle		file pointer (from fopen())
 * \param 	[in]		deletePos		insert position from header of file
 * \param 	[in]		deleteCharsLen	delete characters length
 *
 * \return	0			success
 * \return	-1 to -10	error
 */
int fileCharsDelete(FILE *fileHandle, off_t deletePos, size_t deleteCharsLen);

/**
 * \brief	get file line termination
 *
 * \param 	[in]		fileHandle		file pointer (from fopen())
 * \param 	[out]		lineTerm		line termination (At least 2 bytes must be allocated)
 * \param 	[in]		lineTermLen		line termination length
 *
 * \return	*lineTerm	success
 * \return	NULL 		error
 */
char *fileGetFileLineTerm(FILE *fileHandle, char *lineTerm, int *lineTermLen);


#endif // LIBFILE_H_INCLUDED
