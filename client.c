#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <winsock2.h>
#include <windows.h>
#include <dos.h>
#include <dir.h>
#include "cJSON.h"
#include "cJSON.c"

#define MAX 2047
#define PORT 12345
#define SA struct sockaddr

int pointer_place;

char buffer[MAX];
struct sockaddr_in servaddr;
int client_socket;
char *authtoken;
void menu_karbari();
void menu_asli(const char * user);
void menu_goftogoo(const char * chaname);
void connection();

void clear()
{
    system("@cls||clear");
}

int main(int argc, char *argv[])
{
    int  server_socket;
    struct sockaddr_in cli;

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
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1)
    {
        SetColor(12);
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
    if (connect(client_socket, (SA*)&servaddr, sizeof(servaddr)) != 0)
    {
        SetColor(12);
        printf("Connection to the server failed...\n");
        exit(0);
    }
    else
        printf("Successfully connected to the server..\n");
    clear();
    pointer_place =1;
    menu_karbari();

    closesocket(client_socket);

    return 0;
}
void menu_karbari()
{
    while (true)
    {
        int number=0;
        char password[80], username[80];
        SetColor(YELLOW);
        printf("\n________________________________\n");
        if(pointer_place==1)
            printf("-> Sign Up\n   Login\n\n");
        else
            printf("   Sign Up\n-> Login\n\n");

        //change pointer
        int ch1, ch2;
        ch1 = getch();
        ch2 = 0;
        clear();
        if (ch1 == 0xE0)   // a scroll key was pressed
        {
            ch2=getch();
            if(ch2==72 && pointer_place==2)
                pointer_place=1;
            if(ch2==80 && pointer_place==1)
                pointer_place=2;

        }
        else if(ch1==13)
        {
            number = pointer_place;
        }


        client_socket = socket(AF_INET, SOCK_STREAM, 0);
        if (client_socket == -1)
        {
            SetColor(12);
            printf("Socket creation failed...\n");
            exit(0);
        }
        memset(buffer, 0, sizeof(buffer));
        if(number==1)
        {
            clear();
            SetColor(YELLOW);
            printf("\nSign Up\n\nUsername :  ");
            SetColor(WHITE);
            gets(username);
            SetColor(YELLOW);
            printf("Password :  ");
            SetColor(WHITE);
            gets(password);
            sprintf(buffer,"register %s, %s\n",username,password);
            connection();
            send(client_socket, buffer, sizeof(buffer), 0);
            memset(buffer, 0, sizeof(buffer));
            recv(client_socket, buffer, sizeof(buffer), 0);
            closesocket(client_socket);
            cJSON * recieved =cJSON_Parse(buffer);
            if(strcmp(cJSON_GetObjectItem(recieved,"type")->valuestring,"Error")==0)
            {
                SetColor(12);
                printf("\n%s\n\n",cJSON_GetObjectItem(recieved,"content")->valuestring);
            }
            else
            {
                clear();
                SetColor(10);
                printf("\n... successful Sign Up with \"%s\"...\n",username);
            }

        }
        if(number ==2)
        {
            clear();
            SetColor(YELLOW);
            printf("\nLogin\n\nUsername :  ");
            SetColor(WHITE);
            gets(username);
            SetColor(YELLOW);
            printf("Password :  ");
            SetColor(WHITE);
            gets(password);
            sprintf(buffer,"login %s, %s\n",username,password);
            connection();
            send(client_socket, buffer, sizeof(buffer), 0);

            memset(buffer, 0, sizeof(buffer));

            recv(client_socket, buffer, sizeof(buffer), 0);
            closesocket(client_socket);
            cJSON * recieved =cJSON_Parse(buffer);
            if(strcmp(cJSON_GetObjectItem(recieved,"type")->valuestring,"Error")!=0)
            {
                authtoken=cJSON_GetObjectItem(recieved,"content")->valuestring;
                clear();
                const char * user = username;
                SetColor(10);
                printf("\n... successful Login with \"%s\" ...\n",user);
                pointer_place=1;
                menu_asli(user);
            }
            else
            {
                SetColor(12);
                printf("\n%s\n\n",cJSON_GetObjectItem(recieved,"content")->valuestring);
            }

        }

    }
}
void menu_asli(const char * user)
{
    char channel_name[80];
    while(true)
    {
        int number=0;
        SetColor(11);
        printf("\n________________________________\n");
        if(pointer_place==1)
            printf("-> Create Channel\n   Join Channel\n   Logout\n\n");
        else if(pointer_place==2)
            printf("   Create Channel\n-> Join Channel\n   Logout\n\n");
        else if(pointer_place==3)
            printf("   Create Channel\n   Join Channel\n-> Logout\n\n");

        //change pointer
        int ch1, ch2;
        ch1 = getch();
        ch2 = 0;
        clear();
        if (ch1 == 0xE0)   // a scroll key was pressed
        {
            ch2=getch();
            if(ch2==72 && pointer_place==2)
                pointer_place=1;
            else if(ch2==80 && pointer_place==1)
                pointer_place=2;
            else if(ch2==72 && pointer_place==3)
                pointer_place=2;
            else if(ch2==80 && pointer_place==2)
                pointer_place=3;

        }
        else if(ch1==13)
        {
            number = pointer_place;
        }


        client_socket = socket(AF_INET, SOCK_STREAM, 0);
        if (client_socket == -1)
        {
            SetColor(12);
            printf("Socket creation failed...\n");
            exit(0);
        }
        memset(buffer, 0, sizeof(buffer));

        if(number==1)
        {
            clear();
            SetColor(11);
            printf("\nCreat a new channel\n\nChannel name :  ");
            SetColor(WHITE);
            gets(channel_name);
            sprintf(buffer,"create channel %s, %s\n",channel_name,authtoken);
            connection();
            send(client_socket, buffer, sizeof(buffer), 0);

            memset(buffer, 0, sizeof(buffer));

            recv(client_socket, buffer, sizeof(buffer), 0);
            closesocket(client_socket);

            cJSON * recieved =cJSON_Parse(buffer);
            if(strcmp(cJSON_GetObjectItem(recieved,"type")->valuestring,"Error")!=0)
            {
                clear();
                const char *chaname = channel_name;
                SetColor(10);
                printf("\n... You created and joined \"%s\" ...\n",channel_name);
                pointer_place=1;
                menu_goftogoo(chaname);
            }
            else
            {
                SetColor(12);
                printf("\n%s\n\n",cJSON_GetObjectItem(recieved,"content")->valuestring);
            }

        }
        if(number==2)
        {
            clear();
            SetColor(11);
            printf("\nJoin a channel\n\nChannel name :  ");
            SetColor(WHITE);
            gets(channel_name);
            sprintf(buffer,"join channel %s, %s\n",channel_name,authtoken);
            connection();
            send(client_socket, buffer, sizeof(buffer), 0);
            memset(buffer, 0, sizeof(buffer));
            recv(client_socket, buffer, sizeof(buffer), 0);
            closesocket(client_socket);
            cJSON * recieved =cJSON_Parse(buffer);
            if(strcmp(cJSON_GetObjectItem(recieved,"type")->valuestring,"Error")!=0)
            {
                clear();
                const char *chaname = channel_name;
                SetColor(10);
                printf("\n... You joined \"%s\" ...\n",channel_name);
                pointer_place=1;
                menu_goftogoo(chaname);
            }
            else
            {
                SetColor(12);
                printf("\n%s\n\n",cJSON_GetObjectItem(recieved,"content")->valuestring);
            }
        }
        if(number == 3)
        {
            int pointer_p1=1;
            while(true)
            {
                clear();
                printf("\n________________________________\nAre you sure to logout?\n");
                if(pointer_p1==1)
                    printf("-> No\n   Yes\n\n");
                else
                    printf("   No\n-> Yes\n\n");

                //change pointer
                int ch1, ch2;
                ch1 = getch();
                ch2 = 0;
                clear();
                if (ch1 == 0xE0)   // a scroll key was pressed
                {
                    ch2=getch();
                    if(ch2==72 && pointer_p1==2)
                        pointer_p1=1;
                    if(ch2==80 && pointer_p1==1)
                        pointer_p1=2;

                }
                else if(ch1==13)
                {
                    break;
                }
            }
            if(pointer_p1==1)
                continue;

            clear();
            sprintf(buffer,"logout %s\n",authtoken);
            connection();
            send(client_socket, buffer, sizeof(buffer), 0);
            memset(buffer, 0, sizeof(buffer));
            recv(client_socket, buffer, sizeof(buffer), 0);
            closesocket(client_socket);
            cJSON * recieved =cJSON_Parse(buffer);
            if(strcmp(cJSON_GetObjectItem(recieved,"type")->valuestring,"Error")!=0)
            {
                SetColor(10);
                printf("\n... Successful Logout with \"%s\" ...\n",user);
                pointer_place=1;
                return ;
            }
            else
            {
                SetColor(12);
                printf("\n%s\n\n",cJSON_GetObjectItem(recieved,"content")->valuestring);
            }

        }

    }
}

