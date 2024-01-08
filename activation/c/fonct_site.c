#include "../h/site.h"

int countSite(){
    int  c = 0;
    FILE* file = popen("ls /etc/apache2/sites-available","r");
    char line[300];
    if(file == NULL){
        exit(1);
    }
    while(fgets(line,300,file) != NULL){
        c++;
    }
    pclose(file);
    return c;
}
bool isActive(char* site){
    FILE* file = popen("ls /etc/apache2/sites-enabled","r");
    char line[300];
    if(file == NULL){
        exit(1);
    }
    while(fgets(line,300,file) != NULL){
        if(strstr(line,site) != NULL){
            return true;
        }
    }
    pclose(file);
    return false;
}
Site* getSite(){
    int c = countSite();
    int i = 0;
    Site* s = malloc(c*sizeof(Site));

    FILE* file = popen("ls /etc/apache2/sites-available","r");
    char line[300];
    if(file == NULL){
        exit(1);
    }
    while(fgets(line,300,file) != NULL){
        strcpy(s[i].name,line);
        if(isActive(line)){
            strcpy(s[i].descri,"actived");
        }
        else{
            strcpy(s[i].descri,"deactived");
        }
        i++;
    }
    pclose(file);
    return s;
}

void typeActivation(char* site, char* type){
    char cmd[50];
    if(strcmp(type,"active")==0){
        sprintf(cmd,"sudo a2ensite %s",site);
        popen(cmd,"r");
    }
    else{
        sprintf(cmd,"sudo a2dissite %s",site);
        popen(cmd,"r");
    }
    system("sudo systemctl reload apache2");
}
void siteList(char* nom){
    int c = countSite();
    char line[300];
    Site* s = malloc(c*sizeof(Site));
    s = getSite();

    FILE* file = fopen("../html/site.html","r");
    if(file == NULL){
        exit(1);
    }
    while(fgets(line,300,file) != NULL){
        if(strstr(line,"<body>")!=NULL){
            printf("%s\n",line);
            barNav(nom);
        }
        else if(strstr(line,"class=\"theme\"")!=NULL){
            printf("%s\n",line);
            printf("<div id=\"box\">\n");
            printf("<h2>Activation Web Site</h2>\n");

            for(int i=0; i<c ; i++){
                printf("<div>\n");
                    printf("<h3>%s</h3>\n",s[i].name);
                    printf("<div id=\"buttonBox\">\n");
                    if(strcmp(s[i].descri,"actived")==0){
                        printf("<a href=\"site.cgi?nom=%s&sname=%s&type=deactive\"><button type=\"button\" id=\"left\"></button></a>",nom,s[i].name);
                    }
                    else{
                        printf("<a href=\"site.cgi?nom=%s&sname=%s&type=active\"><button type=\"button\" id=\"right\"></button></a>",nom,s[i].name);
                    }
                    printf("</div >\n");
                printf("</div>\n");
            }
            
            printf("</div>\n");
        }
        else{
            printf("%s\n",line);
        }
    }
    fclose(file);
}