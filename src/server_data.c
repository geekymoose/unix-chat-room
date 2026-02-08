// -----------------------------------------------------------------------------
/**
 * \file	server_data.c
 * \author	Constantin MASSON
 * \date	June 23, 2016
 *
 * \brief	Data in the server component
 * \note	C Library for the Unix Programming Project
 */
// -----------------------------------------------------------------------------

#include "server_data.h"


void server_data_init(ServerData *data){
	assert(data != NULL);
	list_init(&(data->list_users), NULL); //User is destroyer from the thread.
	list_init(&(data->list_rooms), room_free_elt);
	data->is_listening	= 0;
	data->is_working	= 1;
}

int server_data_add_user(ServerData *server, User *user){
	//Check is valid name
	if(user_is_valid_name(user->login) != 1){
		return -1;
	}
	//Check name is not used
	if(server_data_name_is_used(&(server->list_users), user->login) == 1){
		return -2;
	}
	//Add name in server
	list_append(&(server->list_users), user); //Add user in server list
	return 1;
}

int server_data_remove_user(ServerData *server, User *user){
	//TODO Add mutex on the user list
	//Recover current user room
	Room* room = list_get_where(&(server->list_rooms), (void*)(user->room), room_match_name);
	if(room == NULL){
		user->connected = 0; //Disconnect anyway
		list_remove_where(&(server->list_users), (void*)(user->login), user_match_name);
		return -1;
	}
	//Remove user from room and disconnect user
	room_remove_user(room, user);
	user->connected = 0;
	list_remove_where(&(server->list_users), (void*)(user->login), user_match_name);
	return 1;
}

int server_data_name_is_used(const Linkedlist *list, char *name){
	return list_contains_where(list, (void*)name, user_match_name);
}

int server_data_add_room(ServerData *server, User *user, char *name){
	//Check valid name
	if(room_is_valid_name(name) == -1){
		return -1;
	}
	//Check name used in server
	if(server_data_room_is_used(&(server->list_rooms), name) == 1){
		return -2;
	}
	//Create room
	Room *room = room_create(user, name);
	if(room == NULL){
		return -3;
	}
	list_append(&(server->list_rooms), room);
	return 1;
}

int server_data_remove_room(ServerData *server, User *user, char *name){
	//TODO Add mutex on the room list
	//Check if room exists
	Room* room = list_get_where(&(server->list_rooms), (void*)name, room_match_name);
	if(room == NULL){
		return -1;
	}
	//Check whether room is empty
	if(room_is_empty(room) != 1){
		return -2;
	}
	//Check whether user is owner
	if(strcmp(user->login, room->owner_name) != 0){
		return -3;
	}
	//Actually delete the room
	int err = list_free_where(&(server->list_rooms), (void*)name, room_match_name);
	return (err == 1) ? 1 : -4;
}

int server_data_room_is_used(const Linkedlist *list, char *name){
	return list_contains_where(list, (void*)name, room_match_name);
}
