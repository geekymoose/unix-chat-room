// -----------------------------------------------------------------------------
/**
 * \file	messaging.h
 * \author	Constantin MASSON
 * \date	June 23, 2016
 *
 * \brief	Message logic
 * \note	C Library for the Unix Programming Project
 *
 * \todo	An update can be done to regroup these functions a a generic one.
 * 			(The code inside is almost always the same, ... parameters
 * 			could be used instead in order to have only one function)
 */
// -----------------------------------------------------------------------------

#ifndef UNIXPROJECT_MESSAGING_H
#define UNIXPROJECT_MESSAGING_H

#include <stdio.h>
#include <string.h>
#include <stdarg.h> //For variable list of function arguments
#include "wunixlib/stream.h"

#define MSG_DELIMITER ";;;"

// List of possible error value
#define MSG_ERR_CONNECT "msg_err_connect"
#define MSG_ERR_UNKOWN_USER "msg_err_unknown_user"
#define MSG_ERR_GENERAL "msg_err_general"

// List of possible confirm value
#define MSG_CONF_REGISTER "msg_conf_register"
#define MSG_CONF_GENERAL "msg_conf_general"
#define MSG_CONF_ROOM_ENTER "msg_conf_room_enter"
#define MSG_CONF_ROOM_CLOSE "msg_conf_room_close"
#define MSG_CONF_DISCONNECT "msg_conf_disconnect"

// List of possible message type
#define MSG_TYPE_CONNECT "connect"
#define MSG_TYPE_DISCONNECT "bye"
#define MSG_TYPE_WHISPER "whisper"

#define MSG_TYPE_ROOM_OPEN "open"
#define MSG_TYPE_ROOM_CLOSE "close"
#define MSG_TYPE_ROOM_ENTER "enter"
#define MSG_TYPE_ROOM_LEAVE "leave"
#define MSG_TYPE_ROOM_BDCAST "bdcast"

#define MSG_TYPE_CONFIRM "confirm"
#define MSG_TYPE_ERROR "error"

typedef struct _room_bdcast_params{
	char* user_name;
	char* room_name;
	char* msg;
} bdcast_params;

// -----------------------------------------------------------------------------
// Send functions
//
// I didn't write doc for these functions. The working process is simple:
// each function send to the given socket a specific message
// Message is formated using a defined format and delimiter etc.
// Each message send -1 if error, otherwise, 1
//
// Warning: atm, any parameter test is done and parameter should be valid (Not null etc)
// -----------------------------------------------------------------------------

//User messages
int messaging_send_connect(const int socket, const char *name);
int messaging_send_bye(const int socket);
int messaging_send_whisper(const int socket, const char *sender, const char *receiver, const char *msg);

//Room messages
int messaging_send_room_open(const int socket, const char *name);
int messaging_send_room_close(const int socket, const char *name);
int messaging_send_room_enter(const int socket, const char *name);
int messaging_send_room_leave(const int socket);
int messaging_send_room_bdcast(const int socket, const char* sender, const char* room, const char *msg);

//Asset messages
int messaging_send_confirm(const int socket, char *type, const char *msg);
int messaging_send_error(const int socket, char *type, char *msg);



#endif



