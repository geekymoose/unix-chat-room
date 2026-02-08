// -----------------------------------------------------------------------------
/**
 * \file	server_data.h
 * \author	Constantin MASSON
 * \date	June 23, 2016
 *
 * \brief	Data in the server component
 * \note	C Library for the Unix Programming Project
 */
// -----------------------------------------------------------------------------

#ifndef UNIXPROJECT_SERVER_DATA_H
#define UNIXPROJECT_SERVER_DATA_H

#include <signal.h>

#include "wunixlib/linkedlist.h"
#include "constants.h"
#include "user.h"
#include "room.h"


// -----------------------------------------------------------------------------
// Structures / Data
// -----------------------------------------------------------------------------

/**
 * \brief		Represents a server.
 * \details		Keep a list of all connected users and several data about
 * 				server status.
 */
typedef struct _server_data{
	volatile sig_atomic_t is_listening;
	volatile sig_atomic_t is_working;
	Linkedlist list_users; //List of connected users.
	Linkedlist list_rooms;
} ServerData;



// -----------------------------------------------------------------------------
// Functions
// -----------------------------------------------------------------------------

/**
 * \brief		Initialize the server data.
 * \details		Create empty list of user and set "not listening" state.
 * 				Create empty list of rooms.
 */
void server_data_init(ServerData *server);

/**
 * \brief			Add the user in the server
 * \details			User shouldn't be in the server list already.
 * \warning			Not Null parameters expected
 *
 * \param server	Server where to add user
 * \param user		User to add in server
 * \return			1 if successfully added, -1 if invalid name, -2 if already in server
 */
int server_data_add_user(ServerData *server, User *user);

/**
 * \brief			Remove the user from server.
 * \details			Will disconnect user in the same time.
 * 					Note that, even if error occurent, user is disconnected.
 * \warning			Not null parameters expected.
 *
 * \param server	Server where to add user
 * \param user		User to remove from server
 * \return			1 if successfully removed,
 * 					-1 if error (Unable to recover its room).
 */
int server_data_remove_user(ServerData *server, User *user);

/**
 * \brief			Check whether the given name is already used in the server
 *
 * \param list		List where to check.
 * \param name		User name
 * \return			1 if is already used yet in the list, otherwise, return 0
 */
int server_data_name_is_used(const Linkedlist *list, char *name);

/**
 * \brief			Add a room in the server.
 * \details			Room shoudln't be in the server already.
 * \warning			Not Null parameters expected
 *
 * \param server	Server where to add room
 * \param user		User owner of this room
 * \param name		Name to give for this room
 * \return			1 if successfully added,
 * 					-1 if invalid name,
 * 					-2 if already in server
 * 					-3 if internal error (Unable to malloc)
 */
int server_data_add_room(ServerData *server, User *user, char *name);

/**
 * \brief			Remove the room from server.
 * \details			Room must be owned by this user and exists in the server.
 * \warning			Valid parameters expected (Not null)
 *
 * \param server	Server where to remove room
 * \param user		User owner of this room
 * \param name		Room's name (The room to remove)
 * \return			1 if successfully removed,
 * 					-1 if room is not in server,
 * 					-2 if room is not empty,
 * 					-3 if user is not owner,
 * 					-4 if internal error (Unable to free data)
 */
int server_data_remove_room(ServerData *server, User *user, char *name);

/**
 * \brief			Check whether the given name is already used by a room in server.
 *
 * \param list		List of rooms in server.
 * \param name		Name to check
 * \return			1 if is already used, otherwise, return 0
 */
int server_data_room_is_used(const Linkedlist *list, char *name);



#endif



