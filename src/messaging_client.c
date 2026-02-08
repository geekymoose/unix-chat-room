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
static int messaging_client_receiv_confirm(ClientData *client, char* type, const char *msg){
	//Process type for specific action
	if(strcmp(type, MSG_CONF_REGISTER) == 0){
		client->status = CONNECTED;
		commands_welcome_menu(msg);
		return 1;
	}
	else if(strcmp(type, MSG_CONF_ROOM_ENTER) == 0){
		//TODO could change the current room name in local
	}
	else if(strcmp(type, MSG_CONF_DISCONNECT) == 0){
		client->status = DISCONNECTED;
		commands_welcome_menu(msg);
		return 1;
	}

	//Display message if one
	if(msg != NULL && strlen(msg)>0){
		fprintf(stdout, "\n%s\n", msg);
	}
	return 1;
}

static int messaging_client_receiv_error(ClientData *client, char *type, char *msg){
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

	// Asset messages
	if(strcmp(token, MSG_TYPE_CONFIRM) == 0){
		char *type	= strtok(NULL, MSG_DELIMITER);
		char *msg	= strtok(NULL, MSG_DELIMITER);
		messaging_client_receiv_confirm(client, type, msg);
	}
	else if(strcmp(token, MSG_TYPE_ERROR) == 0){
		char *type	= strtok(NULL, MSG_DELIMITER);
		char *msg	= strtok(NULL, MSG_DELIMITER);
		messaging_client_receiv_error(client, type, msg);
	}
	//User message
	else if(strcmp(token, MSG_TYPE_WHISPER) == 0){
		char *sender	= strtok(NULL, MSG_DELIMITER);
		char *receiver	= strtok(NULL, MSG_DELIMITER);
		char *msg		= strtok(NULL, MSG_DELIMITER);
		fprintf(stdout, "\nwhisper [%s]: '%s'\n", sender, msg);
	}
	//Room messages
	else if(strcmp(token, MSG_TYPE_ROOM_BDCAST) == 0){
		char *msg		= strtok(NULL, MSG_DELIMITER);
		char *room		= strtok(NULL, MSG_DELIMITER);
		char *sender	= strtok(NULL, MSG_DELIMITER);
		fprintf(stdout, "\nroom %s [%s]: %s\n", room, sender, msg);
	}
	return -1; //Means no message match
}

