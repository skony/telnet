#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>

typedef struct User
{
  sockaddr_in user_addr;
  char* pass;
} User;

User users[10];
int users_count = 0;
struct sockaddr_in serv_addr, cli_addr;
int sockfd, newsockfd, portno, clilen;

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
      printf("add host OK:\n");
      break;
    }
    else
    {
      printf("passwords dont match\n");
    }
  }
}

void compare( const char req[])
{
  printf("debug: compare func\n");
  if(strcmp(req, "/host\n") == 0)
  {
    printf("debug: addhost\n");
    addHost();
  }
}

void doprocessing (int sock)
{
  int n;
  char buffer[256];

  bzero(buffer,256);

  n = read(newsockfd,buffer,255);
  if (n < 0)
  {
      perror("ERROR reading from socket");
      exit(1);
  }
  printf("Here is the message: %s\n",buffer);
  compare(buffer);
  n = write(newsockfd,"I got your message",18);
  if (n < 0) 
  {
      perror("ERROR writing to socket");
      exit(1);
  }
}

int main( int argc, char *argv[] )
{
	int pid;
  char buffer[256];
  int  n;

  setsockopt(SO_REUSEADDR);
	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	/* Initialize socket structure */
  bzero((char *) &serv_addr, sizeof(serv_addr));
  portno = 5001;
  serv_addr.sin_family = AF_INET;
  //inet_aton (server, &sck_addr.sin_addr);
  serv_addr.sin_addr.s_addr = INADDR_ANY;
  serv_addr.sin_port = htons(portno);

	/* Now bind the host address using bind() call.*/
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
      while(1)
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
