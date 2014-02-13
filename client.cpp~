#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>

char server_ip[20] = "127.0.0.1";
char server_port[10] = "5001";

int main( int argc, char *argv[] )
{
	int sockfd, portno, n;
  struct sockaddr_in serv_addr;
  struct hostent *server;

  char buffer[256];

	portno = atoi(server_port);
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	server = gethostbyname(server_ip);

	bzero((char *) &serv_addr, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  bcopy((char *)server->h_addr, (char*)&serv_addr.sin_addr.s_addr, server->h_length);
  serv_addr.sin_port = htons(portno);
	int* optval;
	setsockopt(sockfd, 1, SO_REUSEADDR, &optval,4);

	if (connect(sockfd, (struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
  {
    perror("ERROR connecting");
    exit(1);
  }

	if(strcmp(argv[1], "-host") == 0)
	{
		bzero(buffer,256);
		write(sockfd,argv[1],strlen(argv[1]));

		while(1)
		{
			bzero(buffer,256);
			read(sockfd,buffer,255);
			printf("%s\n",buffer);

			if(strcmp(buffer,"Now you are hosting") == 0)
			{
				while(1)
				{
					bzero(buffer,256);
					read(sockfd,buffer,255);
					printf("%s\n",buffer);
				}					
			}

			bzero(buffer,256);
			fgets(buffer,255,stdin);
			write(sockfd,buffer,strlen(buffer));
			bzero(buffer,256);
			read(sockfd,buffer,255);
			printf("%s\n",buffer);
			bzero(buffer,256);
			fgets(buffer,255,stdin);
			write(sockfd,buffer,strlen(buffer));
		}
	}	
	else
	{
		write(sockfd,argv[1],strlen(argv[1]));
		bzero(buffer,256);
		read(sockfd,buffer,255);	

		if(strcmp(buffer, "OK") == 0)
		{
			while(1)
			{
				bzero(buffer,256);
				fgets(buffer,255,stdin);

				n = write(sockfd,buffer,strlen(buffer));

				bzero(buffer,256);
				n = read(sockfd,buffer,255);

				printf("%s\n",buffer);
			}
		}
	}

  return 0;
}
