#include "../h/config.h"

int main(){
    char* url = getenv("QUERY_STRING");
    char nom[100];
    sscanf(url,"nom=%[^&]",nom);
    VHost vh = getV_host(url);
    setV_host(vh);
    modeActive(vh);
    addToHosts(vh.sname);
    printf("Location: http://www.cgi-scr.com/config/cgi/config.cgi?nom=%s&site=%s\n\n",nom,vh.sname);
    return 0;
}