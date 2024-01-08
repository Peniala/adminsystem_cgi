#include "../h/infoUser.h"

void getInfoUser(){
	Users* users = malloc(60*sizeof(User));

	FILE* file=fopen("/etc/passwd","r");
	if(file==NULL){
		exit(1);
	}
	
	char ligne[250];
	char x[20];
	int i=0;
	
	printf("<table id=\"user\">");
	printf("<tr id=\"en-tete\">");
	printf("<td>Name</td>");
	printf("<td>UID</td>");
	printf("<td>GID</td>");
	printf("<td>Directory</td>");		
	printf("<td>Command Shell</td>");
	printf("</tr>");
	
	while(fgets(ligne,250,file)!=NULL){
		sscanf(ligne,"%[^:]:%[^:]:%d:%d:%*[^:]:%[^:]:%s\n",users[i].name,x,&users[i].uid,&users[i].gid,users[i].direct,users[i].shell);
		
		if(users[i].uid >= 1000 && users[i].uid <= 1100){
			printf("<tr>");
			printf("<td>%s</td>",users[i].name);
			printf("<td>%d</td>",users[i].uid);
			printf("<td>%d</td>",users[i].gid);
			printf("<td>%s</td>",users[i].direct);		
			printf("<td>%s</td>",users[i].shell);
			printf("</tr>");
		}
		
		i++;
	}
	printf("</table>");
	fclose(file);
	return;
}
