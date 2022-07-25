#include <sys/socket.h>
#include <sys/types.h>

#include <arpa/inet.h>
#include <netdb.h>

#include <errno.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define maxLineSize 1000

int	InitConnection(int *fd, char *server, char *port);
int	findSTR(char *str,char *target,int tlen);

/*
 * An simple IRC bot that says "Hello" to people when
 * they say "Hi" in a specific chat room
*/
int main(int argc, char *argv[]){
	struct addrinfo hints,*res;
	int fd=0,error=0;
	bool lstatus=false;
	char c,buff[maxLineSize];

	if(argc != 3 || strlen(argv[1]) > 60){ printf("Usage: hibot <irc-server> <irc-room>\n"); return EXIT_FAILURE; }

	if(InitConnection(&fd,argv[1],"6667") != -1) printf("CONNECTED!\n");
	else perror("ERROR: ");

	char login[]="NICK hibot\nUSER hibot 0 * :realname\n";
	write(fd,login,sizeof(login));

	char join[70],hello[100],welcome[]="End of message of the day";
	sprintf(join,"JOIN #%s\n",argv[2]);
	sprintf(hello,"PRIVMSG #%s :Hello!!!\n",argv[2]);
	
	int i=0;
	while(read(fd,&c,sizeof(char))){
		putchar(c);
		buff[i++]=c;
		if(c == '\n'){
			if(findSTR(buff,welcome,strlen(welcome)) && lstatus == false){
				lstatus = true;
				printf("LOGGED IN!!\n");
				write(fd,join,sizeof(join));
			}
			if(findSTR(buff,":Hi",strlen(":Hi"))) write(fd,hello,strlen(hello));
			buff[0]='\0';
			i=0;
		}
	}
}

/*
 * Connect to the server
*/
int InitConnection(int *fd,char *ip, char *port){
	struct addrinfo hints,*res;
	int err;
	
	memset(&hints,0,sizeof(hints));
	hints.ai_family=AF_INET;
	hints.ai_socktype=SOCK_STREAM;
	hints.ai_flags=AI_PASSIVE;

	getaddrinfo(ip,"6667",&hints,&res);
	*fd = socket(res->ai_family,res->ai_socktype,res->ai_protocol);
	err = connect(*fd,res->ai_addr,res->ai_addrlen);

	return err;
}

/*
 * Look for the string "target" in "str"
 * if it was found return 1, if not return 0
*/
int findSTR(char *str,char *target,int tlen){
	int i=0,j=0;

	while(str[i] != '\0'){
		if(str[i] == target[j]){
			while(target[j] != '\0' && target[j] == str[i]){i++;j++;}
			if(j == tlen) return 1;
		}else i++;
		j=0;
	}
	return 0;
}
