/*
 * secretary.h
 *
 *  Created on: Apr 13, 2015
 *      Author: root
 */

#ifndef INC_SECRETARY_H_
#define INC_SECRETARY_H_

/*
 * Parse the error info from @err to @info
 * */
int get_error_info(const char *err, const char *info);
int get_str(char *str, char *dst );

void *start_empathy(void *p);



#endif /* INC_SECRETARY_H_ */
