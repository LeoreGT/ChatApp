#include <stdio.h>
#include <string.h>
#include <stdlib.h> /* system */
#include <unistd.h> /* fork */
#include "UI.h"

#define SHOW_ALL_R 0
#define SHOW_MINE_R 1
#define SHOW_USERS_GROUP_R 2
#define CREATE_R 3
#define JOIN_R 4
#define LEAVE_R 5
#define REG_R 6
#define UNREG_R 7
#define LOGIN_R 8
#define LOGOUT_R 9
#define ENTER_R -2
#define PORT 8080

void UI_PrintList(List* _list);

/* -------------------------------------------------- */

Credentials UI_OpeningUI()
{
	Credentials streetCred;
	char password[1024];
	char username[1024];
	int isNew;

  puts("  _          __  _____   _       _____   _____       ___  ___   _____    _");
  puts(" | |        / / | ____| | |     /  ___| /  _  \\     /   |/   | | ____|  | |");
  puts(" | |  __   / /  | |__   | |     | |     | | | |    / /|   /| | | |__    | |");
  puts(" | | /  | / /   |  __|  | |     | |     | | | |   / / |__/ | | |  __|   |_|");
  puts(" | |/   |/ /    | |___  | |___  | |___  | |_| |  / /       | | | |___    _");
  puts(" |___/|___/     |_____| |_____| \\_____| \\_____/ /_/        |_| |_____|  (_)");	  
  printf("\nWelcome to the networking project. Please pick one:\n");
	printf("1. I'm a returning user. Log me in!\n");
	printf("2. I'm new here. Register me!\n");
	scanf("%d", &isNew);
	streetCred.m_isNew = isNew - 1;

	printf("Please enter your username: \n");
	scanf("%s", username);
	printf("Please enter your password: \n");
	scanf("%s", password);

	strcpy(streetCred.m_username, username);
	strcpy(streetCred.m_password, password);

	return streetCred;
}

/* -------------------------------------------------- */

void UI_RespondToUser(int _typeResponse, List* _list)
{
	system ("clear");
  switch(_typeResponse)
  {
		case -2 :
      printf("Your info couldn't be found. Please try again.\n");
      break;
    case -1 :
      printf("Please try again.\n");
      break;
    case 0:
      printf("Now printing the list of all available groups: \n");
      UI_PrintList(_list);
      break;
    case 1:
      printf("Now printing the list of all your groups: \n");
      UI_PrintList(_list);
      break;
    case 3:
      printf("You have successfully created a new group!\n");
      break;
    case 4:
      printf("You have successfully joined the group!\n");
      break;
    case 5:
      printf("You have successfully left the group.\n");
      break;
    case 6:
      printf("You have been successfully registered. Welcome!\n");
      break;
    case 7:
      printf("You have been unregistered. Goodbye!\n");
      break;
    case 8:
      printf("You have successfully logged in. Welcome back!\n");
      break;
    case 9:
      printf("You have successfully logged out. Come again!\n");
      break;
    default:
      break;
  }
}

/* -------------------------------------------------- */

struct UIAction UI_ActionsUI(const char* _username)
{
  UIAction uia;
  int action;
  char group[1024];
  uia.m_req = -1;

  printf("\nWelcome! Please pick one:\n");
  printf("1. Create a new group\n");
  printf("2. Join an existing group\n");
  printf("3. Leave a group you're subscribed to\n");
  printf("4. Log Out\n");
  printf("5. Unregister\n");
  printf("6. Show all available groups\n");
  scanf("%d", &action);
  switch(action)
  {
    case 1:
      uia.m_req =  CREATE_R;
      printf("Please insert the group's ID: \n");
      scanf("%s", group);
      strcpy(uia.m_group, group);
      break;
    case 2:
      uia.m_req =  JOIN_R;
      printf("Please insert the group's ID: \n");
      scanf("%s", group);
      strcpy(uia.m_group, group);
      break;
    case 3:
      uia.m_req =  LEAVE_R;
      printf("Please insert the group's ID: \n");
      scanf("%s", group);
      strcpy(uia.m_group, group);
      break;
    case 4:
      uia.m_req = LOGOUT_R;
      break;
    case 5:
      uia.m_req = UNREG_R;
			break;
    case 6 :
      uia.m_req = SHOW_ALL_R;
      break;
    case 7:
      uia.m_req =  SHOW_MINE_R;
      break;
    default:
      uia.m_req = -1;
      break;
  }
  return uia;
}

/* -------------------------------------------------- */

void UI_PrintList(List* _list)
{ 
	ListItr it = ListItr_Begin(_list);
	ListItr end = ListItr_End(_list);
	char* str;

  printf("List:\n");
	for (; it != end; it = ListItr_Next(it))
	{
		str = (char*)(ListItr_Get(it));
		printf("- %s\n", str);
	}
} 
/* -------------------------------------------------- */
