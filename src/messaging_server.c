// -----------------------------------------------------------------------------
/**
 * \file	messaging_server.c
 * \author	Constantin MASSON
 * \date	June 24, 2016
 *
 * \brief	Message logic / Server side only
 * \note	C Library for the Unix Programming Project
 */
// -----------------------------------------------------------------------------

#include "messaging_server.h"


// -----------------------------------------------------------------------------
// Static functions
// -----------------------------------------------------------------------------
static void messaging_server_exec_connect(ServerData*, User*, const char*);
static void messaging_server_exec_disconnect(ServerData*, User*);
static void messaging_server_exec_whisper(ServerData*, User*, char*, char*);
static void messaging_server_exec_room_open(ServerData*, User*, char*);
static void messaging_server_exec_room_close(ServerData*, User*, char*);
static void messaging_server_exec_room_enter(ServerData*, User*, char*);
static void messaging_server_exec_room_leave(ServerData*, User*);
static void messaging_server_exec_room_bdcast(ServerData*, User*, char*);



// -----------------------------------------------------------------------------
// Receive process Functions
// -----------------------------------------------------------------------------

int messaging_server_exec_receive(ServerData *server, User *user, char *msg){
	if(msg == NULL){ return -1; }

	//Recover the type of message (First element in msg, must be not NULL)
	char *token = strtok(msg, MSG_DELIMITER);
	if(token == NULL){ return -1; }

	//User messages
	if(strcmp(token, MSG_TYPE_CONNECT) == 0){
		messaging_server_exec_connect(server, user, strtok(NULL, MSG_DELIMITER));
		return 1;
	}
	else if(strcmp(token, MSG_TYPE_DISCONNECT) == 0){
		messaging_server_exec_disconnect(server, user);
		return 1;
	}
	else if(strcmp(token, MSG_TYPE_WHISPER) == 0){
		char *sender	= strtok(NULL, MSG_DELIMITER);
		char *receiver	= strtok(NULL, MSG_DELIMITER);
		char *msg		= strtok(NULL, MSG_DELIMITER);
		messaging_server_exec_whisper(server, user, receiver, msg);
		return 1;
	}

	//Room messages
	else if(strcmp(token, MSG_TYPE_ROOM_OPEN) == 0){
		char *name = strtok(NULL, MSG_DELIMITER);
		messaging_server_exec_room_open(server, user, name);
	}
	else if(strcmp(token, MSG_TYPE_ROOM_CLOSE) == 0){
		char *name = strtok(NULL, MSG_DELIMITER);
		messaging_server_exec_room_close(server, user, name);
		return 1;
	}
	else if(strcmp(token, MSG_TYPE_ROOM_ENTER) == 0){
		char *name = strtok(NULL, MSG_DELIMITER);
		messaging_server_exec_room_enter(server, user, name);
		return 1;
	}
	else if(strcmp(token, MSG_TYPE_ROOM_LEAVE) == 0){
		messaging_server_exec_room_leave(server, user);
		return 1;
	}
	else if(strcmp(token, MSG_TYPE_ROOM_BDCAST) == 0){
		char *msg = strtok(NULL, MSG_DELIMITER);
		messaging_server_exec_room_bdcast(server, user, msg);
	}
	return -1; //Means no message match
}


// -----------------------------------------------------------------------------
// Static functions (USER MESSAGES)
// -----------------------------------------------------------------------------

static void messaging_server_exec_connect(ServerData *server, User *user, const char *user_name){
	//name must be not null
	if(user_name == NULL){
		fprintf(stderr, "Connect requested with invalid name (NULL)\n");
		messaging_send_error(user->socket, MSG_ERR_CONNECT, "Name is not valid.");
		return;
	}
	strcpy(user->login, user_name);
	int errstatus = server_data_add_user(server, user);
	//If invalid name
	if(errstatus == -1){
		fprintf(stderr, "[ERR] Connect requested with invalid name: %s\n", user_name);
		messaging_send_error(user->socket, MSG_ERR_CONNECT, "Name is not valid.");
		return;
	}
	//If user already in server
	else if(errstatus == -2){
		fprintf(stderr, "[ERR] Connect requested but name already used: %s\n", user_name);
		messaging_send_error(user->socket, MSG_ERR_CONNECT,  "Name is already used.");
		return;
	}

	//Place user in default room and send registration confirmation
	Room *defaultRoom = list_get_where(&(server->list_rooms), ROOM_WELCOME_NAME, room_match_name);
	if(defaultRoom == NULL){
		//TODO user should be removed from server
		fprintf(stderr, "[ERR] Unable to recover the default room for new user\n");
		messaging_send_error(user->socket, MSG_ERR_CONNECT,  "An error occured, please try again.");
		return;
	}
	fprintf(stdout, "[USER] New user (%s) added in server (Sending confirmation)\n", user_name);
	messaging_send_confirm(user->socket, MSG_CONF_REGISTER, "You have been successfully registered in server");
	room_add_user(defaultRoom, user);
	return;
}

