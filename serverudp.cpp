#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<string.h>
#include<netdb.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<unistd.h>
#include<arpa/inet.h>
#define MAXLINE 80
#define SERV_PORT 8888
void do_echo(int sockfd,struct sockaddr * pcliaddr,socklen_t clilen)
{
	int n;
	socklen_t len;
	char mesg[80];
	for(;;)
	 {
	 	len=clilen;
	 	n=recvfrom(sockfd,mesg,80,0,pcliaddr,&len);
	 	sendto(sockfd,mesg,n,0,pcliaddr,len);
	 	mesg[n]=0;
	 	fputs(mesg,stdout);
	 }
}
int main(void)
{
	int sockfd;
	struct sockaddr_in servaddr, cliaddr;
	sockfd=socket(AF_INET,SOCK_DGRAM,0);
	bzero(&servaddr,sizeof(servaddr));
	servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
	servaddr.sin_port=htons(8888);
	 if(bind(sockfd,(struct sockaddr * )(&servaddr),sizeof(servaddr))==-1)
	  {
	  	perror("bind error");
	  	exit(1);
	  }
	  do_echo(sockfd,(struct sockaddr * )(&cliaddr),sizeof(cliaddr));
	  return 0;
}
