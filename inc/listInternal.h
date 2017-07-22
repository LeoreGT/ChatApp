#ifndef __LIST_INTERN__
#define __LIST_INTERN__

#define MAGIC_NUMER  0xBEEFBEEF
#define REMOVE_MAGIC 0xDEADBEEF

#define TRUE  1
#define FALSE 0

#include "list.h"
#include "list_itr.h" /* LIST ITERATOR 		*/

typedef struct Node node_t;

struct Node
{
    void* m_data;		/* CONTAINS THE DATA OF THE CURRENT NODE 		 */
    node_t* m_next;		/* POINTS TO NEXT NODE IN DOUBLE LINKED LIST	 */
    node_t* m_prev;		/* POINTS TO PREVIOUS NODE IN DOUBLE LINKED LIST */	
};

struct List
{
    node_t m_head;			/*  A HEAD TESTINEL THAT POINTS TO FIRST NODE */
    node_t m_tail;			/*  A TAIL TESTINEL THAT POINTS TO LAST NODE  */
	size_t m_magicNumber;   /*  
						     * MAGICNUMER 0XBEEFBEEF IN CASE OF USE, 		 
						     * 0XDEADBEEF IN CASE OF DESTROYED 			 
						     */
};

int IsValidList(const List* _list);

void DestroyNode(node_t* _nodeToDestory);

List_Result CreateNode(void* _data, node_t **_node);

List_Result PushBefore(node_t* _nodeIter, void* _element);

void* NodeRemove(node_t* _nodeToRemove);

int ListIsEmpty(List *_list);

int IsValidIter(ListItr _iter);

#endif /* __LIST_INTERN__ */
