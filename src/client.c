// -----------------------------------------------------------------------------
/**
 * \file	client.c
 * \author	Constantin MASSON
 * \date	June 19, 2016
 *
 * \brief	Client component
 * \note	C Library for the Unix Programming Project
 */
// -----------------------------------------------------------------------------

#include "client.h"


//Used for thread
struct thread_info{
	pthread_t	id;
	ClientData	*client;
};

// -----------------------------------------------------------------------------
// Static functions (Privates)
// -----------------------------------------------------------------------------

// Listen a socket, simple read it (Meant to be used as thread function)
void *client_listen_socket(void *args){
	struct thread_info *tinfo = (struct thread_info*)args;
	char buffer[MSG_MAX_SIZE];
	while(1){
		//TODO CRIT: Add exit process
		memset(buffer, 0x00, sizeof(buffer));
		recv(tinfo->client->socket, buffer, MSG_MAX_SIZE, 0);
		messaging_exec_client_receive(tinfo->client, tinfo->client->socket, buffer);
	}
}

// -----------------------------------------------------------------------------
// Public functions
// -----------------------------------------------------------------------------
int client_connect_to_server(ClientData *client, const char *address, const uint16_t port){
	if(client->status == CONNECTED || client->status == CONNECTING){
		fprintf(stderr, "You are already connected!\n");
		return -1;
	}
	int socket;
	fprintf(stdout, "Try to connect server (%s) at port %d...\n", address, port);
	socket = create_client_tcp_socket(address, port);
	if(socket < 0){
		fprintf(stderr, "Unable to connect to server.\n");
		return -1;
	}
	client->status	= CONNECTING;
	client->socket	= socket;
	fprintf(stdout, "Connection successfully done.\n");
	return socket;
}

void client_start_listening(ClientData *client){
	struct thread_info tinfo;
	memset(&tinfo, 0x00, sizeof(tinfo));
	pthread_t		thread_id;
	tinfo.client	= client;
	tinfo.id		= thread_id;
	pthread_create(&thread_id, NULL, client_listen_socket, (void*)&tinfo);
	pthread_detach(thread_id);
}

int main(int argc, char **argv){
	fprintf(stdout, "Start client\n");

	//Set handlers
	sethandler(SIG_IGN, SIGPIPE);

	//Create client data and start the console
	ClientData c;
	client_data_init(&c);
	commands_prompt_start(&c, &(c.is_working));

	fprintf(stdout, "Stop client\n");
	return EXIT_SUCCESS;
}