void menu_goftogoo(const char * chaname)
{
    while(true)
    {
        int number=0;
        SetColor(13);
        printf("\n________________________________\n");
        if(pointer_place==1)
            printf("-> Send Message\n   Refresh\n   Search In Messages\n   Channel Members\n   Find Member\n   Leave Channel\n\n");
        else if(pointer_place==2)
            printf("   Send Message\n-> Refresh\n   Search In Messages\n   Channel Members\n   Find Member\n   Leave Channel\n\n");
        else if(pointer_place==3)
            printf("   Send Message\n   Refresh\n-> Search In Messages\n   Channel Members\n   Find Member\n   Leave Channel\n\n");
        else if(pointer_place==4)
            printf("   Send Message\n   Refresh\n   Search In Messages\n-> Channel Members\n   Find Member\n   Leave Channel\n\n");
        else if(pointer_place==5)
            printf("   Send Message\n   Refresh\n   Search In Messages\n   Channel Members\n-> Find Member\n   Leave Channel\n\n");
        else if(pointer_place==6)
            printf("   Send Message\n   Refresh\n   Search In Messages\n   Channel Members\n   Find Member\n-> Leave Channel\n\n");



        //change pointer
        int ch1, ch2;
        ch1 = getch();
        ch2 = 0;
        clear();
        if (ch1 == 0xE0)   // a scroll key was pressed
        {
            ch2=getch();
            if(ch2==72 && pointer_place==2)
                pointer_place=1;
            else if(ch2==80 && pointer_place==1)
                pointer_place=2;
            else if(ch2==80 && pointer_place==2)
                pointer_place=3;
            else if(ch2==72 && pointer_place==3)
                pointer_place=2;
            else if(ch2==72 && pointer_place==4)
                pointer_place=3;
            else if(ch2==80 && pointer_place==3)
                pointer_place=4;
            else if(ch2==80 && pointer_place==4)
                pointer_place=5;
            else if(ch2==80 && pointer_place==5)
                pointer_place=6;
            else if(ch2==72 && pointer_place==6)
                pointer_place=5;
            else if(ch2==72 && pointer_place==5)
                pointer_place=4;

        }
        else if(ch1==13)
        {
            number = pointer_place;
        }

        memset(buffer, 0, sizeof(buffer));
        client_socket = socket(AF_INET, SOCK_STREAM, 0);
        if (client_socket == -1)
        {
            SetColor(RED);
            printf("Socket creation failed...\n");
            exit(0);
        }
        if(number==1)
        {
            clear();
            SetColor(9);
            printf("\n... Write your message to \"%s\" ...\n\n",chaname);
            char message[255];
            memset(message, 0, sizeof(message));
            SetColor(WHITE);
            gets(message);
            sprintf(buffer,"send %s, %s\n",message,authtoken);
            connection();
            send(client_socket, buffer, sizeof(buffer), 0);
            memset(buffer, 0, sizeof(buffer));
            recv(client_socket, buffer, sizeof(buffer), 0);
            cJSON * recieved =cJSON_Parse(buffer);
            closesocket(client_socket);
            if(strcmp(cJSON_GetObjectItem(recieved,"type")->valuestring,"Error")!=0)
            {
                clear();
                SetColor(10);
                printf("\n... Successful send to \"%s\" ...\n",chaname);
                pointer_place=1;
            }
            else
            {
                SetColor(12);
                printf("\n%s\n\n",cJSON_GetObjectItem(recieved,"content")->valuestring);
            }
        }

        if(number==2)
        {
            clear();
            sprintf(buffer,"refresh %s\n",authtoken);
            connection();
            send(client_socket, buffer, sizeof(buffer), 0);
            memset(buffer, 0, sizeof(buffer));
            recv(client_socket, buffer, sizeof(buffer), 0);
            closesocket(client_socket);
            cJSON * recieved = cJSON_Parse( buffer );

            if(strcmp(cJSON_GetObjectItem(recieved,"type")->valuestring,"Error")!=0)
            {
                cJSON * allarray = cJSON_GetObjectItem(recieved, "content");
                int arraylen = cJSON_GetArraySize(allarray);
                SetColor(9);
                printf("... Messages in \"%s\" ...\n\n",chaname);
                SetColor(WHITE);
                for(int i=0; i<arraylen; i++)
                {
                    cJSON * a= cJSON_GetArrayItem(allarray,i);
                    printf("from: %s\nmessage: %s\n\n",cJSON_GetObjectItem(a,"sender")->valuestring,cJSON_GetObjectItem(a,"content")->valuestring);
                }
                pointer_place=1;
            }
            else
            {
                SetColor(12);
                printf("\n%s\n\n",cJSON_GetObjectItem(recieved,"content")->valuestring);
            }
        }

        if(number==3)
        {
            clear();
            SetColor(9);
            printf("\n... Write your word to search in \"%s\" ...\n\n",chaname);
            char message[255];
            memset(message, 0, sizeof(message));
            SetColor(WHITE);
            gets(message);
            sprintf(buffer,"search in messages %s, %s\n",message,authtoken);
            connection();
            send(client_socket, buffer, sizeof(buffer), 0);
            memset(buffer, 0, sizeof(buffer));
            recv(client_socket, buffer, sizeof(buffer), 0);
            cJSON * recieved =cJSON_Parse(buffer);
            closesocket(client_socket);
            if(strcmp(cJSON_GetObjectItem(recieved,"type")->valuestring,"Error")!=0)
            {
                clear();

                cJSON * allarray = cJSON_GetObjectItem(recieved, "content");
                int arraylen = cJSON_GetArraySize(allarray);
                SetColor(9);
                printf("... Messages in \"%s\" with \"%s\" ...\n\n",chaname,message);
                SetColor(WHITE);
                for(int i=0; i<arraylen; i++)
                {
                    cJSON * a= cJSON_GetArrayItem(allarray,i);
                    printf("from: %s\nmessage: %s\n\n",cJSON_GetObjectItem(a,"sender")->valuestring,cJSON_GetObjectItem(a,"content")->valuestring);
                }
                pointer_place=1;

            }
            else
            {
                SetColor(12);
                printf("\n%s\n\n",cJSON_GetObjectItem(recieved,"content")->valuestring);
            }
        }

        if(number==4)
        {
            clear();
            sprintf(buffer,"channel members %s\n",authtoken);
            connection();
            send(client_socket, buffer, sizeof(buffer), 0);
            memset(buffer, 0, sizeof(buffer));
            recv(client_socket, buffer, sizeof(buffer), 0);
            closesocket(client_socket);
            cJSON * recieved =cJSON_Parse(buffer);
            if(strcmp(cJSON_GetObjectItem(recieved,"type")->valuestring,"Error")!=0)
            {
                cJSON * allarray = cJSON_GetObjectItem(recieved, "content");
                int arraylen = cJSON_GetArraySize(allarray);
                SetColor(9);
                printf("... members in \"%s\" ...\n\n",chaname);
                SetColor(WHITE);
                for(int i=0; i<arraylen; i++)
                {
                    printf("member %d: %s\n",i+1,cJSON_GetArrayItem(allarray,i)->valuestring);
                }
                pointer_place=1;
            }

            else
            {
                SetColor(12);
                printf("%s\n\n",cJSON_GetObjectItem(recieved,"content")->valuestring);
            }


        }

        if(number==5)
        {
            clear();
            SetColor(9);
            printf("\n... Write your username to find in \"%s\" ...\n\n",chaname);
            char findmember[255];
            memset(findmember, 0, sizeof(findmember));
            SetColor(WHITE);
            gets(findmember);
            sprintf(buffer,"find member %s, %s\n",findmember,authtoken);
            connection();
            send(client_socket, buffer, sizeof(buffer), 0);
            memset(buffer, 0, sizeof(buffer));
            recv(client_socket, buffer, sizeof(buffer), 0);
            cJSON * recieved =cJSON_Parse(buffer);
            closesocket(client_socket);
            if(strcmp(cJSON_GetObjectItem(recieved,"type")->valuestring,"Error")!=0)
            {
                clear();
                SetColor(9);
                char * isthere = cJSON_GetObjectItem(recieved, "content")->valuestring;
                if(strcmp(isthere,"true")==0)
                    printf("\n  %s is in %s\n",findmember,chaname);
                else
                    printf("\n  %s is not in %s\n",findmember,chaname);

                pointer_place=1;

            }
            else
            {
                SetColor(12);
                printf("\n%s\n\n",cJSON_GetObjectItem(recieved,"content")->valuestring);
            }
        }


        if(number==6)
        {
            int pointer_p1=1;
            while(true)
            {
                clear();
                printf("\n________________________________\nAre you sure to leave the channel?\n");
                if(pointer_p1==1)
                    printf("-> No\n   Yes\n\n");
                else
                    printf("   No\n-> Yes\n\n");

                //change pointer
                int ch1, ch2;
                ch1 = getch();
                ch2 = 0;
                clear();
                if (ch1 == 0xE0)   // a scroll key was pressed
                {
                    ch2=getch();
                    if(ch2==72 && pointer_p1==2)
                        pointer_p1=1;
                    if(ch2==80 && pointer_p1==1)
                        pointer_p1=2;

                }
                else if(ch1==13)
                {
                    break;
                }
            }
            if(pointer_p1==1)
                continue;

            clear();
            sprintf(buffer,"leave %s\n",authtoken);
            connection();
            send(client_socket, buffer, sizeof(buffer), 0);
            memset(buffer, 0, sizeof(buffer));
            recv(client_socket, buffer, sizeof(buffer), 0);
            closesocket(client_socket);
            cJSON * recieved =cJSON_Parse(buffer);
            if(strcmp(cJSON_GetObjectItem(recieved,"type")->valuestring,"Error")!=0)
            {
                SetColor(10);
                printf("\n... successful leave \"%s\" ...\n",chaname);
                pointer_place=1;
                return;
            }
            else
            {
                SetColor(12);
                printf("%s\n\n",cJSON_GetObjectItem(recieved,"content")->valuestring);
            }
        }

    }
}
void connection()
{
    if (connect(client_socket, (SA*)&servaddr, sizeof(servaddr)) != 0)
    {
        SetColor(12);
        printf("Connection to the server failed...\n");
        exit(0);
    }
}
void SetColor(int ForgC)
{
    WORD wColor;

    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;

    //We use csbi for the wAttributes word.
    if(GetConsoleScreenBufferInfo(hStdOut, &csbi))
    {
        //Mask out all but the background attribute, and add in the forgournd color
        wColor = (csbi.wAttributes & 0xF0) + (ForgC & 0x0F);
        SetConsoleTextAttribute(hStdOut, wColor);
    }
    return;
}

