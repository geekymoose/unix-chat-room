// -----------------------------------------------------------------------------
/**
 * \file	linkedlist.c
 * \author	Constantin MASSON
 * \date	June 22, 2016
 *
 * \brief	Simple generic linkedlist.
 * \note	C Library for the Unix Programming Project
 */
// -----------------------------------------------------------------------------

#include "linkedlist.h"


void list_init(Linkedlist *list, freefct f){
	assert(list != NULL);
	list->size		= 0;
	list->first		= NULL;
	list->last		= NULL;
	list->freefct	= f;
}

void list_clear(Linkedlist *list){
	assert(list != NULL);
	LinkedlistNode *current;
	while(list->first != NULL){
		current		= list->first;
		list->first	= list->first->next;
		//If data of node need to be free
		if(list->freefct != NULL){
			list->freefct(current->data);
		}
	}
	//Reset the data (Not required, but one never knows)
	list->size		= 0;
	list->first		= NULL;
	list->last		= NULL;
}

int list_is_empty(const Linkedlist *list){
	assert(list != NULL);
	return (list->first == NULL) ? 1 : 0;
}

int list_size(const Linkedlist *list){
	assert(list != NULL);
	return list->size;
}

void list_iterate(const Linkedlist *list, iteratorfct f){
	assert(list != NULL);
	assert(f != NULL);
	int retval = 1;
	LinkedlistNode *current = list->first;
	while(current != NULL && retval == 1){
		retval = f(current->data);
		current = current->next;
	}
}

void list_iterate_args(const Linkedlist *list, iterator2fct f, void* args){
	assert(list != NULL);
	assert(f != NULL);
	int retval = 1;
	LinkedlistNode *current = list->first;
	while(current != NULL && retval == 1){
		retval = f(current->data, args);
		current = current->next;
	}
}

int list_append(Linkedlist *list, void * const data){
	assert(list != NULL);
	assert(data != NULL);
	
	//Create the new node and set data pointer
	LinkedlistNode* node = (LinkedlistNode*)malloc(sizeof(LinkedlistNode));
	if(node == NULL){
		return -1; //Means malloc failed
	}
	//Set the data in node and place node at the end
	node->data = data;
	node->next = NULL;
	if(list->first == NULL){
		list->first	= node;
		list->last	= node;
	}
	else{
		list->last->next = node;
		list->last = node;
	}
	list->size++;
	return 1;
}

int list_append_first(Linkedlist *list, void * const data){
	assert(list != NULL);
	assert(data != NULL);

	//Create the new node
	LinkedlistNode* node = (LinkedlistNode*)malloc(sizeof(LinkedlistNode));
	if(node == NULL){
		return -1; //Means malloc failed
	}
	//Set the data in node and place node at the beginning
	node->data = data;
	node->next = list->first;
	if(list->first == NULL){
		list->first	= node;
		list->last	= node;
	}
	else{
		list->first = node;
	}
	list->size++;
	return 1;
}

void* list_getfirst(const Linkedlist *list){
	assert(list != NULL);
	return list->first->data;
}

void* list_getlast(const Linkedlist *list){
	assert(list != NULL);
	return list->last->data;
}

int list_contains_where(const Linkedlist *list, void* value, compfct f){
	assert(list != NULL);
	assert(value != NULL);
	assert(f != NULL);
	//Browse each element and check
	LinkedlistNode *current = list->first;
	while(current != NULL){
		if(f(current->data, value) == 1){
			return 1;
		}
		current = current->next;
	}
	return 0;
}

void* list_get_where(const Linkedlist *list, void* value, compfct f){
	assert(list != NULL);
	assert(value != NULL);
	assert(f != NULL);
	//Browse each element and check
	LinkedlistNode *current = list->first;
	while(current != NULL){
		if(f(current->data, value) == 1){
			return current->data;
		}
		current = current->next;
	}
	return NULL;
}

void* list_remove_where(Linkedlist *list, void* value, compfct f){
	assert(list != NULL);
	assert(value != NULL);
	assert(f != NULL);
	//Browse each element
	LinkedlistNode *previous	= NULL;
	LinkedlistNode *current		= list->first;
	while(current != NULL){
		//We found the one to delete
		if(f(current->data, value) == 1){
			//If it was the first element, previous is null
			if(previous == NULL){
				list->first = current->next;
			}
			else{
				previous->next = current->next;
			}
			list->size--;
			return current;
		}
		previous	= current;
		current		= current->next;
	}
	return NULL;
}

int list_free_where(Linkedlist *list, void* value, compfct f){
	assert(list != NULL);
	assert(value != NULL);
	assert(f != NULL);
	//Browse each element
	LinkedlistNode *previous	= NULL;
	LinkedlistNode *current		= list->first;
	while(current != NULL){
		//If is not this one, go next
		if(f(current->data, value) != 1){
			previous	= current;
			current		= current->next;
			continue;
		}
		//We found the one to delete
		//If it was the first element, previous is null
		if(previous == NULL){
			list->first = current->next;
		}
		else{
			previous->next = current->next;
		}
		list->size--;
		if(list->freefct != NULL){
			list->freefct(current);
		}
		return 1;
	}
	return -1;
}
