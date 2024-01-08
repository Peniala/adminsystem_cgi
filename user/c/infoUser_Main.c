#include "../h/infoUser.h"
int main(){

	char* url = getenv("QUERY_STRING");
	char nom[100];

	sscanf(url,"nom=%s",nom);

	if(*url == 0 ||!connected(nom)){
		printf("Location: http://www.cgi-scr.com/log/cgi/login.cgi?error=none\n\n");
	}
	else{
		printf("Content-Type: text/html\n\n");
		FILE* file = fopen("../html/infoUser.html","r");
		char line[300];

		if(file == NULL){
			fprintf(stderr,"error file open");
			exit(1);
		}
		while (fgets(line,300,file) != NULL){
			if(strstr(line,"<body>") != NULL){
				printf("%s\n",line);
				barNav(nom);
			}
			else if(strstr(line,"<h1>") != NULL){
				printf("%s\n",line);
				getInfoUser();
			}
			else{
				printf("%s\n",line);
			}
		}
		fclose(file);
	}  
	return 0;
}
