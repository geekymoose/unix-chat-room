// -----------------------------------------------------------------------------
/**
 * \file	room.h
 * \author	Constantin MASSON
 * \date	June 23, 2016
 *
 * \brief	Room component
 * \note	C Library for the Unix Programming Project
 */
// -----------------------------------------------------------------------------

#ifndef UNIXPROJECT_ROOM_H
#define UNIXPROJECT_ROOM_H

#include <stdio.h>
#include "constants.h"
#include "user.h"

#include "wunixlib/linkedlist.h"


// -----------------------------------------------------------------------------
// Structures
// -----------------------------------------------------------------------------

/**
 * \brief Define a room component.
 */
typedef struct _room{
	char name[ROOM_MAX_SIZE+1]; //+1 for '\0'
	Linkedlist list_users; //List user in this room
	char owner_name[USER_MAX_SIZE];
} Room;


// -----------------------------------------------------------------------------
// General Functions
// -----------------------------------------------------------------------------

/**
 * \brief		Create a new room.
 * \details		Created room must be destroyed when not used anymore.
 * \warning		Parameters must be not null, otherwise, assert error thrown.
 *
 * \param owner	User owner of this room at creation time
 * \param name	Name of the room
 * \return		Pointer to created room or NULL if error
 */
Room* room_create(User *owner, const char *name);

/**
 * \brief		Destroy a room (Free all its memory).
 * \details		The given room won't exists anymore
 * \warning		Thrown assert error if null parameter
 * \note		The list of user remains!! (No free call on it!)
 *
 * \param room	Room to destroy
 * \return		1 if successfully destroyed, otherwise, return -1
 */
int room_destroy(Room *room);

/**
 * \brief		Check whether the given name is valid for a room
 * \details		Null name will return -1
 * \bug			Will accept name with only spaces (But should be invalid)
 * 				name should be trim before this function!
 *
 * \param name	The name to test
 * \return		1 if valid, otherwise, return -1
 */
int room_is_valid_name(const char *name);

/**
 * \brief		Send a message to all user in the char room.
 *
 * \param room	Room where to broadcast
 * \param user	Sender of the message
 * \param msg	Message to send
 */
void room_broadcast_message(Room *room, User *user, char* msg);

/**
 * \brief		Check whether the room is empty (No user inside).
 * \warning		Parameter must be not null and valid.
 *
 * \param room	Room to test
 * \return		1 if empty, otherwise, return 0
 */
int room_is_empty(Room *room);


// -----------------------------------------------------------------------------
// Room / User management
// -----------------------------------------------------------------------------

/**
 * \brief		Add a user in the room.
 * \details		If user already in room, fail and return -1.
 * \warning		Throw assert error if null param.
 *
 * \param room	Room where to place user
 * \param user	User to add in the room
 * \return		1 if successfully added, otherwise, return -1
 */
int room_add_user(Room *room, User *user);

/**
 * \brief		Remove user from room.
 * \details		If user is not in room, fail and return -1.
 * \warning		Throw assert error if null param.
 *
 * \param room	Room where to remove user
 * \param user	User to remove from room
 * \return		1 if successfully removed, otherwise, return -1
 */
int room_remove_user(Room *room, User *user);


// -----------------------------------------------------------------------------
// List Functions
// -----------------------------------------------------------------------------

/**
 * \brief		Used for list.
 * \details		Check whether name given match room name from list.
 * \details		See wunixlib/Linkedlist documentation for further informations.
 *
 * \param Room	Room tested
 * \param name	Name to test
 * \return		1 if match, otherwise, return 0
 */
int room_match_name(void* room, void* name);

/**
 * \brief		Display one room data.
 * \details		Meant to be used as iterate function for list.
 * 				Room can be null (Null will be displayed).
 *
 * \param room	Void pointer to room
 * \return		1
 */
int room_display(void* room);

/**
 * \todo	To implements
 * \brief	Used when destroy list of rooms
 */
void room_free_elt(void* room);


#endif



