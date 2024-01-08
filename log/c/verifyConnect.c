#include "../h/log.h"

int main(){
	
    char nom[100];
    char passwd[500];
    char* auth = getenv("QUERY_STRING");
    
    sscanf(auth,"%*[^=]=%[^&]&%*[^=]=%[^&]",nom,passwd);
	if(!verifyUser(nom,passwd)){
    	printf("Location: http://www.cgi-scr.com/log/cgi/login.cgi?error=incorrect\n\n");
	}
	else{
		connection(nom);
	}
}
