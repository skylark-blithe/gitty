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
int main(int argc,char * argv[])
{
	int sockfd;
	char rBuf[10000],sBuf[10000];
	struct sockaddr_in server_addr;
	struct hostent * host;
	int portnumber,nbytes;
	int i;
	   portnumber=atoi(argv[2]);
	   if((host=gethostbyname(argv[1]))!=NULL)
	    {
	    	server_addr.sin_family=AF_INET;
	    	server_addr.sin_port=htons(portnumber);
	    	server_addr.sin_addr=*((struct in_addr * )host->h_addr);
		}
		sockfd=socket(AF_INET,SOCK_STREAM,0);
		 if(connect(sockfd,(struct sockaddr * )(&server_addr),sizeof(struct sockaddr))==-1)
		  {
		  	perror("connect error: \n");
		  	exit(1);
		  }
		  while(1)
		   {
		   	  for(i=0;;i++)
			   {
			   	sBuf[i]=getchar();
			   	if(sBuf[i]=='\n' && sBuf[i-1]=='n' && sBuf[i-2]=='\\' && sBuf[i-3]=='r' && sBuf[i-4]=='\\' && sBuf[i-5]=='\n')
			   	 {
			   	 	goto here;
					}
					 }	
			here: 
			    sBuf[i]='\0';	   
		   	    send(sockfd,sBuf,strlen(sBuf),0);
		        recv(sockfd,rBuf,10000,0);
		          rBuf[strlen(rBuf)]='\0';
		          printf("server to client:\n");
		   	      printf("%s\n",rBuf);
		   	      bzero(&sBuf,sizeof(sBuf)); 
		   	      bzero(&rBuf,sizeof(rBuf));
		   }
		   close(sockfd);
		   exit(0);
}
