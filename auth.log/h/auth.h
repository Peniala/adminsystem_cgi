#ifndef AUTHLOG
#define AUTHLOG
#define LINE_SHOWN 10
#include "../../log/h/log.h"

////////// Page auth.log
void authLogPage(char* nom,char* u,Session* s,int c,int page);

////////// Session
char* getUser();
bool searchUser(Session* s,int c,char* u);
int countSession(char* u);
Session* getSession(char* u);

////////// Date
bool isB(int y);
int intMois(char* m);
int jourDuMois(int m,int y);
char* volana(char* m);
char* andro(int y,char* m,int jour);

////////// Tableau
void sessionTable(char* nom,Session*s,int c,int page);

////////// Pagination
int getNum();
void lienPagination(int c,char* nom,char* u);

////////// Bar de navigation
char* caseChange(char* ch);
void displayBar(char* nom);

#endif