// -----------------------------------------------------------------------------
/**
 * \file	commands.c
 * \author	Constantin MASSON
 * \date	June 19, 2016
 *
 * \brief	Client commands component
 * \note	C Library for the Unix Programming Project
 */
// -----------------------------------------------------------------------------

#include "commands.h"


// -----------------------------------------------------------------------------
// Static prototypes
// -----------------------------------------------------------------------------

//Commands exec functions
static void commands_exec_connect(ClientData *client, char *str);
static void commands_exec_bye(ClientData* client, char* args);
static void commands_exec_rooms(ClientData* client, char* args);
static void commands_exec_open(ClientData *client, char *args);
static void commands_exec_close(ClientData *client, char *args);
static void commands_exec_enter(ClientData *client, char *args);
static void commands_exec_leave(ClientData *client, char *args);
static void commands_exec_whisper(ClientData *client, char *msg);
static void commands_exec_broadcast(ClientData *client, char *msg);

//Assets functions
static int commands_is_cmd(const char *str);
static int commands_is_whisper(const char *str);
static void commands_process(ClientData *client, char *cmd);


//------------------------------------------------------------------------------
// Public functions
//------------------------------------------------------------------------------

void commands_prompt_start(ClientData *client, volatile sig_atomic_t *condition){
	commands_welcome_menu(NULL);
	commands_general_description();
	//Listen cmd while contition is true
	while(*condition == 1){
		char str[CMD_MAX_SIZE+1];
		//Change promp display according to user status
		switch(client->status){
			case CONNECTED:
				fprintf(stdout, "%s > ", client->login);
				break;
			case CONNECTING:
				fprintf(stdout, "connecting > ");
				break;
			default:
				fprintf(stdout, "> ");
				break;
		}
		readline_stdin(str, CMD_MAX_SIZE);
		commands_prompt_process_line(client, str);
	}
}

void commands_prompt_process_line(ClientData *client, char *str){
	//A command to execute in the server
	if(commands_is_cmd(str) == TRUE){
		commands_process(client, str);
		return;
	}
	//Whipser to specific user
	else if(commands_is_whisper(str) == TRUE){
		commands_exec_whisper(client, str);
		return;
	}
	//Default is the room chat
	commands_exec_broadcast(client, str);
}

void commands_help(void){
	fprintf(stdout, 
			"\nCommands:\n"
			"!connect <username>@<server> [:port]\n"
			"!bye\n"
			"!rooms\n"
			"!open <room_name>\n"
			"!close <room_name>\n"
			"!enter <room_name>\n"
			"!leave\n"
			"*username* message\n"
	);
}

void commands_welcome_menu(const char *msg){
	system("clear");
	fprintf(stdout,
			"\n"
			"  ____ _           _\n"
			" / ___| |__   __ _| |_   _ __ ___   ___  _ __ ___ \n"
			"| |   | '_ \\ / _` | __| | '__/ _ \\ / _ \\| '_ ` _ \\ \n"
			"| |___| | | | (_| | |_  | | | (_) | (_) | | | | | | \n"
			" \\____|_| |_|\\__,_|\\__| |_|  \\___/ \\___/|_| |_| |_| \n"
			"\n"
			"Console. (You can write !help to see the list of commands)\n"
			"\n"
	);

	//Display optional message if requested
	if(msg != NULL){
		fprintf(stdout, "%s\n", msg);
	}
}

void commands_general_description(void){
	//TODO Read from file instead
	fprintf(stdout, 
			"Chatroom is a client-server program to chat with other users.\n"
			"\nFeatures:\n"
			"\tWhipser: send message to specific user.\n"
			"\tRooms: create / delete / enter / leave room.\n"
			"\tChat: send message to all users in room.\n"
			"\tLogin: connect / disconnect from server\n"
			"\t%s: default room where new connected user is placed.\n"
			"\tTo enter a room, user should first go back to %s\n"
			"\n"
			"You must connect to a server with !connect...\n"
			,ROOM_WELCOME_NAME, ROOM_WELCOME_NAME
	);
}


//------------------------------------------------------------------------------
// Static functions for each command process (Exec functions)
//
// Each function process a specific command and execute the required action.
// In all case, str parameter is the command written by user without the command 
// name (For example, without !connect' for connection command)
//------------------------------------------------------------------------------

static void commands_exec_connect(ClientData *client, char *str){
	//If first elt is '\0', means no param given. Show usage
	if(str[0] == '\0'){
		fprintf(stderr, "Invalid command. Usage: !connect <username>@<server> [:port]\n");
		return;
	}

	//Recover values from str
	char args[CMD_MAX_SIZE] = "";
	uint16_t port;
	sscanf(str, "%s %"SCNd16, args, &port); //port optional, 0 if not here
	char *token = NULL, *username = NULL, *servername = NULL;

	//Recover username and servername
	token		= strtok(args, "@");
	username	= token;
	servername	= strtok(NULL, "@");

	//Servername and username must be here
	if(username == NULL || servername == NULL){
		fprintf(stderr, "Invalid command. Usage: !connect <username>@<server> [:port]\n");
		return;
	}
	//If no port given, set to default
	if(port == 0 || port == 65535){
		port = PORT_DEFAULT; //65535 or 0 sounds to be set if no value in sscanf
	}

	//Connect and send name (Or return if unable to connect)
	int socket = client_connect_to_server(client, servername, port);
	if(socket == -1){ return; }
	fprintf(stdout, "Send name to server...\n");
	strcpy(client->login,username);
	messaging_send_connect(socket, username);
	//Warning, I had a bug if starting listening before the send_connect process.
	client_start_listening(client);
}

