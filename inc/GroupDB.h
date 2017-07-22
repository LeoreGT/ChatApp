#ifndef __GROUP_DB_H__
#define __GROUP_DB_H__

/**
 * Leore Golan
 * Networking Project CHAT APP 19/07/2017
 */

#include "Group.h"

typedef enum GroupDBRes
{
  GROUPDB_SUCCESS,
  GROUPDB_ALLOCATION_FAILED,
  GROUPDB_UNINITIALIZED_INPUT,
  GROUPDB_USER_ALREADY_EXISTS,
  GROUPDB_NOT_FOUND,
  GROUPDB_GENERAL_FAIL
}GroupDBRes;

typedef struct GroupDB GroupDB;

/**
 * @brief Creates a Group struct which holds a hashmap of the groups, capacitiy of groups,
 * number of current groups, and a magic number
 *
 * @return NULL upon inability to allocate the struct, otherwise a pointer to the new struct is returned
 */
GroupDB* GroupDBCreate(size_t _maxGroups);

/**
 * @brief Destroys the GroupDB struct
 *
 * @param[in] _grpDB : the struct to be destroyed
 *
 */
void GroupDBDestroy(GroupDB* _grpDB);

/**
 * @brief adds a group to the hashmap in the Group struct
 *
 * @param[in] _grpDB : the struct in which to add the new group
 * @param[in] _groupName : the name of the new group
 *
 * @return GROUPDB_UNINITIALIZED_INPUT insufficient input
 * @return GROUPDB_GENERAL_FAIL the group could not be successfully added to the list
 * @return GROUPDB_SUCCESS if the group is successfully added
 */
GroupDBRes GroupDBAdd(GroupDB* _grpDB, const char* _groupName);

/**
 * @brief removes a group to the hashmap in the Group struct
 *
 * @param[in] _grpDB : the struct in which to remove the new group
 * @param[in] _groupName : the name of the new group
 *
 * @return GROUPDB_UNINITIALIZED_INPUT insufficient input
 * @return GROUPDB_GENERAL_FAIL the group could not be successfully added to the list
 * @return GROUPDB_SUCCESS if the group is successfully added
 */
GroupDBRes GroupDBRemove(GroupDB* _grpDB, const char* _groupName);

/**
 * @brief returns the list of groups froma particular GroupDB struct
 *
 * @param[in] _group : the struct from which to get the list of groups
 *
 * @return the list of groups in the group
 */
List* GroupDBShowGroups(GroupDB* _grpDB);

/**
 * @brief checks if a given group exists in the hashmap of the GroupDB by given name
 *
 * @param[in] _grpDB : the struct in which to check for the group
 * @param[in] _groupName : the name of the group to be searched for
 *
 * @return GROUPDB_SUCCESS if group exists
 * @return GROUPDB_GENERAL_FAIL if  group does not exist
 */
GroupDBRes DoesGroupExist(GroupDB* _grpDB, const char* _groupName);

/**
 * @brief returns the IP of the Group in the hashmap of the GroupDB struct
 *
 * @param[in] _grpDB : the struct in which to check for the group
 * @param[in] _groupName : the name of the group to get the IP for
 *
 * @return the IP address of the group
 */
char* GroupDBGetGroupIP(GroupDB* _grpDB, const char* _groupName);

/**
 * @brief returns the list of users from particular Group in the hashmap fo the GroupDB struct
 *
 * @param[in] _grpDB : the groupDB holding the group from which to get the list
 * @param[in] _groupName : the name of the group to get the list from
 * @param[out] _users : the list of users from the group
 *
 * @return GROUPDB_UNINITIALIZED_INPUT input is not properly initialized
 * @return GROUPDB_GENERAL_FAIL user list could not be retrieved from group
 * @return GROUPDB_SUCCESS if action was successful
 */
GroupDBRes GroupDBGetGroupUsers(GroupDB* _grpDB, const char* _groupName, List** _users);

/**
 * @brief adds a new user from particular Group in the hashmap fo the GroupDB struct
 *
 * @param[in] _grpDB : the groupDB holding the group to which to add the new user
 * @param[in] _groupName : the name of the group to add the user to
 * @param[in] _users : tthe name of the new user to add
 *
 * @return GROUPDB_UNINITIALIZED_INPUT input is not properly initialized
 * @return GROUPDB_GENERAL_FAIL user could not be added to group
 * @return GROUPDB_SUCCESS if action was successful
 */
GroupDBRes GroupDBAddUser2Group(GroupDB* _grpDB, const char* _groupName, char* _username);

/**
 * @brief removes a new user from particular Group in the hashmap fo the GroupDB struct
 *
 * @param[in] _grpDB : the groupDB holding the group to which to remove the new user
 * @param[in] _groupName : the name of the group to remove the user to
 * @param[in] _users : tthe name of the new user to remove
 *
 * @return GROUPDB_UNINITIALIZED_INPUT input is not properly initialized
 * @return GROUPDB_GENERAL_FAIL user could not be removed from group
 * @return GROUPDB_SUCCESS if action was successful
 */
GroupDBRes GroupDBRemoveUserFromGroup(GroupDB* _grpDB, const char* _GroupName, char* _username);

#endif /* __GROUP_DB_H__ */
