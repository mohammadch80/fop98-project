#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#define MAX 100000
char newstring[MAX];

char * cJSON_CreateObject();
char * cJSON_CreateArray();
char * cJSON_AddStringToObject(const char * str,const char * first, const char * second);
char * cJSON_AddNumberToObject(const char * str,const char * first, int second);
char * cJSON_AddItemToArray(const char * str,const char * newitem);
char * cJSON_GetArrayItem(const char * mainarray, int item);
char * cJSON_GetObjectItem(const char * str, const char * first);
char * cJSON_DeleteItemFromObject(const char * str, const char * first);
char * cJSON_DeleteItemFromArray(const char * str, int n);
int cJSON_GetArraySize(const char * mainarray);

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
    printf("first %s  %d",str,lengh);
    strcpy(newstring,str);
    if(lengh>2)
        newstring[lengh-1] = ',';
    else
        newstring[lengh-1] ='\0';

    char notmain[MAX];
    memset(notmain,0,MAX);
    sprintf(notmain,"%s]",newitem);
    strcat(newstring,notmain);

    printf("add item %s\n",newstring);
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
char * cJSON_DeleteItemFromArray(const char * str, int item){
    int len=cJSON_GetArraySize(str);
    char newarr[MAX];
    strcpy(newarr,cJSON_CreateArray());
    for(int i=0; i<len ; i++){
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
                if(mainarray[i]=='['){
                    n++;
                }
                else if(mainarray[i]==']'){
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
int main(){

    return 0;
}















