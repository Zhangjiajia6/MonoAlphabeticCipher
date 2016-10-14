#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#define NUM_OF_LETTERS 26
#define FILE_CLOSE_SUCCESS 0

void printUsage(char *name)
{
	fprintf(stdout, "Usage: %s [IFname] [OFname] [password]\n", name);
	fprintf(stdout, "  IFname: input file name\n");
	fprintf(stdout, "  OFname: output file name\n");
	fprintf(stdout, "  password: password file name\n");
	fprintf(stdout, "Example:\n");
	fprintf(stdout, "  %s input.txt output.txt password.txt\n", name);
}

FILE* OpenFileAndCheck(char *name, char *mode)
{
	FILE *fp;
	
	if ( (fp = fopen(name,mode))== NULL)
	{
		fprintf(stderr, "Can't open File %s with mode %s\n",name, mode);
		exit(EXIT_FAILURE);
	}
	else
		return fp;
}

void CloseFileAndCheck(FILE *fp)
{
	if (fclose(fp) != FILE_CLOSE_SUCCESS)	
	{
		fprintf(stderr, "Close file error!\n");
		exit(EXIT_FAILURE);
	}
}

char *CheckAndReadPwd(char *password)
{
	FILE *fp;
	static char pwd[NUM_OF_LETTERS];
	char ch;
	int index = 0;
	fp = OpenFileAndCheck(password, "r");
	while ((ch = getc(fp)) != EOF && index < NUM_OF_LETTERS)
	{
		if (isgraph(ch))
		{
			pwd[index] = toupper(ch);
			index ++;
		}
	}

	/*check password*/
	if ( index < NUM_OF_LETTERS)
	{
		fprintf(stderr, "Invalid Password!\n");
		exit(EXIT_FAILURE);
	}

	return pwd;
}

char *GetDecryptPwd(char *password)
{
	char *pwd;
	static char decryptPwd[NUM_OF_LETTERS];
	int index, DeIndex;

	pwd = CheckAndReadPwd(password);

	for (index = 0; index < NUM_OF_LETTERS; index++)
	{
		DeIndex = pwd[index] - 'A';
		decryptPwd[DeIndex] = 'A' + index;
	}

	return decryptPwd;
}

void Substitution(char *IFname, char *OFname, char *password)
{
	FILE *input, *output;

	/*open and check file*/

	input = OpenFileAndCheck(IFname, "r");
	output = OpenFileAndCheck(OFname, "w");
	
	char ch;
	int index = 0;

	while ((ch = getc(input)) != EOF)
	{
		if (isalpha(ch))
		{
			ch = toupper(ch);

			/*Substitution*/
			ch = password[ch - 'A'];
			putc(ch,output);
		}

		else if (ch == '\n' || ch == '\r')
		{
			putc(ch,output);
		}

		else
		{
			/********************Do Noting****************************/
			/*add code here if you want to deal with other characters*/
		}

	}

	CloseFileAndCheck(input);
	CloseFileAndCheck(output);
}

double *GetFrequency(char *filename)
{
	FILE *fp;
	fp = OpenFileAndCheck(filename,"r");

	static double Frequency[NUM_OF_LETTERS] = {0};
	char ch;
	int index;
	double SumOfChars = 0;

	while ((ch = getc(fp)) != EOF)
	{
		if (isalpha(ch))
		{
			SumOfChars += 1;
			ch = toupper(ch);
			index = ch - 'A';
			Frequency[index] += 1;
		}
	}

	for (index = 0; index < NUM_OF_LETTERS; index++)
		Frequency[index] = Frequency[index]*100/SumOfChars;

	return Frequency;
}
