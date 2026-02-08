// -----------------------------------------------------------------------------
/**
 * \file	server.c
 * \author	Constantin MASSON
 * \date	June 19, 2016
 *
 * \brief	Server component
 * \note	C Library for the Unix Programming Project
 */
// -----------------------------------------------------------------------------

#include "server.h"


//Used for thread arguments
struct thread_info{
	int			socket;
	ServerData	*server;
};


// -----------------------------------------------------------------------------
// Client management functions
// -----------------------------------------------------------------------------
void *client_handler(void *args){
	//Recover parameters
	fprintf(stdout, "New client request.\n");
	struct thread_info *tinfo = (struct thread_info*)args;
	char buff[MSG_MAX_SIZE];

	//Create the new client user
	ServerData *server = tinfo->server;
	User *user = user_create("new_user");
	if(user == NULL){
		fprintf(stderr, "Unable to create the user for socket %d\n", tinfo->socket);
		pthread_exit(NULL);
	}
	user->socket = tinfo->socket;

	//Listen for message
	while(server->is_working == 1 && user->connected == 1){
		//TODO CRIT: Add exit process
		memset(buff, 0x00, sizeof(buff));
		recv(user->socket, buff, MSG_MAX_SIZE, 0);
		messaging_server_exec_receive(server, user, buff);
	}

	//Free data
	fprintf(stdout, "Client deconnected\n");
	user_destroy(user);
}

void server_start_listening_clients(ServerData *server, const int socket){
	//Server shouldn't already be listening
	if(server->is_listening == TRUE){
		fprintf(stdout, "Server is already listening.\n");
		return;
	}
	server->is_listening = TRUE;
	fprintf(stdout, "Server start listening for new clients.\n");
	//Listen for client, start thread for each new connected
	while(server->is_listening == TRUE){
		fprintf(stdout, "Wait for client...\n");
		int client_socket = accept_client(socket); //accept new client
		//Create thread args
		struct thread_info tinfo;
		pthread_t thread_id;
		memset(&tinfo, 0x00, sizeof(tinfo));
		tinfo.server	= server;
		tinfo.socket	= client_socket;
		pthread_create(&thread_id, NULL, client_handler, (void*)&tinfo);
		pthread_detach(thread_id);
	}
}

void server_stop_listening_clients(ServerData *server){
	if(server->is_listening == FALSE){
		fprintf(stdout, "Server is already not listening\n");
		return;
	}
	fprintf(stdout, "Server stop listening for new clients\n");
	server->is_listening = FALSE;
}


// -----------------------------------------------------------------------------
// Start / Launch functions
// -----------------------------------------------------------------------------

static void siginthandler(ServerData *server, int sig){
	server_stop_listening_clients(server);
	server->is_working = 0;
}

static void usage(char *name){
	fprintf(stderr, "USAGE: %s port\n", name);
	exit(EXIT_FAILURE);
}

int main(int argc, char **argv){
	system("clear");
	fprintf(stdout, "Server start\n");

	//check parameters (Must be: port_number)
	if(argc != 2){
		usage(argv[0]);
	}

	//Init signal process
	//TODO To update
	sigset_t mask, oldmask;
	sethandler(SIG_IGN, SIGPIPE);
	//sethandler(siginthandler, SIGINT);
	sigemptyset(&mask);
	sigaddset(&mask, SIGINT);
	//sigprocmask(SIG_BLOCK, &mask, &oldmask);

	//Create the server socket, bind it, start listening
	int sock = create_server_tcp_socket(atoi(argv[1]), BACKLOG);
	if(sock < 0){
		fprintf(stderr, "Unable to start the server (Unable to create the socket)...\n");
		return EXIT_FAILURE;
	}

	//Initialize server data
	ServerData server;
	server_data_init(&server);
	User *admin = user_create("admin"); //Admin user just for the default room
	server_data_add_room(&server, admin, ROOM_WELCOME_NAME);

	//Start listening for new clients
	server_start_listening_clients(&server, sock);

	//Close the socket
	fprintf(stdout, "Server is closing. Close socket...\n");
	if(TEMP_FAILURE_RETRY(close(sock)) < 0){
		fprintf(stderr, "Error while closing the socket...\n");
		return EXIT_FAILURE;
	}

	fprintf(stdout, "Server is stopped\n");
	return EXIT_SUCCESS;
}
