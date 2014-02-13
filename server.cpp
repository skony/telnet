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

#define NUM_THREADS 100

struct User
{
  struct sockaddr_in user_addr;
  char pass[];
} User;

struct thread_data
{
	unsigned int s_addr;
	int newsockfd;
} thread_data;

struct User users[10];
int users_count = 0;
int threads = 0;
struct sockaddr_in serv_addr;
int sockfd, portno = 5001;
struct thread_data thread_data_array[NUM_THREADS];

void addHost(int newsockfd, unsigned int s_addr)
{
  int n;
  char buffer[256];
  char buffer2[256];

  while(1)
  {
    char msg[50] = "Please write password";
    write(newsockfd, msg, strlen(msg));
		bzero(buffer, 256);
		bzero(buffer2, 256);
    read(newsockfd,buffer,255);
    strcpy(msg, "Please write password again");
    write(newsockfd, msg, strlen(msg));
    read(newsockfd,buffer2,255);

    printf("debug: bufer  %s\n",buffer);
    printf("debug: bufer2 %s\n",buffer2);

    if(strcmp(buffer, buffer2) == 0)
    {
      printf("debug: adding1\n");
      users[users_count].user_addr.sin_family = AF_INET;
      printf("debug: adding2\n");
      users[users_count].user_addr.sin_addr.s_addr = s_addr;
      printf("debug: adding3\n");
      users[users_count].user_addr.sin_port = htons(portno);
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

void* doprocessing (void *arg)
{
  int n, sock_dest, newsockfd;
  char buffer[256];
	bool connected = false;
	struct sockaddr_in dest_addr;
	unsigned int s_addr;
	struct thread_data *my_data;

	my_data = (struct thread_data *) arg;
	newsockfd = my_data->newsockfd;
	s_addr = my_data->s_addr;

	while(1)
	{
		bzero(buffer,256);

		read(newsockfd,buffer,255);
		printf("Here is the message: %s\n",buffer);
		if(strcmp(buffer, "-host") == 0)
		{
		  printf("debug: addhost\n");
		  addHost(newsockfd, s_addr);
		}
		else if(!connected)
		{
			sock_dest = connectToHost(buffer, dest_addr, connected);
			bzero(buffer, 256);
			strcpy(buffer, "OK");
			write(newsockfd, buffer, strlen(buffer));		
		}
		else
		{
			write(newsockfd,"I got your message",18);
		}
		//write(newsockfd,"I got your message",18);
	}
}

int main( int argc, char *argv[] )
{
	int pid, t;
  char buffer[256];
  int  n, newsockfd, clilen;
	struct sockaddr_in cli_addr;

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
		t = threads;
		thread_data_array[t].s_addr = cli_addr.sin_addr.s_addr;
		thread_data_array[t].newsockfd = newsockfd;
    pthread_t pid;
		pthread_create(&pid, NULL, doprocessing, &thread_data_array[t]);
		threads = threads + 1;
  }
	return 0;

}
