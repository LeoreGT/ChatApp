#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "HashMap.h"
#include "GroupDB.h"
#include "Group.h"
#include "list.h"
#include "list_functions.h"

/* ----------------------------------------- */

#define MAGIC 0xbeefbeef

/* ----------------------------------------- */

static int HashFunc(char *str);
static int ActionFunc(char *str, void* val, void* context);
static int IsKeysEqual(char* _key1, char* _key2);
static void DestroyVal(void* _val);
GroupDBRes convertGroupStat(GroupStat _res);

/* ----------------------------------------- */

struct GroupDB
{
  HashMap* m_groups;
  size_t   m_groupsCap;
  size_t   m_numGroups;
  size_t   m_magicNumber;

};

/* ----------------------------------------- */

GroupDB* GroupDBCreate(size_t _maxUsers)
{
  GroupDB* newGDB = NULL;
  newGDB = (GroupDB*) malloc(sizeof(GroupDB));
  if (NULL == newGDB)
  {
    return newGDB;
  }

  newGDB->m_groupsCap = _maxUsers;
  newGDB->m_groups = HashMap_Create(_maxUsers, (HashFunction) HashFunc, (EqualityFunction) IsKeysEqual);
  newGDB->m_magicNumber = MAGIC;
  newGDB->m_numGroups = 0;

  return newGDB;
}

/* ----------------------------------------- */

void GroupDBDestroy(GroupDB* _GroupDB)
{
  if ((!_GroupDB) || (_GroupDB->m_magicNumber != MAGIC))
  {
    return;
  }

  HashMap_Destroy(&_GroupDB->m_groups, NULL, DestroyVal);
  _GroupDB->m_magicNumber = 0;
  _GroupDB->m_numGroups = 0;
  free(_GroupDB);
}

/* ----------------------------------------- */

GroupDBRes GroupDBAdd(GroupDB* _GroupDB, const char* _groupName)
{
  Group* group;

  if ((!_GroupDB) || (_GroupDB->m_magicNumber != MAGIC) || (!_groupName))
  {
    return GROUPDB_UNINITIALIZED_INPUT;
  }
  group = GroupCreate(_groupName);

  if(HashMap_Insert(_GroupDB->m_groups, _groupName, group) == MAP_SUCCESS)
  {
    _GroupDB->m_numGroups++;
    return GROUPDB_SUCCESS;
  }
  else
  {
    free(group);
    return GROUPDB_GENERAL_FAIL;
  }
}

/* ----------------------------------------- */

GroupDBRes GroupDBRemove(GroupDB* _GroupDB, const char* _groupName)
{
  Group* group;

  if ((!_GroupDB) || (_GroupDB->m_magicNumber != MAGIC) || (!_groupName))
  {
    return GROUPDB_UNINITIALIZED_INPUT;
  }
  if(HashMap_Remove(_GroupDB->m_groups, _groupName, (void**)&group) == MAP_SUCCESS)
  {
    _GroupDB->m_numGroups--;
    free(group);
    return GROUPDB_SUCCESS;
  }
  else
  {
    free(group);
    return GROUPDB_GENERAL_FAIL;
  }
}

/* ----------------------------------------- */

GroupDBRes DoesGroupExist(GroupDB* _grpDB, const char* _groupName)
{
  Group* group;

  if(HashMap_Find(_grpDB->m_groups, _groupName, (void**) &group) == MAP_SUCCESS)
  {
    return GROUPDB_SUCCESS;
  }
  else
  {
    return GROUPDB_GENERAL_FAIL;
  }
}

/* ----------------------------------------- */

char* GroupDBGetGroupIP(GroupDB* _grpDB, const char* _groupName)
{
  Group* group;

  if(HashMap_Find(_grpDB->m_groups, _groupName, (void**) &group) == MAP_SUCCESS)
  {
    return GetGroupIP(group);
;
  }
  else
  {
    return NULL;
  }
}

/* ----------------------------------------- */

GroupDBRes GroupDBGetGroupUsers(GroupDB* _GroupDB, const char* _groupName, List** _list)
{
  Group* group;
  List* res = NULL;
  if ((!_GroupDB) || (_GroupDB->m_magicNumber != MAGIC) || (!_groupName) || (!_list)) 
  {
    return GROUPDB_UNINITIALIZED_INPUT;
  }

  if(HashMap_Find(_GroupDB->m_groups, _groupName, (void**) &group) == MAP_SUCCESS)
  {
    res = GroupGetUsers(group);
    *_list = res;
    free(group);
    return GROUPDB_SUCCESS;
  }

  else
  {
    free(group);
    return GROUPDB_GENERAL_FAIL;
  }
}

/* ----------------------------------------- */

GroupDBRes GroupDBAddUser2Group(GroupDB* _GroupDB, const char* _groupName, char* _username)
{
  Group* group;
  GroupStat res;

  if ((!_GroupDB) || (_GroupDB->m_magicNumber != MAGIC) || (!_groupName) || (!_username))
  {
    return GROUPDB_UNINITIALIZED_INPUT;
  }

  if(HashMap_Find(_GroupDB->m_groups, _groupName, (void**) &group) == MAP_SUCCESS)
  {
    res = GroupAddUser(group, _username);
    return convertGroupStat(res);
  }
  else
  {
    return GROUPDB_GENERAL_FAIL;
  }
}

/* ----------------------------------------- */

GroupDBRes GroupDBRemoveUserFromGroup(GroupDB* _GroupDB, const char* _groupName, char* _username)
{
  Group* group;
  GroupStat res;

  if ((!_GroupDB) || (_GroupDB->m_magicNumber != MAGIC) || (!_groupName) || (!_username))
  {
    return GROUPDB_UNINITIALIZED_INPUT;
  }

  if(HashMap_Find(_GroupDB->m_groups, _groupName, (void**) &group) == MAP_SUCCESS)
  {
    res = GroupRemoveUser(group, _username);
    return convertGroupStat(res);
  }
  else
  {
    return GROUPDB_GENERAL_FAIL;
  }
}

/* ----------------------------------------- */

List* GroupDBShowGroups(GroupDB * _grpDB)
{
  List* newList = List_Create();
  if ((!_grpDB) || (_grpDB->m_magicNumber != MAGIC))
  {
    return NULL;
  }
	HashMap_ForEach(_grpDB->m_groups, (KeyValueActionFunction) ActionFunc, (void*)	newList);

  return newList;
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

static int ActionFunc(char *str, void* val, void* context)
{
	List* list = List_Create();
	list = (List*)context;

	List_PushHead(list, val);
	return GROUP_SUCCESS;
}

/* ----------------------------------------- */

static int IsKeysEqual(char* _key1, char* _key2)
{
  return strcmp(_key1, _key2) == 0;
}

/* ----------------------------------------- */

static void DestroyVal(void* _val){}

/* ----------------------------------------- */

GroupDBRes convertGroupStat(GroupStat _res)
{
  switch (_res)
  {
    case GROUP_SUCCESS :
      return GROUPDB_SUCCESS;
      break;
    case GROUP_ALLOCATION_FAILED :
      return GROUPDB_ALLOCATION_FAILED;
      break;
    case GROUP_NOT_INITIALIZED_INPUT :
      return GROUPDB_UNINITIALIZED_INPUT;
      break;
    case USER_ALREADY_EXISTS :
      return GROUPDB_USER_ALREADY_EXISTS;
      break;
    case GROUP_NOT_FOUND :
      return GROUPDB_NOT_FOUND;
      break;
    default :
      return GROUPDB_GENERAL_FAIL;
      break;
  }
}

/* ----------------------------------------- */
