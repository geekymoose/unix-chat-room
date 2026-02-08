// -----------------------------------------------------------------------------
/**
 * \file	sighandler.h
 * \author	Constantin MASSON
 * \date	June 19, 2016
 *
 * \brief	Signal management
 * \note	C Library for the Unix Programming Project
 */
// -----------------------------------------------------------------------------

#ifndef WUNIXLIB_SIGHANDLER_H
#define WUNIXLIB_SIGHANDLER_H

#include <signal.h>
#include <string.h>

#include "assets.h"


// ----------------------------------------------------------------------------
// Prototypes
// ----------------------------------------------------------------------------

/**
 * \brief			Set a handler function with a specific signal
 * \warning			sigNo must be a valid signal
 *
 * \param f			Callback function when sigNo appear
 * \param sigNo		Signal to link with function
 * \return			1 if successfully created, otherwise, return -1
 */
int sethandler(void(*f)(int), int sigNo);


#endif //End WUNIXLIB_SIGHANDLER_H


