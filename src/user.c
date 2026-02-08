// -----------------------------------------------------------------------------
/**
 * \file	user.c
 * \author	Constantin MASSON
 * \date	June 23, 2016
 *
 * \brief	User component
 * \note	C Library for the Unix Programming Project
 */
// -----------------------------------------------------------------------------

#include "user.h"

User* user_create(const char *name){
	assert(name != NULL);
	User *user;
	user = (User*)malloc(sizeof(User));
	memset(user, 0x00, sizeof(User));
	memcpy(user->login, name, sizeof(name));
	user->connected = 1;
	return user;
}

void user_destroy(User* user){
	free(user);
}

int user_is_valid_name(const char *name){
	if(name == NULL){ return -1; }
	size_t size = strlen(name);
	return (size < USER_MIN_SIZE || size > USER_MAX_SIZE) ? -1 : 1;
}


// -----------------------------------------------------------------------------
// List function implementations
// -----------------------------------------------------------------------------

int user_match_name(void* user, void* name){
	User u = *(User*) user;
	name = (char*) name;
	if(strcmp(u.login, name) == 0){
		return 1;
	}
	return 0;
}

int user_display(void* user){
	if(user == NULL){
		fprintf(stdout, "User is null\n");
		return 1;
	}
	User u = *(User*) user;
	fprintf(stdout, "User '%s' %p (Room: %s) / socket '%d' \n", u.login, &u, u.room, u.socket);
	return 1;
}

int user_send_room_bdcast(void* user, void* params){
	User u = *(User*) user;
	bdcast_params p = *(bdcast_params*)params;
	messaging_send_room_bdcast(u.socket, p.user_name, p.room_name, p.msg);
	return 1;
}