static void messaging_server_exec_disconnect(ServerData* server, User* user){
	//Params must be not null
	if(user == NULL){
		fprintf(stderr, "[ERR] Invalid disconnect message (NULL data)\n");
		return;
	}

	int errstatus = server_data_remove_user(server, user);
	if(errstatus != 1){
		fprintf(stdout, "[ERR] Unable to recover the room user %s was before disconnecting\n", user->login);
		return;
	}
	fprintf(stdout, "[USER] '%s' disconnect (In room '%s')\n", user->login, user->room);
	messaging_send_confirm(user->socket, MSG_CONF_DISCONNECT, "You have been successfully disconnected");
}

static void messaging_server_exec_whisper(ServerData *server, User *user, char *receiver, char *msg){
	//Check valid message parameters (not null)
	if(user == NULL || receiver == NULL || msg == NULL){
		fprintf(stderr, "[ERR] Invalid whisper message (NULL data)\n");
		return;
	}

	//Message shouldn't be empty (Or just spaces)
	msg = str_trim(msg);
	if(strlen(msg) == 0){
		fprintf(stderr, "[ERR] Invalid whisper message (Empty message)\n");
		return;
	}

	//Recover the receiver from list of user (Send error if wrong)
	User *u = list_get_where(&(server->list_users), receiver, user_match_name);
	if(u == NULL){
		messaging_send_error(user->socket, MSG_ERR_UNKOWN_USER, "User doesn't exists.");
		return;
	}

	//@TODO add mutex for writing in socket
	messaging_send_whisper(u->socket, user->login, receiver, msg);
}


// -----------------------------------------------------------------------------
// Static functions (ROOM MESSAGES)
// -----------------------------------------------------------------------------

static void messaging_server_exec_room_open(ServerData *server, User *user, char *name){
	//Params must be not null
	if(user == NULL || name == NULL){
		fprintf(stderr, "[ERR] Invalid open message (NULL data)\n");
		messaging_send_error(user->socket, MSG_ERR_GENERAL, "Invalid room name.");
		return;
	}

	//Try to add room and check error
	name = str_trim(name);
	int errstatus = server_data_add_room(server, user, name);
	switch(errstatus){
		case 1: //OK
			fprintf(stdout, "[ROOM] New room created: '%s' (Owner: '%s')\n", name, user->login);
			messaging_send_confirm(user->socket, MSG_CONF_GENERAL, "Room successfully created");
			return;
		case -1: //Invalid name
			fprintf(stderr, "[ERR] Invalid open message: room name '%s'\n", name);
			messaging_send_error(user->socket, MSG_ERR_GENERAL, "Invalid room name.");
			return;
		case -2: //Room already used by server
			fprintf(stderr, "[ERR] Invalid open message: room name '%s' already used.\n", name);
			messaging_send_error(user->socket, MSG_ERR_GENERAL, "Invalid room name: already used.");
			return;
		case -3: //Internal error (Malloc error)
			fprintf(stderr, "[ERR] Unable to create room '%s': internal error (malloc).\n", name);
			messaging_send_error(user->socket, MSG_ERR_GENERAL, "Sorry, we are unable to create the room.");
			return;
	}
}

