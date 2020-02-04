#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <dirent.h>
#include <time.h>

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


char newstring[MAX];

char * cJSON_CreateObject();
char * cJSON_CreateArray();
char * cJSON_AddStringToObject(const char * str,const char * first, const char * second);
char * cJSON_AddNumberToObject(const char * str,const char * first, int second);
char * cJSON_AddItemToArray(const char * str,const char * newitem);
char * cJSON_GetArrayItem(const char * mainarray, int item);
char * cJSON_GetObjectItem(const char * str, const char * first);
char * cJSON_DeleteItemFromObject(const char * str, const char * first);
char * cJSON_DeleteItemFromArray(const char * str, int item);
char * cJSON_AddaArrayToObject(const char * str,const char * first, const char * second);
int cJSON_GetArraySize(const char * mainarray);


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
    if(fopen("resources\\auth.json","r") == NULL)
    {
        FILE *a=fopen("resources\\auth.json","w");
        char str[5],arr[5];
        strcpy(str,cJSON_CreateObject());
        strcpy(arr,cJSON_CreateArray());
        fprintf(a,"%s",cJSON_AddaArrayToObject(str,"users",arr));
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
    char str[MAX];
    strcpy(str,cJSON_CreateObject());
    char user2[80];
    strcpy(user2,user);
    if(fun_tekrar(1,user2)==0)
    {
        strcpy(str,cJSON_AddStringToObject(str,"type","Error"));
        strcpy(str,cJSON_AddStringToObject(str,"content","The username has already exist"));
    }
    else
    {
        strcpy(str,cJSON_AddStringToObject(str,"type","Successful"));
        strcpy(str,cJSON_AddStringToObject(str,"content",""));
        char str2[MAX];
        strcpy(str2,cJSON_CreateObject());
        strcpy(str2,cJSON_AddStringToObject(str2,"username",user));
        strcpy(str2,cJSON_AddStringToObject(str2,"password",pass));
        char address[100];
        sprintf(address,"resources\\users\\%s.user.json",user);
        FILE *a=fopen(address,"w");
        fprintf(a,"%s",str2);
        fclose(a);
    }
    send(client_socket, str, sizeof(str), 0);

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

    char str[MAX];
    strcpy(str,cJSON_CreateObject());

    char user2[80];
    strcpy(user2,user);

    if(fun_tekrar(1,user2))
    {
        strcpy(str,cJSON_AddStringToObject(str,"type","Error"));
        strcpy(str,cJSON_AddStringToObject(str,"content","The username has not registered"));
    }
    else
    {
        char address[100];
        sprintf(address,"resources\\users\\%s.user.json",user);
        FILE *a =fopen(address,"r");
        char str2[MAX];
        memset(str2,0,MAX);
        int i=0;
        while(!feof(a))
        {
            str2[i++]= (char)fgetc(a);
        }
        fclose(a);

        char fileauth[MAX];
        memset(fileauth,0,MAX);

        a =fopen("resources\\auth.json","r");
        i=0;
        while(!feof(a))
        {
            fileauth[i++]= (char)fgetc(a);
        }
        fclose(a);


        if(fun_user(fileauth,user,1)==0)
        {
            strcpy(str,cJSON_AddStringToObject(str,"type","Error"));
            strcpy(str,cJSON_AddStringToObject(str,"content","The user is login yet"));
        }

        else if(strcmp(pass,cJSON_GetObjectItem(str2,"password")) !=0)
        {
            strcpy(str,cJSON_AddStringToObject(str,"type","Error"));
            strcpy(str,cJSON_AddStringToObject(str,"content","The password is invalid"));
        }
        else
        {
            randauth();
            strcpy(str,cJSON_AddStringToObject(str,"type","Authtoken"));
            strcpy(str,cJSON_AddStringToObject(str,"content",auth));
            char allusers[MAX],newuser[MAX];
            strcpy(allusers,cJSON_GetObjectItem(fileauth,"users"));
            strcpy(newuser,cJSON_CreateObject());
            strcpy(newuser,cJSON_AddStringToObject(newuser,"name",user));
            strcpy(newuser,cJSON_AddStringToObject(newuser,"auth",auth));
            strcpy(newuser,cJSON_AddStringToObject(newuser,"channel",""));
            strcpy(newuser,cJSON_AddNumberToObject(newuser,"noseen",0));
            strcpy(allusers,cJSON_AddItemToArray(allusers,newuser));
            strcpy(fileauth,cJSON_DeleteItemFromObject(fileauth,"users"));
            strcpy(fileauth,cJSON_AddaArrayToObject(fileauth,"users",allusers));
            FILE * fil =fopen("resources\\auth.json","w");
            printf("%s\n",fileauth);
            fprintf(fil,"%s",fileauth);
            fclose(fil);
        }
    }
    send(client_socket, str, sizeof(str), 0);

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
    char fileauth[MAX];

    FILE *a=fopen("resources\\auth.json","r");
    i=0;
    while(!feof(a))
    {
        fileauth[i++]= (char)fgetc(a);
    }
    // printf("%s\n",fileauth);
    fclose(a);

    char str[MAX];
    strcpy(str,cJSON_CreateObject());
    //printf("%s\n",auth2);
    if(fun_user(fileauth,auth2,2)==1)
    {
        strcpy(str,cJSON_AddStringToObject(str,"type","Error"));
        strcpy(str,cJSON_AddStringToObject(str,"content","Authentication error"));
    }
    else
    {
        strcpy(str,cJSON_AddStringToObject(str,"type","Successful"));
        strcpy(str,cJSON_AddStringToObject(str,"content",""));

        char allarray[MAX],newarray[MAX];
        strcpy(allarray,cJSON_GetObjectItem(fileauth,"users"));
        strcpy(newarray,cJSON_CreateArray());
        int sizea=cJSON_GetArraySize(allarray);
        printf("\nsize %d\n",sizea);
        printf("\ndelete  %d\n",delete_item);
        for(int i=0; i<sizea; i++)
        {
            if(i==delete_item)
            {
                printf("\n%d\n",i);
            }
            else
            {
                char onlineuser[MAX];
                strcpy(onlineuser,cJSON_GetArrayItem(allarray,i));
                strcpy(newarray,cJSON_AddItemToArray(newarray,onlineuser));
            }
        }
        char newfileauth[MAX];
        strcpy(newfileauth,cJSON_CreateObject());
        strcpy(newfileauth,cJSON_AddaArrayToObject(newfileauth,"users",newarray));
        FILE *au =fopen("resources\\auth.json","w");
        printf("\nnew fileauth %s\n",newfileauth);
        fprintf(au,"%s",newfileauth);
        fclose(au);

    }
    send(client_socket, str, sizeof(str), 0);
}
void fun_leave()
{
    char auth2[MAX];
    char str2[MAX];
    char fileauth[MAX];
    memset(str2, 0, sizeof(str2));
    memset(auth2, 0, sizeof(auth2));
    memset(fileauth, 0, sizeof(fileauth));
    for(int n=0; buffer[n+6]!='\n' ; n++)
    {
        auth2[n]= buffer[n+6];
    }

    FILE *a=fopen("resources\\auth.json","r");
    int i=0;
    while(!feof(a))
    {
        fileauth[i++]= (char)fgetc(a);
    }
    printf("\n fileauth %s\n",fileauth);
    fclose(a);
    char str[MAX];
    strcpy(str,cJSON_CreateObject());
    printf("%s\n",auth2);
    if(fun_user(fileauth,auth2,2)==1)
    {
        strcpy(str,cJSON_AddStringToObject(str,"type","Error"));
        strcpy(str,cJSON_AddStringToObject(str,"content","Authentication error"));
    }
    else if(strcmp(usechannel,"")==0)
    {
        strcpy(str,cJSON_AddStringToObject(str,"type","Error"));
        strcpy(str,cJSON_AddStringToObject(str,"content","The user is not in any channel."));
    }
    else
    {
        strcpy(str,cJSON_AddStringToObject(str,"type","Successful"));
        strcpy(str,cJSON_AddStringToObject(str,"content",""));


        char address[MAX],channelinfo[MAX];
        memset(address,0,sizeof(address));
        memset(channelinfo,0,MAX);

        printf("\n usechannel %s \n",usechannel);

        sprintf(address,"resources\\channels\\%s.channel.json",usechannel);

        FILE *a=fopen(address,"r");
        i=0;
        while(!feof(a))
        {
            channelinfo[i++]= (char)fgetc(a);
        }

        fclose(a);
        char messages[MAX],members[MAX];
        strcpy(messages,cJSON_GetObjectItem(channelinfo,"messages"));
        strcpy(members,cJSON_GetObjectItem(channelinfo,"members"));
        printf("\n members %s \n",members);
        char newmessage[MAX];
        strcpy(newmessage,cJSON_CreateObject());
        strcpy(newmessage,cJSON_AddStringToObject(newmessage,"sender","server"));
        char messa[100];
        memset(messa,0,100);
        sprintf(messa,"%s leaved %s.",useuser,usechannel);
        strcpy(newmessage,cJSON_AddStringToObject(newmessage,"content",messa));
        strcpy(messages,cJSON_AddItemToArray(messages,newmessage));

        int sizea = cJSON_GetArraySize(members);
        for(int i=0 ; i<sizea ; i++)
        {
            if(strcmp( cJSON_GetArrayItem(members,i),useuser)==0)
            {
                strcpy(members,cJSON_DeleteItemFromArray(members,i));
                break;
            }
        }
        char newchannelinfo[MAX];
        strcpy(newchannelinfo,cJSON_CreateObject());
        strcpy(newchannelinfo,cJSON_AddaArrayToObject(newchannelinfo,"messages",messages));
        strcpy(newchannelinfo,cJSON_AddaArrayToObject(newchannelinfo,"members",members));


        FILE *newfi = fopen(address,"w");
        printf("\n%s\n",newchannelinfo);
        fprintf(newfi,"%s",newchannelinfo);
        fclose(newfi);
        char allusers[MAX],mainuser[MAX];
        strcpy(allusers,cJSON_GetObjectItem(fileauth,"users"));
        strcpy(mainuser,cJSON_GetArrayItem(allusers,delete_item));
        strcpy(allusers,cJSON_DeleteItemFromArray(allusers,delete_item));

        strcpy(mainuser,cJSON_DeleteItemFromObject(mainuser,"channel"));
        strcpy(mainuser,cJSON_AddStringToObject(mainuser,"channel",""));
        strcpy(mainuser,cJSON_DeleteItemFromObject(mainuser,"noseen"));
        strcpy(mainuser,cJSON_AddNumberToObject(mainuser,"noseen",0));
        strcpy(allusers,cJSON_AddItemToArray(allusers,mainuser));
        char newfileauth[MAX];
        strcpy(newfileauth,cJSON_CreateObject());
        strcpy(newfileauth,cJSON_AddaArrayToObject(newfileauth,"users",allusers));

        newfi=fopen("resources\\auth.json","w");
        fprintf(newfi,"%s",newfileauth);
        fclose(newfi);

    }
    send(client_socket, str, sizeof(str), 0);


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
    char fileauth[MAX];
    FILE *a=fopen("resources\\auth.json","r");
    i=0;
    while(!feof(a))
    {
        fileauth[i++]= (char)fgetc(a);
    }
    //printf("%s\n",str2);
    fclose(a);
    char str[MAX];
    strcpy(str, cJSON_CreateObject());
    char cha_name2[80];
    strcpy(cha_name2,cha_name);
    if(fun_user(fileauth,auth2,2))
    {
        strcpy(str,cJSON_AddStringToObject(str,"type","Error"));
        strcpy(str,cJSON_AddStringToObject(str,"content","Authentication error"));
    }
    else if(fun_tekrar(2,cha_name2))
    {
        strcpy(str,cJSON_AddStringToObject(str,"type","Error"));
        strcpy(str,cJSON_AddStringToObject(str,"content","The channel doesnt exist"));
    }
    else
    {
        strcpy(str,cJSON_AddStringToObject(str,"type","Successful"));
        strcpy(str,cJSON_AddStringToObject(str,"content",""));
        char address[MAX],channelinfo[MAX];
        memset(address,0,sizeof(address));
        memset(channelinfo,0,MAX);
        sprintf(address,"resources\\channels\\%s.channel.json",cha_name);

        FILE *a=fopen(address,"r");
        i=0;
        while(!feof(a))
        {
            channelinfo[i++]= (char)fgetc(a);
        }
        printf("\n%s\n",channelinfo);
        fclose(a);
        char messages[MAX],members[MAX];
        strcpy(messages,cJSON_GetObjectItem(channelinfo,"messages"));
        strcpy(members,cJSON_GetObjectItem(channelinfo,"members"));
        printf("\n members %s \n",members);
        char newmessage[MAX];
        strcpy(newmessage,cJSON_CreateObject());
        strcpy(newmessage,cJSON_AddStringToObject(newmessage,"sender","server"));
        char messa[100];
        memset(messa,0,100);
        sprintf(messa,"%s joined %s.",useuser,cha_name);
        strcpy(newmessage,cJSON_AddStringToObject(newmessage,"content",messa));
        strcpy(messages,cJSON_AddItemToArray(messages,newmessage));
        strcpy(members,cJSON_AddItemToArray(members,useuser));

        char newchannelinfo[MAX];
        strcpy(newchannelinfo,cJSON_CreateObject());
        strcpy(newchannelinfo,cJSON_AddaArrayToObject(newchannelinfo,"messages",messages));
        strcpy(newchannelinfo,cJSON_AddaArrayToObject(newchannelinfo,"members",members));

        FILE *newfi = fopen(address,"w");
        printf("\n%s\n",newchannelinfo);
        fprintf(newfi,"%s",newchannelinfo);
        fclose(newfi);
        char allusers[MAX],mainuser[MAX];
        strcpy(allusers,cJSON_GetObjectItem(fileauth,"users"));
        strcpy(mainuser, cJSON_GetArrayItem(allusers,delete_item));
        strcpy(allusers,cJSON_DeleteItemFromArray(allusers,delete_item));

        strcpy(mainuser,cJSON_DeleteItemFromObject(mainuser,"channel"));
        strcpy(mainuser,cJSON_AddStringToObject(mainuser,"channel",cha_name));
        strcpy(mainuser,cJSON_DeleteItemFromObject(mainuser,"noseen"));
        strcpy(mainuser,cJSON_AddNumberToObject(mainuser,"noseen",0));
        strcpy(allusers,cJSON_AddItemToArray(allusers,mainuser));
        char newfileauth[MAX];
        strcpy(newfileauth,cJSON_CreateObject());
        strcpy(newfileauth,cJSON_AddaArrayToObject(newfileauth,"users",allusers));

        newfi=fopen("resources\\auth.json","w");
        fprintf(newfi,"%s",newfileauth);
        fclose(newfi);

    }
    send(client_socket,str, sizeof(str), 0);
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
    char str2[MAX],fileauth[MAX];
    memset(fileauth,0,MAX);
    FILE *a=fopen("resources\\auth.json","r");
    i=0;
    while(!feof(a))
    {
        fileauth[i++]= (char)fgetc(a);
    }
    //printf("%s\n",str2);
    fclose(a);
    char str[MAX];
    strcpy(str,cJSON_CreateObject());
    char cha_name2[80];
    strcpy(cha_name2,cha_name);

    if(fun_user(fileauth,auth2,2))
    {
        strcpy(str,cJSON_AddStringToObject(str,"type","Error"));
        strcpy(str,cJSON_AddStringToObject(str,"content","Authentication error"));
    }
    else if(fun_tekrar(2,cha_name2)==0)
    {
        strcpy(str,cJSON_AddStringToObject(str,"type","Error"));
        strcpy(str,cJSON_AddStringToObject(str,"content","The channel name is not available."));
    }
    else
    {
        strcpy(str,cJSON_AddStringToObject(str,"type","Successful"));
        strcpy(str,cJSON_AddStringToObject(str,"content",""));
        char address[MAX];
        sprintf(address,"resources\\channels\\%s.channel.json",cha_name);

        char messages[MAX];
        strcpy(messages,cJSON_CreateArray());
        char newmessage[MAX];
        strcpy(newmessage,cJSON_CreateObject());
        strcpy(newmessage,cJSON_AddStringToObject(newmessage,"sender","server"));
        char messa[100];
        sprintf(messa,"%s create and joined %s.",useuser,cha_name);
        strcpy(newmessage,cJSON_AddStringToObject(newmessage,"content",messa));
        strcpy(messages,cJSON_AddItemToArray(messages,newmessage));
        char newchannelinfo[MAX],members[MAX];
        strcpy(newchannelinfo,cJSON_CreateObject());
        strcpy(members,cJSON_CreateArray());
        strcpy(members,cJSON_AddItemToArray(members,useuser));

        strcpy(newchannelinfo,cJSON_AddaArrayToObject(newchannelinfo,"messages",messages));
        strcpy(newchannelinfo,cJSON_AddaArrayToObject(newchannelinfo,"members",members));

        a=fopen(address,"w");
        fprintf(a,"%s",newchannelinfo);
        fclose(a);

        char allusers[MAX],mainuser[MAX];
        strcpy(allusers,cJSON_GetObjectItem(fileauth,"users"));
        strcpy(mainuser,cJSON_GetArrayItem(allusers,delete_item));
        strcpy(allusers,cJSON_DeleteItemFromArray(allusers,delete_item));

        strcpy(mainuser,cJSON_DeleteItemFromObject(mainuser,"channel"));
        strcpy(mainuser,cJSON_AddStringToObject(mainuser,"channel",cha_name));
        strcpy(mainuser,cJSON_DeleteItemFromObject(mainuser,"noseen"));
        strcpy(mainuser,cJSON_AddNumberToObject(mainuser,"noseen",0));
        strcpy(allusers,cJSON_AddItemToArray(allusers,mainuser));
        char newfileauth[MAX];
        strcpy(newfileauth,cJSON_CreateObject());
        strcpy(newfileauth,cJSON_AddaArrayToObject(newfileauth,"users",allusers));

        FILE *newfi=fopen("resources\\auth.json","w");
        fprintf(newfi,"%s",newfileauth);
        fclose(newfi);



    }

    send(client_socket, str, sizeof(str), 0);

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
    char str2[MAX],fileauth[MAX];
    memset(str2,0,MAX);
    memset(fileauth,0,MAX);
    FILE *a=fopen("resources\\auth.json","r");
    i=0;
    while(!feof(a))
    {
        fileauth[i++]= (char)fgetc(a);
    }
    fclose(a);

    char str[MAX];
    strcpy(str,cJSON_CreateObject());
    if(fun_user(fileauth,auth2,2))
    {
        strcpy(str,cJSON_AddStringToObject(str,"type","Error"));
        strcpy(str,cJSON_AddStringToObject(str,"content","Authentication error"));
    }
    else if(strcmp(usechannel,"")==0)
    {
        strcpy(str,cJSON_AddStringToObject(str,"type","Error"));
        strcpy(str,cJSON_AddStringToObject(str,"content","The user does not exist in any channel"));
    }
    else
    {
        strcpy(str,cJSON_AddStringToObject(str,"type","Successful"));
        strcpy(str,cJSON_AddStringToObject(str,"content",""));
        char address[MAX],channelinfo[MAX];
        for(int i=0; i<MAX; i++)
        {
            channelinfo[i]=0;
            address[i]=0;
        }
        sprintf(address,"resources\\channels\\%s.channel.json",usechannel);

        printf("\nhi\n");
        FILE *a=fopen(address,"r");
        i=0;
        while(!feof(a))
        {
            channelinfo[i++]= (char)fgetc(a);
        }
        fclose(a);
        char messages[MAX],newmessage[MAX],members[MAX];
        memset(members,0,MAX);
        memset(messages,0,MAX);
        memset(newmessage,0,MAX);
        strcpy(messages,cJSON_GetObjectItem(channelinfo,"messages"));
        strcpy(members,cJSON_GetObjectItem(channelinfo,"members"));
        strcpy(newmessage,cJSON_CreateObject());
        strcpy(newmessage,cJSON_AddStringToObject(newmessage,"sender",useuser));
        strcpy(newmessage,cJSON_AddStringToObject(newmessage,"content",smessage));
        strcpy(messages,cJSON_AddItemToArray(messages,newmessage));

        char newchannelinfo[MAX];
        strcpy(newchannelinfo,cJSON_CreateObject());
        strcpy(newchannelinfo,cJSON_AddaArrayToObject(newchannelinfo,"messages",messages));
        strcpy(newchannelinfo,cJSON_AddaArrayToObject(newchannelinfo,"members",members));
        a=fopen(address,"w");
        fprintf(a,"%s",newchannelinfo);
        fclose(a);

    }
    send(client_socket,str, sizeof(str), 0);

}
void fun_refresh()
{
    char auth2[80],str2[MAX],fileauth[MAX];
    memset(auth2,0,80);
    memset(str2,0,MAX);
    memset(fileauth,0,MAX);
    for(int n=0; buffer[n+8]!='\n' ; n++)
    {
        auth2[n]= buffer[n+8];
    }

    FILE *a=fopen("resources\\auth.json","r");
    int i=0;
    while(!feof(a))
    {
        fileauth[i++]= (char)fgetc(a);
    }
    //printf("%s\n",fileauth);
    fclose(a);
    char str[MAX];
    strcpy(str,cJSON_CreateObject());
    if(fun_user(fileauth,auth2,2))
    {
        strcpy(str,cJSON_AddStringToObject(str,"type","Error"));
        strcpy(str,cJSON_AddStringToObject(str,"content","Authentication error"));
    }
    else if(strcmp(usechannel,"")==0)
    {
        strcpy(str,cJSON_AddStringToObject(str,"type","Error"));
        strcpy(str,cJSON_AddStringToObject(str,"content","The user does not exist in any channel"));
    }
    else
    {
        char address[MAX],channelinfo[MAX];
        sprintf(address,"resources\\channels\\%s.channel.json",usechannel);

        for(int i=0; i<MAX; i++)
        {
            channelinfo[i]=0;
        }
        FILE *a=fopen(address,"r");
        i=0;
        while(!feof(a))
        {
            channelinfo[i++]= (char)fgetc(a);
        }
        fclose(a);
        char messages[MAX],newmessages[MAX];
        strcpy(messages,cJSON_GetObjectItem(channelinfo,"messages"));
        strcpy(newmessages,cJSON_CreateArray());
        printf("\n messages %s \n",messages);
        strcpy(str,cJSON_AddStringToObject(str,"type","List"));
        int message_number = cJSON_GetArraySize(messages);
        for(int i = noseen_index ; i< message_number ; i++)
        {
            char item[MAX];
            memset(item,0,MAX);
            strcpy(item,cJSON_GetArrayItem(messages,i));
            strcpy(newmessages,cJSON_AddItemToArray(newmessages,item));
        }
        strcpy(str,cJSON_AddaArrayToObject(str,"content",newmessages));


        char allusers[MAX],mainuser[MAX];
        strcpy(allusers,cJSON_GetObjectItem(fileauth,"users"));
        strcpy(mainuser,cJSON_GetArrayItem(allusers,delete_item));
        strcpy(allusers,cJSON_DeleteItemFromArray(allusers,delete_item));
        strcpy(mainuser,cJSON_DeleteItemFromObject(mainuser,"noseen"));
        strcpy(mainuser,cJSON_AddNumberToObject(mainuser,"noseen",message_number));
        strcpy(allusers,cJSON_AddItemToArray(allusers,mainuser));
        char newfileauth[MAX];
        strcpy(newfileauth,cJSON_CreateObject());
        strcpy(newfileauth,cJSON_AddaArrayToObject(newfileauth,"users",allusers));

        FILE *newfi=fopen("resources\\auth.json","w");
        fprintf(newfi,"%s",newfileauth);
        fclose(newfi);



    }
    printf("\n %s \n",str);
    send(client_socket, str, sizeof(str),0);


}
void fun_search()
{

    char auth2[80],str2[MAX],keyword[80],fileauth[MAX];
    memset(auth2,0,80);
    memset(keyword,0,80);
    memset(str2,0,MAX);
    memset(fileauth,0,MAX);

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
        fileauth[i++]= (char)fgetc(a);
    }
    //printf("%s\n",fileauth);
    fclose(a);
    char str[MAX];
    strcpy(str,cJSON_CreateObject());
    if(fun_user(fileauth,auth2,2))
    {
        strcpy(str,cJSON_AddStringToObject(str,"type","Error"));
        strcpy(str,cJSON_AddStringToObject(str,"content","Authentication error"));
    }
    else if(strcmp(usechannel,"")==0)
    {
        strcpy(str,cJSON_AddStringToObject(str,"type","Error"));
        strcpy(str,cJSON_AddStringToObject(str,"content","The user does not exist in any channel"));
    }
    else
    {
        char address[MAX],channelinfo[MAX];
        sprintf(address,"resources\\channels\\%s.channel.json",usechannel);

        for(int i=0; i<MAX; i++)
        {
            channelinfo[i]=0;
        }
        FILE *a=fopen(address,"r");
        i=0;
        while(!feof(a))
        {
            channelinfo[i++]= (char)fgetc(a);
        }
        fclose(a);
        char messages[MAX],newmessages[MAX];
        strcpy(messages,cJSON_GetObjectItem(channelinfo,"messages"));
        printf("\n messages %s \n",messages);
        int message_number = cJSON_GetArraySize(messages);
        strcpy(newmessages,cJSON_CreateArray());
        for(int i=0 ; i< message_number ; i++)
        {
            char usemessage[MAX];
            strcpy(usemessage,cJSON_GetArrayItem(messages,i));
            char mess[MAX];
            memset(mess,0,MAX);
            strcpy(mess,cJSON_GetObjectItem(usemessage,"content"));
            int keylen = strlen(keyword);
            char * a = strstr(mess,keyword);

            if( a==NULL )
                continue;
            else if( *(a-1)!= 0 && *(a-1)!= 32)
                continue;
            else if( *(a+keylen)!= 0 && *(a+keylen)!= 32)
                continue;
            else
            {
                char newmessage1[MAX];
                memset(newmessage1,0,MAX);
                strcpy(newmessage1,cJSON_CreateObject());
                char sender[80];
                char content[80];
                memset(sender,0,80);
                memset(content,0,80);
                strcpy(sender,cJSON_GetObjectItem(usemessage,"sender"));
                strcpy(content,cJSON_GetObjectItem(usemessage,"content"));
                printf("%s  %s\n",sender,content);

                strcpy(newmessage1,cJSON_AddStringToObject(newmessage1,"sender",sender));
                strcpy(newmessage1,cJSON_AddStringToObject(newmessage1,"content",content));
                strcpy(newmessages,cJSON_AddItemToArray(newmessages,newmessage1));
            }
        }
        if(cJSON_GetArraySize(newmessages)==0)
        {
            strcpy(str,cJSON_AddStringToObject(str,"type","Error"));
            strcpy(str,cJSON_AddStringToObject(str,"content","This keyword not found"));
        }
        else
        {
            strcpy(str,cJSON_AddStringToObject(str,"type","List"));
            strcpy(str,cJSON_AddaArrayToObject(str,"content",newmessages));
        }
    }
    printf("\n %s \n",str);
    send(client_socket, str, sizeof(str),0);

}

