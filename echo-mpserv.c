#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <sys/socket.h>
const int BUF_SIZE = 30;
void error_message(char *message);
void read_childproc(int sig);
int main(int argc, char *argv[])
{
     int serv_sock,clnt_sock;
     struct sockaddr_in serv_addr,clnt_addr;
     socklen_t clnt_sz;

     pid_t pid;
     struct sigaction act;
     int str_len,status;
     char buff[BUF_SIZE];

     if(argc != 2)
     {
         printf("Usage : %s <port>\n",argv[0]);
	 exit(1);
     }
     act.sa_handler = read_childproc;
     act.sa_flags = 0;
     sigemptyset(&act.sa_mask);
     status = sigaction(SIGCHLD,&act,0);
     if( (serv_sock = socket(PF_INET,SOCK_STREAM,0)) == -1)
     	 error_message("socket() error");
     memset(&serv_addr,0,sizeof(serv_addr));
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
     serv_addr.sin_port = htons(atoi(argv[1]));

     if(bind(serv_sock,(struct sockaddr*)&serv_addr,sizeof(serv_addr)) == -1)
         error_message("bind() error");
     if(listen(serv_sock,5) == -1)
     	error_message("listen() error");
     while(1)
     {
        clnt_sz = sizeof(clnt_addr);
        clnt_sock = accept(serv_sock,(struct sockaddr*)&clnt_addr,&clnt_sz);
	if(clnt_sock == -1)
	    continue;
	else
	    puts("new client connected...");
	pid = fork();
	if(pid == -1)
	{
	   close(clnt_sock);
	   continue;
	}
	if(pid == 0) //如果是子进程
	{
	     close(serv_sock);
	     while((str_len = read(clnt_sock,buff,BUF_SIZE)) !=0)
	     	write(clnt_sock,buff,str_len);
	   
	   close(clnt_sock);
	   puts("client disconnected...");
	   return 0;
	}
	else
	    close(clnt_sock);
    }

    close(serv_sock);
    return 0;
}
void error_message(char *message)
{
    fputs(message,stderr);
    fputc('\n',stderr);
    exit(1);
}
void read_childproc(int sig)
{
    pid_t pid;
    int status;
    pid = waitpid(-1,&status,WNOHANG);
    printf("remove proc id: %d \n",pid);
}
	
























