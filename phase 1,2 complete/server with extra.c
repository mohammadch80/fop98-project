#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <dirent.h>
#include <time.h>

#include "cJSON.h"
#include "cJSON.c"

#define MAX 100000
#define PORT 12345
#define SA struct sockaddr

char buffer[MAX];
char useuser[80];
char usechannel[80];
char auth[30];
int delete_item;
int noseen_index;

int server_socket, client_socket;
struct sockaddr_in server, client;

void socketcreating();
void accepting();
void listening();
void binding();
void chat();

// Driver function
int main()
{

    WORD wVersionRequested;
    WSADATA wsaData;
    int err;

    // Use the MAKEWORD(lowbyte, highbyte) macro declared in Windef.h
    wVersionRequested = MAKEWORD(2, 2);

    err = WSAStartup(wVersionRequested, &wsaData);
    if (err != 0)
    {
        // Tell the user that we could not find a usable Winsock DLL.
        printf("WSAStartup failed with error: %d\n", err);
        return 1;
    }

    // Create and verify socket
    socketcreating();

    // Assign IP and port
    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    server.sin_port = htons(PORT);

    // Bind newly created socket to given IP and verify
    binding();

    // Now server is ready to listen and verify
    listening();

    // Accept the data packet from client and verify
    accepting();

    // Function for chatting between client and server
    chat();

    // Close the socket
    closesocket(server_socket);
}

void accepting()
{
    int len=sizeof(client);
    client_socket = accept(server_socket, (SA *)&client, &len);
    if (client_socket < 0)
    {
        printf("Server accceptance failed...\n");
        //exit(0);
    }
    else
        printf("Server acccepted the client..\n");
}
void listening()
{
    if ((listen(server_socket, 5)) != 0)
    {
        printf("Listen failed...\n");
        //exit(0);
    }
    else
        printf("Server listening..\n");
}
void binding()
{
    if ((bind(server_socket, (SA *)&server, sizeof(server))) != 0)
    {
        printf("Socket binding failed...\n");
        //exit(0);
    }
    else
        printf("Socket successfully bound..\n");
}
void socketcreating()
{
    server_socket = socket(AF_INET, SOCK_STREAM, 6);
    if (server_socket == INVALID_SOCKET)
        wprintf(L"socket function failed with error = %d\n", WSAGetLastError() );
    else
        printf("Socket successfully created..\n");
}

void chat()
{
    mkdir("resources");
    mkdir("resources\\users");
    mkdir("resources\\channels");
    //yekbar ejra shavad
    if(fopen("resources\\auth.json","r") == NULL)
    {
        FILE *a=fopen("resources\\auth.json","w");
        cJSON *str =cJSON_CreateObject(), *arr= cJSON_CreateArray();
        cJSON_AddItemToObject(str,"users",arr);
        fprintf(a,"%s",cJSON_PrintUnformatted(str));
        fclose(a);
    }

    while(true)
    {


        memset(buffer, 0, sizeof(buffer));
        accepting();

        recv(client_socket, buffer, sizeof(buffer), 0);

        char oper[20];
        sscanf(buffer,"%s",oper);

        if(strcmp(oper,"register")==0)
        {
            fun_register();
        }
        else if(strcmp(oper,"login")==0)
        {
            fun_login();
        }
        else if(strcmp(oper,"create")==0)
        {
            fun_createch();
        }
        else if(strcmp(oper,"join")==0)
        {
            fun_join();
        }
        else if(strcmp(oper,"send")==0)
        {
            fun_send();
        }
        else if(strcmp(oper,"refresh")==0)
        {
            fun_refresh();
        }
        else if(strcmp(oper,"channel")==0)
        {
            fun_members();
        }
        else if(strcmp(oper,"leave")==0)
        {
            fun_leave();
        }
        else if(strcmp(oper,"logout")==0)
        {
            fun_logout();
        }
        else if(strcmp(oper,"find")==0)
        {
            fun_find();
        }
        else if(strcmp(oper,"search")==0)
        {
            fun_search();
        }

        closesocket(server_socket);
        socketcreating();
        binding();
        listening();

        if (strncmp("exit", buffer, 4) == 0)
        {
            printf("Server stopping...\n");
            return;
        }
    }
}

