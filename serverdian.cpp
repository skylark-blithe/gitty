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
static char * strbuf(char* buf,char* str);
void replyy(char* method,char* version,int cseq,int port,char* sBuf);

int main(int argc,char *argv[])
{
	int sockfd,newfd;
	struct sockaddr_in server_addr;
	struct sockaddr_in client_addr;
	int sin_size,portnumber;
	int opt=1;
	char method[10];
	char url[100];
	char version[10];
	char str[100];
	char rBuf[10000];
	char sBuf[10000];
	char* pbuf;
	int cseq;
	
	//create a socket
	 if((sockfd=socket(AF_INET,SOCK_STREAM,0))==-1)
	  {
	  	perror("socket error: \n");
	  	exit(1);
	  }
	  
	//initializing  
	  portnumber=atoi(argv[2]);
	  bzero(&server_addr,sizeof(struct sockaddr_in));
	  server_addr.sin_family=AF_INET;
	  server_addr.sin_addr.s_addr=htonl(INADDR_ANY);
	  server_addr.sin_port=htons(portnumber);
      setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));
	  
	//bind the socket to port
	   if(bind(sockfd,(struct sockaddr * )(&server_addr),sizeof(struct sockaddr))==-1)
	    {
	    	perror("bind error: \n");
	    	exit(1);
		}
	
	//turn socket into listen mode	
		if(listen(sockfd,10)==-1)
		 {
		 	perror("listen error: \n");
		 	exit(1);
		 }
		 
	    printf("diantp://%s:%d\n",argv[1],portnumber);
		 
		  while(1)
		   {
		   	sin_size=sizeof(struct sockaddr_in);
		   	//accept client socket
		   	 if((newfd=accept(sockfd,(struct sockaddr * )(&client_addr),(socklen_t *)(&sin_size)))==-1)
				{
				    perror("accept error: \n");
		   	  	    exit(1);	
				 } 	 
				 
			 printf("accept client; client ip:%s, client port:%d\n",inet_ntoa(client_addr.sin_addr),client_addr.sin_port);	 
			
			 	
				
			while(1)
			 {
			 	recv(newfd,rBuf,10000,0);
				rBuf[strlen(rBuf)]='\0';
				printf("client to server:\n");
				printf("%s\n",rBuf);
				
			//parse method	
				pbuf=strbuf(rBuf,str);
				sscanf(str,"%s %s %s \r\n",method,url,version);

			//parse cseq	 
				pbuf=strbuf(pbuf,str);
				sscanf(str,"CSeq: %d\r\n",&cseq);

                replyy(method,version,cseq,client_addr.sin_port,sBuf);
				send(newfd,sBuf,strlen(sBuf),0);
				bzero(&sBuf,sizeof(sBuf));  
				bzero(&method,sizeof(method)); 
				bzero(&url,sizeof(url)); 
				bzero(&version,sizeof(version)); 		  
		    }
		   }
		   close(newfd);
		   close(sockfd);
		   exit(0);
}

static char * strbuf(char* buf,char* str)
{
    while(*buf!='\n')
     {
        *str=*buf;
        str++;
        buf++;
     }
    *str='\n';
    str++;
    *str='\0';
    buf++;
    return buf; 
}

void replyy(char* method,char* version,int cseq,int port,char* sBuf)
{
	if(!strcmp(method,"OPTIONS"))
        {
	        sprintf(sBuf,"Version:%s\\r\\n\n"
                         "CSeq: %d\\r\\n\n"
                         "Public: OPTIONS, SETUP, PLAY, TEARDOWN\\r\\n\n"
                         "\\r\\n",
                         version,
                         cseq);
            
        }
    if(!strcmp(method,"SETUP"))
        {
            sprintf(sBuf,"Version:%s\\r\\n\n"
                         "CSeq: %d\\r\\n\n"
                         "Transport: TCP/UDP, client port:%d\\r\\n\n"
                         "\\r\\n",
                         version,
                         cseq,
                         port);
                     }
	if(!strcmp(method,"PLAY"))
        {
	        sprintf(sBuf,"Version:%s\\r\\n\n"
                         "CSeq: %d\\r\\n\n"
                         "Range: npt=0.000-\\r\\n\n"
                         "Session id: 5201314; timeout=60\\r\\n\n"
	            		 "\\r\\n",
						 version,
                         cseq);
         }
    if(!strcmp(method,"TEARDOWN"))
        {
            sprintf(sBuf,"Version:%s\\r\\n\n"
                         "CSeq: %d\\r\\n\n"
                         "Session id: 5201314; timeout=60\\r\\n\n"
	    				 "\\r\\n",
				    	 version,
                         cseq); 		               
        }
}
