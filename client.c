#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#define PORT        8188		/* port number as an integer */
#define IP_ADDRESS "127.0.0.1"	/* IP address as a string */

#define BUFSIZE 8196

pexit(char * msg)
{
	perror(msg);
	exit(1);
}

main()
{
    int i,sockfd;
    char buffer[BUFSIZE];
    static struct sockaddr_in serv_addr;
		
	printf("client trying to connect to %s and port %d\n",IP_ADDRESS,PORT);	
	while(1){
		if((sockfd = socket(AF_INET, SOCK_STREAM,0)) <0) 
			pexit("socket() failed");

		serv_addr.sin_family = AF_INET;
		serv_addr.sin_addr.s_addr = inet_addr(IP_ADDRESS);
		serv_addr.sin_port = htons(PORT);
		if(connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) <0)
			pexit("connect() failed");
			
		char cmd[BUFSIZE];
		gets(cmd);

		/* now the sockfd can be used to communicate to the server */
		
		send(sockfd, cmd, strlen(cmd)+1, 0);
		/* note second space is a delimiter and important */

		/* this displays the raw HTML file as received by the browser */
		while( (i=read(sockfd,buffer,BUFSIZE)) > 0)
			write(1,buffer,i);
		close(sockfd);
	}
}
