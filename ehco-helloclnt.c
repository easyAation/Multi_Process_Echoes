#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
void error_messageing(const char *message);
int main(int argc, char *argv[])
{
   int i,clnt_sock,len,reslen,opetornum,res;
   struct sockaddr_in clnt_addr;
   socklen_t clnt_addr_SZ;
   char message[1024],flag;
   
   clnt_sock = socket(PF_INET,SOCK_STREAM,0);

   memset(&clnt_addr,0,sizeof(clnt_addr));
   clnt_addr.sin_family = AF_INET;
   clnt_addr.sin_addr.s_addr = inet_addr(argv[1]);
   clnt_addr.sin_port = htons(atoi(argv[2]));

   if(connect(clnt_sock,(struct sockaddr*)&clnt_addr,sizeof(clnt_addr)) == -1)
       error_messageing("connet() error");
   
   while(1)
   {
   	printf("请输入信息(Q to quit): \n");
	scanf(" %s",message);
	if(strlen(message)==1 && (message[0] == 'Q' || message[0] == 'q'))
	   break;
	write(clnt_sock,message,strlen(message));
	reslen = 0;
   	len = strlen(message);
	while(reslen < len)
	   reslen += read(clnt_sock,&message[reslen],1023);
	message[len] = 0;
	printf("Message from server: %s\n",message);
   }
   close(clnt_sock);
   return 0;
  
}

void error_messageing(const char *message)
{
    fputs(message,stderr);
    fputc('\n',stderr);
    exit(1);
}
