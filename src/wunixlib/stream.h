// -----------------------------------------------------------------------------
/**
 * \file	stream.h
 * \author	Constantin MASSON
 * \date	June 19, 2016
 *
 * \brief	Stream management
 * \note	C Library for the Unix Programming Project
 */
// -----------------------------------------------------------------------------

#ifndef WUNIXLIB_STREAM_H
#define WUNIXLIB_STREAM_H

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <limits.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>

#include "assets.h"


// ----------------------------------------------------------------------------
// Prototypes
// ----------------------------------------------------------------------------

/**
 * \brief			Read data from a stream
 * \details			Try to read exactly fuffsize element in buffer and block
 * 					untill it's done.
 * \warning			buffsize should be inf or equals to actual buf size.
 *
 * \param fd		File descriptor to read
 * \param buf		Buffer to fill with read data
 * \param buffsize	Max nb of character to read
 * \return			Number of characters read (negative if error)
 */
int64_t bulk_read(int, char*, size_t);

/**
 * \brief			Write data in a file
 * \details			Try to write exactly fuffsize element in buffer and block 
 * 					untill it's done.
 * \warning			Write maximum buffsize char (Must be inferior to actual buff size)
 *
 * \param fd		File descriptor where to write
 * \param buff		Buffer to fill
 * \param buffsize	Max number of char to write
 * \return			Number of written char (negative if error)
 */
int64_t bulk_write(int, char*, size_t);

/**
 * \brief			Add content in file
 * \details			Open the given file, add buf inside (To the end) and close file.
 * 					If buf size is more then 'len', only 'len' elements will be written
 *
 * \param filename	Path and name of the file to write
 * \param buf		Buffer with data to read
 * \param len		Max number of char to read from buff
 * \return			1 if successfully written, otherwise, return -1
 */
int append_to_file(char *filename, char *buf, size_t len);


#endif //end WUNIXLIB_STREAM_H


