// -----------------------------------------------------------------------------
/**
 * \file	network.h
 * \author	Constantin MASSON
 * \date	June 19, 2016
 *
 * \brief	Network / Socket management
 * \note	C Library for the Unix Programming Project
 */
// -----------------------------------------------------------------------------

#ifndef WUNIXLIB_NETWORK_H
#define WUNIXLIB_NETWORK_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <errno.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>

#include "assets.h"


// ----------------------------------------------------------------------------
// Prototypes
// ----------------------------------------------------------------------------

/**
 * \brief			Accept new connection for given socket.
 *
 * \param socket	Socket where to wait for incomming connection
 * \return			The new socket connected or -1 if error and errno is set.
 * 					If socket is not blocking and no connection available,
 * 					return -2 (errno == EAGAIN or EWOULDBLOCK)
 */
int accept_client(const int socket);

/**
 * \brief		Create a new socket.
 *
 * \param domain	Domain for the socket (AF_INET...)
 * \param type		Type of socket (SOCK_STREAM, SOCK_DGRAM...)
 * \return			The created socket or -1 if error
 */
int make_socket(int domain, int type);

/**
 * \brief			Create a new tcp server socket.
 * \details			Created connection is AD_INET at address INADDR_ANY.
 * 					The socket start listening as soon as it's created.
 *
 * \param port		Connection port for this socket
 * \param backlog	Listen backlog
 * \return			The socket value if created successfully, otherwise, return -1
 */
int create_server_tcp_socket(uint16_t port, int backlog);

/**
 * \brief			Create a new socket for a client.
 * \details			The socket is created and connected with the requested server.
 * 					If unable to connect, the function fails (And return -1)
 *
 * \param address	The address to connect with
 * \param port		Port to use for the connection
 * \return			The created socket or -1 if unable to connect
 */
int create_client_tcp_socket(const char *address, const uint16_t port);

/**
 * \brief			Recover the sockaddr_in data from the string name
 *
 * \param address	Address to recover
 * \param port		Port to set in the returned address
 * \param addr		The address to fill with recovered data
 * \return			1 if successfully recovered, otherwise, return -1
 */
int recover_address(const char *address, const uint16_t port, struct sockaddr_in *addr);

/**
 * \brief			Recover the port and IP of the local socket
 * \details			The ip value must be big enought (21) for the IP4 value 
 * 					formated as followed: xxxx.xxxx.xxxx.xxxx
 *
 * \param port		Port variable where to place port value
 * \param ip		Char array where to place result
 * \return			1 if successfully done, otherwise, -1 if error
 */
int load_local_socket_data(int socked, uint16_t *port, char *ip);

/**
 * \brief			Recover the port and IP of the peer socket
 * \details			The ip value must be big enought (21) for the IP4 value 
 * 					formated as followed: xxxx.xxxx.xxxx.xxxx
 *
 * \param port		Port variable where to place port value
 * \param ip		Char array where to place result
 * \return			1 if successfully done, otherwise, -1 if error
 */
int load_peer_socket_data(int socket, uint16_t *port, char *ip);

#endif



