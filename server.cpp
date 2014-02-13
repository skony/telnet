#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <pthread.h>

typedef struct User
{
  struct sockaddr_in user_addr;
  char pass[];
} User;

User users[10];
int users_count = 0;
struct sockaddr_in serv_addr, cli_addr;
int sockfd, newsockfd, portno = 5001, clilen;

void addHost()
{
  int n;
  char buffer[256];
  char buffer2[256];

  while(1)
  {
    char msg[50] = "Please write password";
    n = write(newsockfd, msg, strlen(msg));
    n = read(newsockfd,buffer,255);
    strcpy(msg, "Please write password again");
    write(newsockfd, msg, strlen(msg));
    read(newsockfd,buffer2,255);

    printf("debug: bufer  %s\n",buffer);
    printf("debug: bufer2 %s\n",buffer2);

    if(strcmp(buffer, buffer2) == 0)
    {
      printf("debug: adding1\n");
      users[users_count].user_addr.sin_family = cli_addr.sin_family;
      printf("debug: adding2\n");
      users[users_count].user_addr.sin_addr = cli_addr.sin_addr;
      printf("debug: adding3\n");
      users[users_count].user_addr.sin_port = cli_addr.sin_port;
      printf("debug: adding4\n");
      strcpy(users[users_count].pass, buffer);
      printf("debug: adding5\n");
      users_count = users_count + 1;
      printf("add host OK: %d\n",  users[users_count].user_addr.sin_addr.s_addr);
			bzero(msg,50);
			strcpy(msg, "Now you are hosting");
			write(newsockfd, msg, strlen(msg));
      break;
    }
    else
    {
      printf("passwords dont match\n");
    }
  }
}

int connectToHost(char host[], struct sockaddr_in &dest_addr, bool &connection)
{
	printf("debug: host0\n");
	struct in_addr host_addr;
	inet_aton(host, &host_addr);
	int sock_dest	= socket(AF_INET, SOCK_STREAM, 0);

	for(int i=0; i<9; i++)
	{
		if(users[i].user_addr.sin_addr.s_addr == host_addr.s_addr)
		{
			printf("debug: host1\n");
			dest_addr.sin_family = AF_INET;
  		dest_addr.sin_addr.s_addr = host_addr.s_addr;
  		dest_addr.sin_port = htons(portno);
			if (connect(sock_dest, (struct sockaddr *) &dest_addr,sizeof(dest_addr)) < 0) 
  		{
				perror("ERROR connecting");
				return -1;
			}
			else
			{
				printf("debug: host2\n");
				connection = true;
				return sock_dest;
			}	
		}
	}
}

void doprocessing (int sock)
{
  int n;
  char buffer[256];
	bool connected = false;
	struct sockaddr_in dest_addr;
	int sock_dest;

	while(1)
	{
		bzero(buffer,256);

		read(newsockfd,buffer,255);
		printf("Here is the message: %s\n",buffer);
		if(strcmp(buffer, "-host") == 0)
		{
		  printf("debug: addhost\n");
		  addHost();
		}
		else if(!connected)
		{
			sock_dest = connectToHost(buffer, dest_addr, connected);
		}
		//write(newsockfd,"I got your message",18);
	}
}

int main( int argc, char *argv[] )
{
	int pid;
  char buffer[256];
  int  n;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	/* Initialize socket structure */
  bzero((char *) &serv_addr, sizeof(serv_addr));
  portno = 5001;
  serv_addr.sin_family = AF_INET;
  //inet_aton (server, &sck_addr.sin_addr);
  serv_addr.sin_addr.s_addr = INADDR_ANY;
  serv_addr.sin_port = htons(portno);

	/* Now bind the host address using bind() call.*/
  int* optval;
  setsockopt(sockfd, 1, SO_REUSEADDR, &optval,4);
  if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
  {
		perror("ERROR on binding");
   	exit(1);
  }

	/* start listen */
	listen(sockfd,5);
  clilen = sizeof(cli_addr);

  while(1)
  {

    newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, (socklen_t*) &clilen);
    pid = fork();

    if (pid == 0)  
    {
      /* This is the client process */
      close(sockfd);
      doprocessing(newsockfd);
      exit(0);
    }
    else
    {
      close(newsockfd);
    }
  }
	return 0;

}
