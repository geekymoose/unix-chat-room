// -----------------------------------------------------------------------------
/**
 * \file		linkedlist.h
 * \author		Constantin MASSON
 * \date		June 22, 2016
 *
 * \brief		Simple generic linkedlist.
 * \note		C Library for the Unix Programming Project
 *
 * Header file
 *
 * \attention	List is meant to be used with the same type of data.
 * 				Although it will technically accept any, unexpected behaviors
 * 				might append. (Free function might not work anymore for example).
 * \attention	Each node has a pointer to the data for this node (Instead of copy).
 * 				If data was dynamically created (From malloc...), data must be free while
 * 				destroying the list.
 *
 * \warning		If used with function local variable (Not malloc one), beware 
 * 				with the variable scope. If function terminate (But list still
 * 				used), the pointer in list will be broken.
 *
 * \bug			In case of non dynamic variable passed to the list (Like a simple
 * 				integer), the list will keep a pointer to this int.
 * 				Do not use the same int variable to set several element!
 * 				Example (Of code you shouldn't use):
 * 				<br/><br/>
 * 				int i;
 * 				<br/>
 * 				for(int i=0; i<10; i++){ list_append(&list, &i); }
 * 				<br/><br/>
 * 				The result will be a list of 10 element pointing to the same 
 * 				integer (With value 11).
 */
// -----------------------------------------------------------------------------

#ifndef WUNIXLIB_LINKEDLIST_H
#define WUNIXLIB_LINKEDLIST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>


// -----------------------------------------------------------------------------
// Structure functions definition
// -----------------------------------------------------------------------------

/**
 * \brief	Function to define how to free an element in the list.
 * \details	This function can be null and won't be called.
 *
 * \param	Data of the element
 */
typedef void(*freefct)(void*);

/**
 * \brief	Function that iterate a list.
 * \details	Iterate while iterator return 1
 *
 * \param	Data of the element
 */
typedef int(*iteratorfct)(void*);

/**
 * \brief		Function that iterate a list and receive one extra parameter.
 * \details		Iterate while function return 1. The working process is 
 * 				similar to iteratorfct except that it has also one extra 
 * 				argument defined by user.
 * \param data	Will be filled by each element iterated.
 * \param args	Personal arguments
 * \return		1 to continue, !=1 to stop iteration here
 */
typedef int(*iterator2fct)(void* data, void* args);

/**
 * \brief			Compare 2 elements.
 * \details			Function that define the rule for element to match value.
 * 					First parameter will always be filled with current tested element 
 * 					from the list. Second parameter will be filled by the value parameter.
 *
 * \param current	The current element tested
 * \param value		Value tested
 * \return			1 if current match the value, otherwise, return 0
 */
typedef int(*compfct)(void* current, void* value);


// -----------------------------------------------------------------------------
// Structures / Data
// -----------------------------------------------------------------------------

/** \brief Define a linked list node. */
typedef struct _linkedlistNode{
	void *data;
	struct _linkedlistNode *next;
} LinkedlistNode;

/** \brief	Define a Linkedlist type (Must be initialized with init function). */
typedef struct _linkedlist{
	int				size; //Virtual size of the list.
	freefct			freefct; //Fct to free data in node.
	LinkedlistNode	*first;
	LinkedlistNode	*last;
} Linkedlist;


// -----------------------------------------------------------------------------
// Functions prototypes
// -----------------------------------------------------------------------------

/**
 * \brief		Initialize the list.
 * \details		Must be called on new declared linkedlist before using it.
 * \details		The freefct define the way a Node data must be free. If NULL given,
 * 				node data won't be free at all (Must be done manually or not needed.)
 * 				If not null, this function will be called for each node in the list
 * 				when list is destroyed.
 * \warning		If list is NULL, assert error thrown.
 *
 * \param list	Pointer to the list to initialize
 * \param f		Function used to free a Node data or NULL if not needed
 */
void list_init(Linkedlist *list, freefct f);

/**
 * \brief		Remove all elements from the list and free memory.
 * \details		All its elements will be destroyed and data free (If free defined)
 * \warning		If list is NULL, assert error thrown.
 *
 * \param list	List to destroys
 */
void list_clear(Linkedlist *list);

/**
 * \brief		Check whether the given is is empty.
 * \details		Empty means list has no element inside.
 * \warning		If list is NULL, assert error thrown.
 *
 * \param list	The list to test
 * \return		1 if empty, otherwise, return 0
 */
int list_is_empty(const Linkedlist *list);

/**
 * \brief		Get the list size.
 * \warning		If list is NULL, assert error thrown.
 *
 * \param list	The list to check
 * \return		Current list size
 */
