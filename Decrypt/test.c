#include <stdio.h>
#include <stdlib.h>

extern void decrypt(char *IFname, char *OFname, char *password);
extern void printUsage(char *name);

int main(int argc, char **argv)
{
	if (argc < 4)
	{
		printUsage("decrypt");
		exit(EXIT_FAILURE);
	}

	char *IFname, *OFname, *password;

	IFname = argv[1];
	OFname = argv[2];
	password = argv[3];

	decrypt(IFname, OFname, password);

	return 0;
}