void fun_register()
{
    char user[80],pass[80];

    int i=0,n=0;
    while(buffer[i]!=32)
    {
        i++;
    }
    for(int i=0; i<80 ; i++)
    {
        user[i]=0;
        pass[i]=0;
    }

    i++;
    for(n=0 ; buffer[n+i]!=44 ; n++)
    {
        user[n]=buffer[n+i];
    }
    i =(i+n+2);
    for(n=0; buffer[n+i]!='\n' ; n++)
    {
        pass[n]= buffer[n+i];
    }

    cJSON *str =cJSON_CreateObject();
    char user2[80];
    strcpy(user2,user);
    if(fun_tekrar(1,user2)==0)
    {
        cJSON_AddItemToObject(str,"type",cJSON_CreateString("Error"));
        cJSON_AddItemToObject(str,"content",cJSON_CreateString("The username has already exist"));
    }
    else
    {
        cJSON_AddItemToObject(str,"type",cJSON_CreateString("Successful"));
        cJSON_AddItemToObject(str,"content",cJSON_CreateString(""));
        cJSON *str2 =cJSON_CreateObject();
        cJSON_AddItemToObject(str2,"username",cJSON_CreateString(user));
        cJSON_AddItemToObject(str2,"password",cJSON_CreateString(pass));
        char address[100];
        sprintf(address,"resources\\users\\%s.user.json",user);
        FILE *a=fopen(address,"w");
        memset(buffer, 0, sizeof(buffer));
        strcpy(buffer,cJSON_PrintUnformatted(str2));
        printf("%s",buffer);
        fprintf(a,"%s",buffer);
        fclose(a);
    }

    memset(buffer, 0, sizeof(buffer));
    strcpy(buffer,cJSON_PrintUnformatted(str));

    send(client_socket, buffer, sizeof(buffer), 0);

}
void fun_login()
{
    char user[80],pass[80];

    int i=0,n=0;
    while(buffer[i]!=32)
    {
        i++;
    }
    for(int i=0; i<80 ; i++)
    {
        user[i]=0;
        pass[i]=0;
    }

    i++;
    for(n=0 ; buffer[n+i]!=44 ; n++)
    {
        user[n]=buffer[n+i];
    }
    i =(i+n+2);
    for(n=0; buffer[n+i]!='\n' ; n++)
    {
        pass[n]= buffer[n+i];
    }

    cJSON *str =cJSON_CreateObject();
    char user2[80];
    strcpy(user2,user);

    if(fun_tekrar(1,user2))
    {
        cJSON_AddItemToObject(str,"type",cJSON_CreateString("Error"));
        cJSON_AddItemToObject(str,"content",cJSON_CreateString("The username has not registered"));
    }
    else
    {
        char address[100];
        sprintf(address,"resources\\users\\%s.user.json",user);
        FILE *a =fopen(address,"r");
        char str2[MAX];
        int i=0;
        while(!feof(a))
        {
            str2[i++]= (char)fgetc(a);
        }
        //printf("%s\n",str2);
        cJSON * str1= cJSON_Parse(str2);
        fclose(a);

        // printf("\nhello\n");

        for(i=0; i<MAX; i++)
            str2[i]=0;

        a =fopen("resources\\auth.json","r");
        i=0;
        while(!feof(a))
        {
            str2[i++]= (char)fgetc(a);
        }
        //printf("%s\n",str2);
        cJSON * fileauth= cJSON_Parse(str2);
        fclose(a);


        if(fun_user(fileauth,user,1)==0)
        {
            cJSON_AddItemToObject(str,"type",cJSON_CreateString("Error"));
            cJSON_AddItemToObject(str,"content",cJSON_CreateString("The user is login yet"));
        }

        else if(strcmp(pass,cJSON_GetObjectItem(str1,"password")->valuestring))
        {
            cJSON_AddItemToObject(str,"type",cJSON_CreateString("Error"));
            cJSON_AddItemToObject(str,"content",cJSON_CreateString("The password is invalid"));
        }
        else
        {
            randauth();
            //printf("%s\n",auth);
            cJSON_AddItemToObject(str,"type",cJSON_CreateString("Authtoken"));
            cJSON_AddItemToObject(str,"content",cJSON_CreateString(auth));

            cJSON * allusers= cJSON_GetObjectItem(fileauth,"users");
            cJSON * newuser= cJSON_CreateObject();
            cJSON_AddItemToObject(newuser,"name",cJSON_CreateString(user));
            cJSON_AddItemToObject(newuser,"auth",cJSON_CreateString(auth));
            cJSON_AddItemToObject(newuser,"channel",cJSON_CreateString(""));
            cJSON_AddNumberToObject(newuser,"noseen",0);
            cJSON_AddItemToArray(allusers,newuser);

            FILE * fil =fopen("resources\\auth.json","w");
            memset(buffer, 0, sizeof(buffer));
            strcpy(buffer,cJSON_PrintUnformatted(fileauth));
            printf("%s\n",buffer);
            fprintf(fil,"%s",buffer);
            fclose(fil);
        }
    }
    memset(buffer, 0, sizeof(buffer));
    strcpy(buffer,cJSON_Print(str));
    send(client_socket, buffer, sizeof(buffer), 0);

}
void fun_logout()
{
    char auth2[MAX];
    char str2[MAX];
    int i=0;
    for(i=0; i<MAX; i++)
    {
        str2[i]=0;
        auth2[i]=0;
    }
    for(int n=0; buffer[n+7]!='\n' ; n++)
    {
        auth2[n]= buffer[n+7];
    }

    FILE *a=fopen("resources\\auth.json","r");
    i=0;
    while(!feof(a))
    {
        str2[i++]= (char)fgetc(a);
    }
    //printf("%s\n",str2);
    cJSON * fileauth= cJSON_Parse(str2);
    fclose(a);

    cJSON *str=cJSON_CreateObject();
    //printf("%s\n",auth2);
    if(fun_user(fileauth,auth2,2)==1)
    {
        cJSON_AddItemToObject(str,"type",cJSON_CreateString("Error"));
        cJSON_AddItemToObject(str,"content",cJSON_CreateString("Authentication error"));
    }
    else
    {
        cJSON_AddItemToObject(str,"type",cJSON_CreateString("Successful"));
        cJSON_AddItemToObject(str,"content",cJSON_CreateString(""));


        cJSON* allarray=cJSON_GetObjectItem(fileauth,"users");
        cJSON* newarray=cJSON_CreateArray();
        int sizea=cJSON_GetArraySize(allarray);
        printf("\nsize %d\n",sizea);
        printf("\ndelete  %d\n",delete_item);

        for(int i=0; i<sizea; i++)
        {
            if(i==delete_item)
            {
                printf("\n%d\n",i);
            }
            else{
                cJSON * onlineuser=cJSON_GetArrayItem(allarray,i);
                cJSON * newuser = cJSON_CreateObject();
                char name[80],auth[80],channel[80];
                int noseen = cJSON_GetObjectItem(onlineuser,"noseen")->valueint;
                memset(name,0,80);
                memset(auth, 0 ,80);
                memset(channel,0,80);
                strcpy(name,cJSON_GetObjectItem(onlineuser,"name")->valuestring);
                strcpy(auth,cJSON_GetObjectItem(onlineuser,"auth")->valuestring);
                strcpy(channel,cJSON_GetObjectItem(onlineuser,"channel")->valuestring);

                cJSON_AddItemToObject(newuser,"name",cJSON_CreateString(name));
                cJSON_AddItemToObject(newuser,"auth",cJSON_CreateString(auth));
                cJSON_AddItemToObject(newuser,"channel",cJSON_CreateString(channel));
                cJSON_AddNumberToObject(newuser,"noseen",noseen);
                cJSON_AddItemToArray(newarray,newuser);
            }
        }
        cJSON *newfileauth= cJSON_CreateObject();
        cJSON_AddItemToObject(newfileauth,"users",newarray);
        FILE *au =fopen("resources\\auth.json","w");
        memset(buffer, 0, sizeof(buffer));
        strcpy(buffer,cJSON_PrintUnformatted(newfileauth));
        printf("\nnew fileauth %s\n",buffer);
        fprintf(au,"%s",buffer);
        fclose(au);

    }
    memset(buffer, 0, sizeof(buffer));
    strcpy(buffer,cJSON_PrintUnformatted(str));
    send(client_socket, buffer, sizeof(buffer), 0);
}
void fun_leave()
{
    char auth2[MAX];
    char str2[MAX];
    memset(str2, 0, sizeof(str2));
    memset(auth2, 0, sizeof(auth2));

    for(int n=0; buffer[n+6]!='\n' ; n++)
    {
        auth2[n]= buffer[n+6];
    }

    FILE *a=fopen("resources\\auth.json","r");
    int i=0;
    while(!feof(a))
    {
        str2[i++]= (char)fgetc(a);
    }
    printf("\n fileauth %s\n",str2);
    cJSON * fileauth= cJSON_Parse(str2);
    fclose(a);

    cJSON *str=cJSON_CreateObject();
    printf("%s\n",auth2);
    if(fun_user(fileauth,auth2,2)==1)
    {
        cJSON_AddItemToObject(str,"type",cJSON_CreateString("Error"));
        cJSON_AddItemToObject(str,"content",cJSON_CreateString("Authentication error"));
    }
    else if(strcmp(usechannel,"")==0)
    {
        cJSON_AddItemToObject(str,"type",cJSON_CreateString("Error"));
        cJSON_AddItemToObject(str,"content",cJSON_CreateString("The user is not in any channel."));
    }
    else
    {
        cJSON_AddItemToObject(str,"type",cJSON_CreateString("Successful"));
        cJSON_AddItemToObject(str,"content",cJSON_CreateString(""));

        for(int i=0; i<MAX; i++)
        {
            str2[i]=0;
        }

        char address[MAX];
        memset(address,0,sizeof(address));

        printf("\n usechannel %s \n",usechannel);

        sprintf(address,"resources\\channels\\%s.channel.json",usechannel);

        FILE *a=fopen(address,"r");
        i=0;
        while(!feof(a))
        {
            str2[i++]= (char)fgetc(a);
        }
        cJSON * channelinfo = cJSON_Parse(str2);

        fclose(a);
        cJSON * messages=cJSON_GetObjectItem(channelinfo,"messages");
        cJSON * members = cJSON_GetObjectItem(channelinfo,"members");
        printf("\n members %s \n",cJSON_PrintUnformatted(members));
        cJSON * newmessage=cJSON_CreateObject();
        cJSON_AddItemToObject(newmessage,"sender",cJSON_CreateString("server"));
        char messa[100];
        memset(messa,0,100);
        sprintf(messa,"%s leaved %s.",useuser,usechannel);
        cJSON_AddItemToObject(newmessage,"content",cJSON_CreateString(messa));
        cJSON_AddItemToArray(messages,newmessage);

        int sizea = cJSON_GetArraySize(members);
        for(int i=0 ; i<sizea ; i++)
        {
            if(strcmp( cJSON_GetArrayItem(members,i)->valuestring,useuser)==0)
            {
                cJSON_DeleteItemFromArray(members,i);
                break;
            }
        }

        memset(buffer, 0, sizeof(buffer));
        strcpy(buffer,cJSON_PrintUnformatted(channelinfo));

        FILE *newfi = fopen(address,"w");
        printf("\n%s\n",buffer);
        fprintf(newfi,"%s",buffer);
        fclose(newfi);

        cJSON * allusers = cJSON_GetObjectItem(fileauth,"users");
        cJSON * mainuser = cJSON_GetArrayItem(allusers,delete_item);
        cJSON_DeleteItemFromObject(mainuser,"channel");
        cJSON_DeleteItemFromObject(mainuser,"noseen");
        cJSON_AddNumberToObject(mainuser,"noseen",0);
        cJSON_AddItemToObject(mainuser,"channel",cJSON_CreateString(""));


        newfi=fopen("resources\\auth.json","w");
        memset(buffer, 0, sizeof(buffer));
        strcpy(buffer,cJSON_PrintUnformatted(fileauth));
        fprintf(newfi,"%s",buffer);
        fclose(newfi);

    }
    memset(buffer, 0, sizeof(buffer));
    strcpy(buffer,cJSON_PrintUnformatted(str));
    send(client_socket, buffer, sizeof(buffer), 0);


}
void fun_join()
{
    char cha_name[80],auth2[80];
    for(int i=0; i<80 ; i++)
    {
        cha_name[i]=0;
        auth2[i]=0;
    }

    int i=0,n=0;
    for(n=0 ; buffer[n+13]!=44 ; n++)
    {
        cha_name[n]=buffer[n+13];
    }
    i =(n+15);
    for(n=0; buffer[n+i]!='\n' ; n++)
    {
        auth2[n]= buffer[n+i];
    }

    char str2[MAX];
    FILE *a=fopen("resources\\auth.json","r");
    i=0;
    while(!feof(a))
    {
        str2[i++]= (char)fgetc(a);
    }
    //printf("%s\n",str2);
    cJSON * fileauth= cJSON_Parse(str2);
    fclose(a);

    cJSON *str = cJSON_CreateObject();
    char cha_name2[80];
    strcpy(cha_name2,cha_name);
    if(fun_user(fileauth,auth2,2))
    {
        cJSON_AddItemToObject(str,"type",cJSON_CreateString("Error"));
        cJSON_AddItemToObject(str,"content",cJSON_CreateString("Authentication error"));
    }
    else if(fun_tekrar(2,cha_name2))
    {
        cJSON_AddItemToObject(str,"type",cJSON_CreateString("Error"));
        cJSON_AddItemToObject(str,"content",cJSON_CreateString("The channel doesnt exist"));
    }
    else
    {
        cJSON_AddItemToObject(str,"type",cJSON_CreateString("Successful"));
        cJSON_AddItemToObject(str,"content",cJSON_CreateString(""));
        char address[MAX];
        memset(address,0,sizeof(address));
        sprintf(address,"resources\\channels\\%s.channel.json",cha_name);

        for(int i=0; i<MAX; i++)
        {
            str2[i]=0;
        }

        FILE *a=fopen(address,"r");

        i=0;
        while(!feof(a))
        {
            str2[i++]= (char)fgetc(a);
        }
        cJSON * channelinfo= cJSON_Parse(str2);
        printf("\n%s\n",str2);
        fclose(a);
        cJSON * messages=cJSON_GetObjectItem(channelinfo,"messages");
        cJSON * members = cJSON_GetObjectItem(channelinfo,"members");
        cJSON * newmessage=cJSON_CreateObject();
        cJSON_AddItemToObject(newmessage,"sender",cJSON_CreateString("server"));
        char messa[100];
        memset(messa,0,100);
        sprintf(messa,"%s joined %s.",useuser,cha_name);
        printf("\n messa %s joined %s.\n",useuser,cha_name);
        cJSON_AddItemToObject(newmessage,"content",cJSON_CreateString(messa));
        cJSON_AddItemToArray(messages,newmessage);

        cJSON_AddItemToArray(members,cJSON_CreateString(useuser));

        memset(buffer, 0, sizeof(buffer));
        strcpy(buffer,cJSON_PrintUnformatted(channelinfo));

        FILE *newfi=fopen(address,"w");
        printf("\n%s\n",buffer);
        fprintf(newfi,"%s",buffer);
        fclose(newfi);

        cJSON * allusers = cJSON_GetObjectItem(fileauth,"users");
        cJSON * mainuser = cJSON_GetArrayItem(allusers,delete_item);
        cJSON_DeleteItemFromObject(mainuser,"channel");
        cJSON_DeleteItemFromObject(mainuser,"noseen");
        cJSON_AddItemToObject(mainuser,"channel",cJSON_CreateString(cha_name));
        cJSON_AddNumberToObject(mainuser,"noseen",0);

        newfi=fopen("resources\\auth.json","w");
        memset(buffer, 0, sizeof(buffer));
        strcpy(buffer,cJSON_PrintUnformatted(fileauth));
        fprintf(newfi,"%s",buffer);
        fclose(newfi);

    }

    memset(buffer, 0, sizeof(buffer));
    strcpy(buffer,cJSON_PrintUnformatted(str));
    send(client_socket, buffer, sizeof(buffer), 0);
}
void fun_createch()
{
    char cha_name[80],auth2[80];
    for(int i=0; i<80 ; i++)
    {
        cha_name[i]=0;
        auth2[i]=0;
    }

    int i=0,n=0;
    for(n=0 ; buffer[n+15]!=44 ; n++)
    {
        cha_name[n]=buffer[n+15];
    }
    i =(n+17);
    for(n=0; buffer[n+i]!='\n' ; n++)
    {
        auth2[n]= buffer[n+i];
    }
    char str2[MAX];
    FILE *a=fopen("resources\\auth.json","r");
    i=0;
    while(!feof(a))
    {
        str2[i++]= (char)fgetc(a);
    }
    //printf("%s\n",str2);
    cJSON * fileauth= cJSON_Parse(str2);
    fclose(a);

    cJSON *str=cJSON_CreateObject();
    char cha_name2[80];
    strcpy(cha_name2,cha_name);

    if(fun_user(fileauth,auth2,2))
    {
        cJSON_AddItemToObject(str,"type",cJSON_CreateString("Error"));
        cJSON_AddItemToObject(str,"content",cJSON_CreateString("Authentication error"));
    }
    else if(fun_tekrar(2,cha_name2)==0)
    {
        cJSON_AddItemToObject(str,"type",cJSON_CreateString("Error"));
        cJSON_AddItemToObject(str,"content",cJSON_CreateString("The channel name is not available."));
    }
    else
    {
        cJSON_AddItemToObject(str,"type",cJSON_CreateString("Successful"));
        cJSON_AddItemToObject(str,"content",cJSON_CreateString(""));
        char address[MAX];
        sprintf(address,"resources\\channels\\%s.channel.json",cha_name);

        cJSON * messages=cJSON_CreateArray();
        cJSON * newmessage=cJSON_CreateObject();
        cJSON_AddItemToObject(newmessage,"sender",cJSON_CreateString("server"));
        char messa[100];
        sprintf(messa,"%s create and joined %s.",useuser,cha_name);
        cJSON_AddItemToObject(newmessage,"content",cJSON_CreateString(messa));
        cJSON_AddItemToArray(messages,newmessage);

        cJSON * newchannelinfo=cJSON_CreateObject();
        cJSON * members = cJSON_CreateArray();
        cJSON_AddItemToArray(members,cJSON_CreateString(useuser));

        cJSON_AddItemToObject(newchannelinfo,"messages",messages);
        cJSON_AddItemToObject(newchannelinfo,"members",members);

        a=fopen(address,"w");
        memset(buffer, 0, sizeof(buffer));
        strcpy(buffer,cJSON_PrintUnformatted(newchannelinfo));
        fprintf(a,"%s",buffer);
        fclose(a);

        cJSON * allusers = cJSON_GetObjectItem(fileauth,"users");
        cJSON * mainuser = cJSON_GetArrayItem(allusers,delete_item);
        cJSON_DeleteItemFromObject(mainuser,"channel");
        cJSON_DeleteItemFromObject(mainuser,"noseen");
        cJSON_AddItemToObject(mainuser,"channel",cJSON_CreateString(cha_name));
        cJSON_AddNumberToObject(mainuser,"noseen",0);


        a=fopen("resources\\auth.json","w");
        memset(buffer, 0, sizeof(buffer));
        strcpy(buffer,cJSON_PrintUnformatted(fileauth));
        fprintf(a,"%s",buffer);
        fclose(a);

    }

    memset(buffer, 0, sizeof(buffer));
    strcpy(buffer,cJSON_PrintUnformatted(str));
    send(client_socket, buffer, sizeof(buffer), 0);

}
void fun_send()
{

    char smessage[80],auth2[80];
    for(int i=0; i<80 ; i++)
    {
        smessage[i]=0;
        auth2[i]=0;
    }

    int i=0,n=0;
    for(n=0 ; buffer[n+5]!=44 ; n++)
    {
        smessage[n]=buffer[n+5];
    }
    i =(n+7);
    printf("\n message %s\n",smessage);
    for(n=0; buffer[n+i]!='\n' ; n++)
    {
        auth2[n]= buffer[n+i];
    }
    char str2[MAX];
    memset(str2,0,MAX);

    FILE *a=fopen("resources\\auth.json","r");
    i=0;
    while(!feof(a))
    {
        str2[i++]= (char)fgetc(a);
    }
    printf("\n fileauth %s\n",str2);
    cJSON * fileauth= cJSON_Parse(str2);
    fclose(a);

    cJSON *str=cJSON_CreateObject();
    if(fun_user(fileauth,auth2,2))
    {
        cJSON_AddItemToObject(str,"type",cJSON_CreateString("Error"));
        cJSON_AddItemToObject(str,"content",cJSON_CreateString("Authentication error"));
    }
    else if(strcmp(usechannel,"")==0)
    {
        cJSON_AddItemToObject(str,"type",cJSON_CreateString("Error"));
        cJSON_AddItemToObject(str,"content",cJSON_CreateString("The user does not exist in any channel"));
    }
    else
    {
        cJSON_AddItemToObject(str,"type",cJSON_CreateString("Successful"));
        cJSON_AddItemToObject(str,"content",cJSON_CreateString(""));
        char address[MAX];
        for(int i=0; i<MAX; i++)
        {
            str2[i]=0;
            address[i]=0;
        }
        sprintf(address,"resources\\channels\\%s.channel.json",usechannel);

        printf("\nhi\n");
        FILE *a=fopen(address,"r");
        i=0;
        while(!feof(a))
        {
            str2[i++]= (char)fgetc(a);
        }
        printf("\n channel info %s\n",str2);
        cJSON * channelinfo= cJSON_Parse(str2);
        fclose(a);
        cJSON * messages=cJSON_GetObjectItem(channelinfo,"messages");
        cJSON * newmessage = cJSON_CreateObject();
        cJSON_AddItemToObject(newmessage,"sender",cJSON_CreateString(useuser));
        cJSON_AddItemToObject(newmessage,"content",cJSON_CreateString(smessage));
        cJSON_AddItemToArray(messages,newmessage);

        a=fopen(address,"w");
        memset(buffer, 0, sizeof(buffer));
        strcpy(buffer,cJSON_PrintUnformatted(channelinfo));
        fprintf(a,"%s",buffer);
        fclose(a);

    }

    memset(buffer, 0, sizeof(buffer));
    strcpy(buffer,cJSON_PrintUnformatted(str));
    send(client_socket, buffer, sizeof(buffer), 0);

}
void fun_refresh()
{
    char auth2[80],str2[MAX];
    memset(auth2,0,80);
    memset(str2,0,MAX);

    for(int n=0; buffer[n+8]!='\n' ; n++)
    {
        auth2[n]= buffer[n+8];
    }

    FILE *a=fopen("resources\\auth.json","r");
    int i=0;
    while(!feof(a))
    {
        str2[i++]= (char)fgetc(a);
    }
    //printf("%s\n",str2);
    cJSON * fileauth= cJSON_Parse(str2);
    fclose(a);

    cJSON *str=cJSON_CreateObject();
    if(fun_user(fileauth,auth2,2))
    {
        cJSON_AddItemToObject(str,"type",cJSON_CreateString("Error"));
        cJSON_AddItemToObject(str,"content",cJSON_CreateString("Authentication error"));
    }
    else if(strcmp(usechannel,"")==0)
    {
        cJSON_AddItemToObject(str,"type",cJSON_CreateString("Error"));
        cJSON_AddItemToObject(str,"content",cJSON_CreateString("The user does not exist in any channel"));
    }
    else
    {
        char address[MAX];
        sprintf(address,"resources\\channels\\%s.channel.json",usechannel);

        for(int i=0; i<MAX; i++)
        {
            str2[i]=0;
        }
        FILE *a=fopen(address,"r");
        i=0;
        while(!feof(a))
        {
            str2[i++]= (char)fgetc(a);
        }
        cJSON * channelinfo= cJSON_Parse(str2);
        fclose(a);

        cJSON * messages = cJSON_GetObjectItem(channelinfo,"messages");
        printf("\n messages %s \n",cJSON_PrintUnformatted(messages));
        cJSON_AddItemToObject(str,"type",cJSON_CreateString("List"));
        int message_number = cJSON_GetArraySize(messages);

        int start = (int)noseen_index;
        cJSON * sending = cJSON_CreateArray();
        cJSON* item = cJSON_GetArrayItem(messages,start);
        cJSON_AddItemToArray(sending,item);
        printf("\n %s \n",cJSON_PrintUnformatted(sending));
        cJSON_AddItemToObject(str,"content",sending);


        cJSON * allusers = cJSON_GetObjectItem(fileauth,"users");
        cJSON * mainuser = cJSON_GetArrayItem(allusers,delete_item);
        cJSON_DeleteItemFromObject(mainuser,"noseen");
        cJSON_AddNumberToObject(mainuser,"noseen",message_number);


        a=fopen("resources\\auth.json","w");
        memset(buffer, 0, sizeof(buffer));
        strcpy(buffer,cJSON_PrintUnformatted(fileauth));
        fprintf(a,"%s",buffer);
        fclose(a);



    }

    memset(buffer, 0, sizeof(buffer));
    strcpy(buffer,cJSON_PrintUnformatted(str));
    printf("\n %s \n",buffer);
    send(client_socket, buffer, sizeof(buffer),0);


}
void fun_search(){

    char auth2[80],str2[MAX],keyword[80];
    memset(auth2,0,80);
    memset(keyword,0,80);
    memset(str2,0,MAX);

    int n=0,i=0;

    for(n=0; buffer[n+19]!=44 ; n++)
    {
        keyword[n]= buffer[n+19];
    }
    i=(n+21);
    printf("key word %s\n",keyword);

    for(n=0; buffer[n+i]!='\n' ; n++)
    {
        auth2[n] = buffer[n+i];
    }

    FILE *a=fopen("resources\\auth.json","r");
    i=0;
    while(!feof(a))
    {
        str2[i++]= (char)fgetc(a);
    }
    //printf("%s\n",str2);
    cJSON * fileauth= cJSON_Parse(str2);
    fclose(a);

    cJSON *str=cJSON_CreateObject();
    if(fun_user(fileauth,auth2,2))
    {
        cJSON_AddItemToObject(str,"type",cJSON_CreateString("Error"));
        cJSON_AddItemToObject(str,"content",cJSON_CreateString("Authentication error"));
    }
    else if(strcmp(usechannel,"")==0)
    {
        cJSON_AddItemToObject(str,"type",cJSON_CreateString("Error"));
        cJSON_AddItemToObject(str,"content",cJSON_CreateString("The user does not exist in any channel"));
    }
    else
    {
        char address[MAX];
        sprintf(address,"resources\\channels\\%s.channel.json",usechannel);

        for(int i=0; i<MAX; i++)
        {
            str2[i]=0;
        }
        FILE *a=fopen(address,"r");
        i=0;
        while(!feof(a))
        {
            str2[i++]= (char)fgetc(a);
        }
        cJSON * channelinfo= cJSON_Parse(str2);
        fclose(a);

        cJSON * messages = cJSON_GetObjectItem(channelinfo,"messages");
        printf("\n messages %s \n",cJSON_PrintUnformatted(messages));
        int message_number = cJSON_GetArraySize(messages);
        cJSON *newmessages = cJSON_CreateArray();
        for(int i=0 ; i< message_number ; i++){
            cJSON *usemessage = cJSON_GetArrayItem(messages,i);
            char mess[MAX];
            memset(mess,0,MAX);
            strcpy(mess,cJSON_GetObjectItem(usemessage,"content")->valuestring);
            int keylen = strlen(keyword);
            char * a = strstr(mess,keyword);

            if( a==NULL )
                continue;
            else if( *(a-1)!= 0 && *(a-1)!= 32)
                continue;
            else if( *(a+keylen)!= 0 && *(a+keylen)!= 32)
                continue;
            else{
                cJSON * newmessage1 = cJSON_CreateObject();
                char sender[80];
                char content[80];
                memset(sender,0,80);
                memset(content,0,80);
                strcpy(sender,cJSON_GetObjectItem(usemessage,"sender")->valuestring);
                strcpy(content,cJSON_GetObjectItem(usemessage,"content")->valuestring);
                printf("%s  %s\n",sender,content);

                cJSON_AddItemToObject(newmessage1,"sender",cJSON_CreateString(sender));
                cJSON_AddItemToObject(newmessage1,"content",cJSON_CreateString(content));
                cJSON_AddItemToArray(newmessages,newmessage1);
            }
        }
        if(cJSON_GetArraySize(newmessages)==0){
            cJSON_AddItemToObject(str,"type",cJSON_CreateString("Error"));
            cJSON_AddItemToObject(str,"content",cJSON_CreateString("This keyword not found"));
        }
        else{
            cJSON_AddItemToObject(str,"type",cJSON_CreateString("List"));
            cJSON_AddItemToObject(str,"content",newmessages);
        }
    }

    memset(buffer, 0, sizeof(buffer));
    strcpy(buffer,cJSON_PrintUnformatted(str));
    printf("\n %s \n",buffer);
    send(client_socket, buffer, sizeof(buffer),0);


}

