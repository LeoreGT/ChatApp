#include "list.h"
#include "intern_list.h"
#include <stdlib.h>

#define MAGIC_NUMBER 0xdeadbeef
#define DEALLOCATED 0

#define CHECK_AND_RETURN(check, checkAgainst,returnVal) if(checkAgainst == check) \
							{ \
								return returnVal; \
							} 
#define IFEQ_NULL_RETURN(list, item)			\
	if (NULL == _list)				\
		return LIST_UNINITIALIZED_ERROR;	\
	if (NULL == _item)				\
		return LIST_NULL_ELEMENT_ERROR;		\


#define IFEQ_NULL_RETURN3(_list, _pItem, _nItems)					\
	if (NULL == _list || NULL == _pItem)	return LIST_UNINITIALIZED_ERROR;	\
	if (_nItems == 0)			return LIST_UNDERFLOW;			\


static void InsertInbetween(Node* prev, Node* next, Node* new);

List* List_Create(void)
{
	List* list = calloc(1, sizeof(List));
	if (NULL == list) 
		return NULL;
	
	(list->m_head).m_next = &(list->m_tail);
	(list->m_tail).m_prev = &(list->m_head);	
	
	list->m_magicNumber = MAGIC_NUMBER;
	
	return list;
}

void List_Destroy(List** _pList, void (*_elementDestroy)(void* _item))
{
	Node* temp;
	Node* end = &((*_pList)->m_tail);
	
	if(NULL == _pList || NULL == *_pList || (*_pList)->m_magicNumber != MAGIC_NUMBER)
		return;
	
	temp = ((*_pList)->m_head).m_next;
	
	if(_elementDestroy != NULL)
	{
		while(temp != end)
		{
			_elementDestroy(temp->m_item);
			temp = temp->m_next;
			free(temp->m_prev);
		}
	}
	else
	{
		while(temp != end)
		{
			temp = temp->m_next;			
			free(temp->m_prev);
		}
	}
	
	(*_pList)->m_magicNumber = DEALLOCATED;
	free(*_pList);
	*_pList = NULL;
}

List_Result List_PushHead(List* _list, void* _item)
{
	Node* newNode;
	Node* head = &(_list->m_head);
	
	IFEQ_NULL_RETURN(_list, _item);
	
	newNode = (Node*) malloc(sizeof(Node));
	newNode->m_item = _item;
	InsertInbetween(head, head->m_next, newNode);
	
	++_list->m_numOfItems;
	return LIST_SUCCESS;
}

List_Result List_PushTail(List* _list, void* _item)
{
	Node* newNode;
	Node* tail = &(_list->m_tail);
	
	IFEQ_NULL_RETURN(_list, _item);
	/*if (NULL == _list)
		return LIST_UNINITIALIZED_ERROR;
	if (NULL == _item)
		return LIST_NULL_ELEMENT_ERROR;
	*/
	newNode = (Node*) malloc(sizeof(Node));
	newNode->m_item = _item;
	/* InsertInbetween(tail->m_prev, tail, newNode); */
	newNode->m_next = tail;
	newNode->m_prev = tail->m_prev;
	/* tail->m_prev->m_next = tail->m_prev = newNode; */
	tail->m_prev = newNode;
	newNode->m_prev->m_next = newNode;/* @@@ws */
	++_list->m_numOfItems;
	return LIST_SUCCESS;
}

List_Result List_PopHead(List* _list, void** _pItem)
{
	Node* temp;	
	Node* head = &(_list->m_head);
	
	IFEQ_NULL_RETURN3(_list, _pItem, _list->m_numOfItems);
	/*if (NULL == _list || NULL == _pItem)	return LIST_UNINITIALIZED_ERROR;
	if (_list->m_numOfItems == 0)			return LIST_UNDERFLOW;
	*/
	
	temp = head->m_next;
	
	head->m_next = temp->m_next;
	temp->m_next->m_prev = head;
	
	*_pItem = temp->m_item;
	free(temp);
	
	--_list->m_numOfItems;
	
	return LIST_SUCCESS;
}

List_Result List_PopTail(List* _list, void** _pItem)
{
	Node* temp;	
	Node* tail = &(_list->m_tail);
	
	IFEQ_NULL_RETURN3(_list, _pItem, _list->m_numOfItems);
	
	temp = tail->m_prev;
	
	tail->m_prev = temp->m_prev;
	temp->m_prev->m_next = tail;
	
	*_pItem = temp->m_item;
	free(temp);
	
	--_list->m_numOfItems;
	
	return LIST_SUCCESS;
}


size_t List_Size(const List* _list)
{
	if(NULL == _list)
	{
		return 0;
	}
	
	return _list->m_numOfItems;
}

/*-------------------Static Functions Implementation--------------------------*/

static void InsertInbetween(Node* prev, Node* next, Node* new)
{
	new->m_next = next;
	new->m_prev = prev;
	prev->m_next = next->m_prev = new;
}

