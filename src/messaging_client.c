// -----------------------------------------------------------------------------
/**
 * \file	messaging_client.c
 * \author	Constantin MASSON
 * \date	June 24, 2016
 *
 * \brief	Message logic / Client side only
 * \note	C Library for the Unix Programming Project
 */
// -----------------------------------------------------------------------------

#include "messaging_client.h"


// -----------------------------------------------------------------------------
// Static function for server message execution
// -----------------------------------------------------------------------------
static int messaging_client_exec_confirm(ClientData *client){
	fprintf(stdout, "\nUser (%s) successfully registered in server.\n", client->login);
	client->status = CONNECTED;
}

static int messaging_client_exec_error(ClientData *client, char *type, char *msg){
	//MSG_ERR_CONNECT
	if(strcmp(type, MSG_ERR_CONNECT) == 0){
		fprintf(stderr, "\nUnable to connect: %s\n", msg);
		client->status = DISCONNECTED;
	}
	//DEFAULT
	else{
		fprintf(stderr, "\nError message: %s\n", msg);
	}
	return 1;
}


// -----------------------------------------------------------------------------
// Receive process Functions
// -----------------------------------------------------------------------------

int messaging_exec_client_receive(ClientData *client, const int socket, char *msg){
	if(msg == NULL){ return -1; }

	//Recover the type of message (First element in msg, must be not NULL)
	char *token = strtok(msg, MSG_DELIMITER);
	if(token == NULL){ return -1; }

	//Process each possible message
	if(strcmp(token,"confirm") == 0){
		messaging_client_exec_confirm(client);
	}
	else if(strcmp(token, "error") == 0){
		char *type	= strtok(NULL, MSG_DELIMITER);
		char *msg	= strtok(NULL, MSG_DELIMITER);
		messaging_client_exec_error(client, type, msg);
	}
	return -1; //Means no message match
}

