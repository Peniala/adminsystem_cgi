#ifndef ACCESS
#define ACCESS
#define LINE_SHOWN 10

#include "../../log/h/log.h"

typedef struct{
    char ip[20];
    char date[60];
    char task[150];
    char visit[200];
    char plateforme[150];
}Access;

char* getConf();
char* getFile(char* conf);
int countVisit(char* conf);
Access* getVisit(char* conf);
bool snameCommented(char* conf);
char* getSitebyConf(char* conf);
void selectionConf(char* nom);
void accessTable(char* conf,int page);
int getNum();
void lienPagination(int c,char* nom,char* conf);

#endif