static void commands_exec_bye(ClientData* client, char* args){
	//User must be connected
	if(client->status != CONNECTED){
		fprintf(stderr, "You must be connected to a server...\n");
		return;
	}
	messaging_send_bye(client->socket);
}

static void commands_exec_rooms(ClientData* client, char* args){
	//TODO To implements
	fprintf(stdout, "Command not working yet...\n");
}

static void commands_exec_open(ClientData *client, char *args){
	//User must be connected
	if(client->status != CONNECTED){
		fprintf(stderr, "You must be connected to a server...\n");
		return;
	}
	//Check whether room name is valid (Note: deeply done server side)
	if(args == NULL || *args == '\n' || *args == '\0'){
		fprintf(stderr, "Invalid command. Usage: !open <room_name>\n");
		return;
	}
	//Send request
	messaging_send_room_open(client->socket, args);
}

static void commands_exec_close(ClientData *client, char *args){
	//User must be connected
	if(client->status != CONNECTED){
		fprintf(stderr, "You must be connected to a server...\n");
		return;
	}
	//Check whether room name is valid (Note: deeply done server side)
	if(args == NULL || *args == '\n' || *args == '\0'){
		fprintf(stderr, "Invalid command. Usage: !close <room_name>\n");
		return;
	}
	//Send request
	messaging_send_room_close(client->socket, args);
}

static void commands_exec_enter(ClientData *client, char *args){
	//User must be connected
	if(client->status != CONNECTED){
		fprintf(stderr, "You must be connected to a server...\n");
		return;
	}

	//Check whether room name is valid (Note: deeply done server side)
	if(args == NULL || *args == '\n' || *args == '\0'){
		fprintf(stderr, "Invalid command. Usage: !enter <room_name>\n");
		return;
	}

	//Send request
	messaging_send_room_enter(client->socket, str_trim(args));
}

static void commands_exec_leave(ClientData *client, char *args){
	//User must be connected
	if(client->status != CONNECTED){
		fprintf(stderr, "You must be connected to a server...\n");
		return;
	}
	messaging_send_room_leave(client->socket);
}

static void commands_exec_whisper(ClientData *client, char *msg){
	//User must be connected to whisper
	if(client->status != CONNECTED){
		fprintf(stderr, "You must be connected to a server before using whisper...\n");
		return;
	}

	//Check whether msg size is correct
	if(strlen(msg) > CMD_MAX_SIZE){
		fprintf(stderr, "Message is to long.");
		return;
	}

	//Recover the receiver and the message to send
	msg++; //Skip first *
	char *receiver	= strtok(msg, "*");
	char *value		= strtok(NULL, "");

	//Receiver and value must be given
	if(receiver == NULL || value == NULL){
		fprintf(stderr, "Invalid command. Usage: *receiver* message\n");
		return;
	}
	value = str_trim(value);
	messaging_send_whisper(client->socket, client->login, receiver, value);
}

static void commands_exec_broadcast(ClientData *client, char *msg){
	//User must be connected
	if(client->status != CONNECTED){
		fprintf(stderr, "You must be connected to a server...\n");
		return;
	}

	//Skipp if empty message
	msg = (msg == NULL) ? NULL : str_trim(msg);
	if(msg == NULL || *msg == '\n' || *msg == '\0'){
		return;
	}

	//Check whether size is correct
	if(strlen(msg) > CMD_MAX_SIZE){
		fprintf(stderr, "Invalid format: message is too long...\n");
		return;
	}

	//Send message
	messaging_send_room_bdcast(client->socket, client->login, client->room, msg);
}


//------------------------------------------------------------------------------
// Asset functions
//------------------------------------------------------------------------------

static int commands_is_cmd(const char *str){
	if(str == NULL){ return FALSE; }
	return str[0] == '!' ? TRUE : FALSE;
}

static int commands_is_whisper(const char *str){
	if(str == NULL){ return FALSE; }
	return str[0] == '*' ? TRUE : FALSE;
}

static void commands_process(ClientData *client, char *cmd){
	//Check whether command size is correct
	if(strlen(cmd) > CMD_MAX_SIZE){
		fprintf(stderr, "Command is to long.\n");
		return;
	}

	//Recover the command name
	cmd++; //Skip the '!' char
	char cmd_name[CMD_MAX_SIZE+1]; //+1 for '\0'
	sscanf(cmd, "%s", cmd_name);
	char *args = cmd+strlen(cmd_name); //args start just after the cmd name

	//Call function according to command received
	if(strcmp(cmd_name, "connect") == 0){
		commands_exec_connect(client, args);
	}
	else if(strcmp(cmd_name, "bye") == 0){
		commands_exec_bye(client, args);
	}
	else if(strcmp(cmd_name, "rooms") == 0){
		commands_exec_rooms(client, args);
	}
	else if(strcmp(cmd_name, "open") == 0){
		commands_exec_open(client, args);
	}
	else if(strcmp(cmd_name, "close") == 0){
		commands_exec_close(client, args);
	}
	else if(strcmp(cmd_name, "enter") == 0){
		commands_exec_enter(client, args);
	}
	else if(strcmp(cmd_name, "leave") == 0){
		commands_exec_leave(client, args);
	}
	else if(strcmp(cmd_name, "help") == 0){
		commands_help();
	}
	else{
		fprintf(stderr, "Unknown command...\n");
	}
}


