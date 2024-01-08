#include "../h/access.h"

int main(){
    char nom[100];
    char* url = getenv("QUERY_STRING");
    sscanf(url,"nom=%[^&]",nom);
    
    if(*url==0 || !connected(nom)){
    	printf("Location: http://www.cgi-scr.com/log/login.cgi?error=none\n\n");
	}
	else{
        printf("Content-Type: text/html\n\n");

        FILE* file = fopen("../html/access.html","r");
        char line[300];

        if(file == NULL){
            printf("error file open\n");
            exit(1);
        }
        while(fgets(line,300,file) != NULL){
            if(strstr(line,"<body>") != NULL){
                printf("%s\n",line);
                barNav(nom);
            }
            else if(strstr(line,"class=\"theme\"") != NULL){
                printf("%s\n",line);
                selectionConf(nom);
                
                char* conf = getConf();

                if(*conf != 0){
                    int p = getNum();
                    accessTable(conf,p);
                    lienPagination(countVisit(conf),nom,conf);
                }
                
            }
            else{
                printf("%s\n",line);
            }
        }
        fclose(file);
    }
    return 0;
}