static void messaging_server_exec_room_close(ServerData* server, User* user, char* name){
	//Process param (Check if valid name)
	name = (name == NULL) ? NULL : str_trim(name);
	if(user == NULL || name == NULL || room_is_valid_name(name) != 1){
		fprintf(stderr, "[ERR] Invalid enter message\n");
		messaging_send_error(user->socket, MSG_ERR_GENERAL, "Invalid room name.");
		return;
	}
	
	//Try to remove room and send err message if error (See doc fct for err value)
	int errstatus = server_data_remove_room(server, user, name);
	switch(errstatus){
		case 1:
			messaging_send_confirm(user->socket, MSG_CONF_GENERAL, "Room successfully closed.");
			fprintf(stdout, "[ROOM] '%s' closed. (Owner: '%s')\n", name, user->login);
			return;
		case -1:
			messaging_send_error(user->socket, MSG_ERR_GENERAL, "Room doesn't exists.");
			return;
		case -2:
			messaging_send_error(user->socket, MSG_ERR_GENERAL, "Room must be empty in order to be closed.");
			return;
		case -3:
			messaging_send_error(user->socket, MSG_ERR_GENERAL, "You must own this room.");
			return;
		case -4:
			messaging_send_error(user->socket, MSG_ERR_GENERAL, "Error occured while closing.");
			return;
	}
}

static void messaging_server_exec_room_enter(ServerData* server, User* user, char* name){
	//Params must be not null
	name = (name == NULL) ? NULL : str_trim(name);
	if(user == NULL || name == NULL || room_is_valid_name(name) != 1){
		fprintf(stderr, "[ERR] Invalid enter message\n");
		messaging_send_error(user->socket, MSG_ERR_GENERAL, "Invalid room name.");
		return;
	}

	//To enter a room, user must be first in the default room (The one from connection)
	if(strcmp(user->room, ROOM_WELCOME_NAME) != 0){
		messaging_send_error(user->socket, MSG_ERR_GENERAL, "You must leave your current room first.");
		return;
	}

	//Check whether the requested room exists
	//TODO Add mutex on the room list
	Room* new_room = list_get_where(&(server->list_rooms), (void*)name, room_match_name);
	Room* old_room = list_get_where(&(server->list_rooms), (void*)user->room, room_match_name);
	if(new_room == NULL || old_room == NULL){
		messaging_send_error(user->socket, MSG_ERR_GENERAL, "Room doesn't exists...");
		return;
	}

	//Change user room
	room_remove_user(old_room, user);
	room_add_user(new_room, user);
	messaging_send_confirm(user->socket, MSG_CONF_ROOM_ENTER, "You successfully enterred the room.");
	fprintf(stdout, "[ROOM] User '%s' moved from '%s' to '%s'\n", user->login, old_room->name, new_room->name);
}

static void messaging_server_exec_room_leave(ServerData* server, User* user){
	//Params must be not null
	if(user == NULL){
		fprintf(stderr, "[ERR] Invalid leave message\n");
		return;
	}

	//If was already in welcome room, then disconnect user instead.
	if(strcmp(user->room, ROOM_WELCOME_NAME) == 0){
		messaging_server_exec_disconnect(server, user);
		return;
	}

	//Recover current user room and welcome room
	//TODO Add mutex on the room list
	Room* old_room = list_get_where(&(server->list_rooms), (void*)user->room, room_match_name);
	Room* new_room = list_get_where(&(server->list_rooms), (void*)ROOM_WELCOME_NAME, room_match_name);
	if(old_room == NULL || new_room == NULL){
		messaging_send_error(user->socket, MSG_ERR_GENERAL, "Error occurent, unable to leave room.");
		return;
	}

	//Change user room
	room_remove_user(old_room, user);
	room_add_user(new_room, user);
	messaging_send_confirm(user->socket, MSG_CONF_ROOM_ENTER, "You successfully leaved the room.");
	fprintf(stdout, "[ROOM] User '%s' leave room '%s'\n", user->login, old_room->name);
}

static void messaging_server_exec_room_bdcast(ServerData *server, User *user, char* msg){
	//Skipp invalid data
	if(user == NULL || msg == NULL){
		fprintf(stdout, "[ERR] Invalid message '%s' from '%s'\n", user->login, msg);
		return;
	}

	//Recover room where user is
	Room* room = list_get_where(&(server->list_rooms), user->room, room_match_name);
	if(room == NULL){
		fprintf(stderr, "[ERR] Unable to recover the room of user '%s'\n", user->login);
		messaging_send_error(user->socket, MSG_ERR_GENERAL, "Unable to send message in room.");
		return;
	}
	fprintf(stdout, "[CHAT] '%s': '%s' send '%s'\n", user->room, user->login, msg);
	room_broadcast_message(room, user, msg);
}


