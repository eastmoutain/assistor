/*
 * ccurl.h
 *
 *  Created on: Mar 19, 2015
 *      Author: root
 */

#ifndef INC_CCURL_H_
#define INC_CCURL_H_

#define JSON_OBJ_SIZE 1024
extern struct curl_result curl_result;

static char *curl_error[] = {
		[0] = "CURLE OK",
		[1] = "CURLE UNSUPPORTED PROTOCOL",
		[2] = "CURLE FAILED INIT",
		[3] = "CURLE URL MALFORMAT",
		[4] = "CURLE NOT BUILD IN",
		[5] = "CURLE COULDNT RESOLVE PROXY",
		[6] = "CURLE COULDNT RESOLVE HOST",
		[7] = "CURLE COULDNT CONNECT",
		[8] = "CUREL FTP WEIRE SERVER REPLY",
		[9] = "CURLE REMOTE ACCESS DENIED",
		[10] = "CURLE FTP ACCESS DENIED",
		[11] = "CURLE FTP WEIRD PASS RETRY",
		[12] = "CURLE FTP ACCEPT TIMEOUT",
		[13] = "CURLE FTP WEIRD PASV REPLY",
		[14] = "CURLE FTP WEIRD 227 FORMAT",
		[15] = "CURLE FTP CANT GET HOST",
		[16] = "CURLE HTTP2",
		[17] = "CURLE FTP COULDNT SET TYPE",
		[18] = "CURLE PARTIAL FILE",
		[19] = "CURLE FTP COULDNT RETR FILE",
		[21] = "CURLE QUOTE ERROR",
		[22] = "CURLE HTTP RETURNED ERROR",
		[23] = "CURLE WRITE ERROR",
		[24] = "CURLE UPLOAD FAILED",
		[25] = "CURLE UPLOAD FAILED",
		[26] = "CURLE READ ERROR",
		[27] = "CURLE OUT OF MEMORY",
		[28] = "CURLE OPERATION TIMEDOUT",
		[30] = "CURLE FTP PORT FAILED",
		[31] = "CURLE FTP COULDNT USE REST",
		[33] = "CURLE RANGE ERROR",
		[34] = "CURLE HTTP POST ERROR",
		[35] = "CURLE SSL CONNECT ERROR",
		[36] = "CURLE_BAD DOWNLOAD RESUME",
		[37] = "CURLE FILE COULDNT READ FILE",
		[38] = "CURLE LDAP CANNOT BIND",
		[39] = "CURLE LDAP SEARCH FAILED",
		[41] = "CURLE FUNCTION NOT FOUND",
		[42] = "CURLE ABORTED BY CALLBACK",
		[43] = "CURLE BAD FUNCTION ARGUMENT",
		[45] = "CURLE INTERFACE FAILED",
		[47] = "CURLE TOO MANY REDIRECTS",
		[48] = "CURLE UNKNOWN OPTION",
		[49] = "CURLE TELNET OPTION SYNTAX",
		[51] = "CURLE PEER FAILED VERIFICATION",
		[52] = "CURLE GOT NOTHING",
		[53] = "CURLE SSL ENGINE NOTFOUND",
		[54] = "CURLE SSL ENGINE SETFAILED",
		[55] = "CURLE SEND ERROR",
		[56] = "CURLE RECV ERROR",
		[58] = "CURLE SSL CERTPROBLEM",
		[59] = "CURLE SSL CIPHER",
		[60] = "CURLE SSL CACERT",
		[61] = "CURLE BAD CONTENT ENCODING",
		[62] = "CURLE LDAP INVALID URL",
		[63] = "CURLE FILESIZE EXCEEDED",
		[64] = "CURLE USE SSL FAILED",
		[65] = "CURLE SEND FAIL REWIND",
		[66] = "CURLE SSL ENGINE INITFAILED",
		[67] = "CURLE LOGIN DENIED",
		[68] = "CURLE TFTP NOTFOUND",
		[69] = "CURLE TFTP PERM",
		[70] = "CURLE REMOTE DISK FULL",
		[71] = "CURLE TFTP ILLEGAL",
		[72] = "CURLE TFTP UNKNOWNID",
		[73] = "CURLE REMOTE FILE EXISTS",
		[75] = "CURLE CONV FAILED",
		[76] = "CURLE CONV REQD",
		[77] = "CURLE SSL CACERT BADFILE",
		[78] = "CURLE REMOTE FILE NOT FOUND",
		[79] = "CURLE SSH",
		[80] = "CURLE SSL SHUTDOWN FAILED",
		[81] = "CURLE_AGAIN",
		[82] = "CURLE SSL CRL BADFILE",
		[83] = "CURLE SSL ISSUER ERROR",
		[84] = "CURLE FTP PRET FAILED",
		[85] = "CURLE RTSP CSEQ ERROR",
		[86] = "CURLE RTSP SESSION ERROR",
		[87] = "CURLE FTP BAD FILE LIST",
		[88] = "CURLE CHUNK FAILED",
		[89] = "CURLE NO CONNECTION AVAILABLE"

};

enum {
	false = 0,
	true = 1
};

typedef enum{
	POST = 0,
	PUT,
}CURL_T;

typedef enum{
	LOGIN = 0,
	CHANGE_PASSWORD,
	SUBMMIT_QUESTION,
	ASSIST_DIALOG,
	GET_SUPPORTER_INFO,
	DELETE_DIALOG
}CURL_ACTION;

struct curl_result{
	char *res;
	union {
	int err_no;
	size_t size;
	};

};

struct curl_request{
	size_t size;
	char *body;
};

struct curl_result curl_request(void *arg1, void *arg2, char *arg3, CURL_ACTION type);

#endif /* INC_CCURL_H_ */
