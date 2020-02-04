#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <winsock2.h>
#include "cJSON.h"
#include "cJSON.c"

#define MAX 2047
#define PORT 12345
#define SA struct sockaddr

char buffer[MAX];
struct sockaddr_in servaddr;
int client_socket;
char *authtoken;
void menu_karbari();
void menu_asli(const char * user);
void menu_goftogoo(const char * chaname);
void connection();

void clear(){
    system("@cls||clear");
}

int main(int argc, char *argv[]) {
	int  server_socket;
	struct sockaddr_in cli;

	WORD wVersionRequested;
    WSADATA wsaData;
    int err;

	// Use the MAKEWORD(lowbyte, highbyte) macro declared in Windef.h
    wVersionRequested = MAKEWORD(2, 2);

    err = WSAStartup(wVersionRequested, &wsaData);
    if (err != 0) {
        // Tell the user that we could not find a usable Winsock DLL.
        printf("WSAStartup failed with error: %d\n", err);
        return 1;
    }

	// Create and verify socket
	client_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (client_socket == -1) {
		printf("Socket creation failed...\n");
		exit(0);
	}
	else
		printf("Socket successfully created..\n");

	// Assign IP and port
	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	servaddr.sin_port = htons(PORT);

	// Connect the client socket to server socket
	if (connect(client_socket, (SA*)&servaddr, sizeof(servaddr)) != 0) {
		printf("Connection to the server failed...\n");
		exit(0);
	}
	else
		printf("Successfully connected to the server..\n");
    clear();
	menu_karbari();

    closesocket(client_socket);

	return 0;
}
void menu_karbari(){
	int number;
	while (true){
		int number;
		char password[80], username[80];
		printf("\n________________________________\n");
		printf("1: Sign Up\n2: Login\n\n");
		scanf("%d",&number);
        client_socket = socket(AF_INET, SOCK_STREAM, 0);
        if (client_socket == -1) {
            printf("Socket creation failed...\n");
            exit(0);
        }
        memset(buffer, 0, sizeof(buffer));
		if(number==1){
            clear();
            getchar();
			printf("\nSign Up\n\nUsername :  ");
			gets(username);
			printf("Password :  ");
			gets(password);
			sprintf(buffer,"register %s, %s\n",username,password);
			connection();
            send(client_socket, buffer, sizeof(buffer), 0);
			memset(buffer, 0, sizeof(buffer));
			recv(client_socket, buffer, sizeof(buffer), 0);
			closesocket(client_socket);
			cJSON * recieved =cJSON_Parse(buffer);
			if(strcmp(cJSON_GetObjectItem(recieved,"type")->valuestring,"Error")==0){
                printf("\n%s\n\n",cJSON_GetObjectItem(recieved,"content")->valuestring);
			}
			else{
                clear();
                printf("\n... successful Sign Up with \"%s\"...\n",username);
			}

		}
		if(number ==2){
            clear();
            getchar();
			printf("\nLogin\n\nUsername :  ");
			gets(username);
			printf("Password :  ");
			gets(password);
			sprintf(buffer,"login %s, %s\n",username,password);
			connection();
			send(client_socket, buffer, sizeof(buffer), 0);

			memset(buffer, 0, sizeof(buffer));

			recv(client_socket, buffer, sizeof(buffer), 0);
            closesocket(client_socket);
			cJSON * recieved =cJSON_Parse(buffer);
			if(strcmp(cJSON_GetObjectItem(recieved,"type")->valuestring,"Error")!=0){
                authtoken=cJSON_GetObjectItem(recieved,"content")->valuestring;
                clear();
                const char * user = username;
                printf("\n... successful Login with \"%s\" ...\n",user);
                menu_asli(user);
			}
			else{
                printf("\n%s\n\n",cJSON_GetObjectItem(recieved,"content")->valuestring);
			}

		}

	}
}
void menu_asli(const char * user){
	int number;
	char channel_name[80];
	while(true){
        printf("\n________________________________\n");
		printf("1: Create Channel\n2: Join Channel\n3: Logout\n\n");
		scanf("%d",&number);
		client_socket = socket(AF_INET, SOCK_STREAM, 0);
        if (client_socket == -1) {
            printf("Socket creation failed...\n");
            exit(0);
        }
        memset(buffer, 0, sizeof(buffer));

		if(number==1){
            clear();
            getchar();
			printf("\nCreat a new channel\n\nChannel name :  ");
			gets(channel_name);
			sprintf(buffer,"create channel %s, %s\n",channel_name,authtoken);
			connection();
			send(client_socket, buffer, sizeof(buffer), 0);

			memset(buffer, 0, sizeof(buffer));

			recv(client_socket, buffer, sizeof(buffer), 0);
			closesocket(client_socket);

			cJSON * recieved =cJSON_Parse(buffer);
			if(strcmp(cJSON_GetObjectItem(recieved,"type")->valuestring,"Error")!=0){
                clear();
                const char *chaname = channel_name;
                printf("\n... You created and joined \"%s\" ...\n",channel_name);
                menu_goftogoo(chaname);
			}
			else{
                printf("\n%s\n\n",cJSON_GetObjectItem(recieved,"content")->valuestring);
			}

		}
		if(number==2){
            clear();
            getchar();
			printf("\nJoin a channel\n\nChannel name :  ");
			gets(channel_name);
			sprintf(buffer,"join channel %s, %s\n",channel_name,authtoken);
			connection();
			send(client_socket, buffer, sizeof(buffer), 0);
			memset(buffer, 0, sizeof(buffer));
			recv(client_socket, buffer, sizeof(buffer), 0);
			closesocket(client_socket);
			cJSON * recieved =cJSON_Parse(buffer);
			if(strcmp(cJSON_GetObjectItem(recieved,"type")->valuestring,"Error")!=0){
                clear();
                const char *chaname = channel_name;
                printf("\n... You joined \"%s\" ...\n",channel_name);
                menu_goftogoo(chaname);
			}
			else{
                printf("\n%s\n\n",cJSON_GetObjectItem(recieved,"content")->valuestring);
			}
		}
		if(number == 3){
		    clear();
			sprintf(buffer,"logout %s\n",authtoken);
			connection();
			send(client_socket, buffer , sizeof(buffer), 0);
			memset(buffer, 0, sizeof(buffer));
			recv(client_socket, buffer, sizeof(buffer), 0);
			closesocket(client_socket);
			cJSON * recieved =cJSON_Parse(buffer);
			if(strcmp(cJSON_GetObjectItem(recieved,"type")->valuestring,"Error")!=0){
                printf("\n... Successful Logout with \"%s\" ...\n",user);
                return ;
			}
			else{
                printf("\n%s\n\n",cJSON_GetObjectItem(recieved,"content")->valuestring);
			}

		}

	}
}

