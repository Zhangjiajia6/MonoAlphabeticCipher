#include <stdio.h>
#include <stdlib.h>

extern void encrypt(char *IFname, char *OFname, char *password);
extern void printUsage(char *name);

int main(int argc, char **argv)
{
	if (argc < 4)
	{
		printUsage("encrypt");
		exit(EXIT_FAILURE);
	}

	char *IFname, *OFname, *password;

	IFname = argv[1];
	OFname = argv[2];
	password = argv[3];

	encrypt(IFname, OFname, password);

	return 0;
}
