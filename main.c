#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <arpa/inet.h>

#define maxLineSize 1000

int InitConnection(int *fd, char *server, char *port);
int findSTR(char *str,char *target,int tlen);

int main(int argc, char *argv[]){
	struct addrinfo hints,*res;
	int fd=0,error=0;
	char c,buff[maxLineSize];

	if(argc != 3 || strlen(argv[1]) > 60){
		printf("Usage: hibot <irc-server> <irc-room>\n");
		return EXIT_FAILURE;
	}

	if((error=InitConnection(&fd,argv[1],"6667")) >= 0)
		printf("CONNECTED!\n");
	else{
		perror(strerror(error));
		return EXIT_FAILURE;
	}

	char login[]="NICK hibot\nUSER hibot 0 * :realname\n";
	write(fd,login,sizeof(login));

	char join[70],hello[100],welcome[]="End of message of the day.";
	sprintf(join,"JOIN #%s\n",argv[2]);
	sprintf(hello,"PRIVMSG #%s :Hello!!!\n",argv[2]);
	
	int i=0,j=0;
	while(read(fd,&c,sizeof(char)) != EOF){
//		putchar(c);
		buff[i]=c;
		i++;
		if(c == '\n'){
			if(findSTR(buff,welcome,strlen(welcome)) != -1){
				printf("LOGGED IN!!\n");
				write(fd,join,sizeof(join));
			}
			if(findSTR(buff,":Hi",strlen(":Hi")) != -1) write(fd,hello,strlen(hello));
			buff[0]='\0';
			i=0;
		}
	}
}

int InitConnection(int *fd,char *ip, char *port){
	struct addrinfo hints,*res;
	int err;
	
	memset(&hints,0,sizeof(hints));
	hints.ai_family=AF_INET;
	hints.ai_socktype=SOCK_STREAM;
	hints.ai_flags=AI_PASSIVE;

	getaddrinfo(NULL,"6667",&hints,&res);
	*fd = socket(res->ai_family,res->ai_socktype,res->ai_protocol);
	err=connect(*fd,res->ai_addr,res->ai_addrlen);

	return err;
}

int findSTR(char *str,char *target,int tlen){
	int i=0,j=0,k=0;

	while(str[i] != '\0'){
		if(str[i] == target[j]){
			k=i;
			while(target[j] != '\0' && target[j] == str[k]) {k++;j++;}
			if(j == tlen) return i;
		}
		i++;j=0;
	}
	return -1;
}
