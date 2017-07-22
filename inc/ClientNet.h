#ifndef __CLIENTNET_H__
#define __CLIENTNET_H__

/**
 * Leore Golan
 * Networking Project CHAT APP 19/07/2017
 */

#include <stddef.h>

typedef enum ClientNet_Result {
	CLIENTNET_SUCCESS,
	CLIENTNET_UNINITIALIZED_ERROR,
	CLIENTNET_ALLOCATION_ERROR,
	CLIENTNET_TRANSFER_ERROR
} ClientNet_Result;

typedef struct ClientNet ClientNet;

/**
 * @brief Create a new client that will send and recieve messages to/from a server, the struct
 * will hold m_magicNumber, m_port, m_ip, m_soketFD, and m_serverAddr
 *
 * @return NULL upon inability to allocate the struct, otherwise the newly allocated struct
 */
ClientNet* ClientNetCreate();


/**
 * @brief Takes in a port and IP address to hook up to a server side
 *
 * @param[in] _client: the struct with which to connect the server
 * @param[in] _port	:	 the given port to hook up to
 * @param[in] _ip	:		 the given IP address to hook up to
 *
 * @return CLIENTNET_SUCCESS if connection works
 * @return CLIENTNET_UNINITIALIZED_ERROR upon insufficient input
 * @return CLIENTNET_ALLOCATION_ERROR inability to create socket
 */
ClientNet_Result ClientNetConnect(ClientNet* _client, size_t _port, const char* _ip);

/**
 * @brief Transmits a given message to the server side
 *
 * @param[in] _client: the struct from which to send the message
 * @param[in] _message: the given message to send
 * @param[in] _size: the size of the given message buffer to be sent
 *
 *
 * @return CLIENTNET_UNINITIALIZED_ERROR upon insufficient input
 * @return CLIENTNET_TRANSFER_ERROR upon failure of the send function
 * @return CLIENTNET_SUCCESS upon successfully sending the message
 */
ClientNet_Result	ClientNetSendMessage(ClientNet* _client, const char* _message, size_t _size);

/**
 * @brief Receives incoming messages from the server side
 *
 * @param[in] _client: the struct with which to receive incoming messages from the server
 * @param[in] _outMex: The buffer to store the recieved message in
 * @param[in] _max: The max size of buffer
 *
 * @return CLIENTNET_UNINITIALIZED_ERROR upon insufficient input
 * @return CLIENTNET_ALLOCATION_ERROR the size of the message is larger than the maximum allowed length
 * @return CLIENTNET_TRANSFER_ERROR inability to receive messages
 * @return CLIENTNET_SUCCESS upon successful retrieval of messages
 */
ClientNet_Result  ClientNetRecieveMessage(ClientNet* _ClientNet, char* _outMex, size_t *_max);

/**
 * @brief Disconnects the given client from the server side
 *
 * @param[in] _client: the struct with which to disconnect the server
 *
 * @return CLIENTNET_UNINITIALIZED_ERROR upon insufficient input
 * @return CLIENTNET_SUCCESS upon successful disconnecting from the server side
 */
ClientNet_Result	ClientNetDisConnect(ClientNet* _client);

/**
 * @brief Destroys a given client
 *
 * @param[in] _server : the struct with which to connect the server
 *
 */
void ClientNetDestroy(ClientNet** _client);

#endif /* __CLIENTNET_H__ */
