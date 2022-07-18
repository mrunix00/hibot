#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <arpa/inet.h>

#include <unistd.h>

#define maxBufferSize 1000
#define maxWordSize 100

int InitConnection(int *fd, char *server, char *port);
int divideSTR(char *str,int strlen,char **dest);
int findSTR(char **str,int len,char *target);

int main(){
	struct addrinfo hints,*res;
	int fd,error;

	if((error=InitConnection(&fd,NULL,"6667"))>=0)
		printf("CONNECTED!\n");
	else{
		printf("ERROR: %d\n",error);
		return EXIT_FAILURE;
	}

	char login[]="NICK hibot0\nUSER hibot0 0 * :realname\n";
	write(fd,login,sizeof(login));

	char c,buff[maxBufferSize][maxWordSize];

	int i=0,j=0;
	while(read(fd,&c,sizeof(c))){
	
		if(c == '\n'){
			buff[i][j]='\0';
//			printf("%s\n",buff[i]);
			
			char **temp=malloc(maxBufferSize*sizeof(char));
			for(int l=0;l<maxBufferSize;l++) temp[l]=malloc(maxWordSize*sizeof(char));

			int tmpINT=divideSTR(buff[i],strlen(buff[i]),temp);

			if(findSTR(temp,tmpINT,":Hi") != -1){
				char hi[]="PRIVMSG #lol :Hello!\n";
				write(fd,hi,sizeof(hi));
			}

			free(temp);
			for(int l=0;l<maxBufferSize;l++) free(temp[l]);

			i++;
			j=0;
		}else{
			
			if((strcmp(buff[i],":unknown.local 376 hibot0 :End of message of the day.")) == 0){
				printf("LOGGED IN!!\n");
				char join[]="JOIN #lol\n";
				write(fd,join,sizeof(join));
			}

			buff[i][j++]=c;
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

int divideSTR(char *str,int strlen ,char **dest){
	int j=0,k=0;

	for(int i=0; i <= strlen; i++){
		 if(str[i] == ' '){
			dest[j++][k]='\0';
			k=0;
		 }else{
			dest[j][k++]=str[i];
		 }
	}

	return j;
}

int findSTR(char **str,int len,char *target){
	for(int g=0;g < len;g++){
		if(strcmp(str[g],target) == 0){
			return g;
		}
	}

	return -1;
}
