#ifndef LOG
#define LOG
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

typedef struct {
    char mois[5];
    int jour;
    char heure[30];
    char session[20];
    char user[30];
} Session;

typedef struct{
	char name[100];
	char pswd[200];
	char connex[200];
}User;

////////// Utilisateur création
void signInput(char* url);
void addLog(char* nom, char* passwd);

////////// Utilisateur log
void loginInput(char* url);
int countUser();
User* getUsers();
bool inUsers(char* nom);
bool verifyUser(char* nom, char* passwd);

/////////// Connection
bool connected(char* nom);
void connection(char* nom);
void deconnection(char* nom);

/////////// Bar de navigation vertical
void barNav(char* nom);

#endif