void fun_members()
{
    char auth2[80],fileauth[MAX];
    memset(auth2,0,80);
    memset(fileauth,0,MAX);

    for(int n=0; buffer[n+16]!='\n' ; n++)
    {
        auth2[n]= buffer[n+16];
    }

    FILE *a=fopen("resources\\auth.json","r");
    int i=0;
    while(!feof(a))
    {
        fileauth[i++]= (char)fgetc(a);
    }
    //printf("%s\n",fileauth);
    fclose(a);
    char str[MAX];
    strcpy(str,cJSON_CreateObject());
    if(fun_user(fileauth,auth2,2))
    {
        strcpy(str,cJSON_AddStringToObject(str,"type","Error"));
        strcpy(str,cJSON_AddStringToObject(str,"content","Authentication error"));
    }
    else if(strcmp(usechannel,"")==0)
    {
        strcpy(str,cJSON_AddStringToObject(str,"type","Error"));
        strcpy(str,cJSON_AddStringToObject(str,"content","The user does not exist in any channel"));
    }
    else
    {
        char address[MAX],channelinfo[MAX];
        sprintf(address,"resources\\channels\\%s.channel.json",usechannel);

        for(int i=0; i<MAX; i++)
        {
            channelinfo[i]=0;
        }
        FILE *a=fopen(address,"r");
        i=0;
        while(!feof(a))
        {
            channelinfo[i++]= (char)fgetc(a);
        }
        fclose(a);

        char members[MAX];
        strcpy(members,cJSON_GetObjectItem(channelinfo,"members"));
        strcpy(str,cJSON_AddStringToObject(str,"type","List"));
        strcpy(str,cJSON_AddaArrayToObject(str,"content",members));

    }
    send(client_socket, str, sizeof(str), 0);



}
void fun_find()
{

    char auth2[80],fileauth[MAX],findmember[80];
    memset(auth2,0,80);
    memset(findmember,0,80);
    memset(fileauth,0,MAX);

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
        fileauth[i++]= (char)fgetc(a);
    }
    //printf("%s\n",fileauth);
    fclose(a);
    char str[MAX];
    strcpy(str,cJSON_CreateObject());
    if(fun_user(fileauth,auth2,2))
    {
        strcpy(str,cJSON_AddStringToObject(str,"type","Error"));
        strcpy(str,cJSON_AddStringToObject(str,"content","Authentication error"));
    }
    else if(strcmp(usechannel,"")==0)
    {
        strcpy(str,cJSON_AddStringToObject(str,"type","Error"));
        strcpy(str,cJSON_AddStringToObject(str,"content","The user does not exist in any channel"));
    }
    else
    {
        char address[MAX],channelinfo[MAX];
        sprintf(address,"resources\\channels\\%s.channel.json",usechannel);

        for(int i=0; i<MAX; i++)
        {
            channelinfo[i]=0;
        }
        FILE *a=fopen(address,"r");
        i=0;
        while(!feof(a))
        {
            channelinfo[i++]= (char)fgetc(a);
        }
        fclose(a);

        strcpy(str,cJSON_AddStringToObject(str,"type","Mode"));
        char members[MAX];
        memset(members,0,MAX);
        strcpy(members,cJSON_GetObjectItem(channelinfo,"members"));
        int arraylen = cJSON_GetArraySize(members);

        int k=0;
        for(int i=0; i<arraylen; i++)
        {
            if(strcmp(findmember,cJSON_GetArrayItem(members,i))==0)
            {
                k=1;
                break;
            }
        }
        if(k==1)
            strcpy(str,cJSON_AddStringToObject(str,"content","true"));
        else
            strcpy(str,cJSON_AddStringToObject(str,"content","false"));

    }

    send(client_socket, str, sizeof(str), 0);


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

