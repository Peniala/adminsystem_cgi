#ifndef LISTE
#define LISTE
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../../log/h/log.h"

typedef struct{
    char name[100];
    char descri[20];
}Site;

int countSite();
bool isActive(char* site);
Site* getSite();
void typeActivation(char* site, char* type);
void siteList(char* nom);

#endif