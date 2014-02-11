#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <unistd.h>
#include <strings.h>
#include <arpa/inet.h>

int main( int argc, char *argv[] )
{
	int sockfd, newsockfd, portno, clilen;
  char buffer[256];
  struct sockaddr_in serv_addr, cli_addr;
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
  if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
  {
		perror("ERROR on binding");
   	exit(1);
  }

	/* start listen */
	listen(sockfd,5);
  clilen = sizeof(cli_addr);

  newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, (socklen_t*) &clilen);

	bzero(buffer,256);
  n = read( newsockfd,buffer,255 );

	printf("Here is the message: %s\n",buffer);

	n = write(newsockfd,"I got your message",18);

	return 0;

}
