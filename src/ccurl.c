/*
 * curl.c

 *
 *  Created on: Mar 19, 2015
 *      Author: root
 */
#include <stdio.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <string.h>
#include "ccurl.h"
#include <string.h>
#include <sys/time.h>

//#include <conio.h>

static char AcceptType[] = "Accept:application/json";
static char header[] = "Content-Type:application/json";
static char charsets[] = "charsets:utf-8";

static char *port = "8100";
static char loginURL[100];// = "http://10.1.122.173:8100/clientlogin";
static char changePasswordURL[100];// = "http://10.1.122.173:8100/api/user/client/password";
static char submmitQuestionURL[100];//= "http://10.1.122.173:8100/api/assist?uname=<str>&reqtime=<str>";
static char assistDialogURL[100];// = "http://10.1.122.173:8100/api/assist/count?uname=<str>&reqtime=<str>";
static char getSupporterInfoURL[100];
static char requestPOST[] = "POST";
static char requestPUT[] = "PUT";
static char requestGET[] = "GET";
static char requestDELETE[] = "DELETE";

static char *get_url_prefix(void)
{
    FILE * fp, * cmd;
    static char prefix[50] ;//= "http://";
    char ipaddress[16], *ip;
    int len;
    fp = fopen("/etc/puppet/puppet.conf","r");
    if (fp == NULL) {
        printf("file puppet.conf is not exist");
        return NULL;
    }

    else{
        fclose(fp);
        cmd = popen("cat /etc/puppet/puppet.conf | grep 'server'|awk -F '=' {'print $2'}", "r");
        fgets(ipaddress, 16, cmd);
        for(ip = ipaddress; *ip == ' '; ++ip);

        len = strlen(ip);
        if (len)
            ip[len - 1]='\0';
        else
            return NULL;

        memset(prefix, '\0', sizeof(prefix));
        strcat(prefix, "http://");
        strcat(prefix,ip);
        strcat(prefix, ":8100");
        //strcat(prefix,end);
        return prefix;
    }
}

static  char* get_ip_addr(void)
{
	int num = 0;
	FILE *file = NULL;
#define IP_ADDR_LEN 64
#define IP_NUM 4
	char temp[IP_ADDR_LEN];
	static char ip[IP_ADDR_LEN * IP_NUM];


	memset(temp, 0, sizeof(temp));
	memset(ip, 0, sizeof(ip));
	//file = popen("ifconfig | grep 'inet addr' | awk -F ' ' {'print $2'} "
	//		"| awk -F ':' {'print $2'}", "r");
	file = popen("cat /etc/sysconfig/network-scripts/ifcfg*"
			"| grep IPADDR*", "r");
	while (fgets(temp, sizeof(temp), file)) {

		char *p = temp;
		while (*p == ' ')
			p++;
		if (*p == '#')
			continue;

		p = strchr(p, '=');
		while(*++p == ' ');

		// delete the last '\n' in @temp string.
		temp[strlen(temp) - 1] = 0;
		if (!strcmp(p, "127.0.0.1"))
			continue;
		if (num > 0)
			strcat(ip, "/");

		strcat(ip, p);
		num++;
	}

	if (num > 0)
		return ip;
	else
		return NULL;

}

static char *get_time(void)
{
	static char tim[14];

	struct timeval t;
	gettimeofday(&t, NULL);
	//g_print("time %ld\n",t.tv_sec*1000 + t.tv_usec/1000);
	memset(tim, '\0', sizeof(tim));
	sprintf(tim, "%ld", t.tv_sec*1000 + t.tv_usec/1000);
	//g_print("time %s", tim);
	return tim;
}



