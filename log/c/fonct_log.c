#include "../../log/h/log.h"

////////// Utilisateur création
void signInput(char* url){
	char ligne[300];
	
	FILE* file = fopen("../../log/html/sign.html","r");
	
	if(file == NULL){
		fprintf(stderr,"error open file");
		exit(1);
	}
	while(fgets(ligne,300,file)!=NULL){
		if(strstr(ligne,"<h1>")!=NULL && strstr(url,"error=name")!=NULL){
			printf("%s\n",ligne);
			printf("<h3>Cet identifiant est déja pris.</h3>");
		}
		else{
			printf("%s\n",ligne);
		}
	}
	
	fclose(file);
}
void addLog(char* nom, char* passwd){
	FILE* f = fopen("../../log/user/user.txt","a+");
				
	if(f == NULL){
		fprintf(stderr,"error open file");
		exit(1);
	}
				
	fprintf(f,"%s:%s:disconnected\n",nom,passwd);
					
	fclose(f);
}

////////// Utilisateur log
void loginInput(char* url){
	char ligne[300];
	
	FILE* file = fopen("../../log/html/login.html","r");
	
	if(file==NULL){
		fprintf(stderr,"error open file");
		exit(1);
	}
	while(fgets(ligne,300,file)!=NULL){
		if (strstr(ligne,"<h1>")!=NULL && strstr(url,"error=")==NULL && *url!=0 && strstr(url,"disconnect=")==NULL){
			printf("%s\n",ligne);
			printf("<h3 id=\"succes\">Registration à succès.</h3>");
		}
		else if (strstr(ligne,"<h1>")!=NULL && strstr(url,"error=incorrect")!=NULL){
			printf("%s\n",ligne);
			printf("<h3>Nom ou mot de passe incorrecte.</h3>");
		}
		else if (strstr(ligne,"<h1>")!=NULL && strstr(url,"error=none")!=NULL){
			printf("%s\n",ligne);
			printf("<h3>Connectez vous à votre identifiant.</h3>");
		}
		else{
			printf("%s",ligne);
		}
	}
	
	fclose(file);
}
int countUser(){
	int c = 0;
	char ligne[300];
	
	FILE* file = fopen("../../log/user/user.txt","a+");

	if(file==NULL){
		fprintf(stderr,"error open file");
		exit(1);
	}
	while(fgets(ligne,300,file)!=NULL){
		c++;
	}
	fclose(file);
	return c;
}
User* getUsers(){
	int c = countUser();
	int i=0;
	char ligne[300];
	User* users = malloc(c*sizeof(User));
	
	FILE* file = fopen("../../log/user/user.txt","a+");
	
	if(file==NULL){
		fprintf(stderr,"error open file");
		exit(1);
	}
	while(fgets(ligne,300,file)!=NULL){
		sscanf(ligne,"%[^:]:%[^:]:%s",users[i].name,users[i].pswd,users[i].connex);
		i++;
	}
	fclose(file);
	return users;
}
bool inUsers(char* nom){
	int c=countUser();
	User* users = malloc(c*sizeof(User));
	users = getUsers();
	
	for(int i=0; i<c; i++){
		if(strcmp(nom,users[i].name)==0){
			return true;
		}
	}
	return false;
}
bool verifyUser(char* nom, char* passwd){
	int c=countUser();
	User* users = malloc(c*sizeof(User));
	users = getUsers();
	
	for(int i=0; i<c; i++){
		if(strcmp(nom,users[i].name)==0 && strcmp(passwd,users[i].pswd)==0){
			return true;
		}
	}
	return false;
}

/////////// Connection
bool connected(char* nom){
	FILE* file = fopen("../../log/user/user.txt","a+");
	char ligne[300];
	
	if(file==NULL){
		fprintf(stderr,"error open file");
		exit(1);
	}
	while(fgets(ligne,300,file)!=NULL){
		if(strstr(ligne,nom)!=NULL && strstr(ligne,":connected")!=NULL){
			return true;
		}
	}
	return false;
}
void connection(char* nom){
	int n_user = countUser();
	User* users = malloc(n_user*sizeof(User));
	users = getUsers();
		
	for(int i=0; i<n_user;i++){
		if(strcmp(nom,users[i].name)==0){
			strcpy(users[i].connex,"connected");
		}
	}
		
	FILE* file = fopen("../../log/user/user.txt","w");
	for(int i=0; i<n_user;i++){
		fprintf(file,"%s:%s:%s\n",users[i].name,users[i].pswd,users[i].connex);
	}
		
	fclose(file);

	printf("Location: http://www.cgi-scr.com/user/cgi/infoUser.cgi?nom=%s\n\n",nom);
}
void deconnection(char* nom){
	int n_user = countUser();
	User* users = malloc(n_user*sizeof(User));
	users = getUsers();
			
	for(int i=0; i<n_user;i++){
		if(strcmp(nom,users[i].name)==0){
			strcpy(users[i].connex,"disconnected");
		}
	}
			
	FILE* file = fopen("../../log/user/user.txt","w+");

	if(file == NULL){
		fprintf(stderr,"error open file");
		exit(1);
	}	
	for(int i=0; i<n_user;i++){
		fprintf(file,"%s:%s:%s\n",users[i].name,users[i].pswd,users[i].connex);
	}
			
	fclose(file);
}

/// Vertical nav bar

void barNav(char* nom){
	printf("<section id=\"bar\">\n");
	printf("<hr><div id=\"item\"><div class=\"identifier one\"><abbr title=\"User : %s\">%c</abbr></div><div class=\"identifier two\">%s</div></div><hr>",nom,toupper(nom[0]),nom);
	printf("<a href=\"../../user/cgi/infoUser.cgi?nom=%s\"><button><i class=\"fa-solid fa-user\"> View User information</i></button></a><br>\n",nom);
	printf("<a href=\"../../auth.log/cgi/auth.cgi?nom=%s&user=allusers\"><button><i class=\"fa-solid fa-file-circle-check\"> View auth.log file</i></button></a><br>\n",nom);
	printf("<a href=\"../../config/cgi/config.cgi?nom=%s\"><button><i class=\"fa-solid fa-server\"> Configure Web Site</i></button></a><br>\n",nom);
	printf("<a href=\"../../activation/cgi/site.cgi?nom=%s\"><button><i class=\"fa-solid fa-sitemap\"> Activation Web Site</i></button></a><br>\n",nom);
	printf("<a href=\"../../access.log/cgi/access.cgi?nom=%s\"><button><i class=\"fa-solid fa-eye\"> Access Web Site</i></button></a><br>\n",nom);
	printf("<hr><a href=\"../../log/cgi/login.cgi?disconnect=%s\"><button class=\"submit\">Log out</button></a>",nom);
	printf("</section>\n");
}
