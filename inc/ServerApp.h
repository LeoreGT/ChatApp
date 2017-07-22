#ifndef __SERVER_APP_H__
#define __SERVER_APP_H__

/**
 * Leore Golan
 * Networking Project CHAT APP 19/07/2017
 */

typedef struct ServerApp ServerApp;

/**
 * @brief allocates memory for ServerApp struct which holds pointers to UserDB and GroupDB
 *
 * @return the newly allocated struct, or NULL upon inability to allocate
 */
ServerApp* ServerAppCreate();

/**
 * @brief takes in a received request from the client side, processes it, and constructs the appropriate response message
 *
 * @param[in] _serverApp : the struct to process the incoming message and decide upon a returning message
 * @param[in] _data : the buffer with the incoming data, holding the received message from the client side
 * @param[in] _sizeData : the size of the incoming data buffer
 * @param[out] outBuf : the buffer into which the response message will be written
 * @param[out] outSize : the size of the response buffer
 *
 */
void ServerAppProcessRequest(ServerApp* _serverApp, char* _data, size_t _sizeData, char* outBuf, int* outSize);

/**
 * @brief destroys the ServerApp struct, along with the UserDB and GroupDB in the struct
 *
 * @param[in] _oldSV : the ServerApp struct to destroy
 *
 */
void ServerAppDestroy(ServerApp* _oldSV);

#endif /* __SERVER_APP_H__ */