static struct curl_request makeupJsonObj(const char *arg1, const char *arg2,
		const char *arg3, char *jsonObj, CURL_ACTION action)
{
	struct curl_request req;
	int num;
	if(action != ASSIST_DIALOG) {
		if(NULL == arg1 || NULL == arg2 || NULL == jsonObj){
			printf("name or password or jsonObj is null\n");
			goto error;
			//return NULL;
		}
	}
	if (LOGIN == action){
		num = sprintf(jsonObj, "{\"username\":\"%s\",\"password\":\"%s\"}",
				(char *)arg1, (char *)arg2);
	}
	else if(CHANGE_PASSWORD == action){
		if(NULL == arg2){
			printf("old password is null\n");
			goto error;
		}
		num = sprintf(jsonObj, "{\"username\":\"%s\",\"oldpasswd\":\"%s\",\"newpasswd\":\"%s\"}",
				arg1, arg2, arg3);
	} else if(SUBMMIT_QUESTION == action){
		char *ip = NULL;
		ip = get_ip_addr();
		if(NULL == ip)
			goto error;
		strcat(jsonObj, "{\"promoter\":\"");
		strcat(jsonObj, arg1);
		strcat(jsonObj, "\",\"desc\":\"");
		strcat(jsonObj, arg2);
		strcat(jsonObj, "\",\"hostaddr\":\"");
		strcat(jsonObj, "\"}");
		/*num = sprintf(jsonObj, "{\"promoter\":\"%s\",\"desc\":\"%s\",\"hostaddr\":\"%s\"}",
					arg1, arg2, ip);
					*/
		num = strlen(jsonObj);
		if(JSON_OBJ_SIZE <= num)
			goto error;
	} else if (ASSIST_DIALOG == action){
		;
	} else if (GET_SUPPORTER_INFO == action) {

	} else if(DELETE_DIALOG == action){
		;
	}
	else
		goto error;

	jsonObj[num] = '\0';
	req.size = num;
	req.body = jsonObj;
	return req;
error:
	req.size = 0;
	req.body = NULL;
	return req;
}

