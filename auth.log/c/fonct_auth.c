#include "../h/auth.h"

////////// Page auth.log

void authLogPage(char* nom,char* u,Session*s,int c,int page){
	char ligne[500];
	FILE * file = fopen("../html/auth.html","r");
    
	if(file == NULL){
		printf("error");
		exit(0);
	}
		
	while(fgets(ligne,500,file) != NULL){
		if(strstr(ligne,"<body>") != NULL){
			printf("%s\n",ligne);
			barNav(nom);
		}
		else if(strstr(ligne,"class=\"theme\"") != NULL){
			printf("%s\n",ligne);
				
			displayBar(nom);
			if(*u!=0){
				if(searchUser(s,c,u)) {
					printf("<table id=\"auth\"><tr id=\"en-tete\"><td>Time session</td><td>Type session</td><td>User</td></tr>");
				}
				else printf("<h1>Aucun utilisateur du nom \"%s\" n'a ouvert de session</h1>\n",u);
						
				sessionTable(nom,s,c,page);
					
				lienPagination(c,nom,u);
			}
		}
		else{
			printf("%s\n",ligne);
		}
	}
	free(s);
	fclose(file);
}

////////// Session
char* getUser(){
	char* t = malloc(100*sizeof(char));
	*t = 0;
	char* u = malloc(100*sizeof(char));
	t = getenv("QUERY_STRING");
	
	if(*t == 0){
		u = t;
	}
	else{
		sscanf(getenv("QUERY_STRING"),"nom=%*[^&]&user=%[^&]",u);
	}
	return u;
}
bool searchUser(Session* s,int c,char* u){
	for(int i=0; i<c; i++){
		if(strstr(caseChange(s[i].user),caseChange(u))!=NULL){
			return true;
		}
	}
	if(strcmp(u,"allusers") == 0){
		return true;
	}
	return false;
}
int countSession(char* u){
    FILE * file = fopen("/var/log/auth.log.1","r");
    int c = 0;
    char ligne[300];
	char* user = malloc(60*sizeof(char));
    while(fgets(ligne,300,file)!=NULL){
		sscanf(ligne,"%*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %s\n",user);	
        if((strstr(ligne,"session opened")!= NULL || strstr(ligne,"session closed") != NULL) && ((strstr(caseChange(user),caseChange(u))!=NULL && *u!=0) || strcmp(u,"allusers") == 0)){
            c++;
        }
    }
    fclose(file);
    return c;
}
Session* getSession(char* u){
    int c = countSession(u);
    Session * s = malloc(c*sizeof(Session));
    int i = 0;
    char ligne[300];
    FILE * file = fopen("/var/log/auth.log.1","r");
	char* user = malloc(60*sizeof(char));
    
    if(file==NULL){
		exit(0);
	}	
    while(fgets(ligne,300,file)!=NULL){
    	sscanf(ligne,"%*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %s\n",user);
        if((strstr(ligne,"session opened") || strstr(ligne,"session closed")) && ((strstr(caseChange(user),caseChange(u))!=NULL && *u!=0)|| strcmp(u,"allusers") == 0)){
			sscanf(ligne,"%s %d %s %*s %*s %*s %*s %s %*s %*s %s\n", s[i].mois, &s[i].jour, s[i].heure, s[i].session, s[i].user);
            i++;
        }
    }
    fclose(file);
    return s;
}

////////// Date
bool isB(int y){
	return (y%4==0 && y%100!=0);
}
int intMois(char* m){
	char mois[12][4] = {"Jan","Fev","Mar","Avr","Mai","Jun","Jui","Aou","Sep","Oct","Nov","Dec"};
	int i=0;
	while(strcmp(mois[i],m)!=0){
		i++;
	}
	return i;
}
int jourDuMois(int m,int y){
	if(m==1){
		if (isB(y)) return 29 ; 
		else return 28 ;
	}
	else if((m%2==0 && m <=6) || (m%2!=0 && m > 6)){
		return 31;
	}
	else{
		return 30;
	}
}
char* volana(char* m){
	int i=0;
	char mois[12][4] = {"Jan","Fev","Mar","Avr","Mai","Jun","Jui","Aou","Sep","Oct","Nov","Dec"};
	char* v[] = {"Janoary","Febroary","Martsa","Aprily","Mey","Jona","Jolay","Aogositra","Septambra","Oktobra","Novambra","Desambra"};
	for(; i<12; i++){
		if(strcmp(mois[i],m)==0){
			break;
		}
	} 
	return v[i];
}
char* andro(int y,char* m,int jour){
	char* weekday[] = {"Alatsinainy","Talata","Alarobia","Alakamisy","Zoma","Sabotsy","Alahady"};
	
	int mo = intMois(m);
	
	int l = 6;
	int k = 0;
	for(int a=2023; a<=y; a++){
		for(int i=0; i<=mo; i++){
			if(i==mo) k=jour ; 
			else k=jourDuMois(i,a);
			for(int j=1 ; j<=k; j++){
				l++;
			}
		}
	}
	l = (l-1)%7;
	return weekday[l];
}

