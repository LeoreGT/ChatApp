#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "HashMap.h"
#include "UserDB.h"
#include "User.h"
#include "list.h"

/* ----------------------------------------- */

#define MAGIC 0xbeefbeef
#define TEMP_STR 1024

/* ----------------------------------------- */

static int  HashFunc(char *str);
static int  IsKeysEqual(char* _key1, char* _key2);
static void DestroyVal(void* _val);
UserDBRes   convertUserStat(UserStat _res);

/* ----------------------------------------- */

struct UserDB
{
  HashMap* m_users;
  size_t   m_userCap;
  size_t   m_numUsers;
  size_t   m_magicNumber;
};

/* ----------------------------------------- */

UserDB* UserDBCreate(size_t _maxUsers)
{
  UserDB* newUDB = NULL;
  newUDB = (UserDB*) malloc(sizeof(UserDB));
  if (NULL == newUDB)
  {
    return newUDB;
  }

  newUDB->m_userCap = _maxUsers;
  newUDB->m_users = HashMap_Create(_maxUsers, (HashFunction) HashFunc, (EqualityFunction) IsKeysEqual);
  newUDB->m_magicNumber = MAGIC;
  newUDB->m_numUsers = 0;

  return newUDB;
}

/* ----------------------------------------- */

void UserDBDestroy(UserDB* _userDB)
{
  if ((!_userDB) || (_userDB->m_magicNumber != MAGIC))
  {
    return;
  }

  HashMap_Destroy(&_userDB->m_users, NULL, DestroyVal);
  _userDB->m_magicNumber = 0;
  _userDB->m_numUsers = 0;
  free(_userDB);
}

/* ----------------------------------------- */

UserDBRes UserDBAdd(UserDB* _userDB, const char* _username, const char* _password)
{
  User* user;

  if ((!_userDB) || (_userDB->m_magicNumber != MAGIC) || (!_username) || (!_password))
  {
    return USERDB_UNINITIALIZED_INPUT;
  }

  user = UserCreate(_username, _password);
  if(HashMap_Insert(_userDB->m_users, _username, user) != MAP_SUCCESS)
  {
    free(user);
    return USERDB_GENERAL_FAIL;
  }

  _userDB->m_numUsers++;
  return USERDB_SUCCESS;
}

/* ----------------------------------------- */

UserDBRes UserDBRemove(UserDB* _userDB, const char* _username)
{
  User* user = NULL;

  if ((!_userDB) || (_userDB->m_magicNumber != MAGIC) || (!_username))
  {
    return USERDB_UNINITIALIZED_INPUT;
  }
  if(HashMap_Remove(_userDB->m_users, _username, (void**)&user) == MAP_SUCCESS)
  {
    _userDB->m_numUsers--;
    free(user);
    return USERDB_SUCCESS;
  }
  else
  {
    free(user);
    return USERDB_GENERAL_FAIL;
  }
}

/* ----------------------------------------- */

UserDBRes UserDBGetUserGroups(UserDB* _userDB, const char* _username, List** _list)
{
  User* user = NULL;

  if ((!_userDB) || (_userDB->m_magicNumber != MAGIC) || (!_username) || (!_list))
  {
    return USERDB_UNINITIALIZED_INPUT;
  }

  if(HashMap_Find(_userDB->m_users, _username, (void**) &user) == MAP_SUCCESS)
  {
    *_list = (List*)UserGetGroups(user);
    return USERDB_SUCCESS;
  }

  else
  {
    return USERDB_GENERAL_FAIL;
  }
}

/* ----------------------------------------- */

UserDBRes UserDBCheckUserPassword(UserDB* _userDB, const char* _username, const char* _password)
{
  User* user = NULL;
  char* passRes = (char*) malloc(sizeof(char) * TEMP_STR);

  if ((!_userDB) || (_userDB->m_magicNumber != MAGIC) || (!_username) || (!_password))
  {
    return USERDB_UNINITIALIZED_INPUT;
  }

  if(HashMap_Find(_userDB->m_users, _username, (void**) &user) == MAP_SUCCESS)
  {
    strcpy(passRes,UserGetPassword(user));
    if(strcmp(passRes,_password) == 0)
    {
      free(passRes);
      return USERDB_SUCCESS;
    }
    else
    {
      free(passRes);
      return USERDB_GENERAL_FAIL;
    }
  }

  else
  {
    free(passRes);
    free(user);
    return USERDB_NOT_FOUND;
  }
}

