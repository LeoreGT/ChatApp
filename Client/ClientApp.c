#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h> 

#include "ClientApp.h"
#include "UI.h"
#include "ClientNet.h"
#include "ChatManager.h"
#include "Protocol.h"

#define PORT 8080
#define IP "192.168.0.58"
/*#define IP "127.0.0.1"
GEORGE
#define IP "192.168.0.81"
WESLEY
#define IP "192.168.0.176"
MEIR
#define IP "192.168.0.79"*/

#define ENTER -2
#define EXIT 1
#define FAIL -1

/* -------------------------------------------------- */

size_t ClientApp_PackRequest(Data _data2Pack, char* _buffer);
void   ClientApp_PassPackedData2Server(char* _buffer, size_t _length);
int    ClientApp_UnpackData(char* _messageBuffer, size_t _length, Data* _outgoingPack);
Data   ClientApp_SetData(char* _username, UIAction _uia);
Data   ClientApp_SetDataInitial(Credentials _cred);
int    ClientApp_RunRequests(ClientApp* _cApp, char* _username);
int 	 ClientApp_ReadResponse(ChatManager* _cMan, Data inData);
/* int 	 ClientApp_GetIPAndPort(int* _port, char* _IPAddress); */

/* -------------------------------------------------- */

struct ClientApp
{
  ClientNet*   m_cNet;
  ChatManager* m_cManager;
};

/* -------------------------------------------------- */

ClientApp* ClientApp_Create()
{
  ClientApp* newCApp = (ClientApp*) malloc(sizeof(ClientApp));
  if (NULL == newCApp)
  {
    return newCApp;
  }

  if(NULL == (newCApp->m_cNet = ClientNetCreate()))
  {
    free(newCApp);
    printf("failed to create ClientNet\n");
    return NULL;
  }

	if(NULL == (newCApp->m_cManager = ChatManager_Create()))
  {
    perror("failed to create ChatManager");
    ClientNetDestroy(&newCApp->m_cNet);
		free(newCApp);
    return NULL;
  }

  if(CLIENTNET_SUCCESS != ClientNetConnect(newCApp->m_cNet, PORT, IP))
  {
    perror("Failed to connect ClientNet to server");
    ClientNetDestroy(&newCApp->m_cNet);
		ChatManager_Destroy(newCApp->m_cManager);
    free(newCApp);
    return NULL;
  }

  return newCApp;
}

/* -------------------------------------------------- */

void ClientApp_Destroy(ClientApp* _cApp)
{
  if(CLIENTNET_SUCCESS !=  ClientNetDisConnect(_cApp->m_cNet))
  {
    printf("ClientNet failed to disconnect\n");
    return;
  }

  ClientNetDestroy(&_cApp->m_cNet);
	ChatManager_Destroy(_cApp->m_cManager);
  free(_cApp);
}

/* -------------------------------------------------- */

void ClientApp_Run(ClientApp* _cApp)
{
  char buff[MAX_STRING_LENGTH];
  Data inData;
	size_t length;

  Credentials streetCred = UI_OpeningUI();
  inData = ClientApp_SetDataInitial(streetCred);
	length = ClientApp_PackRequest(inData, buff);

  if(CLIENTNET_SUCCESS != ClientNetSendMessage(_cApp->m_cNet, buff, length))
  {
    printf("ClientNet failed to send message\n");
    return;
  }
	if (CLIENTNET_SUCCESS != ClientNetRecieveMessage(_cApp->m_cNet, buff, (size_t*)&length))
  {
    return;
  }

  Protocol_UnpackData(buff, length, &inData);
	if (EXIT == ClientApp_ReadResponse(_cApp->m_cManager, inData))
	{
		return;
	}

	if (inData.m_response == FAIL)
	{
		UI_RespondToUser(ENTER, NULL);
		ClientApp_Run(_cApp);
	}

  while(1)
  {
		if (1 == ClientApp_RunRequests(_cApp, strdup(streetCred.m_username)))
		{
			exit(1);
		}
  }
}

/* -------------------------------------------------- */

