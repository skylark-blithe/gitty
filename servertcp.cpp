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
int main(int argc,char *argv[])
{
	int sockfd,new_fd;
	struct sockaddr_in server_addr;
	struct sockaddr_in client_addr;
	int sin_size,portnumber;
	const char hello[]="Hello\n";
	if(argc!=2)
	{
		fprintf(stderr,"Usage: %s portnumber\a\n",argv[0]);
		exit(1);
	}
	if((portnumber=atoi(argv[1]))<0)
	 {
	 	fprintf(stderr,"Usage: %s portnumber\a\n",argv[0]);
		exit(1);
	 }
	 if((sockfd=socket(AF_INET,SOCK_STREAM,0))==-1)
	  {
	  	fprintf(stderr,"Socket error: %s portnumber\a\n",strerror(errno));
	  	exit(1);
	  }
	  bzero(&server_addr,sizeof(struct sockaddr_in));
	  server_addr.sin_family=AF_INET;
	  server_addr.sin_addr.s_addr=htonl(INADDR_ANY);
	  server_addr.sin_port=htons(portnumber);
       int opt=1;
       setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));
	   if(bind(sockfd,(struct sockaddr * )(&server_addr),sizeof(struct sockaddr))==-1)
	    {
	    	fprintf(stderr,"Bind error: %s\a\n",strerror(errno));
	    	exit(1);
		}
		if(listen(sockfd,5)==-1)
		 {
		 	fprintf(stderr,"Listen error:%s\a\n",strerror(errno));
		 	exit(1);
		 }
		  while(1)
		   {
		   	sin_size=sizeof(struct sockaddr_in);
		   	 if((new_fd=accept(sockfd,(struct sockaddr * )(&client_addr),(socklen_t *)(&sin_size)))==-1)
		   	  {
		   	  	fprintf(stderr,"Accept error: %s\n\a",strerror(errno));
		   	  	exit(1);
				 }
				fprintf(stderr,"Server get connection from %s\n",inet_ntoa(client_addr.sin_addr));
				 if(write(new_fd,hello,strlen(hello))==-1)
				  {
				  	fprintf(stderr,"Write error: %s\n",strerror(errno));
				  	exit(1);
				   } 
				close(new_fd);  
		   }
		   close(sockfd);
		   exit(0);
}
