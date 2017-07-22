#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "Group.h"
#include "list_itr.h"
#include "listInternal.h"
#include "list_functions.h"

#define MAGIC 0xbeefbeef
#define NEXT_IP 16
#define MULTICAST_IP_STARTER "224.0.0.15"

/* ----------------------------------------- */

struct Group
{
  size_t m_magicNumber;
  char   m_groupName[MAX_STRING_LEN];
  char   m_IPAddress[MAX_STRING_LEN];
  List*  m_groupUsers;
  int    m_currentUsers;
};

/* ----------------------------------------- */

Group* GroupCreate(const char* _groupName)
{
  static char multicastIP[] = MULTICAST_IP_STARTER;
  static int multicastIPlastByte  = NEXT_IP;

  Group* newGroup = NULL;

  if(_groupName == NULL)
  {
    return NULL;
  }

  newGroup = (Group*) malloc(sizeof(Group));
  if(newGroup == NULL)
  {
    return newGroup;
  }

  newGroup->m_groupUsers = List_Create();
  if(newGroup->m_groupUsers == NULL)
  {
    free(newGroup);
    return NULL;
  }
  strcpy(newGroup->m_IPAddress, multicastIP);

  printf("%s\n", newGroup->m_IPAddress);
  sprintf(multicastIP + 8, "%d", multicastIPlastByte);
  newGroup->m_currentUsers = 0; /*the first user will be added in a second function call from the server app*/
  newGroup->m_magicNumber = MAGIC;
  strcpy(newGroup->m_groupName, _groupName);
	++multicastIPlastByte; /*update multicast address to be given to the next group created*/
  return newGroup;
}

/* ----------------------------------------- */

void GroupDestroy(Group* _group)
{
  if((!_group) || (_group->m_magicNumber != MAGIC))
  {
    return;
  }

  List_Destroy(&(_group->m_groupUsers), free);
  _group->m_magicNumber = 0;
  free(_group);
}

/* ----------------------------------------- */

List* GroupGetUsers(const Group* _group)
{
  if((!_group) || (_group->m_magicNumber != MAGIC))
  {
    return NULL;
  }

  return (List*)_group->m_groupUsers;
}

/* ----------------------------------------- */

int FindGroup(void* _element, void* _context)
{
  return !!strcmp((char*)_element, (char*)_context);
}

/* ----------------------------------------- */

GroupStat GroupAddUser(Group* _group, char* _username)
{
  ListItr outcome;

  if((!_group) || (_group->m_magicNumber != MAGIC) || (!_username))
  {
    return GROUP_NOT_INITIALIZED_INPUT;
  }

  outcome = ListItr_FindFirst(ListItr_Begin(_group->m_groupUsers), ListItr_End(_group->m_groupUsers), FindGroup, (void*)_username);

  if(outcome == ListItr_End(_group->m_groupUsers))
  {
    return USER_ALREADY_EXISTS;
  }

  if(List_PushHead(_group->m_groupUsers, (void*)strdup(_username)) != LIST_SUCCESS)
  {
    return GROUP_ALLOCATION_FAILED;
  }

  _group->m_currentUsers++;
  return GROUP_SUCCESS;
}

/* ----------------------------------------- */

GroupStat GroupRemoveUser(Group* _group, char* _usernam)
{
  ListItr outcome;

  if((!_group) || (_group->m_magicNumber != MAGIC) || (!_usernam))
  {
    return GROUP_NOT_INITIALIZED_INPUT;
  }

  outcome = ListItr_FindFirst(ListItr_Begin(_group->m_groupUsers), ListItr_End(_group->m_groupUsers), FindGroup, (void*)_usernam);

  if(outcome == ListItr_End(_group->m_groupUsers))
  {
    return GROUP_NOT_FOUND;
  }

  ListItr_Remove(outcome);
  _group->m_currentUsers--;
  if (_group->m_currentUsers == 0)
  {
    GroupDestroy(_group);
  }

  return GROUP_SUCCESS;
}

/* ----------------------------------------- */

int IsGroupEmpty(const Group* _group)
{
  if(!_group)
  {
    return GROUP_NOT_INITIALIZED_INPUT;
  }

  return 0 == _group->m_currentUsers;
}

/* ----------------------------------------- */

int GroupNumUsers(const Group* _group)
{
  if(!_group)
  {
    return GROUP_NOT_INITIALIZED_INPUT;
  }

  return _group->m_currentUsers;
}

/* ----------------------------------------- */

char* GetGroupIP(const Group* _group)
{
  if(!_group)
  {
    return NULL;
  }

  return strdup(_group->m_IPAddress);
}

/* ----------------------------------------- */

char* GetGroupName(const Group* _group)
{
  if(!_group)
  {
    return NULL;
  }

  return strdup(_group->m_groupName);
}

/* ----------------------------------------- */
