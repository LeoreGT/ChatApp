#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/select.h>
#include <arpa/inet.h>
#include <netdb.h>
#include "server.h"
#include "ServerApp.h"

#define PORT 8080
#define BUFSIZE 1024
#define FAIL -1
#define TRUE 0

/* ------------------------------------------------------------- */

struct ServerNet
{
  ServerApp* serverApp;
};

/* ------------------------------------------------------------- */

int main()
{
  ServerNet* newSN;
  newSN = ServerNetCreate();
  ServerNetSetUp(newSN);
  return 0;
}

/* ------------------------------------------------------------- */

int ActionFunc(ServerNet* serverNet, void* _data, int _sizeData, void* _contex, int* _responseSize, void* _responseData)
{
  ServerAppProcessRequest(serverNet->serverApp, _data, _sizeData, (char*)_responseData, _responseSize);
  return TRUE;
}

/* ------------------------------------------------------------- */

ServerNet* ServerNetCreate() /*create func allocates struct and creates the serverapp*/
{
  ServerNet* serverNet = malloc(sizeof(ServerNet));
  serverNet->serverApp = ServerAppCreate();
  return serverNet;
}

/* ------------------------------------------------------------- */

int ServerNetSetUp(ServerNet* _sN)
{
  fd_set master;
  fd_set clientFDs;
  int highestSocketNum, currentSocket;
  int masterSocket = 0;
  struct sockaddr_in client_addr;

  FD_ZERO(&master);    /*clear the socket using the fd macro*/
  FD_ZERO(&clientFDs); /*clear the socket using the fd macro*/
  masterSocket = SetServerToListen();
  FD_SET(masterSocket, &master); /*set the masterSocket into the master set*/

  highestSocketNum = masterSocket; /*set the highest number*/
  while(1)
  {
    clientFDs = master; /*duplicate the master fd_set*/
    if(FAIL != select(highestSocketNum + 1, &clientFDs, NULL, NULL, NULL))
    {
      for (currentSocket = 0; currentSocket <= highestSocketNum; currentSocket++)
      {
        if (FD_ISSET(currentSocket, &clientFDs))
        {
          if (currentSocket == masterSocket)
          {
            SetConnection(&master, &highestSocketNum, masterSocket, &client_addr);
          }
          else
          {
            RecieveClient(_sN, currentSocket, &master, masterSocket, highestSocketNum, ActionFunc);
          }
        }
      }
    }

    else
    {
      perror("select");
      return FAIL;
    }
  }
  return 0;
}

/* ------------------------------------------------------------- */

void RecieveClient(ServerNet* serverNet, int currentSocket, fd_set *master, int masterSocket, int highestSocketNum, actionFunc func)
{
  int readBytes;
  char message[BUFSIZE];

  if ((readBytes = recv(currentSocket, message, BUFSIZE, 0)) <= 0)
  {
    if (readBytes == 0) {
      printf("socket %d hung up\n", currentSocket);
    }
    else
    {
      perror("recv");
    }
    close(currentSocket);
    FD_CLR(currentSocket, master);
  }
  else
  {
    SendMessage(serverNet, currentSocket, masterSocket, readBytes, message, func);
  }
}

/* ------------------------------------------------------------- */

void SendMessage(ServerNet* serverNet, int currentSocket, int masterSocket, int readBytes, char* inMex, actionFunc actionFunk)
{
  char response[1024];
  int responseSize;

  actionFunk(serverNet, (void*)inMex, readBytes, NULL, &responseSize, (void*)response);
  printf("Buffer: %.*s, Size: %d\n", responseSize, response, responseSize);
  if (FAIL == send(currentSocket, response, responseSize, 0))
  {
    perror("send");
  }
}

/* ------------------------------------------------------------- */

void SetConnection(fd_set *master, int* highestSocketNum, int masterSocket, struct sockaddr_in *client_addr)
{
  socklen_t addrlen;
  int new_socket;

  addrlen = sizeof(struct sockaddr_in);
  if(FAIL != (new_socket = accept(masterSocket, (struct sockaddr *)client_addr, &addrlen)))
  {
    FD_SET(new_socket, master);
    if (new_socket > *highestSocketNum)
    {
      *highestSocketNum = new_socket;
    }
    printf("new connection from %s on port %d \n",inet_ntoa(client_addr->sin_addr), ntohs(client_addr->sin_port));
  }
  else
  {
    perror("Error upon accepting");
    return;
  }
}

/* ------------------------------------------------------------- */

int SetServerToListen() /*socket, setsockopt, bind, & listen*/
{
  int socket_desc, opt = 1;
  struct sockaddr_in* _address = malloc (sizeof(struct sockaddr_in));

  if (FAIL == (socket_desc = socket(AF_INET, SOCK_STREAM, 0)))
  {
    perror("Error upon socket");
    return FAIL;
  }
  _address->sin_family = AF_INET;
  _address->sin_port = htons(PORT);
  _address->sin_addr.s_addr = INADDR_ANY;
  memset(_address->sin_zero, 0, sizeof _address->sin_zero);

  if (FAIL == setsockopt(socket_desc, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(int)))
  {
    perror("Error upon setsockopt.");
    return FAIL;
  }

  if (FAIL == bind(socket_desc, (struct sockaddr *)_address, sizeof(struct sockaddr)))
  {
    perror("Error upon binding.");
    return FAIL;
  }

  if (FAIL == listen(socket_desc, 10))
  {
    perror("Error upon listening.");
    return FAIL;
  }
  return socket_desc;
}

/* ------------------------------------------------------------- */
