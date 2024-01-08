#include "../h/access.h"

char* getConf(){
    char* url = getenv("QUERY_STRING");
    char* conf = malloc(100);
    sscanf(url,"nom=%*[^&]&conf=%[^&]",conf);
    return conf;
}

char* getFile(char* conf){
    char* nfile = malloc(100);
    char f[100];

    sprintf(f,"/etc/apache2/sites-available/%s",conf);

    FILE* file = fopen(f,"r");
    char line[600];
    if(file == NULL){
        printf("getFile: error open file\n");
        exit(1);
    }
    while(fgets(line,600,file) != NULL){
        if(strstr(line,"CustomLog") != NULL){
            sscanf(line,"%*[^CustomLog]CustomLog ${APACHE_LOG_DIR}/%[^ ] combined",nfile);
        }
    }
    fclose(file);
    return nfile;
}

int countVisit(char* conf){
    char* nfile = getFile(conf);
    char f[100];
    
    sprintf(f,"/var/log/apache2/%s",nfile);
    
    FILE * file = fopen(f,"r");
    int c = 0;
    char line[600];
    if(file == NULL){
        printf("countVisit: error open file %s\n",nfile);
        exit(1);
    }
    while(fgets(line,600,file)!=NULL){
        c++;
    }
    fclose(file);
    return c;
}

Access* getVisit(char* conf){
    char* nfile = getFile(conf);
    int c = countVisit(conf);
    Access* acs = malloc(c*sizeof(Access)); 
    char f[100];

    sprintf(f,"/var/log/apache2/%s",nfile);
    
    FILE * file = fopen(f,"r");
    char line[600];
    int i = 0;
    if(file == NULL){
        printf("getVisit: error open file\n");
        exit(1);
    }
    while(fgets(line,600,file)!=NULL){
        sscanf(line,"%[^ ] %*[^[][%[^]]] \"%[^\"]\" %*[^\"]\"%[^\"]\" \"%[^\"]\"", acs[i].ip, acs[i].date, acs[i].task, acs[i].visit, acs[i].plateforme);
        i++;
    }
    fclose(file);
    return acs;
}
bool snameCommented(char* conf){
    char l[100];
    char f[100];

    sprintf(l,"ServerName %s",getSitebyConf(conf));
    sprintf(f,"/etc/apache2/sites-available/%s",conf);

    FILE* file = fopen(f,"r");
    char line[600];
    if(file == NULL){
        printf("getFile: error open file\n");
        exit(1);
    }
    while(fgets(line,600,file) != NULL){
        if(strstr(line,l) != NULL){
            char preChar[100];
            sscanf(line,"%[^ServerName]ServerName",preChar);
            if(strstr(preChar,"#") != NULL){
                return true;
            }
        }
    }
    fclose(file);
    return false;
}
char* getSitebyConf(char* conf){
    char* nsite = malloc(100);
    strcpy(nsite,"");
    char f[100];

    sprintf(f,"/etc/apache2/sites-available/%s",conf);

    FILE* file = fopen(f,"r");
    char line[600];
    if(file == NULL){
        printf("getFile: error open file\n");
        exit(1);
    }
    while(fgets(line,600,file) != NULL){
        if(strstr(line,"ServerName") != NULL){
            sscanf(line,"%*[^ServerName]ServerName %[^ ] ",nsite);
        }
    }

    if(strcmp(nsite,"") == 0) strcpy(nsite,"#not-named");

    fclose(file);
    return nsite;
}
void selectionConf(char* nom){
    FILE* file = popen("ls /etc/apache2/sites-available","r");
    char line[100];

    if(file == NULL){
        printf("getVisit: error open file\n");
        exit(1);
    }
    printf("<nav>\n");
    printf("<form action=\"\" method=\"get\" >\n");
    printf("<input name=\"nom\" value=\"%s\" type=\"hidden\"/>\n",nom);
    printf("<select name=\"conf\">\n");
    printf("<option disabled>Site Web</option>\n");
    while(fgets(line,100,file) != NULL){
        sscanf(line,"%s",line);
        if(!snameCommented(line) && strcmp(getSitebyConf(line),"#not-named")!=0){
            printf("<option value=\"%s\">%s</option>\n",line,getSitebyConf(line));
        }
    }
    printf("</select>\n");
     printf("<input value=\"Valid\" type=\"submit\"/>\n");
    printf("</form>\n");
    printf("</nav>\n");
    pclose(file);
}

void accessTable(char* conf,int page){
    int c = countVisit(conf);
    Access* acs = getVisit(conf);

    printf("<h1>%s</h1>",getSitebyConf(conf));
    printf("<table id=\"access\">\n");
    printf("<tr id=\"en-tete\">\n");
    printf("<td>IP</td>\n");
    printf("<td>Date</td>\n");
    printf("<td>Task</td>\n");
    printf("<td>URL visited</td>\n");
    printf("<td>Plateforme</td>\n");
    printf("</tr>\n");

    for (int i = page*LINE_SHOWN; i < page*LINE_SHOWN + LINE_SHOWN && i < c; i++){
        printf("<tr>\n");
        printf("<td>%s</td>\n",acs[i].ip);
        printf("<td>%s</td>\n",acs[i].date);
        printf("<td>%s</td>\n",acs[i].task);
        printf("<td>%s</td>\n",acs[i].visit);
        printf("<td>%s</td>\n",acs[i].plateforme);
        printf("</tr>\n");
    }

    printf("</table>\n");
}
int getNum(){
    int p = 0;
    char* url = getenv("QUERY_STRING");
    sscanf(url,"nom=%*[^&]&conf=%*[^&]&p=%d",&p);
    return p;
}
void lienPagination(int c,char* nom,char* conf){
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
		printf("<a href=\"access.cgi?nom=%s&conf=%s&p=%d\" class=\"page boite\">«</a> ",nom,conf,(getNum()-(getNum()%10))-10);
	}
	if(getNum() > 0){
		printf("<a href=\"access.cgi?nom=%s&conf=%s&p=%d\" class=\"page boite\"> < </a> ",nom,conf,getNum()-1);
	}
	
	for(; i<j && i<n_page; i++){
		if(getNum()==i){
			strcpy(class,"page inpage");
		}
		else{
			strcpy(class,"page");
		}
		if(i%10==0 && i!=0){
			printf("<a href=\"access.cgi?nom=%s&conf=%s&p=%d\" class=\"%s\">%d</a> ",nom,conf,i-10,class,i+1);
		}
		else {
			printf("<a href=\"access.cgi?nom=%s&conf=%s&p=%d\" class=\"%s\">%d</a> ",nom,conf,i,class,i+1);
		}
	}
	
	if(getNum()+1 < n_page){
		printf("<a href=\"access.cgi?nom=%s&conf=%s&p=%d\" class=\"page boite\"> > </a> ",nom,conf,getNum()+1);
	}
	if(getNum()+10 < n_page){
		printf("<a href=\"access.cgi?nom=%s&conf=%s&p=%d\" class=\"page boite\">»</a> ",nom,conf,(getNum()-(getNum()%10))+10);
	}
}
