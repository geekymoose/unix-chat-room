// -----------------------------------------------------------------------------
/**
 * \file	network.c
 * \author	Constantin MASSON
 * \date	June 19, 2016
 *
 * \brief	Network / Socket management
 * \note	C Library for the Unix Programming Project
 */
// -----------------------------------------------------------------------------

#include "network.h"

int accept_client(const int socket){
	int client_socket;
	client_socket = TEMP_FAILURE_RETRY(accept(socket, NULL, NULL));
	if(client_socket == -1){
		//If errno == EAGAIN or EWOULDBLOCK, means socket is non blocking
		if(errno == EAGAIN || errno == EWOULDBLOCK){
			return -2;
		}
		LOG_ERR("accept");
		return -1;
	}
	return client_socket;
}

int make_socket(int domain, int type){
	int sock;
	sock = socket(domain, type, 0);
	if(sock < 0){
		LOG_ERR("socket");
		return -1;
	}
	return sock;
}

int create_server_tcp_socket(const uint16_t port, const int backlog){
	struct sockaddr_in addr;
	int sock;

	//Create socket
	sock = make_socket(PF_INET, SOCK_STREAM);
	if(sock < 0){
		return -1;
	}

	//Create address
	addr.sin_family			= AF_INET;
	addr.sin_port			= htons(port);
	addr.sin_addr.s_addr	= htonl(INADDR_ANY);
	
	//Bind socket with create address
	if(bind(sock, (struct sockaddr*)&addr, sizeof(addr)) < 0){
		LOG_ERR("bind");
		return -1;
	}

	//Start listening on this socket
	if(listen(sock, backlog) < 0){
		LOG_ERR("listen");
		return -1;
	}
	return sock; //Return created socket
}

int create_client_tcp_socket(const char *address, const uint16_t port){
	struct sockaddr_in addr;
	int socket;
	
	//Create the socket
	socket = make_socket(AF_INET, SOCK_STREAM);
	if(socket < 0){
		return -1;
	}

	//Recover the server struct sockaddr_in
	if(recover_address(address, port, &addr) < 0){
		return -1;
	}

	//Connect the created socket with the given remote socket.
	//TODO Add INTERRUPT management (See connect documentation)
	if(connect(socket, (struct sockaddr*)&addr, sizeof(struct sockaddr_in)) < 0){
		LOG_ERR("connect");
		return -1;
	}
	return socket;
}

int recover_address(const char *address, const uint16_t port, struct sockaddr_in *addr){
	struct hostent *hostinfo;
	
	hostinfo = gethostbyname(address);
	if(hostinfo == NULL){
		LOG_ERR("hostinfo");
		return -1;
	}

	//Set addr values and return it
	addr->sin_family	= AF_INET;
	addr->sin_port		= htons(port);
	addr->sin_addr		= *(struct in_addr*) hostinfo->h_addr;
	return 1;
}

int load_local_socket_data(int socked, uint16_t *port, char *ip){
	struct sockaddr_in addr;
	socklen_t length;
	length = sizeof(addr);
	if(getsockname(socked, (struct sockaddr *)&addr, &length) == -1){
		LOG_ERR("getsockname");
		return -1;
	}

	*port	= ntohs(addr.sin_port);
	ip		= inet_ntoa(addr.sin_addr);
	return 1;
}

int load_peer_socket_data(int socket, uint16_t *port, char *ip){
	struct sockaddr_in addr;
	socklen_t length;
	length = sizeof(addr);
	if(getpeername(socket, (struct sockaddr *)&addr, &length) == -1){
		LOG_ERR("getpeername");
		return -1;
	}
	*port	= ntohs(addr.sin_port);
	ip		= inet_ntoa(addr.sin_addr);
	return 1;
}
