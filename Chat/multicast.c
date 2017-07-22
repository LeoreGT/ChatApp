#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define MAX_STRING_LENGTH 1024

/* -------------------------------------------------- */

void DoSend(int sock, struct sockaddr_in* addr_p, const char* _IPAddress, const char* _username)
{
	char buffer[MAX_STRING_LENGTH];
	char message[MAX_STRING_LENGTH];
	size_t len;
	int cnt; 
	int addrlen = sizeof(*addr_p);
	addr_p->sin_addr.s_addr = inet_addr(_IPAddress);

	while (1)
	{
		printf("Type in your message: "); 
		
		fgets(buffer, MAX_STRING_LENGTH, stdin);
		len = strlen(buffer); buffer[len - 1] = 0;
		sprintf(message, "%s : %s", _username, buffer);
		cnt = sendto(sock, message, sizeof(message), 0, (struct sockaddr *) addr_p, addrlen);
		if (cnt < 0)
		{
      perror("sendto");
      exit(1);
		}
	}
}
/* -------------------------------------------------- */

void DoRecieve(int sock, struct sockaddr_in* addr_p, const char* _IPAddress)
{
	struct ip_mreq mreq;
	char message[MAX_STRING_LENGTH];
	int addrlen = sizeof(*addr_p);
	int cnt;
	time_t t;
	char buffer[MAX_STRING_LENGTH];

	if (bind(sock, (struct sockaddr *) addr_p, sizeof(*addr_p)) < 0)
	{
    perror("bind");
    exit(1);
	}
	t = time(0);
	sprintf(buffer, "%-24.24s", ctime(&t));
	mreq.imr_multiaddr.s_addr = inet_addr(_IPAddress);
	mreq.imr_interface.s_addr = htonl(INADDR_ANY);
	if (setsockopt(sock, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq)) < 0)
	{
    perror("setsockopt mreq");
    exit(1);
	}

	while (1)
	{
		cnt = recvfrom(sock, message, sizeof(message), 0, (struct sockaddr *) addr_p, (socklen_t*)&addrlen);
		if (cnt < 0)
		{
			perror("recvfrom");
			exit(1);
		}
		else if (cnt == 0)
		{
			break;
		}
    printf(" _____________________________________________________\n");
    printf("|                                                     |\n");
    printf("|  %s \n", message);
    printf("|                                                   \n");
    printf("|                                                     |\n");
		printf("| Sent at: %s                   |\n", buffer);
    printf("|_____   _____________________________________________|\n");
    printf("      | /\n");
    printf("      |/\n");
    printf("      .\n");
  }
}

/* -------------------------------------------------- */

int main(int argc, char* argv[])
{
	int sock, port;
	FILE* fPtr;
	char username[1024] = "user";

	struct sockaddr_in addr;
	fPtr = fopen("./pid.txt", "w+");
	if (NULL == fPtr)
	{
		perror("error opening file.");
		return -1;
	}
	fprintf(fPtr, "%d", getpid());
	fclose(fPtr);
	
	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock < 0)
	{
    perror("socket");
    exit(1);
  }
	if (argv[4])
	{
		strcpy(username,argv[4]);
	}

	port = atoi(argv[2]);

  bzero((char *)&addr, sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = inet_addr(argv[1]);
  addr.sin_port = htons(port);

  if (argc > 6)
  {
    DoSend(sock, &addr, argv[1], username);
  }
  else
  {
    DoRecieve(sock, &addr, argv[1]);
  }
	return 0;
}

/* -------------------------------------------------- */

