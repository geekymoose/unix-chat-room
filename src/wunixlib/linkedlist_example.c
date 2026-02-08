// -----------------------------------------------------------------------------
/**
 * \file	linkedlist_example.c
 * \author	Constantin MASSON
 * \date	June 22, 2016
 *
 * \brief	Example of implementation of linkedlist
 * \note	C Library for the Unix Programming Project
 */
// -----------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "linkedlist.h"


typedef struct _example_user{
	char*	name;
	int		id;
} user;

int display_player(void *data){
	if(data == NULL){
		fprintf(stdout, "User pointer is null...\n");
		return 1;
	}
	user u = *(struct _example_user*)data;
	fprintf(stdout, "User name: %s / ID: %d\n", u.name, u.id);
	return 1;
}

int match_name(void *data, void *name){
	user u = *(struct _example_user*)data;
	name = (char*)name;
	if(strcmp(u.name, name) == 0){
		return 1;
	}
	return 0;
}

void test_struct_list(){
	Linkedlist list;
	list_init(&list, NULL);

	user u1 = {"User1", 1};
	user u2 = {"User2", 2};
	user u3 = {"User3", 3};
	list_append(&list, &u1);
	list_append(&list, &u2);

	list_iterate(&list, display_player);

	//Test list_contains_where
	char *retval;
	retval = (list_contains_where(&list, "User1", match_name)) ? "TRUE" : "FALSE";
	fprintf(stdout, "Contains 'User1'? : %s\n", retval);
	retval = (list_contains_where(&list, "User4", match_name)) ? "TRUE" : "FALSE";
	fprintf(stdout, "Contains 'User4'? : %s\n", retval);

	//Test list_get_where
	fprintf(stdout, "Get user\n");
	user *ptr_u = NULL;

	ptr_u = list_get_where(&list, "User1", match_name);
	fprintf(stdout, "User1 find: ");
	display_player((void*)ptr_u);

	ptr_u = list_get_where(&list, "User4", match_name);
	fprintf(stdout, "User4 find: ");
	display_player((void*)ptr_u);


	//Test remove
	fprintf(stdout, "\nRemove User1 from list: \n");
	list_remove_where(&list, "User1", match_name);
	list_iterate(&list, display_player);
	list_remove_where(&list, "Unknown", match_name); //Should do nothing

	//Re add user1
	fprintf(stdout, "\nRe add User1 in list: \n");
	list_append(&list, &u1);
	list_iterate(&list, display_player);
	
	//Remove user2
	fprintf(stdout, "\nRemove User2 from list: \n");
	list_remove_where(&list, "User2", match_name);
	list_iterate(&list, display_player);
	list_remove_where(&list, "Unknown2", match_name);
	list_remove_where(&list, "Unknown3", match_name);

	//Add user 3
	fprintf(stdout, "\nAdd User3 in list: \n");
	list_append(&list, &u3);
	list_iterate(&list, display_player);

	//Finish
	list_clear(&list);
}

int main(int argc, char **argv){
	fprintf(stdout, "Linkedlist - Use case Example\n");
	test_struct_list();
	fprintf(stdout, "Linkedlist - Example done\n");
	return EXIT_SUCCESS;
}