void fun_members()
{
    char auth2[80],str2[MAX];
    memset(auth2,0,80);
    memset(str2,0,MAX);

    for(int n=0; buffer[n+16]!='\n' ; n++)
    {
        auth2[n]= buffer[n+16];
    }

    FILE *a=fopen("resources\\auth.json","r");
    int i=0;
    while(!feof(a))
    {
        str2[i++]= (char)fgetc(a);
    }
    //printf("%s\n",str2);
    cJSON * fileauth= cJSON_Parse(str2);
    fclose(a);

    cJSON *str=cJSON_CreateObject();
    if(fun_user(fileauth,auth2,2))
    {
        cJSON_AddItemToObject(str,"type",cJSON_CreateString("Error"));
        cJSON_AddItemToObject(str,"content",cJSON_CreateString("Authentication error"));
    }
    else if(strcmp(usechannel,"")==0)
    {
        cJSON_AddItemToObject(str,"type",cJSON_CreateString("Error"));
        cJSON_AddItemToObject(str,"content",cJSON_CreateString("The user does not exist in any channel"));
    }
    else
    {
        char address[MAX];
        sprintf(address,"resources\\channels\\%s.channel.json",usechannel);

        for(int i=0; i<MAX; i++)
        {
            str2[i]=0;
        }
        FILE *a=fopen(address,"r");
        i=0;
        while(!feof(a))
        {
            str2[i++]= (char)fgetc(a);
        }
        cJSON * channelinfo= cJSON_Parse(str2);
        fclose(a);

        cJSON * members = cJSON_GetObjectItem(channelinfo,"members");
        cJSON_AddItemToObject(str,"type",cJSON_CreateString("List"));
        cJSON_AddItemToObject(str,"content",members);

    }

    memset(buffer, 0, sizeof(buffer));
    strcpy(buffer,cJSON_PrintUnformatted(str));
    send(client_socket, buffer, sizeof(buffer), 0);



}
void fun_find()
{

    char auth2[80],str2[MAX],findmember[80];
    memset(auth2,0,80);
    memset(findmember,0,80);
    memset(str2,0,MAX);

    int n=0,i=0;
    for(n=0 ; buffer[n+12]!=44 ; n++)
    {
        findmember[n]=buffer[n+12];
    }
    i=(n+14);
    for(n=0; buffer[n+i]!='\n' ; n++)
    {
        auth2[n]= buffer[n+i];
    }

    FILE *a=fopen("resources\\auth.json","r");
    i=0;
    while(!feof(a))
    {
        str2[i++]= (char)fgetc(a);
    }
    //printf("%s\n",str2);
    cJSON * fileauth= cJSON_Parse(str2);
    fclose(a);

    cJSON *str=cJSON_CreateObject();
    if(fun_user(fileauth,auth2,2))
    {
        cJSON_AddItemToObject(str,"type",cJSON_CreateString("Error"));
        cJSON_AddItemToObject(str,"content",cJSON_CreateString("Authentication error"));
    }
    else if(strcmp(usechannel,"")==0)
    {
        cJSON_AddItemToObject(str,"type",cJSON_CreateString("Error"));
        cJSON_AddItemToObject(str,"content",cJSON_CreateString("The user does not exist in any channel"));
    }
    else
    {
        char address[MAX];
        sprintf(address,"resources\\channels\\%s.channel.json",usechannel);

        for(int i=0; i<MAX; i++)
        {
            str2[i]=0;
        }
        FILE *a=fopen(address,"r");
        i=0;
        while(!feof(a))
        {
            str2[i++]= (char)fgetc(a);
        }
        cJSON * channelinfo= cJSON_Parse(str2);
        fclose(a);

        cJSON_AddItemToObject(str,"type",cJSON_CreateString("Mode"));

        cJSON * members = cJSON_GetObjectItem(channelinfo,"members");
        int arraylen = cJSON_GetArraySize(members);

        int k=0;
        for(int i=0;i<arraylen;i++){
            if(strcmp(findmember,cJSON_GetArrayItem(members,i)->valuestring)==0){
                k=1;
                break;
            }
        }
        if(k==1)
            cJSON_AddItemToObject(str,"content",cJSON_CreateString("true"));
        else
            cJSON_AddItemToObject(str,"content",cJSON_CreateString("false"));

    }

    memset(buffer, 0, sizeof(buffer));
    strcpy(buffer,cJSON_PrintUnformatted(str));
    send(client_socket, buffer, sizeof(buffer), 0);


}

