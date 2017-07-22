#ifndef __CHAT_MANAGER_H__
#define __CHAT_MANAGER_H__

/**
 * Leore Golan
 * Networking Project CHAT APP 19/07/2017
 */

#include "list.h"

typedef struct ChatManager ChatManager;

typedef enum ChatRes
{
  CHATM_SUCCESS,
  CHATM_ALLOCATION_FAILED,
  CHATM_NOT_INITIALIZED_INPUT
}ChatRes;


/**
 * @brief Creates the ChatManager struct which holds a hashmap of PIDs, m_magicNumber, and a list of groups
 *
 * @param[in] _cApp : ChatManager struct to be initiated
 *
 */
ChatManager* ChatManager_Create();

/**
 * @brief Starts a multicast chat with the given IP address, automatically opens windows
 *
 * @param[in] _CM : ChatManager struct to be initiated
 * @param[in] _userName : username of the user
 * @param[in] _IPAddress : the multicast IP address of the chat
 *
 */
void ChatManager_StartChat(ChatManager* _CM, char* _userName, char* _IPAddress);

/**
 * @brief Kills the chat windows for the user
 *
 * @param[in] _CM : ChatManager struct holding the PIDs of the chat windows to be closed
 * @param[in] _groupName : the name of the chat to be killed
 *
 */
ChatRes ChatManager_ExitChat(ChatManager* _CM, const char* _groupName);

/**
 * @brief Kills all open chats for the user
 *
 * @param[in] _CM : ChatManager struct holding the PIDs of the chat windows to be closed
 *
 */
ChatRes ChatManager_ExitAllChats(ChatManager* _CM);

/**
 * @brief Returns all the names of the groups that the useris subscribed to
 *
 * @param[in] _CM : ChatManager struct holding the list of the user's groups
 *
 */
List* ChatManager_ReturnGroupList(ChatManager* _CM);

/**
 * @brief Destroys the ChatManager struct
 *
 * @param[in] _CM : ChatManager struct to be destroyed
 *
 */
void ChatManager_Destroy(ChatManager* _CM);

#endif /* __CHAT_MANAGER_H__ */
