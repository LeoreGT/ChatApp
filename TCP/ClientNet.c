#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>

#include "ClientNet.h"

/* -------------------------------------------------------- */

#define MAX_BUF_LEN 1024
#define MAGIC	0xBEEFBEEF

/* -------------------------------------------------------- */

struct ClientNet
{
	size_t m_magicNumber;
	size_t m_port;
	char*	 m_ip;
	int		 m_soketFD;
  struct sockaddr_in m_serverAddr;
};

/* -------------------------------------------------------- */

ClientNet* ClientNetCreate(void)
{
	ClientNet* newClientNet;

	newClientNet = (ClientNet*)malloc(sizeof(ClientNet));
	if(NULL == newClientNet)
	{
		return newClientNet;
	}

	newClientNet->m_soketFD	= 0;
	newClientNet->m_port		= 0;
	newClientNet->m_ip			= NULL;
 	newClientNet->m_magicNumber	= MAGIC;

	return newClientNet;
}

/* -------------------------------------------------------- */

ClientNet_Result ClientNetConnect(ClientNet* _ClientNet, size_t _port, const char* _IPAddress)
{
	struct sockaddr_in sin;
	int socketNum = 0;

	if((!_ClientNet) || (_ClientNet->m_magicNumber != MAGIC) || (_port < 1000) || (!_IPAddress))
	{
		return CLIENTNET_UNINITIALIZED_ERROR;
	}

	_ClientNet->m_ip	= (char*)_IPAddress;
	_ClientNet->m_port = _port;

	if((socketNum = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		return CLIENTNET_ALLOCATION_ERROR;
	}
	_ClientNet->m_soketFD = socketNum;


	memset((char *)&(sin), 0, sizeof(struct sockaddr_in));
	sin.sin_family 		 = AF_INET;
	sin.sin_port 			 = htons(_ClientNet->m_port);
	sin.sin_addr.s_addr = inet_addr(_ClientNet->m_ip);
	if(connect(_ClientNet->m_soketFD, (struct sockaddr*)&(sin), sizeof(sin)) < 0)
	{
		return CLIENTNET_TRANSFER_ERROR;
	}
	_ClientNet->m_serverAddr = sin;

	return CLIENTNET_SUCCESS;
}

/* -------------------------------------------------------- */

ClientNet_Result	ClientNetDisConnect(ClientNet* _ClientNet)
{
	if((!_ClientNet) || (_ClientNet->m_magicNumber != MAGIC))
	{
		return CLIENTNET_UNINITIALIZED_ERROR;
	}

	_ClientNet->m_ip	 = NULL;
	_ClientNet->m_port = 0;

	close(_ClientNet->m_soketFD);
	_ClientNet->m_soketFD	= 0;
	_ClientNet->m_port		= 0;
	_ClientNet->m_ip		  = NULL;

	return CLIENTNET_SUCCESS;
}

/* -------------------------------------------------------- */

ClientNet_Result	ClientNetSendMessage(ClientNet* _ClientNet, const char* _message, size_t _size)
{
	if((!_message) || (!_ClientNet) || (_ClientNet->m_magicNumber != MAGIC))
	{
		return CLIENTNET_UNINITIALIZED_ERROR;
	}

	if(!_ClientNet->m_soketFD)
	{
		return CLIENTNET_TRANSFER_ERROR;
	}

	if(0 > sendto(_ClientNet->m_soketFD, _message, _size, 0, (struct sockaddr*)(&_ClientNet->m_serverAddr), sizeof(_ClientNet->m_serverAddr)))
	{
		return CLIENTNET_TRANSFER_ERROR;
	}

	return CLIENTNET_SUCCESS;
}

/* -------------------------------------------------------- */

ClientNet_Result	ClientNetRecieveMessage(ClientNet* _ClientNet, char* _mex, size_t* _max)
{
	struct sockaddr_in sin = {0};
	char buffer[MAX_BUF_LEN] = {0};
	int  soket_length;

	if((!_mex) || (!_ClientNet) || (_ClientNet->m_magicNumber != MAGIC))
	{
		return CLIENTNET_UNINITIALIZED_ERROR;
	}

	if(!_ClientNet->m_soketFD)
	{
		return CLIENTNET_TRANSFER_ERROR;
	}

	if(*_max >= MAX_BUF_LEN)
	{
		return CLIENTNET_ALLOCATION_ERROR;
	}

	memset((char *)&(sin), 0, sizeof(struct sockaddr_in));
	sin.sin_family 		  = AF_INET;
	sin.sin_port 		    = htons(_ClientNet->m_port);
	sin.sin_addr.s_addr 	= inet_addr(_ClientNet->m_ip);
	soket_length = sizeof(sin);

	if((*_max = recvfrom(_ClientNet->m_soketFD, buffer, MAX_BUF_LEN, 0, (struct sockaddr*)(&sin), (socklen_t *)&soket_length)) <= 0)
	{
		ClientNetDisConnect(_ClientNet);
	 	return CLIENTNET_TRANSFER_ERROR;
	}

	memcpy(_mex, buffer, *_max);

	return CLIENTNET_SUCCESS;
}

/* -------------------------------------------------------- */

void ClientNetDestroy(ClientNet** _ClientNet)
{
  if(!_ClientNet)
  {
    return;
  }

  (*_ClientNet)->m_magicNumber = 0;
  free(*_ClientNet);
  *_ClientNet = NULL;
}

/* -------------------------------------------------------- */
