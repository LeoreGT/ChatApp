#ifndef __PROTOCOL_H__
#define __PROTOCOL_H__

/**
 * Leore Golan
 * Networking Project CHAT APP 19/07/2017
 */

#include <stdint.h>
#include "list.h"
#include "list_itr.h"
#include "list_functions.h"
#include "UI.h"

#define CLIENT 0
#define SERVER 1

#define SUCCESS_FROM_SERVER 1
#define FAIL_FROM_SERVER 2

#define MAX_BUFF_LEN 4096

/* -------------------------------------------------- */

typedef enum{SHOW_ALL, SHOW_MINE, SHOW_USER_GROUP, CREATE, JOIN, LEAVE, REG, UNREG, LOGIN, LOGOUT}Request_e;

/* -------------------------------------------------- */

struct User{
  char m_username[MAX_STRING_LENGTH];
  char m_password[MAX_STRING_LENGTH];
};

typedef struct {
  struct User m_user;
  char        m_context[MAX_STRING_LENGTH];
  char        m_IPAddress[MAX_STRING_LENGTH];
  List*       m_listOfStrings; /*can be users or groups*/
  Request_e   m_req;
  int  		    m_response;  /* -1 upon failure */
}Data;

/* -------------------------------------------------- */

/**
 * @brief takes a struct and copies all of the relevant information into the outgoing buffer, with delimiters in between
 *
 * @param[in] _data2Pack : the struct holding all of the data to be packed into the buffer
 * @param[out] _messageBuffer : the buffer to which the newly packed data is put
 *
 * @return the length of the output buffer
 */
size_t Protocol_PackData(const Data* _data2Pack, char* _messageBuffer);

/**
 * @brief takes a packed buffer and parses the data, filling it back into the given Data struct
 *
 * @param[in] _messageBuffer : the buffer holding the packed data to be parsed
 * @param[out] _data2Pack : the struct holding all of the relevant fields, to be filled by the function
 *
 * @return FAIL in the case of missing elements
 * @return SUCCESS upon completion of parsing and filling the struct
 */
int Protocol_UnpackData(char* _messageBuffer, size_t _length, Data* _outgoingPack);

/* -------------------------------------------------- */

#endif /* __PROTOCOL_H__ */