////////// Tableau

void sessionTable(char* nom,Session*s,int c,int page){
	char class[50];
	for(int i=page*LINE_SHOWN; i<page*LINE_SHOWN+LINE_SHOWN && i<c ;i++){
		if(strcmp(s[i].session,"opened")==0){
			strcpy(class,"class=\"open\"");
		}
		else{
			strcpy(class,"class=\"close\"");
		}
		printf("<tr %s>\n",class);						
		printf("\t<td>%s %d %s %s </td>\n",andro(2023,s[i].mois,s[i].jour),s[i].jour,volana(s[i].mois),s[i].heure);
		printf("\t<td> %s </td>\n",s[i].session);
		printf("\t<td><a href=\"auth.cgi?nom=%s&user=%s\"> %s </a></td>\n",nom,s[i].user,s[i].user);
		printf("</tr>\n");
	}
	printf("</table>");
}

////////// Pagination
int getNum(){
	int n = 0;
	char* url = malloc(50*sizeof(char));
	url = getenv("QUERY_STRING");
	sscanf(url,"nom=%*[^&]&user=%*[^=]=%d",&n);
	return n;
}
void lienPagination(int c,char* nom,char* u){
	int i=0;
	int j=0;
	char class[30];
	int n_page = (c/LINE_SHOWN);
			
	if((c%LINE_SHOWN)!=0 && c/LINE_SHOWN > 0){
		n_page++;
	}
	
	if(getNum()%10==0 && getNum()!=0){
		i=getNum()-1;
		j = i+11;
	}
	else{
		i = (getNum()+1) - ((getNum()+1)%10);
		j = i+10;
	}
	/*
	i = (getNum()+1) - ((getNum()+1)%10);
	j = i+10;
	*/
	if(getNum()-(getNum()%10) > 0){
		printf("<a href=\"auth.cgi?nom=%s&user=%s&p=%d\" class=\"page boite\">«</a> ",nom,u,(getNum()-(getNum()%10))-10);
	}
	if(getNum() > 0){
		printf("<a href=\"auth.cgi?nom=%s&user=%s&p=%d\" class=\"page boite\"> < </a> ",nom,u,getNum()-1);
	}
	
	for(; i<j && i<n_page; i++){
		if(getNum()==i){
			strcpy(class,"page inpage");
		}
		else{
			strcpy(class,"page");
		}
		if(i%10==0 && i!=0){
			printf("<a href=\"auth.cgi?nom=%s&user=%s&p=%d\" class=\"%s\">%d</a> ",nom,u,i-10,class,i+1);
		}
		else {
			printf("<a href=\"auth.cgi?nom=%s&user=%s&p=%d\" class=\"%s\">%d</a> ",nom,u,i,class,i+1);
		}
	}
	
	if(getNum()+1 < n_page){
		printf("<a href=\"auth.cgi?nom=%s&user=%s&p=%d\" class=\"page boite\"> > </a> ",nom,u,getNum()+1);
	}
	if(getNum()+10 < n_page){
		printf("<a href=\"auth.cgi?nom=%s&user=%s&p=%d\" class=\"page boite\">»</a> ",nom,u,(getNum()-(getNum()%10))+10);
	}
}

////////// Bar de navigation
char* caseChange(char* ch){
	for(int i=0; i<strlen(ch); i++){
		ch[i] = tolower(ch[i]);
	}
	return ch;
}
void displayBar(char* nom){
	printf("<nav>");
	printf("<form action=\"auth.cgi\" method=\"get\">");
	printf("<input type=\"hidden\" name=\"nom\" value=\"%s\">",nom);
	printf("<input id=\"nom\" type=\"text\" name=\"user\" placeholder=\"User\">");
	printf("<input class=\"submit\" type=\"submit\" value=\"Search\">");
	printf("</form>");
	printf("<a href=\"auth.cgi?nom=%s&user=allusers\"><button type=\"button\" class=\"submit\">All</button></a>",nom);
	printf("</nav>");	
}