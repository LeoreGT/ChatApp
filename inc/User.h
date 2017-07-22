#ifndef _USER_H_
#define _USER_H_

/**
 * Leore Golan
 * Networking Project CHAT APP 19/07/2017
 */

#include "list.h"

#define MAX_STRING_LEN 1024

typedef enum UserStat
{
	USER_SUCCESS,
	USER_ALLOCATION_FAILED,
	USER_NOT_INITIALIZED_INPUT,
	GROUP_ALREADY_EXISTS,
 	USER_NOT_FOUND
}UserStat;

typedef struct User_t User;

/**
 * @brief Creates User struct which holds flag for login, list of user's groups, username,
 * password, and magic number
 *
 * @param[in] _userName : the username of the the new user
 * @param[in] _password : the user's password to be checked upon login
 *
 * @return NULL if the allocation failed, the new struct if succeeded
 */
User* UserCreate(const char* _userName, const char* _password);

/**
 * @brief Destroys a given User struct
 *
 * @param[in] _user : the struct to be destroyed
 *
 */
void UserDestroy(User* _user);

/**
 * @brief Returns the list of a given user's groups
 *
 * @param[in] _user : the struct from which to get the list of groups
 *
 * @return the user's list of groups
 */
List* UserGetGroups(User* _user);

/**
 * @brief Returns a username from a given User struct
 *
 * @param[in] _user : the struct from which to get the username
 *
 * @return the username from the user struct
 */
char* UserGetName(const User* _user);

/**
 * @brief Returns a password from a given User struct
 *
 * @param[in] _user : the struct from which to get the password
 *
 * @return the password from the user struct
 */
char* UserGetPassword(const User* _user);

/**
 * @brief Adds a group to the list of groups in the User struct
 *
 * @param[in] _user : the struct in which to add the the new group
 * @param[in] _groupName : the name of the group to be added to the user's list
 *
 * @return USER_NOT_INITIALIZED_INPUT insufficient input
 * @return GROUP_ALREADY_EXISTS the group is already in the user's list, cannot be added twice
 * @return USER_ALLOCATION_FAILED the string could not be successfully added to the list
 * @return USER_SUCCESS if the group has been successfully added
 */
UserStat UserAddGroup(const User* _user, const char* _groupName);

/**
 * @brief Removes a group from the list of groups in the User struct
 *
 * @param[in] _user : the struct from which to remove the the new group
 * @param[in] _groupName : the name of the group to be removed to the user's list
 *
 * @return USER_NOT_INITIALIZED_INPUT insufficient input
 * @return USER_ALLOCATION_FAILED the string could not be successfully removed to the list
 * @return USER_SUCCESS if the group has been successfully removed
 */
UserStat UserRemoveGroup(User* _user, const char* _groupName);

/**
 * @brief Checks whether a given user is already logged in or not
 *
 * @param[in] _user: the struct to check logged in status
 *
 * @return boolean-style answer, 1 if logged in, 0 otherwise
 */
int UserIsLoggedIn(const User* _user); /* -1 if NULL input */

/**
 * @brief Logs in a given user by changing the login flag
 *
 * @param[in] _user : the user to log in
 *
 * @return USER_NOT_INITIALIZED_INPUT insufficient input
 * @return USER_SUCCESS the user has been logged in
 */
UserStat UserLogin(User* _user);

/**
 * @brief Logs out a given user by changing the logout flag
 *
 * @param[in] _user : the user to log out
 *
 * @return USER_NOT_INITIALIZED_INPUT insufficient input
 * @return USER_SUCCESS the user has been logged out
 */
UserStat UserLogout(User* _user);


#endif /* _USER_H_ */