int list_size(const Linkedlist *list);

/**
 * \brief		Iterate the given list according to iterator function.
 * \details		Call f on each Node (While iterator returns 1)
 * \warning		If list is NULL, assert error thrown.
 * \warning		If f is NULL, assert error thrown.
 *
 * \param list	List to iterate
 * \param f		Iterator function
 */
void list_iterate(const Linkedlist *list, iteratorfct f);

/**
 * \brief		Iterate the list according to iterator function with args.
 * \details		Call f on each Node (While iterator returns 1). 
 * 				Pass the args parameter to iterator function.
 * \warning		If list is NULL, assert error thrown.
 * \warning		If f is NULL, assert error thrown.
 * \note		args can be null, depending on user implementation of iterator fct.
 *
 * \param list	List to iterate
 * \param f		Iterator function
 */
void list_iterate_args(const Linkedlist *list, iterator2fct f, void* args);

/**
 * \brief		Add data element at the end of the list.
 * \details		Element in list point to given data.
 * \warning		If list is NULL, assert error thrown.
 * \warning		If internal error occurred (Unable to allocate memory), -1 returned.
 *
 * \param list	List where to add element (Can be empty)
 * \param data	Data to place in list
 * \return		1 if successfully added, otherwise, -1
 */
int list_append(Linkedlist *list, void * const data);

/**
 * \brief		Add element at the beginning of the list.
 * \details		Element in list point to given data.
 * \warning		If list is NULL, assert error thrown.
 * \warning		If internal error occurred (Unable to allocate memory), -1 returned.
 *
 * \param list	List where to add element (Can be empty)
 * \param data	Data to place in list
 * \return		1 if successfully added, otherwise, -1
 */
int list_append_first(Linkedlist *list, void * const data);

/**
 * \brief		Return the first elements in the list.
 * \warning		If list is NULL, assert error thrown.
 *
 * \param list	List where to look for
 * \return		Pointer to the first element
 */
void* list_getfirst(const Linkedlist *list);

/**
 * \brief		Return the last elements in the list.
 * \warning		If list is NULL, assert error thrown.
 *
 * \param list	List where to look for
 * \return 		Pointer to the last element
 */
void* list_getlast(const Linkedlist *list);

/**
 * \brief		Check whether the element is in the list.
 * \details		2 elements are considered same according to the compfct implementation.
 * \warning		If list is NULL, assert error thrown.
 * \warning		If value is NULL, assert error thrown.
 * \warning		If function is NULL, assert error thrown.
 *
 * \param list	List where to look for
 * \param value	Element to check
 * \param f		Function used to check if element match the value (See compfct doc)
 * \return		1 if contains, otherwise, return 0
 */
int list_contains_where(const Linkedlist *list, void* value, compfct f);

/**
 * \brief		Get the element from the list that match the given test function.
 * \details		Iterate the list and return the first element that satisfied 
 * 				the compfct implementation (Given as parameter).
 * \warning		If list is NULL, assert error thrown.
 * \warning		If value is NULL, assert error thrown.
 * \warning		If function is NULL, assert error thrown.
 *
 * \param list	List where to look for
 * \param value	Value to match
 * \param f		Function used to check if match (See compfct doc)
 * \return		The element if found, otherwise, return NULL
 */
void* list_get_where(const Linkedlist *list, void* value, compfct f);

/**
 * \brief		Remove element from list.
 * \details		Simple remove, no 'free' call done here. The removed element
 * 				is returned by the function.
 * \warning		If list is NULL, assert error thrown.
 * \warning		If value is NULL, assert error thrown.
 * \warning		If function is NULL, assert error thrown.
 *
 * \param list	List where to remove
 * \param value	Value to match for removing
 * \param f		Function used to determine the element to remove. (See compfct doc)
 * \return		The removed element
 */
void* list_remove_where(Linkedlist *list, void* value, compfct f);

/**
 * \brief		Remove element from list and free its memory.
 * \details		Do 2 steps: remove element from list and then, call the free
 * 				function on it. (If NULL free function was set, no free call done.)
 * 				The element shouldn't be used anymore (Has been free).
 * \warning		If list is NULL, assert error thrown.
 * \warning		If value is NULL, assert error thrown.
 * \warning		If function is NULL, assert error thrown.
 *
 * \param list	List where to remove
 * \param value	Value to match for removing
 * \param f		Function used to determine element to remove. (See compfct doc)
 * \return		1 if successfully removed and free, otherwise, return -1
 */
int list_free_where(Linkedlist *list, void* value, compfct f);


#endif



