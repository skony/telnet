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

void execute(char buffer[], char command[])
{
	FILE *fpipe;
	bzero(buffer, 256);
	char line[16];
	strcpy(buffer, " ");

	if (0 == (fpipe = (FILE*)popen(command, "r")))
    {
      perror("popen() failed.");
      exit(1);
    }

	 while (fread(line, sizeof line, 1, fpipe))
    {
        printf("line: %s\n", line);
				strcat(buffer, line);
				printf("bufer: %s\n", buffer);
    }

	pclose(fpipe);
	
	printf("return %s\n", buffer);
}

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
				bzero(buffer,256);
				strcpy(buffer, "port");
				write(sockfd, buffer, 255);
				read(sockfd,buffer,255);
				int my_port = atoi(buffer); printf("portyyy %d\n", my_port);
				close(sockfd);
				struct sockaddr_in cli_addr, my_addr;
				my_addr.sin_family = AF_INET;
  			my_addr.sin_addr.s_addr = INADDR_ANY;
  			my_addr.sin_port = htons(my_port);
				sockfd = socket(AF_INET, SOCK_STREAM, 0);

				int* optval;
				setsockopt(sockfd, 1, SO_REUSEADDR, &optval,4);
				if (bind(sockfd, (struct sockaddr *) &my_addr, sizeof(my_addr)) < 0)
				{
					perror("ERROR on binding");
				 	exit(1);
				}

				int clilen, newsockfd;
				bzero(buffer,256);
				listen(sockfd,3);
				clilen = sizeof(cli_addr);
				char buffer2[256];

				while(1)
				{
					newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, (socklen_t*) &clilen);
					
					while(1)
					{
						read(newsockfd,buffer,255);
						int len = strlen(buffer);
						buffer[len - 1] = '\0';
						printf("%s\n",buffer);
						bzero(buffer2, 256);
						execute(buffer2, buffer);
						printf("loop: %s\n", buffer2);
						write(newsockfd,buffer2,255);	
					}
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
		else
		{
			printf("connecting error\n");
			return -1;
		}
	}

  return 0;
}
