#ifndef __SERVER_NET_H__
#define __SERVER_NET_H__

/* Leore Golan
 * Functions related to a single server in charge of accepting and receiving/sending messages to/from multiple clients
 * Changes : now uses fd_sets and associates macros
 */

#include "ServerApp.h"

typedef struct ServerNet ServerNet;

/* User-assigned action function regarding messages to and from the client
 * param [in] _data : num bytes read from the specific client
 * param [in] _sizeData : the message buffer recieved from the client
 * param [in] _context : context of the function
 */
typedef int (*actionFunc)(ServerNet* serverNet , void* _data, int _sizeData, void* _contex, int* _responseSize, void* _responseData);

ServerNet* ServerNetCreate();

int ServerNetSetUp(ServerNet* _sN);

/* Function in charge of sending a message back to a particular client after recieving a message
 * param [in] currentSocket : the client socket currently being worked on
 * param [in] masterSocket : the master socket int
 * param [in] readBytes : num bytes read from the specific client
 * param [in] inMex : the message buffer recieved from the client
 * param [in] actionFunk : user-assigned action function
 */
void SendMessage(ServerNet* serverNet, int currentSocket, int masterSocket, int readBytes, char* inMex, actionFunc actionFunk);

/* Function in charge of recieving information from the client, uses SendMessage() as an inner function
 * param [in] currentSocket : the client socket currently being worked on
 * param [in] master : the master socket int
 * param [in] masterSocket : the master socket int
 * param [in] highestSocketNum : the current highest socket number
 * param [in] actionFunk : user-assigned action function
 */
void RecieveClient(ServerNet* serverNet, int currentSocket, fd_set* master, int masterSocket, int highestSocketNum, actionFunc actionFunk);

/* Function is in charge of accepting new clients into the master set and revaluing the highestSocketNum
 * param [in] master : the set with the master socket in it
 * param [in] masterSocket : master socket, retVal from SetServerToListen()
 * param [in] highestSocketNum : the current highest socket number (is pointer so it can be updated within the function instead of returning it)
 * param [in] client_addr : sockadder_in struct to be used to set info for the client socket
 */
void SetConnection(fd_set* master, int* highestSocketNum, int masterSocket, struct sockaddr_in *client_addr);

/* Function in charge of socket, setsockopt, bind, & listen functions in the server
 * return value : the number of the socket
 */
int SetServerToListen();

#endif /* __SERVER_NET_H__ */
