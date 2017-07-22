#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include "ChatManager.h"
#include "HashMap.h"
#include "list.h"

#define MAGIC 0xbeefbeef
#define MAX_GROUPS 1024
#define MAX_STRING_LENGTH 1024
#define PID 200
#define PORT 8080
#define FILE_PID "pid.txt"

/* ----------------------------------------- */

static int HashFunc(char *str);
static int IsKeysEqual(char* _key1, char* _key2);
int myKeyValueActionFunction(const void* _key, void* _value, void* _context);

/* ----------------------------------------- */

typedef struct DoublePIDs{
	char m_pid1[PID];
	char m_pid2[PID];
}DoublePIDs;

struct ChatManager{
  HashMap* m_groupPIDs;
  size_t   m_magicNumber;
	List* 	 m_myGroups;
};

/* ----------------------------------------- */

ChatManager* ChatManager_Create()
{
  ChatManager* newCM;
  newCM = (ChatManager*) malloc(sizeof(ChatManager));
  if (NULL == newCM)
  {
    return newCM;
  }

  newCM->m_groupPIDs = HashMap_Create(MAX_GROUPS, (HashFunction) HashFunc, (EqualityFunction) IsKeysEqual);
	newCM->m_myGroups = List_Create();
  newCM->m_magicNumber = MAGIC;

  return newCM;
}

/* ----------------------------------------- */


void ChatManager_StartChat(ChatManager* _CM, char* _userName, char* _IPAddress)
{
	char command[MAX_STRING_LENGTH];
	FILE* fPtr;
	char line[PID];
	char line2[PID];
	int len = PID;
	DoublePIDs* newPIDs = malloc(sizeof(DoublePIDs));
	fPtr = fopen("./pid.txt", "r+");
	List_PushHead(_CM->m_myGroups, _IPAddress);
	
	sprintf(command, "gnome-terminal --geometry=55x24+55+185 sh --command \"./messanger %s %d %s %s; bash\"", _IPAddress, PORT, FILE_PID, _userName);
	system(command);
	rewind(fPtr);
	fgets(line, len, fPtr);

	sprintf(command, "gnome-terminal --geometry=55x13+55+620 sh --command \"./messanger %s %d %s %s send; bash\"", _IPAddress, PORT, FILE_PID, _userName);
	system(command);
	rewind(fPtr);
	fgets(line2, len, fPtr);
	fclose(fPtr);

	strcpy(newPIDs->m_pid1, line);
	strcpy(newPIDs->m_pid2, line2);

  HashMap_Insert(_CM->m_groupPIDs, (void*)_IPAddress, (void*)newPIDs);
  return;
}

/* ----------------------------------------- */

ChatRes ChatManager_ExitChat(ChatManager* _CM, const char* _IPAddress)
{
	DoublePIDs* oldPIDs = malloc(sizeof(DoublePIDs));

  if ((!_CM)||(_CM->m_magicNumber != MAGIC) || (!_IPAddress))
  {
    return CHATM_NOT_INITIALIZED_INPUT;
  }
	HashMap_Find(_CM->m_groupPIDs, (void*)_IPAddress, (void*)&oldPIDs);
  HashMap_Remove(_CM->m_groupPIDs, (void*)_IPAddress, (void**)&oldPIDs);

	kill((pid_t)atoi(oldPIDs->m_pid1), SIGKILL);
	kill((pid_t)atoi(oldPIDs->m_pid2), SIGKILL);

	free(oldPIDs);
	return CHATM_SUCCESS;
}

/* ----------------------------------------- */

ChatRes ChatManager_ExitAllChats(ChatManager* _CM)
{
	if ((!_CM)||(_CM->m_magicNumber != MAGIC))
  {
    return CHATM_NOT_INITIALIZED_INPUT;
  }

	HashMap_ForEach(_CM->m_groupPIDs, (KeyValueActionFunction) myKeyValueActionFunction, (void*)_CM); 
	/*HashMap_Destroy(&_CM->m_groupPIDs, free, free);*/
  return CHATM_SUCCESS;
}

/* ----------------------------------------- */

int myKeyValueActionFunction(const void* _key, void* _value, void* _CM)
{
  kill((pid_t)atoi(((DoublePIDs*)_value)->m_pid1), SIGKILL);
	kill((pid_t)atoi(((DoublePIDs*)_value)->m_pid2), SIGKILL);
	
  return 1;
}

/* ----------------------------------------- */

List* ChatManager_ReturnGroupList(ChatManager* _CM)
{
	if ((!_CM)||(_CM->m_magicNumber != MAGIC))
  {
    return NULL;
  }
	return _CM->m_myGroups;
}

/* ----------------------------------------- */

void ChatManager_Destroy(ChatManager* _CM)
{
  if ((!_CM)||(_CM->m_magicNumber != MAGIC))
  {
    return;
  }

  HashMap_Destroy(&_CM->m_groupPIDs, free, free);
  _CM->m_magicNumber = 0;
	List_Destroy(&_CM->m_myGroups, NULL);

  free(_CM);
}

/* ----------------------------------------- */

static int HashFunc(char *str)
{
  int hash = 5381;
  int c;

  while ((c = *(str++)))
    hash = ((hash << 5) + hash) + c;

  return hash;
}

/* ----------------------------------------- */

static int IsKeysEqual(char* _key1, char* _key2)
{
  return strcmp(_key1, _key2) == 0;
}

/* ----------------------------------------- */