int  fun_user(const char *a, char user[],int n)
{
    char fea[5],allarray[MAX];
    strcpy(allarray,cJSON_GetObjectItem(a,"users"));

    if(n==1)
        strcpy(fea,"name");
    else if(n==2)
        strcpy(fea,"auth");

    int sizearray= cJSON_GetArraySize(allarray);
    for(int i=0; i<sizearray; i++)
    {
        char us[MAX];
        strcpy(us,cJSON_GetArrayItem(allarray,i));
        if(strcmp(user,cJSON_GetObjectItem(us,fea))==0)
        {
            if(n==2)
            {
                delete_item=i;
                strcpy(useuser,cJSON_GetObjectItem(us,"name"));
                strcpy(usechannel,cJSON_GetObjectItem(us,"channel"));
                char noseen[10];
                strcpy(noseen,cJSON_GetObjectItem(us,"noseen"));
                sscanf(noseen,"%d",&noseen_index);

            }
            return 0;
        }
    }

    return 1;
}

char * cJSON_CreateObject()
{
    char a[5] = "{}";
    memset(newstring,0,MAX);
    strcpy(newstring,a);
    return newstring;
}
char * cJSON_CreateArray()
{
    char  a[5] ="[]";
    memset(newstring,0,MAX);
    strcpy(newstring,a);
    return newstring;
}
char * cJSON_AddStringToObject(const char * str,const char * first, const char * second)
{
    int lengh = strlen(str);
    strcpy(newstring,str);
    if(lengh>2)
        newstring[lengh-1] = ',';
    else
        newstring[lengh-1] ='\0';

    char notmain[MAX];
    sprintf(notmain,"\"%s\":\"%s\"}",first,second);
    strcat(newstring,notmain);
    return newstring;

}
char * cJSON_AddaArrayToObject(const char * str,const char * first, const char * second)
{
    int lengh = strlen(str);
    strcpy(newstring,str);
    if(lengh>2)
        newstring[lengh-1] = ',';
    else
        newstring[lengh-1] ='\0';

    char notmain[MAX];
    sprintf(notmain,"\"%s\":%s}",first,second);
    strcat(newstring,notmain);
    return newstring;

}


