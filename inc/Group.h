#ifndef _GROUP_H_
#define _GROUP_H_

/**
 * Leore Golan
 * Networking Project CHAT APP 19/07/2017
 */

#include "list_itr.h"
#include "listInternal.h"
#include "list_functions.h"

#define MAX_STRING_LEN 1024

typedef enum GroupStat
{
  GROUP_SUCCESS,
  GROUP_ALLOCATION_FAILED,
  GROUP_NOT_INITIALIZED_INPUT,
  USER_ALREADY_EXISTS,
  GROUP_NOT_FOUND
}GroupStat;

typedef struct Group Group;

/**
 * @brief Creates a Group struct which holds the group's name, IP address, number of current users,
 * a list of the current group users, and a magic number
 *
 * @return NULL upon inability to allocate the struct, otherwise a pointer to the new struct is returned
 */
Group* GroupCreate(const char* _groupName);

/**
 * @brief Destroys the given Group struct
 *
 * @param[in] _group : the struct to be destroyed
 *
 */
void GroupDestroy(Group* _group);

/**
 * @brief returns the list of users froma particular Group struct
 *
 * @param[in] _group : the struct from which to get the list of users
 *
 * @return the list of users in the group
 */
List* GroupGetUsers(const Group* _group);

/**
 * @brief returns the IP of the Group struct
 *
 * @param[in] _group : the struct from which to get the group's IP address
 *
 * @return the IP address of the group
 */
char* GetGroupIP(const Group* _group);

/**
 * @brief returns the name of the Group struct
 *
 * @param[in] _group : the struct from which to get the group's name address
 *
 * @return the name address of the group
 */
char* GetGroupName(const Group* _group);

/**
 * @brief adds a user to the list of users in the Group struct
 *
 * @param[in] _group : the struct in which to add the the new user
 *
 * @return GROUP_NOT_INITIALIZED_INPUT insufficient input
 * @return USER_ALREADY_EXISTS the user is already in the list, cannot be added twice
 * @return GROUP_ALLOCATION_FAILED the string could not be successfully added to the list
 * @return GROUP_SUCCESS if the user is successfully added
 */
GroupStat GroupAddUser(Group* _group, char* _username);

/**
 * @brief removes a user from the list of users in the Group struct
 *
 * @param[in] _group : the struct in which to remove the the new user
 *
 * @return GROUP_NOT_INITIALIZED_INPUT insufficient input
 * @return USER_ALREADY_EXISTS the user is already in the list, cannot be added twice
 * @return GROUP_ALLOCATION_FAILED the string could not be successfully added to the list
 * @return GROUP_SUCCESS if the user is successfully added
 */
GroupStat GroupRemoveUser(Group* _group, char* _userName);

/**
 * @brief checks if the Group has any users in its user list
 *
 * @param[in] _group : the struct to check
 *
 * @return boolean-style return, 1 for empty, 0 for non-empty
 */
int IsGroupEmpty(const Group* _group);

/**
 * @brief checks if the Group how many users in its user list
 *
 * @param[in] _group : the struct to check
 *
 * @return the number of current users in the group
 */
int GroupNumUsers(const Group* _group);


#endif /* _GROUP_H_ */