void menu_goftogoo(const char * chaname){
	int number;
	while(true){
        printf("\n________________________________\n");
		printf("1: Send Message\n2: Refresh\n3: Channel Members\n4: Leave Channel\n\n");
		scanf("%d",&number);
		memset(buffer, 0, sizeof(buffer));
		client_socket = socket(AF_INET, SOCK_STREAM, 0);
        if (client_socket == -1) {
            printf("Socket creation failed...\n");
            exit(0);
        }
		if(number==1){
            clear();
			printf("\n... Write your message to \"%s\" ...\n\n",chaname);
			char message[255];
			memset(message, 0, sizeof(message));
			getchar();
			gets(message);
			sprintf(buffer,"send %s, %s\n",message,authtoken);
			connection();
			send(client_socket, buffer, sizeof(buffer), 0);
			memset(buffer, 0, sizeof(buffer));
			recv(client_socket, buffer, sizeof(buffer), 0);
			cJSON * recieved =cJSON_Parse(buffer);
			closesocket(client_socket);
            if(strcmp(cJSON_GetObjectItem(recieved,"type")->valuestring,"Error")!=0){
                clear();
                printf("\n... Successful send to \"%s\" ...\n",chaname);
			}
			else{
                printf("\n%s\n\n",cJSON_GetObjectItem(recieved,"content")->valuestring);
			}
		}

		if(number==2){
            clear();
			sprintf(buffer,"refresh %s\n",authtoken);
			connection();
			send(client_socket, buffer, sizeof(buffer), 0);
			memset(buffer, 0, sizeof(buffer));
			recv(client_socket, buffer, sizeof(buffer), 0);
			closesocket(client_socket);
			cJSON * recieved = cJSON_Parse( buffer );
			if(strcmp(cJSON_GetObjectItem(recieved,"type")->valuestring,"Error")!=0){
                cJSON * allarray = cJSON_GetObjectItem(recieved, "content");
                int arraylen = cJSON_GetArraySize(allarray);
                printf("... Messages in \"%s\" ...\n\n",chaname);
                for(int i=0;i<arraylen;i++){
                    cJSON * a= cJSON_GetArrayItem(allarray,i);
                    printf("from: %s\nmessage: %s\n\n",cJSON_GetObjectItem(a,"sender")->valuestring,cJSON_GetObjectItem(a,"content")->valuestring);
                }
			}
			else{
                printf("\n%s\n\n",cJSON_GetObjectItem(recieved,"content")->valuestring);
			}
		}

		if(number==3){
            clear();
			sprintf(buffer,"channel members %s\n",authtoken);
			connection();
			send(client_socket, buffer, sizeof(buffer), 0);
			memset(buffer, 0, sizeof(buffer));
			recv(client_socket, buffer, sizeof(buffer), 0);
			closesocket(client_socket);
			cJSON * recieved =cJSON_Parse(buffer);
			if(strcmp(cJSON_GetObjectItem(recieved,"type")->valuestring,"Error")!=0){
                cJSON * allarray = cJSON_GetObjectItem(recieved, "content");
                int arraylen = cJSON_GetArraySize(allarray);
                printf("... members in \"%s\" ...\n\n",chaname);
                for(int i=0;i<arraylen;i++){
                    printf("member %d: %s\n",i+1,cJSON_GetArrayItem(allarray,i)->valuestring);
                }
			}

			else{
                printf("%s\n\n",cJSON_GetObjectItem(recieved,"content")->valuestring);
			}


		}
		if(number==4){
            clear();
			sprintf(buffer,"leave %s\n",authtoken);
			connection();
			send(client_socket, buffer, sizeof(buffer), 0);
			memset(buffer, 0, sizeof(buffer));
			recv(client_socket, buffer, sizeof(buffer), 0);
			closesocket(client_socket);
			cJSON * recieved =cJSON_Parse(buffer);
			if(strcmp(cJSON_GetObjectItem(recieved,"type")->valuestring,"Error")!=0){
                printf("\n... successful leave \"%s\" ...\n",chaname);
                return;
			}
			else{
                printf("%s\n\n",cJSON_GetObjectItem(recieved,"content")->valuestring);
			}
		}

	}
}
void connection(){
    if (connect(client_socket, (SA*)&servaddr, sizeof(servaddr)) != 0) {
                printf("Connection to the server failed...\n");
                exit(0);
			}
}