char * cJSON_AddNumberToObject(const char * str,const char * first, int second)
{
    int lengh = strlen(str);
    strcpy(newstring,str);
    if(lengh>2)
        newstring[lengh-1] = ',';
    else
        newstring[lengh-1] ='\0';

    char notmain[MAX];
    sprintf(notmain,"\"%s\":%d}",first,second);
    strcat(newstring,notmain);
    return newstring;

}
char * cJSON_AddItemToArray(const char * str,const char * newitem)
{
    int lengh = strlen(str);
    strcpy(newstring,str);
    if(lengh>2)
        newstring[lengh-1] = ',';
    else
        newstring[lengh-1] ='\0';

    char notmain[MAX];
    sprintf(notmain,"%s]",newitem);
    strcat(newstring,notmain);
    return newstring;
}
char * cJSON_GetArrayItem(const char * mainarray, int item)
{
    int k=0;
    int i=1;
    while(k!=item)
    {
        if(mainarray[i]=='{')
        {
            int n=1;
            while(n!=0)
            {
                i++;
                if(mainarray[i]=='{')
                    n++;
                else if(mainarray[i]=='}')
                    n--;
            }
            i++;
        }
        else if(mainarray[i]=='[')
        {
            int n=1;
            while(n!=0)
            {
                i++;
                if(mainarray[i]=='[')
                    n++;
                else if(mainarray[i]==']')
                    n--;
            }
            i++;
        }

        else
        {
            while(mainarray[i]!=',')
                i++;
        }
        i++;
        k++;
    }
    memset(newstring,0,MAX);
    if(mainarray[i]=='{')
    {
        int k=0;
        int n=1;
        while(n!=0)
        {
            newstring[k]=mainarray[k+i];
            k++;
            if(mainarray[k+i]=='{')
                n++;
            else if(mainarray[k+i]=='}')
                n--;
        }
        newstring[k]=mainarray[k+i];
        return newstring;
    }
    else if(mainarray[i]=='[')
    {
        int k=0;
        int n=1;
        while(n!=0)
        {
            newstring[k]=mainarray[k+i];
            k++;
            if(mainarray[k+i]=='[')
                n++;
            else if(mainarray[k+i]==']')
                n--;
        }
        newstring[k]=mainarray[k+i];
        return newstring;
    }

    else
    {
        int n=0;
        while(mainarray[i+n]!=',' && mainarray[i+n]!=']')
        {
            newstring[n] = mainarray[n+i];
            n++;
        }
        return newstring;
    }

}
char * cJSON_DeleteItemFromArray(const char * str, int item)
{
    int len=cJSON_GetArraySize(str);
    char newarr[MAX];
    strcpy(newarr,cJSON_CreateArray());
    for(int i=0; i<len ; i++)
    {
        if(i==item)
            continue;
        char a[MAX];
        strcpy(a,cJSON_GetArrayItem(str,i));
        strcpy(newarr,cJSON_AddItemToArray(newarr,a));
    }
    strcpy(newstring,newarr);
    return newstring;
}


