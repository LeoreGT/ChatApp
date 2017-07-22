#include <string.h>
#include <stdio.h>

#include "ServerRequestHandler.h"
#include "Protocol.h"
#include "GroupDB.h"
#include "UserDB.h"

/* -------------------------------------------------- */

#define STARTER_SIZE 1024
#define MAGIC 0xbeefbeef
#define FAIL -1

/* -------------------------------------------------- */

Data ServerRequestHandler_Read(GroupDB* _groups, UserDB*  _users, Data _inData)
{
  switch(_inData.m_req)
  {
    case SHOW_ALL :
		  _inData = ServerRequestHandler_ShowAll(_groups, _users, _inData);
		  break;
		case SHOW_MINE :
		  _inData = ServerRequestHandler_ShowMine(_groups, _users, _inData);
		  break;
    case CREATE :
		  _inData = ServerRequestHandler_Create(_groups, _users, _inData);
		  break;
    case JOIN :
		  _inData = ServerRequestHandler_Join(_groups, _users, _inData);
		  break;
    case LEAVE :
		  _inData = ServerRequestHandler_Leave(_groups, _users, _inData);
		  break;
    case REG :
		  _inData = ServerRequestHandler_Reg(_groups, _users, _inData);
		  break;
    case UNREG :
		  _inData = ServerRequestHandler_Unreg(_groups, _users, _inData);
		  break;
    case LOGIN :
		  _inData = ServerRequestHandler_Login(_groups, _users, _inData);
		  break;
    case LOGOUT :
		  _inData = ServerRequestHandler_Logout(_groups, _users, _inData);
		  break;
    default :
		  _inData.m_response = FAIL;
		  break;
  }
  return _inData;
}

/* -------------------------------------------------- */

Data ServerRequestHandler_ShowAll(GroupDB* _groups, UserDB*  _users, Data _inData)
{
  _inData.m_listOfStrings = GroupDBShowGroups(_groups);
  _inData.m_response = _inData.m_req;
  return _inData;
}

/* -------------------------------------------------- */

Data ServerRequestHandler_ShowMine(GroupDB* _groups, UserDB*  _users, Data _inData)
{
  UserDBRes res;

  res = UserDBGetUserGroups(_users, _inData.m_user.m_username, &_inData.m_listOfStrings);
  if (res == USERDB_SUCCESS)
  {
    _inData.m_response = _inData.m_req;
    return _inData;
  }
  _inData.m_response = FAIL;
  return _inData;
}

/* -------------------------------------------------- */

Data ServerRequestHandler_ShowUsersInGroup(GroupDB* _groups, UserDB*  _users, Data _inData)
{
  GroupDBRes res;
  res = GroupDBGetGroupUsers(_groups, _inData.m_context, &_inData.m_listOfStrings);
  if (res == GROUPDB_SUCCESS)
  {
    _inData.m_response = _inData.m_req;
    return _inData;
  }
  _inData.m_response = FAIL;
  return _inData;
}

/* -------------------------------------------------- */

Data ServerRequestHandler_Create(GroupDB* _groups, UserDB*  _users, Data _inData)
{
  GroupDBRes resG;
  UserDBRes resU;
	char group[MAX_STRING_LENGTH];
	strcpy(group, _inData.m_context);

  resG = GroupDBAdd(_groups, _inData.m_context);

  if (resG == GROUPDB_SUCCESS)
  {
		strcpy(_inData.m_IPAddress, GroupDBGetGroupIP(_groups, group));
    resU = UserDBAddGroup2User(_users, _inData.m_user.m_username,  _inData.m_context);
    if (resU == USERDB_SUCCESS)
    {
			strcpy(_inData.m_context, group);
      _inData.m_response = _inData.m_req;
      return _inData;
    }
  }
  _inData.m_response = FAIL;
  return _inData;
}

/* -------------------------------------------------- */

Data ServerRequestHandler_Join(GroupDB* _groups, UserDB*  _users, Data _inData)
{
  UserDBRes resU;
  GroupDBRes resG;

  resG = DoesGroupExist(_groups, _inData.m_context);
  if(resG == GROUPDB_SUCCESS)
  {
    resU = UserDBAddGroup2User(_users, _inData.m_user.m_username,  _inData.m_context);
    if (resU == USERDB_SUCCESS)
    {
      strcpy(_inData.m_IPAddress, GroupDBGetGroupIP(_groups, _inData.m_context));
      _inData.m_response = _inData.m_req;
      return _inData;
    }
  }

  _inData.m_response = FAIL;
  return _inData;
}

