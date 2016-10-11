#include <stdio.h>
#include <stdlib.h>

extern char *GetDecryptPwd(char *EncryptPassword);
extern void Substitution(char *IFname, char *OFname, char *password);
extern void printUsage(char *name);

void decrypt(char *IFname, char *OFname, char *password)
{
	/*get decrypt password from encrypt password*/

	char *decryptPwd;
	decryptPwd = GetDecryptPwd(password);

	/*substitution using decrypt password to get the plaintext*/

	Substitution(IFname, OFname, decryptPwd);
}
