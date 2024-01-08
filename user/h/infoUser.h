#ifndef USER
#define USER

#include "../../log/h/log.h"

typedef struct{
		char name[30];
		int uid;
		int gid;
		char direct[50];
		char shell[30];
}Users;

void getInfoUser();
#endif