int ClientApp_RunRequests(ClientApp* _cApp, char* _username)
{
  char buff[MAX_STRING_LENGTH];
  char buff2[MAX_STRING_LENGTH];
  Data inData, sendData;
  size_t length, length2;
  UIAction uia;
  inData.m_listOfStrings = List_Create();
  sendData.m_listOfStrings = List_Create();


  
  uia = UI_ActionsUI(_username);
	if (uia.m_req == -1)
	{
		perror("How did we get here?");
	}

  sendData = ClientApp_SetData(_username, uia);
  length2 = ClientApp_PackRequest(sendData, buff2);
  if(CLIENTNET_SUCCESS !=  ClientNetSendMessage(_cApp->m_cNet, buff2, length2))
  {
    printf("ClientNet failed to send message\n");
    return 0;
  }
	
if (CLIENTNET_SUCCESS != ClientNetRecieveMessage(_cApp->m_cNet, buff, (size_t*)&length))
  {
    return 0;
  }

  Protocol_UnpackData(buff, length, &inData);
	if (EXIT == ClientApp_ReadResponse(_cApp->m_cManager, inData))
	{
		return EXIT;
	}

	return 0;
}

/* -------------------------------------------------- */

Data ClientApp_SetDataInitial(Credentials _cred)
{
  Data newData;
  strcpy(newData.m_user.m_username, _cred.m_username);
  strcpy(newData.m_user.m_password, _cred.m_password);
  strcpy(newData.m_context, "0");
  strcpy(newData.m_IPAddress, "0");
  newData.m_listOfStrings = NULL;
  newData.m_req = (_cred.m_isNew == 0) ? LOGIN : REG;
  newData.m_response = 0;

  return newData;
}

/* -------------------------------------------------- */

Data ClientApp_SetData(char* _username, UIAction _uia)
{
  Data newData;
  strcpy(newData.m_user.m_username, _username);
  strcpy(newData.m_user.m_password, "0");
  if (strlen(_uia.m_group) >= 2)
  {
    strcpy(newData.m_context, _uia.m_group);
  }
  else
  {
   strcpy(newData.m_context, "0");
  }
  strcpy(newData.m_IPAddress, "0");
  newData.m_listOfStrings = NULL;
  newData.m_req = _uia.m_req;
  newData.m_response = 0;

  return newData;
}

/* -------------------------------------------------- */

int ClientApp_ReadResponse(ChatManager* _cMan, Data inData)
{
	if (((inData.m_req == CREATE) || (inData.m_req == JOIN))  && (inData.m_response != -1))
	{
		ChatManager_StartChat(_cMan, inData.m_user.m_username, inData.m_IPAddress);
		UI_RespondToUser(inData.m_response, inData.m_listOfStrings);
		return 0;
	}

	else if((inData.m_req == LEAVE) && (inData.m_response != -1))
	{
		ChatManager_ExitChat(_cMan, inData.m_IPAddress);
	}

  else if ((inData.m_req == LOGOUT) || (inData.m_req == UNREG))
  {
		printf("~~~~ Goodbye! ~~~~\n");
		printf("    Come again!\n");;
    ChatManager_ExitAllChats(_cMan);
		return EXIT;
  }

	UI_RespondToUser(inData.m_response, inData.m_listOfStrings);
	return 0;
}


/* -------------------------------------------------- */

size_t ClientApp_PackRequest(Data _data2Pack, char* _buffer)
{
  return Protocol_PackData(&_data2Pack, _buffer);
}

/* -------------------------------------------------- */

int ClientApp_UnpackData(char* _messageBuffer, size_t _length, Data* _outgoingPack)
{
  return Protocol_UnpackData(_messageBuffer, _length, _outgoingPack);
}

/* -------------------------------------------------- */
/*
int ClientApp_GetIPAndPort(int* _port, char* _IPAddress)
{
	FILE* fPtr;
	int port;

	fPtr = fopen("../config.txt", "r+");
	if (NULL == fPtr)
	{
		return FAIL;
	}
	fscanf(fPtr, "%d %s", &port, _IPAddress);
	printf("NOW GETTING %d As PORT AND %s AS IP", *_port, _IPAddress);
	*_port = port;
	fclose(fPtr);
	return 0;
}*/

/* -------------------------------------------------- */

int main()
{
  ClientApp* CAp;
  CAp = ClientApp_Create();
  ClientApp_Run(CAp);
  ClientApp_Destroy(CAp);
  return 0;
}

/* -------------------------------------------------- */

