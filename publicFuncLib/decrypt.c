#include <stdio.h>
#include <stdlib.h>
#define NUM_OF_LETTERS 26

extern char (*GetDecryptPwd(char *passwordfile))[NUM_OF_LETTERS];
extern void Substitution(char *IFname, char *OFname, char (*password)[NUM_OF_LETTERS]);
extern void printUsage(char *name);

void decrypt(char *IFname, char *OFname, char *passwordfile)
{
	/*get decrypt password from encrypt password*/

	char (*decryptPwd)[NUM_OF_LETTERS];
	decryptPwd = GetDecryptPwd(passwordfile);

	/*substitution using decrypt password to get the plaintext*/

	Substitution(IFname, OFname, decryptPwd);
}
