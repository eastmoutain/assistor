/*
 * secretary.c
 *
 *  Created on: Apr 13, 2015
 *      Author: root
 */
#include <stdlib.h>
#include <stdio.h>
#include "secretary.h"


int get_error_info(const char *err, const char *info){
	char *start = err;
	char *end = NULL;
	char *p = info;
	int num = 0;

	start = strstr(err, "error");
	if(start){
		if(!(start = strstr(start, ":")))
			return 0;
		if(!(end = strstr(start, "\"}")))
			return 0;
		if(!(start = strstr(start, "\"")))
			return 0;
		for(++start; start != end; ++start, ++p){
			*p = *start;
			++num;
		}
		*p = '\0';
		return num;
	}
	return 0;
}
// get the content of xxxx from " "abc": xxxx " format.
int get_str(char *str, char *dst )
{
	if (NULL == str || NULL == dst)
		return -1;

	char *find_str;
	char *end_str;

	if ((find_str = strstr(str, ":"))) {
		find_str++;
		if ((end_str = strstr(find_str, ","))) {
			int num = 0;
			for (; find_str != end_str; ++find_str) {
				if (*find_str == ' ')
					continue;
				dst[num++] = *find_str;
			}
			dst[num] = 0;
		} else
			return -1;

	} else
		return -1;

	return 0;
}


void *start_empathy(void *p)
{
	system("empathy");
	return NULL;
}




