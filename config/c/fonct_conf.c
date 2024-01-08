#include "../h/config.h"

char* transformString(char* s){
    char* new_s = malloc(strlen(s)*sizeof(char));
    int j = 0;
    for(int i=0; i<strlen(s); i++){
        if(s[i]=='%' && s[i+1]=='2' && s[i+2]=='F' && i+2 < strlen(s)){
            *(new_s + j) = '/';
            i = i + 2;
        }
        else{
            *(new_s + j) = s[i];
        }
        j++;
    }
    return new_s;
}
VHost getV_host(char* url){
    VHost vh;
    vh.opt.index = 0, vh.opt.exec = 0,vh.opt.link = 0;
    vh.add.py = 0, vh.add.cgi = 0, vh.add.pl = 0;
    *vh.scalias = 0;

    sscanf(url,"%*[^&]&vname=%[^&]&sname=%[^&]&droot=%[^&]&scalias=%[^&]",vh.vname,vh.sname,vh.droot,vh.scalias);
    
    strcpy(vh.droot,transformString(vh.droot));
    strcpy(vh.scalias,transformString(vh.scalias));

    if(strstr(url,"index=")!=NULL){
        vh.opt.index = 1;
    }
    if(strstr(url,"exec=")!=NULL){
        vh.opt.exec = 1;
    }
    if(strstr(url,"link=")!=NULL){
        vh.opt.link = 1;
    }
    if(strstr(url,"cgi=")!=NULL){
        vh.add.cgi = 1;
    }
    if(strstr(url,"py=")!=NULL){
        vh.add.py = 1;
    }
    if(strstr(url,"pl=")!=NULL){
        vh.add.pl = 1;
    }

    sscanf(strstr(url,"allow="),"allow=%s",vh.allow);

    return vh;
}
void setV_host(VHost vh){
    char conf[100];
    sprintf(conf,"/etc/apache2/sites-available/%s.conf",vh.vname);
    // sprintf(conf,"%s.conf",vh.vname);

    FILE* file = fopen(conf,"w");

    fprintf(file,"<VirtualHost *:80>\n");
        fprintf(file,"\tServerName %s\n\n",vh.sname);
        fprintf(file,"\tDocumentRoot %s\n",vh.droot);
    
    
    if(*vh.scalias != 0){
        fprintf(file,"\tScriptAlias %s %s\n",vh.scalias,vh.droot);
    }
    fprintf(file,"\n");

    fprintf(file,"\t<Directory %s>\n",vh.droot);
        
    if(vh.opt.index==1 || vh.opt.exec==1 || vh.opt.link==1){
        fprintf(file,"\t\tOptions ");
        if(vh.opt.index==1){
            fprintf(file,"Indexes ");
        }
        if(vh.opt.exec==1){
            fprintf(file,"ExecCGI ");
        }
        if(vh.opt.link==1){
            fprintf(file,"FollowSymLinks ");
        }
            fprintf(file,"\n");
    }
    if(vh.add.cgi==1 || vh.add.py==1 || vh.add.pl==1){
        fprintf(file,"\t\tAddHandler cgi-script ");
        if(vh.add.cgi==1){
            fprintf(file,".cgi ");
        }
        if(vh.add.py==1){
            fprintf(file,".py ");
        }
        if(vh.add.pl==1){
            fprintf(file,".pl ");
        }
            fprintf(file,"\n");
    }

    if(*vh.allow != 0){
        fprintf(file,"\t\tAllowOverride %s\n",vh.allow);
    }
        fprintf(file,"\t\tRequire all granted\n");
    fprintf(file,"\t</Directory>\n\n");

    fprintf(file,"\tErrorLog ${APACHE_LOG_DIR}/error_%s.log\n",vh.vname);
    fprintf(file,"\tCustomLog ${APACHE_LOG_DIR}/access_%s.log combined\n\n",vh.vname);

    fprintf(file,"</VirtualHost>\n");

    fclose(file);
}
void activeCGI(){
    popen("sudo a2enmod cgi","r");
}
void activeSite(char* conf){
    char cmd[100];
    sprintf(cmd,"sudo a2ensite %s",conf);
    popen(cmd,"r");
}
void modeActive(VHost vh){
    if (vh.opt.exec == 1){
        activeCGI();
    }
    activeSite(vh.vname);
}
void addToHosts(char* sname){
    FILE* file = fopen("/etc/hosts","a");
    fprintf(file,"192.168.1.170  %s\n",sname);
    fclose(file);
}
void displayForm(char* nom,char* site){
    FILE* file = fopen("../html/config.html","r");
    char line[300];
    if(file == NULL){
        exit(1);
    }
    while(fgets(line,300,file)!=NULL){
        if(strstr(line,"<body>") != NULL){
            printf("%s\n",line);
            barNav(nom);
        }
        else if(strstr(line,"<h1>VirtualHost</h1>") != NULL){
            printf("%s\n",line);
            printf("<input type=\"hidden\" name=\"nom\" value=\"%s\">",nom);
        }
        else if(strstr(line,"</body>") != NULL && *site != 0){
            printf("<script>alert(\"%s configuration succeed\");</script>",site);
            printf("%s\n",line);
        }
        else{
            printf("%s\n",line);
        }
    }
    fclose(file);
}