// -----------------------------------------------------------------------------
/**
 * \file	client.h
 * \author	Constantin MASSON
 * \date	June 19, 2016
 *
 * \brief	Client component
 * \note	C Library for the Unix Programming Project
 */
// -----------------------------------------------------------------------------

#ifndef UNIXPROJECT_CLIENT_H
#define UNIXPROJECT_CLIENT_H

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <pthread.h>

#include "wunixlib/assets.h"
#include "wunixlib/sighandler.h"
#include "wunixlib/network.h"

#include "client_data.h"
#include "messaging_client.h"
#include "commands.h"


//------------------------------------------------------------------------------
// Prototypes
//------------------------------------------------------------------------------

/**
 * \brief			Connect the client with the given server.
 * \details			Try to connect client with server. If successfully connected, 
 * 					client start listening for any in coming message.
 * \warning			Client should be not null.
 *
 * \param client	Client to connect
 * \param address	Server to connect with (IP or name)
 * \param port		Port to use
 * \return			The socket or -1 if unable to connect
 */
int client_connect_to_server(ClientData *client, const char *address, const uint16_t port);

/**
 * \brief			Start listening any message from client socket.
 * \warning			Should be called one time only. 
 * 					(Undefined result if called several times)
 *
 * \param client	Client who start listening
 */
void client_start_listening(ClientData *client);
#endif




