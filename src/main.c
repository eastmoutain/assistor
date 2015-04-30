/*
 * mian.c
 *
 *  Created on: Mar 18, 2015
 *      Author: root
 */

#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include <curl/curl.h>
#include <string.h>
#include <pwd.h>
#include <pthread.h>
#include "window.h"


// global variable
struct appWindows app;
int assist_state = UNLOGED_IN;

/*	get the current process's user name. */
static int getUserName(char **n, const int nz)
{
    if(!n || !*n)
        return -1;
    char *p = NULL;
    uid_t uid = getuid();
    printf("uid: %d\n", uid);
    struct passwd *pw = getpwuid(uid);
    if(pw) {
        if(strlen(pw->pw_name) > nz){
            if(!(p = malloc(strlen(pw->pw_name)+1)))
                return -1;
            free(*n);
            *n = p;
            bzero(*n, strlen(pw->pw_name) + 1);
        }
        printf("find one user, name: %s\n", pw->pw_name);

        strcpy((char *)*n, (char *)pw->pw_name);

        return 0;
    }
    return -1;
}

static int checkBrother(void)
{
	FILE * cmd;
#define NAME_LEN 512
	char user[NAME_LEN];
	int p_num = 0;

	char *owner_name = (char *)malloc(sizeof(char) * NAME_LEN);
	if(!owner_name)
		return -1;

	if(getUserName(&owner_name, NAME_LEN)){
	    free(owner_name);
	    printf("get user name failed\n");
	    return -1;
	}
	printf("the current user of this machine: %s\n", owner_name);

	bzero(user, sizeof(user));
	cmd = popen("ps acu | grep 'assistor'|awk -F ' ' {'print $1, $2'}", "r");
	int num = 0;
	int cnt = 0;
	char name[20];
	char Pid[10];
	char *p = NULL;
	while(NULL != fgets(user, sizeof(user), cmd)){
		/* "awk" add '\n' at the end of each line, so kick out it. */
	    user[strlen(user)-1] = 0;
	    memset(name, '\0', sizeof(name));
	    memset(Pid, '\0', sizeof(Pid));
	    for(p = user, num = 0; *p != ' '; ++p, ++num)
	    	name[num] = *p;
	    // skip the whitespace
	    ++p;
		for(num = 0; *p != '\0'; ++p, ++num)
			Pid[num] = *p;


	    printf("user[%d]: %s, Pid: %s\n", cnt, name, Pid);
	    if(!strcmp(name, owner_name))
	        p_num++;
	    cnt++;
	}
	free(owner_name);

	if(p_num >= 2){


	    printf("assistor is already running \n");
	    return 0;
	} else
		return -1;
}



int main(int argc, char **argv)
{

	gtk_init(&argc, &argv);

	if(!checkBrother()) {
		createReloginWindow();
		// Register "Ctrl C" singal
		signal(SIGINT, sigint_h2);

	} else {
		app.mw = createMainWindow();
		app.cpw = createChangePasswordWindow();
		app.dw = createDialogWindow();
		// Register "Ctrl C" singal
		signal(SIGINT, sigint_h1);


	}
	gtk_main();

	//pthread_join(empathy, NULL);
	return 0;
}
