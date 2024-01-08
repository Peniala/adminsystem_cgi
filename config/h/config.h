#ifndef CONFIG
#define CONFIG
#include "../../log/h/log.h"

typedef struct{
    int index;
    int exec;
    int link;
}Options;
typedef struct{
    int py;
    int cgi;
    int pl;
}Addhandler;
typedef struct{
    char vname[50];
    char sname[50];
    char droot[100];
    char scalias[100];
    Options opt;
    Addhandler add;
    char allow[20];
}VHost;

char* transformString(char* s);
VHost getV_host(char* url);
void setV_host(VHost vh);
void activeCGI();
void activeSite(char* conf);
void modeActive(VHost vh);
void addToHosts(char* sname);
void displayForm(char* nom,char* site);

#endif