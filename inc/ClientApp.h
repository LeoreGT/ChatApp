#ifndef __CLIENT_APP_H__
#define __CLIENT_APP_H__

/**
 * Leore Golan
 * Networking Project CHAT APP 19/07/2017
 */

#include "Protocol.h"

typedef struct ClientApp ClientApp;

/**
 * @brief Creates a ClientApp struct which holds a pointer to the ClientNet
 * also calls the create function of the ClientNet
 *
 * @return NULL upon inability to allocate the struct, otherwise a pointer to the new struct is returned
 */
ClientApp* ClientApp_Create();

/**
 * @brief Runs the ClientApp by calling the UI, Protocol, and ClientNet functions when necessary
 *
 * @param[in] _cApp : ClientApp struct to be initiated
 *
 */
void ClientApp_Run(ClientApp* _cApp);

/**
 * @brief Destroys the ClientApp pointer, first destroys the ClientNet in the struct
 *
 * @param[in] _cApp : ClientApp struct to be initiated
 *
 */
void ClientApp_Destroy(ClientApp* _cApp);

#endif /* __CLIENT_APP_H__ */
