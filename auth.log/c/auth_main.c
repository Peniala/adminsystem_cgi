#include "../h/auth.h"

int main(){
    char nom[100];
    char* auth = getenv("QUERY_STRING");
    sscanf(auth,"nom=%[^&]",nom);
    
    if(*auth==0 || !connected(nom)){
    	printf("Location: http://www.cgi-scr.com/log/login.cgi?error=none\n\n");
		//printf("Location: http://www.cgi-script.com/l/login.cgi?error=none\n\n");		// Tsy mitovy
	}
	else{
		printf("Content-Type: text/html\n\n");
    
		char* u = malloc(100*sizeof(char));
		strcpy(u,getUser());

		int c = countSession(u);
		int page = getNum();

		Session * s = malloc(c*sizeof(Session));
		s = getSession(u);
		
		authLogPage(nom,u,s,c,page);
	}
    return 0;
}
