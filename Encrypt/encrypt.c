#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

extern char* CheckAndReadPwd(char *password);
extern void Substitution(char *IFname, char *OFname, char *password);
extern void printUsage(char *name);

void encrypt(char *IFname, char *OFname, char *password)
{
	/*check and read the password*/
	
	char *pwd;
	pwd = CheckAndReadPwd(password);

	/*process of substitution*/
	
	Substitution(IFname, OFname, pwd);
}
