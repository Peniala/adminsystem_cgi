#include "../h/log.h"

int main(){
	printf("Content-Type: text/html\n\n");
	
	char* url = getenv("QUERY_STRING");
	signInput(url);
	
	return 0;
}
