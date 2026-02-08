// -----------------------------------------------------------------------------
/**
 * \file	assets.h
 * \author	Constantin MASSON
 * \date	June 19, 2016
 *
 * \brief	Assets functions and maccros
 * \note	C Library for the Unix Programming Project
 */
// -----------------------------------------------------------------------------

#ifndef WUNIXLIB_ASSETS_H
#define WUNIXLIB_ASSETS_H

#include <stdio.h>
#include <assert.h>


// ----------------------------------------------------------------------------
// Defines
// ----------------------------------------------------------------------------
#define TRUE 1
#define FALSE 0


// ----------------------------------------------------------------------------
// MACROS
// ----------------------------------------------------------------------------

/**
 * \brief	TEMP_FAILURE_RETRY macro
 * \details	Implementation from _GNU_SOURCE
 */
#define TEMP_FAILURE_RETRY(expression) \
		(__extension__ ({ long int __result; \
			do __result = (long int) (expression);\
			while (__result == -1L && errno == EINTR);\
		__result; }))

/**
 * \brief			Display log error message
 * \param source	Source function of the error
 */
#define LOG_ERR(source) \
		perror(source); \
		fprintf(stderr, "[ERR] file %s, line %d\n", __FILE__, __LINE__);

/**
 * \brief		Display simple log message
 * \param msg	Message to display
 */
#define LOG_MSG(msg) \
	fprintf(stderr, "[ERR] file %s, line %d\n", __FILE__, __LINE__);


// ----------------------------------------------------------------------------
// Prototypes
// ----------------------------------------------------------------------------

/**
 * \brief		Read one line from stdin buffer.
 * \details		If the read line is bigger than buffer, only 'size-1' elements 
 * 				are recovered, then the buffer is cleaned.
 *
 * \param buff	Buffer where to place read line
 * \param size	Buffer size
 */
int readline_stdin(char buff[], int size);

/**
 * \brief Clear the current stdin buffer
 */
void clear_buffer();

/**
 * \brief		Remove the first space characters from the string
 * \details		Simple implementation of trim for first characters
 * \warning		Assert error thrown if str is NULL.
 *
 * \param str	String to trim
 * \return		Pointer to the new first position in the string
 */
char* str_trim(char *str);

#endif



