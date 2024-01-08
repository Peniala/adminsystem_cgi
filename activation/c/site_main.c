#include "../h/site.h"

int main(){
    char* url = getenv("QUERY_STRING");
    char nom[100];
    char sname[100];
    char type[20];
    
    sscanf(url,"nom=%[^&]&sname=%[^&]&type=%s",nom,sname,type);

    if(*sname != 0 && *type != 0){
        typeActivation(sname,type);
    }

    if(*url == 0 || !connected(nom)){
        printf("Location: http://www.cgi-scr.com/log/cgi/login.cgi?error=none\n\n");
    }
    else{
        printf("Content-Type: text/html\n\n");

        siteList(nom);
    }
    return 0;
}