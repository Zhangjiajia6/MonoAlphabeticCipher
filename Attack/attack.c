#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define NUM_OF_POSSIBLE_PWD 10
#ifndef NUM_OF_LETTERS
#define NUM_OF_LETTERS 26
#endif

void GetMostPossiblePwd(char *IFname);
void decrypt(char *IFname, char *OFname, char *passwordfile);

void attack(char *IFname)
{	
	GetMostPossiblePwd(IFname);
	
	int i;
	char kname[10] = "k";
	char pname[10] = "p";
    char suffix[10];
    for (i = 0 ; i < 10; i++)
    {
        
        suffix[0] = '0' + i;
        suffix[1] = '\0';
        strcat(suffix, ".txt");
        strcat(kname, suffix);
        strcat(pname, suffix);               
        
        decrypt(IFname, pname, kname);
        
        kname[1] = '\0';
        pname[1] = '\0';        
    }	
	
}

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        printf("Usage: attack [IFname]\n");
        exit(EXIT_FAILURE);
    }
    
    attack(argv[1]);
    
    return 0;
}
