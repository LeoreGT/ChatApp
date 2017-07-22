#ifndef __UI_H__
#define __UI_H__

/**
 * Leore Golan
 * Networking Project CHAT APP 19/07/2017
 */

#include "list.h"
#include "list_itr.h"
#include "list_functions.h"

#define MAX_STRING_LENGTH 1024

/* -------------------------------------------------- */

typedef struct Credentials
{
  int  m_isNew;
  char m_username[MAX_STRING_LENGTH];
  char m_password[MAX_STRING_LENGTH];
}Credentials;

/* -------------------------------------------------- */

typedef struct UIAction
{
  char m_group[MAX_STRING_LENGTH];
  int  m_req;
}UIAction;

/* -------------------------------------------------- */

/**
 * @brief runs UI prompt that allows the user to choose to either register or log in
 *
 * @return struct containing the user's input username, password, and whether the user is logging in or registering
 */
Credentials UI_OpeningUI();

/**
 * @brief runs UI prompt that allows the user to choose a request such as creating, joining, or leaving a group, showing groups, or exiting
 *
 * @param[in] _username : the username of the user, given by the ClientApp (who calls the function after UI_OpeningUI)
 *
 * @return struct containing the user's request and the groupname (if needed by the particular request)
 */
struct UIAction UI_ActionsUI(const char* _username);

/**
 * @brief returns a response to a user after a request has been made, for example either fail or success of request
 *
 * @param[in] _typeResponse : notifies the function of which response to give the user
 * @param[in] _list : teh list requested by the user to be printed, if relevant depending on the request
 *
 */
void UI_RespondToUser(int _typeResponse, List* _list);

/**
 * @brief opens chat after a user has created or joined a group
 * opens two windows for a chat, one to send and one to recieve messages
 *
 * @param[in] _username : the username of the user
 * @param[in] _IPAddress : the multicast IP address of the chat
 *
 
void UI_StartChat(char* _userName, char* _IPAddress);*/

/* -------------------------------------------------- */

#endif /* __UI_H__ */

