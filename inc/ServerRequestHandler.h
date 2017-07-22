#ifndef __SERVER_REQUEST_HANDLER_H__
#define __SERVER_REQUEST_HANDLER_H__

#include "GroupDB.h"
#include "UserDB.h"
#include "Protocol.h"

/**
 * Leore Golan
 * Networking Project CHAT APP 19/07/2017
 */


/**
 * @brief Function takes in a Data struct and holding a request and calls the appropriate functions to generate a response
 *
 * @param[in] _groups : pointer to the GroupsDB from the ServerApp that holds all the groups and their relevant information
 * @param[in] _users : pointer to the UsersDB from the ServerApp that holds all the users and their relevant information
 * @param[in] _inData : the Data struct holding the information of the incoming request
 *
 * @return the Data struct holding the relevant information in response to the request
 */
Data ServerRequestHandler_Read(GroupDB* _groups, UserDB*  _users, Data _inData);

/**
 * @brief Function takes in a Data struct and holding a request to registrater
 *
 * @param[in] _groups : pointer to the GroupsDB from the ServerApp that holds all the groups and their relevant information
 * @param[in] _users : pointer to the UsersDB from the ServerApp that holds all the users and their relevant information
 * @param[in] _inData : the Data struct holding the information of the incoming request
 *
 * @return the Data struct holding the relevant information in response to the request
 */
Data ServerRequestHandler_Reg(GroupDB* _groups, UserDB*  _users, Data _inData);

/**
 * @brief Function takes in a Data struct and holding a request to join a group
 *
 * @param[in] _groups : pointer to the GroupsDB from the ServerApp that holds all the groups and their relevant information
 * @param[in] _users : pointer to the UsersDB from the ServerApp that holds all the users and their relevant information
 * @param[in] _inData : the Data struct holding the information of the incoming request
 *
 * @return the Data struct holding the relevant information in response to the request
 */
Data ServerRequestHandler_Join(GroupDB* _groups, UserDB*  _users, Data _inData);

/**
 * @brief Function takes in a Data struct and holding a request to leave a group
 *
 * @param[in] _groups : pointer to the GroupsDB from the ServerApp that holds all the groups and their relevant information
 * @param[in] _users : pointer to the UsersDB from the ServerApp that holds all the users and their relevant information
 * @param[in] _inData : the Data struct holding the information of the incoming request
 *
 * @return the Data struct holding the relevant information in response to the request
 */
Data ServerRequestHandler_Leave(GroupDB* _groups, UserDB*  _users, Data _inData);

/**
 * @brief Function takes in a Data struct and holding a request to unregister
 *
 * @param[in] _groups : pointer to the GroupsDB from the ServerApp that holds all the groups and their relevant information
 * @param[in] _users : pointer to the UsersDB from the ServerApp that holds all the users and their relevant information
 * @param[in] _inData : the Data struct holding the information of the incoming request
 *
 * @return the Data struct holding the relevant information in response to the request
 */
Data ServerRequestHandler_Unreg(GroupDB* _groups, UserDB*  _users, Data _inData);

/**
 * @brief Function takes in a Data struct and holding a request to login
 *
 * @param[in] _groups : pointer to the GroupsDB from the ServerApp that holds all the groups and their relevant information
 * @param[in] _users : pointer to the UsersDB from the ServerApp that holds all the users and their relevant information
 * @param[in] _inData : the Data struct holding the information of the incoming request
 *
 * @return the Data struct holding the relevant information in response to the request
 */
Data ServerRequestHandler_Login(GroupDB* _groups, UserDB*  _users, Data _inData);

/**
 * @brief Function takes in a Data struct and holding a request to create a new group
 *
 * @param[in] _groups : pointer to the GroupsDB from the ServerApp that holds all the groups and their relevant information
 * @param[in] _users : pointer to the UsersDB from the ServerApp that holds all the users and their relevant information
 * @param[in] _inData : the Data struct holding the information of the incoming request
 *
 * @return the Data struct holding the relevant information in response to the request
 */
Data ServerRequestHandler_Create(GroupDB* _groups, UserDB*  _users, Data _inData);

/**
 * @brief Function takes in a Data struct and holding a request to logout
 *
 * @param[in] _groups : pointer to the GroupsDB from the ServerApp that holds all the groups and their relevant information
 * @param[in] _users : pointer to the UsersDB from the ServerApp that holds all the users and their relevant information
 * @param[in] _inData : the Data struct holding the information of the incoming request
 *
 * @return the Data struct holding the relevant information in response to the request
 */
Data ServerRequestHandler_Logout(GroupDB* _groups, UserDB*  _users, Data _inData);

/**
 * @brief Function takes in a Data struct and holding a request to show all groups
 *
 * @param[in] _groups : pointer to the GroupsDB from the ServerApp that holds all the groups and their relevant information
 * @param[in] _users : pointer to the UsersDB from the ServerApp that holds all the users and their relevant information
 * @param[in] _inData : the Data struct holding the information of the incoming request
 *
 * @return the Data struct holding the relevant information in response to the request
 */
Data ServerRequestHandler_ShowAll(GroupDB* _groups, UserDB*  _users, Data _inData);

/**
 * @brief Function takes in a Data struct and holding a request to show a user's groups
 *
 * @param[in] _groups : pointer to the GroupsDB from the ServerApp that holds all the groups and their relevant information
 * @param[in] _users : pointer to the UsersDB from the ServerApp that holds all the users and their relevant information
 * @param[in] _inData : the Data struct holding the information of the incoming request
 *
 * @return the Data struct holding the relevant information in response to the request
 */
Data ServerRequestHandler_ShowMine(GroupDB* _groups, UserDB*  _users, Data _inData);

/**
 * @brief Function takes in a Data struct and holding a request to show all the users in a group
 *
 * @param[in] _groups : pointer to the GroupsDB from the ServerApp that holds all the groups and their relevant information
 * @param[in] _users : pointer to the UsersDB from the ServerApp that holds all the users and their relevant information
 * @param[in] _inData : the Data struct holding the information of the incoming request
 *
 * @return the Data struct holding the relevant information in response to the request
 */
Data ServerRequestHandler_ShowUsersInGroup(GroupDB* _groups, UserDB*  _users, Data _inData);

/**
 * @brief Function takes in a Data struct and holding a request to remove a user from all of their groups
 *
 * @param[in] _groups : pointer to the GroupsDB from the ServerApp that holds all the groups and their relevant information
 * @param[in] _users : pointer to the UsersDB from the ServerApp that holds all the users and their relevant information
 * @param[in] _inData : the Data struct holding the information of the incoming request
 * @param[in] _username : the username of the user to remove from all their groups
 *
 * @return the Data struct holding the relevant information in response to the request
 */
void LeaveAllGroups(GroupDB* _groups, UserDB*  _users, List* _list, char* _username);


#endif /* __SERVER_REQUEST_HANDLER_H__ */
