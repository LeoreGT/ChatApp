#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

#include "Protocol.h"
#include "listInternal.h"
#include "list.h"
#include "list_itr.h"

#define SUCCESS 0
#define FAIL 1

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

static void PackString(const char* _str, char* _msgBuf, size_t* _length);
static void PackInt(int _num, char* _msgBuf, size_t* _length);
static void PackList(List* _lst, char* _msgBuf, size_t* _length);
static void PrintListOfStrings(List* _lst);
static void InsertListItems(List* _lst, char* _msgBuf, size_t* _length);
static void UnpackListItems(char* _item, List* _list);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

Data* CreateDataStruct()
{ 
	Data* newData;
	newData = (Data*) malloc(sizeof(Data));
	if (newData == NULL) return NULL;
	
	if (NULL == (newData->m_listOfStrings = List_Create()))
	{ 
		free(newData);
		return NULL;
	} 
	return newData;
} 

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

void DestroyString(void* str)
{ 
	if (str)
	free(str);
} 

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

void  DestroyDataStruct(Data** data)
{ 
	List* lst;
	if (*data)
	{ 
		lst  = (*data)->m_listOfStrings;
		if (lst)
		{ 
			List_Destroy(&lst, DestroyString);
		} 
		free(*data);
		*data = NULL;
	} 
} 

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

size_t Protocol_PackData(const Data* _data2Pack, char* _msgBuf)
{ 
	size_t length = 0;
	
	_msgBuf[0] = '\0';
	
	if (!_data2Pack) 
		return FAIL;

	PackString(_data2Pack->m_user.m_username, _msgBuf, &length);
	PackString(_data2Pack->m_user.m_password, _msgBuf, &length);
	PackString(_data2Pack->m_context, _msgBuf, &length);
	PackString(_data2Pack->m_IPAddress, _msgBuf, &length);
	PackInt(_data2Pack->m_req, _msgBuf, &length);
	PackInt(_data2Pack->m_response, _msgBuf, &length);
	PackList(_data2Pack->m_listOfStrings, _msgBuf, &length);

	return length;
} 

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

static void PackString(const char* _str, char* _msgBuf, size_t* _length)
{ 
	if ('\0' == _str[0]) 
	{ 
		 strcat(_msgBuf, " ");
		 (*_length)++;
	} 
	strcat(_msgBuf, _str);
	strcat(_msgBuf, "|");
	*_length += strlen(_str) + 1;
} 

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

static void PackInt(int _num, char* _msgBuf, size_t* _length)
{ 
	char strNum[10];
	
	sprintf(strNum, "%d", _num);
	strcat(_msgBuf, strNum);
	strcat(_msgBuf, "|");
	*_length += strlen(strNum) + 1;
} 

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

static void PackList(List* _lst, char* _msgBuf, size_t* _length)
{ 
	if (NULL == _lst || 0 == List_Size(_lst))
	{
		*_length += sizeof("0");
		strcat(_msgBuf, "0");
	}
	else
	{
		InsertListItems(_lst, _msgBuf, _length);
	}

	strcat(_msgBuf, "|");
	(*_length)++;
} 

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

static void InsertListItems(List* _lst, char* _msgBuf, size_t* _length)
{ 
	ListItr it = ListItr_Begin(_lst);
	ListItr end = ListItr_End(_lst);
			
	char* str;

	for (; it != end; it = ListItr_Next(it))
	{
		str = (char*)(ListItr_Get(it));
		strcat(_msgBuf, str);
		strcat(_msgBuf, "+");
		*_length += strlen(str) + 1;
	}
} 

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

int Unpack_String(char* item, char* dst)
{ 
	item = strtok(NULL, "|");
	if (NULL == item)
		return FAIL;
	strncpy(dst, item, 1 + strlen(item));
	return SUCCESS;
} 

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

int Unpack_Int(char* item, int* dst)
{ 
	item = strtok(NULL, "|");
	if (NULL == item)
		return FAIL;
	*dst = atoi(item);
	return SUCCESS;
} 

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

int Protocol_UnpackData(char* _msgBuf, size_t _length, Data* _data)
{ 
	char spacer[] = "|";
	char* item;
	char* tempBuf = NULL;
	
	if ((!_msgBuf) || (_length <= 0) || (!_data)) {return FAIL;}

	tempBuf = (char*)malloc(_length + 1);
	if(!tempBuf)
	{
		return (0);
	}
	strncpy(tempBuf, _msgBuf, _length);

	item = strtok(tempBuf, spacer);
	if (item == NULL)
		return FAIL;

	strncpy(_data->m_user.m_username, item, 1 + strlen(item));
	
	if (FAIL == Unpack_String(item, _data->m_user.m_password)) return FAIL;
	if (FAIL == Unpack_String(item, _data->m_context)) return FAIL;
	if (FAIL == Unpack_String(item, _data->m_IPAddress)) return FAIL;
	if (FAIL == Unpack_Int(item, (int*)&_data->m_req)) return FAIL;
	if (FAIL == Unpack_Int(item, &_data->m_response)) return FAIL;
	
	item = strtok(NULL, spacer);
	if (NULL == item)
		return FAIL;
	if (0 == strcmp(item,"0"))
		_data->m_listOfStrings = NULL;
	else
		UnpackListItems(item, _data->m_listOfStrings);

	return SUCCESS;
} 

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

static void UnpackListItems(char* _strings, List* _list)
{ 
	char* item;
	char* str;
		
	item = strtok(_strings, "+");
	while (item)
	{
		str = malloc(1 + strlen(item));
		strcpy(str, item);
		List_PushTail(_list, str);
		item = strtok(NULL, "+");
	}
} 

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

void PrintUnpackedData(const Data* _data2Pack)
{ 
	
	printf("\t m_username:	%s\n", _data2Pack->m_user.m_username);
	printf("\t m_password:	%s\n", _data2Pack->m_user.m_password);
	printf("\t m_context:	%s\n", _data2Pack->m_context);
	printf("\t m_IPAddress:	%s\n", _data2Pack->m_IPAddress);
	PrintListOfStrings(_data2Pack->m_listOfStrings);
	printf("\t m_req:		%d\n", _data2Pack->m_req);
	printf("\t m_response:	%d\n", _data2Pack->m_response);
} 

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

static void PrintListOfStrings(List* _lst)
{ 
	ListItr it = ListItr_Begin(_lst);
	ListItr end = ListItr_End(_lst);
	char* str;

	printf("List: \n");
	for (; it != end; it = ListItr_Next(it))
	{
		str = (char*)(ListItr_Get(it));
		printf("\t - %s\n", str);
	}
} 

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

void PrintPackedData(const char* _msgBuf)
{ 
  printf("Packed message: %s\n", _msgBuf);
  return;
} 

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

