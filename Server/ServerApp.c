#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "ServerApp.h"
#include "Protocol.h"
#include "ServerRequestHandler.h"
#include "GroupDB.h"
#include "UserDB.h"

/* -------------------------------------------------- */

#define STARTER_SIZE 1024
#define MAGIC 0xbeefbeef
#define FAIL -1

/* -------------------------------------------------- */


struct ServerApp
{
  GroupDB* m_groups;
  UserDB*  m_users;
  size_t   m_magicNumber;
};

/* -------------------------------------------------- */

ServerApp* ServerAppCreate()
{
  ServerApp* newSV;
  newSV = (ServerApp*) malloc(sizeof(ServerApp));

  newSV->m_groups = GroupDBCreate(STARTER_SIZE);
  newSV->m_users  = UserDBCreate(STARTER_SIZE);
  newSV->m_magicNumber = MAGIC;

  return newSV;
}

/* -------------------------------------------------- */

void ServerAppDestroy(ServerApp* _oldSV)
{
  if ((!_oldSV)|| (_oldSV->m_magicNumber))
  {
    return;
  }

  GroupDBDestroy(_oldSV->m_groups);
  UserDBDestroy(_oldSV->m_users);
  _oldSV->m_magicNumber = 0;
  free(_oldSV);

  return;
}

/* -------------------------------------------------- */

void ServerAppProcessRequest(ServerApp* _serverApp, char* _data, size_t _sizeData, char* outBuf, int* outSize)
{
  Data inData;
  Data response;
	size_t size;
	char* packedData;
  inData.m_listOfStrings = List_Create();
  response.m_listOfStrings = List_Create();

  packedData = (char*) malloc(sizeof(char) * MAX_STRING_LENGTH);

  if ((NULL == _serverApp) || (_serverApp->m_magicNumber != MAGIC))
  {
    return;
  }

  Protocol_UnpackData(_data, _sizeData, &inData);
  response = ServerRequestHandler_Read(_serverApp->m_groups, _serverApp->m_users, inData);
  size = Protocol_PackData(&response, packedData);

  *outSize = size;
  strcpy(outBuf, packedData);
  /*printf("In Server App-- Buffer: %.*s, Size: %d\n", *outSize, packedData, *outSize);*/

  free(packedData);
  return;
}

/* -------------------------------------------------- */

