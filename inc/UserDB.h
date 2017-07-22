#ifndef __USER_DB_H__
#define __USER_DB_H__

/**
 * Leore Golan
 * Networking Project CHAT APP 19/07/2017
 */

#include "list.h"

typedef struct UserDB UserDB;

typedef enum UserDBRes
{
  USERDB_SUCCESS,
  USERDB_ALLOCATION_FAILED,
  USERDB_UNINITIALIZED_INPUT,
  USERDB_GROUP_ALREADY_EXISTS,
  USERDB_NOT_FOUND,
  USERDB_GENERAL_FAIL
}UserDBRes;

/**
 * @brief Creates UserDB struct which holds a hashmap ogf the current users, the user capacity,
 * the number of current users, and magic number
 *
 * @param[in] _maxUsers : the maximum number of users allowed at once
 *
 * @return NULL if the allocation failed, the new struct if succeeded
 */
UserDB*   UserDBCreate(size_t _maxUsers);

/**
 * @brief Destroys a given UserDB struct
 *
 * @param[in] _userDB : the struct to be destroyed
 *
 */
void      UserDBDestroy(UserDB* _userDB);

/**
 * @brief Creates a new user and adds it to the UserDB struct's hashmap of users
 *
 * @param[in] _userDB : the struct holding the hashmap in which to add the the new user
 * @param[in] _username : the name of the user to be added
 * @param[in] _password : the password of the user to be added
 *
 * @return USERDB_UNINITIALIZED_INPUT insufficient input
 * @return USERDB_GENERAL_FAIL the user couldn't be put into the hashmap
 * @return USERDB_SUCCESS the new user has successfully been created
 */
UserDBRes UserDBAdd(UserDB* _userDB, const char* _userName, const char* _password);

/**
 * @brief Creates a new user and removes it from the UserDB struct's hashmap of users
 *
 * @param[in] _userDB : the struct holding the hashmap from which to remove the the new user
 * @param[in] _username : the name of the user to be removed
 *
 * @return USERDB_UNINITIALIZED_INPUT insufficient input
 * @return USERDB_GENERAL_FAIL the user couldn't be put found in the hashmap
 * @return USERDB_SUCCESS the new user has successfully been created
 */
UserDBRes UserDBRemove(UserDB* _userDB, const char* _userName);

/**
 * @brief Returns the list of a particular user's groups as an out parameter
 *
 * @param[in] _userDB : the struct holding the hashmap from which to get the user's group list
 * @param[in] _username : the name of the user from which to get the list
 * @param[out] _list : the list of the given user
 *
 * @return USERDB_UNINITIALIZED_INPUT insufficient input
 * @return USERDB_GENERAL_FAIL the user couldn't be found in the hashmap
 * @return USERDB_SUCCESS the user's group has been successfully returned
 */
UserDBRes UserDBGetUserGroups(UserDB* _userDB, const char* _username, List** _list);

/**
 * @brief Checks a given password against the one a user stored upon registration
 *
 * @param[in] _userDB : the struct holding the hashmap from which to get the user's stored password
 * @param[in] _username : the name of the user from which to get the password
 * @param[in] _password : the password to be checked against the user's stored password
 *
 * @return USERDB_UNINITIALIZED_INPUT insufficient input
 * @return USERDB_GENERAL_FAIL the given password doesn't match the stored one
 * @return USERDB_SUCCESS the given password correctly matches the stored one
 */
UserDBRes UserDBCheckUserPassword(UserDB* _userDB, const char* _username, const char* _password);

/**
 * @brief Adds a group to the list of groups in a particular User struct
 *
 * @param[in] _userDB : the struct holding the hashmap in which to get the appropriate user struct
 * @param[in] _username : the name of the user to add the new group to
 * @param[in] _groupName : the name of the group to be added to the user's list
 *
 * @return USERDB_NOT_INITIALIZED_INPUT insufficient input
 * @return GROUP_ALREADY_EXISTS the group is already in the user's list, cannot be added twice
 * @return USERDB_ALLOCATION_FAILED the string could not be successfully added to the list
 * @return USERDB_SUCCESS if the group has been successfully added
 */
UserDBRes UserDBAddGroup2User(UserDB* _userDB, const char* _username, const char* groupName);

/**
 * @brief Removes a group from the list of groups in a particular User struct
 *
 * @param[in] _userDB : the struct holding the hashmap in which to get the appropriate user struct
 * @param[in] _username : the name of the user to remove the group from
 * @param[in] _groupName : the name of the group to be removed from the user's list
 *
 * @return USERDB_NOT_INITIALIZED_INPUT insufficient input
 * @return USERDB_ALLOCATION_FAILED the string could not be successfully removed from the list
 * @return USERDB_SUCCESS if the group has been successfully removed
 */
UserDBRes UserDBRemoveGroupFromUser(UserDB* _userDB, const char* _username, const char* groupName);

/**
 * @brief Checks whether a given user is already logged in or not
 *
 * @param[in] _userDB : the struct holding the hashmap in which to get the appropriate user struct
 * @param[in] _username : the name of the user to check logged in status
 *
 * @return boolean-style answer, 1 if logged in, 0 otherwise
 */
int UserDBIsUserLoggedIn(UserDB* _userDB, const char* _username);

/**
 * @brief Logs in a given user by changing the login flag
 *
 * @param[in] _userDB : the struct holding the hashmap in which to get the appropriate user struct
 * @param[in] _username : the name of the user to log in
 *
 * @return USERDB_NOT_INITIALIZED_INPUT insufficient input
 * @return USERDB_SUCCESS the user has been logged in
 */

UserDBRes UserDBLoginUser(UserDB* _userDB, const char* _username);

/**
 * @brief Logs out a given user by changing the logout flag
 *
 * @param[in] _userDB : the struct holding the hashmap in which to get the appropriate user struct
 * @param[in] _username : the name of the user to log out
 *
 * @return USERDB_NOT_INITIALIZED_INPUT insufficient input
 * @return USERDB_SUCCESS the user has been logged out
 */
UserDBRes UserDBLogoutUser(UserDB* _userDB, const char* _username);

#endif /* __USER_DB_H__ */
