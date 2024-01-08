#include "../h/config.h"

int main(){
    char* url = getenv("QUERY_STRING");
    char nom[100];
    char site[100];
    *site = 0;

    sscanf(url,"nom=%[^&]&site=%s",nom,site);

    if(*url == 0 || !connected(nom)){
        printf("Location: http://www.cgi-scr.com/log/cgi/login.cgi?error=none\n\n");
    }
    else{
        printf("Content-Type: text/html\n\n");
        displayForm(nom,site);
    }
    return 0;
}