// -----------------------------------------------------------------------------
/**
 * \file	messaging.c
 * \author	Constantin MASSON
 * \date	June 23, 2016
 *
 * \brief	Message logic
 * \note	C Library for the Unix Programming Project
 */
// -----------------------------------------------------------------------------

#include "messaging.h"


// -----------------------------------------------------------------------------
// Static prototypes
// -----------------------------------------------------------------------------

/**
 * \brief			Send a formated message on the network.
 * \details			Create the message starting with cmd and followed by 
 * 					each ... parameter (with a specific delimiter before each).
 * \details			The given size will be used for malloc and should fit the 
 * 					total size of variable parameters.
 * \warning			All parameters must be valid!
 *
 * \param socket	Socket where to send
 * \param cmd		Command name
 * \param nb		Number of variable parameter (Important)
 * \param size_fmt	Total size of variable parameter (ex: strlen(str) )
 */
static int messaging_sender(const int socket, const char *cmd, const int nb, const int size_fmt, ...);


// -----------------------------------------------------------------------------
// User messages
// -----------------------------------------------------------------------------

int messaging_send_connect(const int socket, const char *name){
	return messaging_sender(socket, MSG_TYPE_CONNECT, 1, strlen(name), name);
}
int messaging_send_bye(const int socket){
	return messaging_sender(socket, MSG_TYPE_DISCONNECT, 0, 0);
}
int messaging_send_whisper(const int socket, const char *sender, const char *receiver, const char *msg){
	int size = strlen(sender) + strlen(receiver) + strlen(msg);
	return messaging_sender(socket, MSG_TYPE_WHISPER, 3, size, sender, receiver, msg);
}


// -----------------------------------------------------------------------------
// Room messages
// -----------------------------------------------------------------------------

int messaging_send_room_open(const int socket, const char *name){
	return messaging_sender(socket, MSG_TYPE_ROOM_OPEN, 1, strlen(name), name);
}
int messaging_send_room_close(const int socket, const char *name){
	return messaging_sender(socket, MSG_TYPE_ROOM_CLOSE, 1, strlen(name), name);
}
int messaging_send_room_enter(const int socket, const char *name){
	return messaging_sender(socket, MSG_TYPE_ROOM_ENTER, 1, strlen(name), name);
}
int messaging_send_room_leave(const int socket){
	return messaging_sender(socket, MSG_TYPE_ROOM_LEAVE, 0, 0);
}
int messaging_send_room_bdcast(const int socket, const char* sender, const char* room, const char *msg){
	//Warning: the sending order differ from parameter order.. Due to bad design.
	int size = strlen(sender) + strlen(room) + strlen(msg);
	return messaging_sender(socket, MSG_TYPE_ROOM_BDCAST, 3, size, msg, room, sender);
}


// -----------------------------------------------------------------------------
// Asset messages
// -----------------------------------------------------------------------------

int messaging_send_confirm(const int socket, char *type, const char *msg){
	int size = strlen(type) + strlen(msg);
	return messaging_sender(socket, "confirm", 2, size, type, msg);
}
int messaging_send_error(const int socket, char *type, char *msg){
	int size = strlen(type) + strlen(msg);
	return messaging_sender(socket, "error", 2, size, type, msg);
}


// -----------------------------------------------------------------------------
// Static inner functions
// -----------------------------------------------------------------------------

static int messaging_sender(const int socket, const char *cmd, const int nb, const int size_fmt, ...){
	//Prepare elements
	int		k;
	char	*ptr		= NULL;
	size_t	buffer_size	= strlen(cmd) + size_fmt + (nb*strlen(MSG_DELIMITER)) + 1;
	char	*buffer		= (char*)malloc(sizeof(char)*buffer_size);
	//Check if malloc failed.
	if(buffer == NULL){
		fprintf(stderr, "[ERR] Internal error: malloc failed (%s:%d)\n", __FILE__, __LINE__);
		return -1;
	}
	//Add cmd in first place in buffer
	strcpy(buffer, cmd);

	//Add each agurment (With MSG_DELIMITER before it)
	va_list args;
	va_start(args, size_fmt);
	for(k=0; k<nb; k++){
		ptr = va_arg(args, char*);
		strcat(buffer, MSG_DELIMITER);
		strcat(buffer, ptr);
	}
	va_end(args);

	//Send message and free buffer
	bulk_write(socket, buffer, strlen(buffer));
	free(buffer);
	return 1;
}