/* ----------------------------------------- */

UserDBRes UserDBAddGroup2User(UserDB* _userDB, const char* _username, const char* _groupName)
{
  User* user;
  UserStat res;
  if ((!_userDB) || (_userDB->m_magicNumber != MAGIC) || (!_username) || (!_groupName))
  {
    return USERDB_UNINITIALIZED_INPUT;
  }

  if(HashMap_Find(_userDB->m_users, _username, (void**) &user) == MAP_SUCCESS)
  {
    res = UserAddGroup(user, _groupName);
    return convertUserStat(res);
  }
  else
  {
    return USERDB_GENERAL_FAIL;
  }
}

/* ----------------------------------------- */

UserDBRes UserDBRemoveGroupFromUser(UserDB* _userDB, const char* _username, const char* _groupName)
{
  User* user;
  UserStat res;

  if ((!_userDB) || (_userDB->m_magicNumber != MAGIC) || (!_username) || (!_groupName))
  {
    return USERDB_UNINITIALIZED_INPUT;
  }

  if(HashMap_Find(_userDB->m_users, _username, (void**) &user) == MAP_SUCCESS)
  {
    res = UserRemoveGroup(user, _groupName);
    return convertUserStat(res);
  }
  else
  {
    return USERDB_GENERAL_FAIL;
  }
}

/* ----------------------------------------- */

int UserDBIsUserLoggedIn(UserDB* _userDB, const char* _username)
{
  User* user;

  if ((!_userDB) || (_userDB->m_magicNumber != MAGIC) || (!_username))
  {
    return USERDB_GENERAL_FAIL;
  }

  if(HashMap_Find(_userDB->m_users, _username, (void**) &user) == MAP_SUCCESS)
  {
    return UserIsLoggedIn(user);
  }
  else
  {
    free(user);
    return USERDB_GENERAL_FAIL;
  }
}

/* ----------------------------------------- */

UserDBRes UserDBLoginUser(UserDB* _userDB, const char* _username)
{
  User* user;
  UserStat res;

  if ((!_userDB) || (_userDB->m_magicNumber != MAGIC) || (!_username))
  {
    return USERDB_UNINITIALIZED_INPUT;
  }

  if(HashMap_Find(_userDB->m_users, _username, (void**) &user) == MAP_SUCCESS)
  {
    res = UserLogin(user);
    return convertUserStat(res);
  }
  else
  {
    return USERDB_GENERAL_FAIL;
  }
}

/* ----------------------------------------- */

UserDBRes UserDBLogoutUser(UserDB* _userDB, const char* _username)
{
  User* user;
  UserStat res;

  if ((!_userDB) || (_userDB->m_magicNumber != MAGIC) || (!_username))
  {
    return USERDB_UNINITIALIZED_INPUT;
  }

  if(HashMap_Find(_userDB->m_users, _username, (void**) &user) == MAP_SUCCESS)
  {
    res = UserLogout(user);
    return convertUserStat(res);
  }
  else
  {
    return USERDB_GENERAL_FAIL;
  }
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

static void DestroyVal(void* _val)
{
  free(_val);
}

/* ----------------------------------------- */

UserDBRes convertUserStat(UserStat _res)
{
  switch (_res)
  {
    case USER_SUCCESS :
      return USERDB_SUCCESS;
      break;
    case USER_ALLOCATION_FAILED :
      return USERDB_ALLOCATION_FAILED;
      break;
    case USER_NOT_INITIALIZED_INPUT :
      return USERDB_UNINITIALIZED_INPUT;
      break;
    case GROUP_ALREADY_EXISTS :
      return USERDB_GROUP_ALREADY_EXISTS;
      break;
    case USER_NOT_FOUND :
      return USERDB_NOT_FOUND;
      break;
    default :
      return USERDB_GENERAL_FAIL;
      break;
  }
}

/* ----------------------------------------- */
