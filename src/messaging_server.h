// -----------------------------------------------------------------------------
/**
 * \file	messaging_server.h
 * \author	Constantin MASSON
 * \date	June 24, 2016
 *
 * \brief	Message logic / Server side only
 * \note	C Library for the Unix Programming Project
 */
// -----------------------------------------------------------------------------

#ifndef UNIXPROJECT_MESSAGING_SERVER_H
#define UNIXPROJECT_MESSAGING_SERVER_H


#include <stdio.h>
#include <string.h>

#include "wunixlib/stream.h"
#include "server_data.h"
#include "messaging.h"
#include "room.h"
#include "user.h"


// -----------------------------------------------------------------------------
// Receive process
// -----------------------------------------------------------------------------

/**
 * \brief			Process the given message in server side.
 * \details			Recover the type of message from the given msg and execute
 * 					the action for that kind of message.
 * 					NULL message return -1.
 * \note			This function is meant to be used by server side.
 * \warning			Message parameter will be altered and shouldn't be used anymore.
 * \warning			Server shouldn't be null.
 *
 * \param server	Server used
 * \param user		User who sent the message
 * \param msg		Message to process
 * \return			1 if successfully processed, otherwise, -1 (Unknown message)
 */
int messaging_server_exec_receive(ServerData *server, User *user, char *msg);


#endif