char * cJSON_GetObjectItem(const char * str, const char * first)
{
    int i=0;
    while(1)
    {
        while(str[i]!='"' && str[i]!='}')
            i++;
        if(str[i]=='}')
            return NULL;
        i++;
        int n = 0;
        char name[100];
        memset(name,0,100);
        while(str[n+i]!='"')
        {
            name[n]=str[n+i];
            n++;
        }
        i+=n;
        char newstr[MAX];
        memset(newstr,0,MAX);
        i++;
        while(str[i]!='"' && str[i]!='{' && str[i]!='[' && isdigit(str[i])==0 )
            i++;

        if(str[i]=='"')
        {
            i++;
            int n=0;
            while(str[n+i]!='"')
            {
                newstr[n]=str[n+i];
                n++;
            }
            i+=n;
            i++;
        }

        else if(str[i]=='{')
        {
            int k=1;
            int n=0;
            newstr[0]='{';
            while(k!=0)
            {
                n++;
                newstr[n]=str[n+i];
                if(str[i+n]=='{')
                    k++;
                else if(str[i+n]=='}')
                    k--;
            }
            i+=n;
            i++;
        }
        else if(str[i]=='[')
        {
            int k=1;
            int n=0;
            newstr[0]='[';
            while(k!=0)
            {
                n++;
                newstr[n]=str[n+i];
                if(str[i+n]=='[')
                    k++;
                else if(str[i+n]==']')
                    k--;
            }
            i+=n;
            i++;
        }
        else if(isdigit(str[i]))
        {
            int n=0;
            while(isdigit(str[n+i]))
            {
                newstr[n]=str[n+i];
                n++;
            }
            i+=n;
        }

        if(strcmp(first,name)==0)
        {
            memset(newstring,0,MAX);
            strcpy(newstring,newstr);
            return newstring;
        }

    }
}
char * cJSON_DeleteItemFromObject(const char * str, const char * first)
{
    strcpy(newstring,cJSON_CreateObject());
    int i=0;
    while(1)
    {
        while(str[i]!='"' && str[i]!='}')
            i++;
        if(str[i]=='}')
            break;
        i++;
        int n = 0;
        char name[100];
        memset(name,0,100);
        while(str[n+i]!='"')
        {
            name[n]=str[n+i];
            n++;
        }
        i+=n;
        char newstr[MAX];
        memset(newstr,0,MAX);
        i++;
        while(str[i]!='"' && str[i]!='{' && str[i]!='[' && isdigit(str[i])==0 )
            i++;

        if(str[i]=='"')
        {
            i++;
            int n=0;
            while(str[n+i]!='"')
            {
                newstr[n]=str[n+i];
                n++;
            }
            i+=n;
            i++;
        }

        else if(str[i]=='{')
        {
            int k=1;
            int n=0;
            newstr[0]='{';
            while(k!=0)
            {
                n++;
                newstr[n]=str[n+i];
                if(str[i+n]=='{')
                    k++;
                else if(str[i+n]=='}')
                    k--;
            }
            i+=n;
            i++;
        }
        else if(str[i]=='[')
        {
            int k=1;
            int n=0;
            newstr[0]='[';
            while(k!=0)
            {
                n++;
                newstr[n]=str[n+i];
                if(str[i+n]=='[')
                    k++;
                else if(str[i+n]==']')
                    k--;
            }
            i+=n;
            i++;
        }
        else if(isdigit(str[i]))
        {
            int n=0;
            while(isdigit(str[n+i]))
            {
                newstr[n]=str[n+i];
                n++;
            }
            i+=n;
        }

        if(strcmp(first,name)!=0)
        {
            strcpy(newstring,cJSON_AddStringToObject(newstring,name,newstr));
        }

    }
    return newstring;
}

int cJSON_GetArraySize(const char * mainarray)
{
    int k =1;
    int i =1;
    if(mainarray[1]==']')
        return 0;
    while(true)
    {
        if(mainarray[i]=='{')
        {
            int n=1;
            while(n!=0)
            {
                i++;
                if(mainarray[i]=='{')
                    n++;
                else if(mainarray[i]=='}')
                    n--;
            }
            i++;
        }
        else if(mainarray[i]=='[')
        {
            int n=1;
            while(n!=0)
            {
                i++;
                if(mainarray[i]=='[')
                {
                    n++;
                }
                else if(mainarray[i]==']')
                {
                    n--;
                }
            }
            i++;
        }

        else
        {
            while(mainarray[i]!=',' && mainarray[i]!=']')
                i++;
        }
        if(mainarray[i]==']')
            return k;

        i++;
        k++;
    }
}