/* -------------------------------------------------- */

Data ServerRequestHandler_Leave(GroupDB* _groups, UserDB*  _users, Data _inData)
{
  GroupDBRes resG;
  UserDBRes  resU;

	if (!GroupDBGetGroupIP(_groups, _inData.m_context))  
	{
		_inData.m_response = FAIL;
	  return _inData;
	}

	strcpy(_inData.m_IPAddress, GroupDBGetGroupIP(_groups, _inData.m_context));
	resG = GroupDBRemoveUserFromGroup(_groups, _inData.m_context, _inData.m_user.m_username);

	if (resG == GROUPDB_SUCCESS)
	{
	  resU = UserDBRemoveGroupFromUser(_users, _inData.m_user.m_username, _inData.m_context);
	  if (resU == USERDB_SUCCESS)
	  {
	    _inData.m_response = _inData.m_req;
	    return _inData;
	  }
	}

	_inData.m_response = FAIL;
  return _inData;
}

/* -------------------------------------------------- */

Data ServerRequestHandler_Reg(GroupDB* _groups, UserDB*  _users, Data _inData)
{
  UserDBRes res;
  res = UserDBAdd(_users, _inData.m_user.m_username,  _inData.m_user.m_password);
  if (res == USERDB_SUCCESS)
  {
    _inData.m_response = _inData.m_req;
    return _inData;
  }
  _inData.m_response = FAIL;
  return _inData;
}

/* -------------------------------------------------- */

Data ServerRequestHandler_Unreg(GroupDB* _groups, UserDB*  _users, Data _inData)
{
  List* deleterList = List_Create();
  UserDBRes res;

  UserDBLogoutUser(_users, _inData.m_user.m_username);
  res = UserDBRemove(_users, _inData.m_user.m_username);
  if (res == USERDB_SUCCESS)
  {
    if (USERDB_SUCCESS == UserDBGetUserGroups(_users, _inData.m_user.m_username, &deleterList))
    {
      LeaveAllGroups( _groups, _users, deleterList, _inData.m_user.m_username);
    }
    _inData.m_response = _inData.m_req;
    return _inData;
  }
  _inData.m_response = FAIL;
  return _inData;
}



/* -------------------------------------------------- */

Data ServerRequestHandler_Login(GroupDB* _groups, UserDB*  _users, Data _inData)
{
  UserDBRes res;

  res = UserDBCheckUserPassword(_users, _inData.m_user.m_username, _inData.m_user.m_password);
  if ((res == USERDB_SUCCESS)  && (0 == UserDBIsUserLoggedIn(_users, _inData.m_user.m_username)))
  {
    res = UserDBLoginUser(_users, _inData.m_user.m_username);
    if (res == USERDB_SUCCESS)
    {
      _inData.m_response = _inData.m_req;
      return _inData;
    }
  }

  _inData.m_response = FAIL;
  return _inData;
}

/* -------------------------------------------------- */

Data ServerRequestHandler_Logout(GroupDB* _groups, UserDB*  _users, Data _inData)
{
  List* deleterList = List_Create();
  UserDBRes res;

  if (1 == UserDBIsUserLoggedIn(_users, _inData.m_user.m_username))
  {
		res = UserDBLogoutUser(_users, _inData.m_user.m_username);
		if (res == USERDB_SUCCESS)
		{
			if (USERDB_SUCCESS == UserDBGetUserGroups(_users, _inData.m_user.m_username, &deleterList))
			{
				LeaveAllGroups( _groups, _users, deleterList, _inData.m_user.m_username);
				_inData.m_response = _inData.m_req;
				return _inData;
			}
		}  
	}
  _inData.m_response = FAIL;
  return _inData;
}

/* -------------------------------------------------- */

void LeaveAllGroups(GroupDB* _groups, UserDB*  _users, List* _list, char* _username)
{
  GroupDBRes resG;
  UserDBRes resU;
  ListItr it = ListItr_Begin(_list);
  ListItr end = ListItr_End(_list);
  char* str;

  for (; it != end; it = ListItr_Next(it))
  {
    str = (ListItr_Get(it));

    resG = GroupDBRemoveUserFromGroup(_groups, str, _username);
    if (resG == GROUPDB_SUCCESS)
    {
      resU = UserDBRemoveGroupFromUser(_users, _username, str);
      if (resU != USERDB_SUCCESS)
      {
        return;
      }
    }
  }
}

/* -------------------------------------------------- */