static size_t write_data(void *ptr, size_t size, size_t nmemb, void *p)
{
	struct curl_result *c_result = (struct curl_result *)p;

	c_result->res = strndup(ptr, (size_t)size*nmemb);
	if(NULL == c_result->res) {
		c_result->size = 0;
		return -1;
	}
	else
	{
		c_result->size = size * nmemb;
		return c_result->size;
	// TODO remember to free the memory strndup malloced.
	}
}

 struct curl_result curl_request(void *arg1, void *arg2, char *arg3, CURL_ACTION type)
 {
	 CURL *curl;
	 struct curl_slist *headers = NULL;
	 CURLcode res;
	 struct curl_result c_result;

	 char jsonObj[JSON_OBJ_SIZE], *obj;
	 struct curl_request req;
	 char *prefix = NULL;

	 memset(&req, '\0', sizeof(struct curl_request));
	 memset(jsonObj, '\0', sizeof(jsonObj));
	 memset(&c_result, '\0', sizeof(curl_result));
	 if(LOGIN == type)
		 req = makeupJsonObj(arg1, arg2, NULL, jsonObj, type);
	 else if(CHANGE_PASSWORD == type)
		 req = makeupJsonObj(arg1, arg2, arg3, jsonObj, type);
	 else if (	SUBMMIT_QUESTION == type){
		 req = makeupJsonObj(arg1, arg2, NULL, jsonObj, type);
	 }
	 else if(ASSIST_DIALOG == type){
		// req = makeupJsonObj(arg1, NULL, NULL, jsonObj, type);
	 }
	 else if(DELETE_DIALOG == type){
		 //req = makeupJsonObj(arg1, arg2, NULL, jsonObj, type);
	 }

	 else ;
#if 1
	 printf("post body: %s\n", req.body);
#endif

	 curl_global_init(CURL_GLOBAL_ALL);
	 curl = curl_easy_init();
	 if(curl){
		 headers = curl_slist_append(headers, AcceptType);
		 headers = curl_slist_append(headers, header);
		 headers = curl_slist_append(headers, charsets);
		 curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

		 switch(type) {
		 case LOGIN: {
			 curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, requestPOST);
			 prefix = get_url_prefix();
			 memset(loginURL, '\0', sizeof(loginURL));
			 strncpy(loginURL, prefix, strlen(prefix));

			 strcat(loginURL, "/clientlogin");
			 g_print("loginURL: %s \n", loginURL);
			 curl_easy_setopt(curl, CURLOPT_URL, loginURL);
			 break;
		 }
		 case CHANGE_PASSWORD: {
		 	 curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, requestPUT);
			 prefix = get_url_prefix();
			 memset(changePasswordURL, '\0', sizeof(changePasswordURL));
			 strncpy(changePasswordURL, prefix, strlen(prefix));

			 strcat(changePasswordURL, "/api/user/client/password");
			 g_print("changePasswordURL: %s \n", changePasswordURL);
		 	 curl_easy_setopt(curl, CURLOPT_URL, changePasswordURL);
		 	break;
		 }
		 case SUBMMIT_QUESTION: {
			 curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, requestPOST);
			 prefix = get_url_prefix();
			 memset(submmitQuestionURL, '\0', sizeof(submmitQuestionURL));
			 strncpy(submmitQuestionURL, prefix, strlen(prefix));
			 strcat(submmitQuestionURL, "/api/assist?uname=");
			 strcat(submmitQuestionURL, arg1);
			 strcat(submmitQuestionURL, "&reqtime=");
			 strcat(submmitQuestionURL, get_time());

			 g_print("submmitQuestionURL: %s \n", submmitQuestionURL);
			 curl_easy_setopt(curl, CURLOPT_URL, submmitQuestionURL);
			 break;
		 }
		 case ASSIST_DIALOG: {
			 curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, requestGET);
			 prefix = get_url_prefix();
			 memset(assistDialogURL, '\0', sizeof(assistDialogURL));
			strncpy(assistDialogURL, prefix, strlen(prefix));
			strcat(assistDialogURL, "/api/assist/count?uname=");
			strcat(assistDialogURL, arg1);
			strcat(assistDialogURL, "&reqtime=");
			strcat(assistDialogURL, get_time());
			g_print("assistDialogURL: %s \n", assistDialogURL);
			curl_easy_setopt(curl, CURLOPT_URL, assistDialogURL);
			 break;
		 }
		 case GET_SUPPORTER_INFO: {
			 curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, requestGET);
			 prefix = get_url_prefix();
			 memset(getSupporterInfoURL, '\0', sizeof(getSupporterInfoURL));
			 strncpy(getSupporterInfoURL, prefix, strlen(prefix));
			 strcat(getSupporterInfoURL, "/api/assist/");
			 strcat(getSupporterInfoURL, arg2);
			 strcat(getSupporterInfoURL, "?uname=");
			 strcat(getSupporterInfoURL, arg1);
			 strcat(getSupporterInfoURL, "&reqtime=");
			 strcat(getSupporterInfoURL, get_time());
			 curl_easy_setopt(curl, CURLOPT_URL, getSupporterInfoURL);
			 g_print("getSupporterInfoURL: %s\n", getSupporterInfoURL);
			 break;
		 }
		 case DELETE_DIALOG: {
			 curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, requestDELETE);
			 prefix = get_url_prefix();
			 memset(assistDialogURL, '\0', sizeof(assistDialogURL));
			 strncpy(assistDialogURL, prefix, strlen(prefix));
			 strcat(assistDialogURL, "/api/assist/");
			 strcat(assistDialogURL, arg2);
			 strcat(assistDialogURL, "?uname=");
			 strcat(assistDialogURL, arg1);
			 strcat(assistDialogURL, "&reqtime=");
			 strcat(assistDialogURL, get_time());
			 g_print("assistDialogURL: %s \n", assistDialogURL);
			 curl_easy_setopt(curl, CURLOPT_URL, assistDialogURL);
			 break;
		 }
		 default:
			 break;
		 }

		 if(ASSIST_DIALOG != type){
			 curl_easy_setopt(curl, CURLOPT_POSTFIELDS, req.body);
			 curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, req.size);
		 }
		 curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
		 curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
		 curl_easy_setopt(curl, CURLOPT_WRITEDATA, &c_result);

		 res = curl_easy_perform(curl);
		 // if curl perform ok, then the result is written into @c_result
		 // else @c_result.res stores error info, and free the memory in advance.
		 if(CURLE_OK != res) {
			 printf("错误码： %d, %s\n", res, curl_error[res]);
			 free(c_result.res);
			 c_result.res = curl_error[res];
		 }
		 c_result.err_no = res;
		 curl_slist_free_all(headers);
		 curl_easy_cleanup(curl);
	 }
	curl_global_cleanup();
	return c_result;
 }