int fun_tekrar(int n,char user[])
{

    struct dirent *de;
    DIR *dr;

    if(n==1)
    {
        dr = opendir("resources//users");
        strcat(user,".user.json");
    }

    else if(n==2)
    {
        dr = opendir("resources//channels");
        strcat(user,".channel.json");
    }

    if (dr == NULL)
    {
        printf("Could not open current directory" );
        return 0;
        //return fun_tekrar(n,user);
    }

    while ((de = readdir(dr)) != NULL)
    {
        if(strcmp(de->d_name,user)==0)
        {
            closedir(dr);
            return 0;
        }
    }
    closedir(dr);
    return 1;
}
void randauth()
{
    srand(time(0));
    for(int i=0; i<30; i++)
    {
        int n=(rand()%36);
        if(n<10)
            auth[i]=(n+48);
        else
            auth[i]=(n+87);
    }
}

int  fun_user(cJSON *a, char user[],int n)
{
    char fea[5];
    cJSON * allarray = cJSON_GetObjectItem(a,"users");

    if(n==1)
        strcpy(fea,"name");
    else if(n==2)
        strcpy(fea,"auth");

    int sizearray= cJSON_GetArraySize(allarray);
    for(int i=0; i<sizearray; i++)
    {
        cJSON * us=cJSON_GetArrayItem(allarray,i);
        if(strcmp(user,cJSON_GetObjectItem(us,fea)->valuestring)==0)
        {
            if(n==2)
            {
                delete_item=i;
                strcpy(useuser,cJSON_GetObjectItem(us,"name")->valuestring);
                strcpy(usechannel,cJSON_GetObjectItem(us,"channel")->valuestring);
                //strcpy(noseen_index,cJSON_GetObjectItem(us,"noseen")->valueint);
                noseen_index = cJSON_GetObjectItem(us,"noseen")->valueint;
            }
            return 0;
        }
    }

    return 1;
}




