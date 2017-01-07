#include <stdio.h>
#include <stdlib.h>
#define NUM_OF_LETTERS 26

extern char (*CheckAndReadPwd(char *passwordfile))[NUM_OF_LETTERS];
extern void Substitution(char *IFname, char *OFname, char (*password)[NUM_OF_LETTERS]);
extern void printUsage(char *name);

void encrypt(char *IFname, char *OFname, char *passwordFile)
{
	/*check and read the password*/
	
	char (*pwd)[NUM_OF_LETTERS];
	pwd = CheckAndReadPwd(passwordFile);

	/*process of substitution*/
	
	Substitution(IFname, OFname, pwd);
}
