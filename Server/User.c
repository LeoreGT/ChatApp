#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "User.h"
#include "list_itr.h"
#include "listInternal.h"
#include "list_functions.h"

#define MAGIC	0xbeefbeef

/* ----------------------------------------- */

void CopyString(char* _in, const char* _out);

/* ----------------------------------------- */

struct User_t
{
	size_t m_magicNumber;
	int		 m_isLoggedIn;
	List*  m_myGroups;
  char   m_username[MAX_STRING_LEN];
  char   m_password[MAX_STRING_LEN];
};

/* ----------------------------------------- */

User* UserCreate(const char* _userName, const char* _password)
{
	User* newUser = NULL;

	if((_userName == NULL) || (_password == NULL))
	{
		return NULL;
	}

	newUser = (User*) malloc(sizeof(User));
	if(newUser == NULL)
	{
		return newUser;
	}

	newUser->m_myGroups = List_Create();
	if(newUser->m_myGroups == NULL)
	{
		free(newUser);
		return NULL;
	}

	newUser->m_isLoggedIn = 0;
	newUser->m_magicNumber = MAGIC;
  CopyString(newUser->m_username, _userName);
  CopyString(newUser->m_password, _password);

	return newUser;
}

/* ----------------------------------------- */

void CopyString(char* _in, const char* _out)
{
  size_t stringLength;
  stringLength = strlen(_out);
  stringLength = (stringLength > MAX_STRING_LEN) ? (MAX_STRING_LEN) : (stringLength);
  memcpy(_in, _out, stringLength);
}

/* ----------------------------------------- */

void UserDestroy(User* _user)
{
	if((!_user) || (_user->m_magicNumber != MAGIC))
	{
		return;
	}

	List_Destroy(&(_user->m_myGroups), free);
	_user->m_magicNumber = 0;
	free(_user);
}

/* ----------------------------------------- */

List* UserGetGroups(User* _user)
{
  List* newList = List_Create();
  if((!_user) || (_user->m_magicNumber != MAGIC))
	{
		return NULL;
	}
  newList = _user->m_myGroups;
	return newList;
}

/* ----------------------------------------- */

char* UserGetPassword(const User* _user)
{
  if((!_user) || (_user->m_magicNumber != MAGIC))
	{
		return NULL;
	}

	return strdup(_user->m_password);
}

/* ----------------------------------------- */

char* UserGetName(const User* _user)
{
  if((!_user) || (_user->m_magicNumber != MAGIC))
	{
		return NULL;
	}

	return strdup(_user->m_username);
}

/* ----------------------------------------- */

int Find(void* _element, void* _context)
{
	return !!strcmp((char*)_element, (char*)_context);
}

/* ----------------------------------------- */

UserStat UserAddGroup(const User* _user, const char* _groupName)
{
	ListItr outcome;
	if((!_user) || (_user->m_magicNumber != MAGIC) || (!_groupName))
	{
		return USER_NOT_INITIALIZED_INPUT;
	}

  outcome = ListItr_FindFirst(ListItr_Begin(_user->m_myGroups), ListItr_End(_user->m_myGroups), Find, (void*)_groupName);

	if(outcome == ListItr_End(_user->m_myGroups))
	{
		return GROUP_ALREADY_EXISTS;
	}

	if(List_PushHead(_user->m_myGroups, (void*)_groupName) != LIST_SUCCESS)
	{
		return USER_ALLOCATION_FAILED;
	}
	return USER_SUCCESS;
}

/* ----------------------------------------- */

UserStat UserRemoveGroup(User* _user, const char* _groupName)
{
	ListItr outcome;

	if((!_user) || (_user->m_magicNumber != MAGIC) || (!_groupName))
	{
		return USER_NOT_INITIALIZED_INPUT;
	}

	outcome = ListItr_FindFirst(ListItr_Begin(_user->m_myGroups), ListItr_End(_user->m_myGroups), Find, (void*)_groupName);

  if(outcome == ListItr_End(_user->m_myGroups))
  {
		return USER_NOT_FOUND;
	}

	ListItr_Remove(outcome);
	return USER_SUCCESS;
}

/* ----------------------------------------- */

int UserIsLoggedIn(const User* _user)
{
	return (_user) && (_user->m_magicNumber == MAGIC)  && (_user->m_isLoggedIn);
}

/* ----------------------------------------- */

UserStat UserLogin(User* _user)
{
  if((!_user) || (_user->m_magicNumber != MAGIC) || (UserIsLoggedIn(_user)))
	{
		return USER_NOT_INITIALIZED_INPUT;
	}

	_user->m_isLoggedIn = 1;
	return USER_SUCCESS;
}

/* ----------------------------------------- */

UserStat UserLogout(User* _user)
{
  if((!_user) || (_user->m_magicNumber != MAGIC) || (!UserIsLoggedIn(_user)))
  {
    return USER_NOT_INITIALIZED_INPUT;
  }
	_user->m_isLoggedIn = 0;
	return USER_SUCCESS;
}

/* ----------------------------------------- */
