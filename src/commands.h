// -----------------------------------------------------------------------------
/**
 * \file	commands.h
 * \author	Constantin MASSON
 * \date	June 19, 2016
 *
 * \brief	Client commands component
 * \note	C Library for the Unix Programming Project
 */
// -----------------------------------------------------------------------------

#ifndef UNIXPROJECT_COMMANDS_H
#define UNIXPROJECT_COMMANDS_H

#include <stdio.h>
#include <string.h>
#include <netinet/in.h>
#include <inttypes.h>

#include "wunixlib/assets.h"

#include "client_data.h"
#include "helper.h"
#include "messaging.h"
#include "constants.h"
#include "client.h"


/**
 * \brief			Display one command prompt and wait for user cmd.
 * \details			Function life: wait (block) for user keyboard str, process str,
 * 					return beginning.
 * \warning			ClientData should be not null.
 * \warning			condition should be not null.
 *
 * \param client	Client owner of the command prompt
 * \param condition	Run while value pointed by condition is 1
 */
void commands_prompt_start(ClientData *client, volatile sig_atomic_t *condition);

/**
 * \brief			Process a command line.
 * \details			String can be a simple message, a command, a whisper.
 * \warning			ClientData should be not null.
 *
 * \param client	Client who wrote the line
 * \param str		String to process
 */
void commands_prompt_process_line(ClientData *client, char *str);

/**
 * \brief Display all available commands
 */
void commands_help(void);

/**
 * \brief		Display the welcome menu for commands prompt.
 * \details		Usually called before starting the command prompt.
 *
 * \param msg	Optional message to display at the end
 */
void commands_welcome_menu(const char *msg);

/**
 * \brief		Display chatroom description
 */
void commands_general_description(void);


#endif



