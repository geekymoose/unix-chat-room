// -----------------------------------------------------------------------------
/**
 * \file	client_data.c
 * \author	Constantin MASSON
 * \date	June 24, 2016
 *
 * \brief	Client data component
 * \note	C Library for the Unix Programming Project
 */
// -----------------------------------------------------------------------------

#include "client_data.h"

void client_data_init(ClientData *client){
	memset(client, 0x00, sizeof(ClientData));
	client->is_working	= TRUE;
	client->status		= DISCONNECTED;
}
