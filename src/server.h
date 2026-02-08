// -----------------------------------------------------------------------------
/**
 * \file	server.h
 * \author	Constantin MASSON
 * \date	June 19, 2016
 *
 * \brief	Server component
 * \note	C Library for the Unix Programming Project
 */
// -----------------------------------------------------------------------------

#ifndef UNIXPROJECT_SERVER_H
#define UNIXPROJECT_SERVER_H

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <pthread.h>

#include "wunixlib/network.h"
#include "wunixlib/sighandler.h"
#include "wunixlib/assets.h"
#include "wunixlib/stream.h"

#include "server_data.h"
#include "user.h"
#include "messaging.h"
#include "messaging_server.h"
#include "constants.h"

/** \brief Max number of client possible in accept queue */
#define BACKLOG 10



/**
 * \brief			Start listening to new client.
 * \details			If server is already listening, do nothing.
 * \warning			Server must be not null.
 *
 * \param server	Server to start listening
 * \param socket	Server socket where to listen
 */
void server_start_listening_clients(ServerData *server, const int socket);

/**
 * \brief			Stop listening for new client.
 * \details			Do nothing if already not listening.
 * \warning			Server must be not null.
 *
 * \param server	Server to stop listening.
 */
void server_stop_listening_clients(ServerData *server);


#endif



