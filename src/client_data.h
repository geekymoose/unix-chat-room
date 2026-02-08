// -----------------------------------------------------------------------------
/**
 * \file	client_data.h
 * \author	Constantin MASSON
 * \date	June 24, 2016
 *
 * \brief	Client data component
 * \note	C Library for the Unix Programming Project
 */
// -----------------------------------------------------------------------------


//------------------------------------------------------------------------------
// Structure
//------------------------------------------------------------------------------

#ifndef UNIXPROJECT_CLIENT_DATA_H
#define UNIXPROJECT_CLIENT_DATA_H

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>

#include "wunixlib/assets.h"
#include "constants.h"


//------------------------------------------------------------------------------
// Structures
//------------------------------------------------------------------------------

typedef enum _clientstatus{
	CONNECTED,		//Is connected and approved
	DISCONNECTED,		//Is not connected to any server
	CONNECTING,		//Try to connect a server
} ClientStatus;

/**
 * \brief	Define the client status
 */
typedef struct _client{
	volatile sig_atomic_t	is_working;
	ClientStatus			status;
	char					login[USER_MAX_SIZE];
	char					room[ROOM_MAX_SIZE];
	int						socket;
} ClientData;


//------------------------------------------------------------------------------
// Prototypes
//------------------------------------------------------------------------------
/**
 * \brief			Initialize a client.
 * \details			Set the client values to default.
 *
 * \param client	Client to initialize
 */
void client_data_init(ClientData *client);

#endif



