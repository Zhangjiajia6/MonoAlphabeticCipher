#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <math.h>
#define NUM_OF_LETTERS 26
#define NUM_OF_POSSIBLE_PWD 10
#define FILE_CLOSE_SUCCESS 0

void printUsage(char *name)
{
	printf("Usage: %s [IFname] [OFname] [password]\n", name);
	printf("  IFname: input file name\n");
	printf("  OFname: output file name\n");
	printf("  password: password file name\n");
	printf("Example:\n");
	printf("  %s input.txt output.txt password.txt\n", name);
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

char (*CheckAndReadPwd(char *passwordfile))[NUM_OF_LETTERS]
{
	FILE *fp;
	static char pwd[2][NUM_OF_LETTERS];
	int ch;
	int index = 0;
	fp = OpenFileAndCheck(passwordfile, "r");
	while ((ch = getc(fp)) != EOF && index < NUM_OF_LETTERS)
	{
		if (isgraph(ch))
		{
		    pwd[0][index] = 'A'+index;
			pwd[1][index] = toupper(ch);
			index ++;
		}
	}

	/*check password*/
	if (index < NUM_OF_LETTERS)
	{
		fprintf(stderr, "Invalid Password!\n");
		exit(EXIT_FAILURE);
	}

	return pwd;
}

char (*GetDecryptPwd(char *passwordfile))[NUM_OF_LETTERS]
{
	char (*pwd)[NUM_OF_LETTERS];
	static char decryptPwd[2][NUM_OF_LETTERS];
	int index;

	pwd = CheckAndReadPwd(passwordfile);

	for (index = 0; index < NUM_OF_LETTERS; index++)
	{
		decryptPwd[0][index] = pwd[1][index];
		decryptPwd[1][index] = pwd[0][index];
	}

	return decryptPwd;
}

int GetIndex(char *array, char ch)
{
    int i;
    for (i = 0; i < NUM_OF_LETTERS; i++)
        if (ch == array[i])
            return i;
    return -1;
}

/*hash table to improve the efficiency*/
/*with load equal to 0.5*/
/*哈希表键为字符的int值，值为该字符在密码数组中的位置*/
/*此举目的为以空间换时间*/
#define NUM_OF_CHARS 256
static int HashTable[NUM_OF_CHARS];
static int TotalNum  = 0;
void InsertIntoHashTable(char key, int value)
{
    int index = key;
	HashTable[index] = value;
	TotalNum++;
}
int SearchHashTable(char key)
{
	int index = key;
	if (HashTable[index] != -1)
	    return HashTable[index];
	return -1;
}
void ResetHashTable(void)
{
	int i;
	for (i = 0; i < NUM_OF_CHARS; i++)
		HashTable[i] = -1;
	TotalNum = 0;
}

void Substitution(char *IFname, char *OFname, char (*password)[NUM_OF_LETTERS])
{
	FILE *input, *output;

	/*open and check file*/

	input = OpenFileAndCheck(IFname, "r");
	output = OpenFileAndCheck(OFname, "a+");
	
	int ch;
	int index = 0;

	while ((ch = getc(input)) != EOF)
	{
		if (isgraph(ch))
		{
            index = GetIndex(password[0], toupper(ch));
            if (index != -1)
            {
                ch = password[1][index];
			    putc(ch,output);
			}
		}

		else if (ch == '\n' || ch == '\r')
		{
			putc(ch,output);
		}
	}

	CloseFileAndCheck(input);
	CloseFileAndCheck(output);
}

/* 字符-频率 键值对*/
struct Dict
{
    char Key[NUM_OF_LETTERS];
    double Value[NUM_OF_LETTERS];
};


struct Dict GetFrequency(char *filename)
{
	FILE *fp;
	fp = OpenFileAndCheck(filename,"r");

	static struct Dict Frequency;
	int ch;
	int Count = 0;
	int ArrayIndex;
	double SumOfChars = 0;

    ResetHashTable();

	while ((ch = getc(fp)) != EOF)
	{
	    if (isgraph(ch))
	    {
			SumOfChars += 1;
			if ((ArrayIndex = SearchHashTable(ch)) == -1)
			{
				if (Count < NUM_OF_LETTERS)
				{
					Frequency.Key[Count] = ch;
					InsertIntoHashTable(ch,Count);
					Count++;
				}
				ArrayIndex = Count - 1;		    
			}
			Frequency.Value[ArrayIndex] += 1;
		}
	}
	
	int i;
	for (i = 0; i < NUM_OF_LETTERS; i++)
	    Frequency.Value[i] /= SumOfChars;

	return Frequency;
}


/*for test use, printArray func*/
void PrintFloatArray(const double *a, int len)
{
    int i,count = 0;
    for (i = 0; i < len; i++)
    {
        printf("%6lf",a[i]);
        count++;
        if (count%5 == 0)
            printf("\n");
    }
}
void PrintIntArray(const int *a, int len)
{
    int i,count = 0;
    for (i = 0; i < len; i++)
    {
        printf("%6d",a[i]);
        count++;
        if (count%5 == 0)
            printf("\n");
    }
}

void fswap(double *a, double *b)
{
    double temp;
    temp = *a;
    *a = *b;
    *b = temp;
}

void cswap(char *a, char *b)
{
    char temp;
    temp = *a;
    *a = *b;
    *b = temp;
}

void Dswap(struct Dict *A, int p1, int p2)
{
    fswap(&A->Value[p1],&A->Value[p2]);
    cswap(&A->Key[p1],&A->Key[p2]);
}

void RankDict(struct Dict *Frequency, int len)
{
    int i,j, maxi;
    double maxv;
    
    for (i = 0; i < len; i++)
    {
        maxi = i;
        maxv = Frequency->Value[i];
        
        for (j = i + 1; j < len; j++)
        { 
            if (Frequency->Value[j] > maxv)
            {
                maxv = Frequency->Value[j];
                maxi = j;
            }
        }
        
        fswap(&Frequency->Value[i], &Frequency->Value[maxi]);
        cswap(&Frequency->Key[i], &Frequency->Key[maxi]);
    }
}

int ifindMax(const int *array, int len)
{
    int i, max = 0, maxi = 0;
    for (i = 0; i < len; i++)
    {
        if (array[i] > max)
        {
            max = array[i];
            maxi = i;
        }
    }
    
    return maxi;
}

int ffindMax(const double *array, int len)
{
    int i, maxi = 0;
    float max = 5;    /*计算知，相对熵最大值不会超过5*/
    for (i = 0; i < len; i++)
    {
        if (array[i] > max)
        {
            max = array[i];
            maxi = i;
        }
    }
    
    return maxi;
}

int *GetMinGap(const double *array, int len)
{
    double MinGap[4] = {1, 1, 1, 1}; /*确定4个频率间隔最小的位置，交换时可得到16(2^4)个可能的密码*/
    static int index[4] = {0, 1, 2, 3};
    int i, gmax;
    double gap;
    for (i = 0; i < len - 1; i++)
    {
        gap = array[i+1] - array[i];
        gmax = ffindMax(MinGap, 4);
        if (gap < MinGap[gmax])
        {
            MinGap[gmax] = gap;
            index[gmax] = i;
        }
    }
    
    return index;
}

double GetEntrophy(const double *array, const double *stdarray)
{
    float sum = 0;
    int i;
    for (i = 0; i < NUM_OF_LETTERS; i++)
    {
        if (array[i] == 0)
        {
            sum = 5; /*熵设为最大值*/
            break;
        }
        sum += stdarray[i]*log(stdarray[i]/array[i]);
    }
    
    return sum;    
}

void writeFrTo(const struct Dict Frequency, char *name)
{
    /*A~Z字母位置*/
    int order[NUM_OF_LETTERS] = {
    3, 20, 12, 10, 1,
    16, 17, 8, 5, 23,
    22, 11, 14, 6, 4,
    19, 25, 9, 7, 2,
    13, 21, 15, 24, 18, 26 
    };
    
    char pwd[NUM_OF_LETTERS];
    int i;
    for (i = 0; i < NUM_OF_LETTERS; i++)
        pwd[i] = Frequency.Key[order[i]-1];
    FILE *fp;
    fp = OpenFileAndCheck(name, "w");
    for (i = 0; i < NUM_OF_LETTERS; i++)
        putc(pwd[i], fp);
        
    CloseFileAndCheck(fp);
}

void GetMostPossiblePwd(char *IFname)
{
	char StandartCharTable[NUM_OF_LETTERS] = {
		'E',    'T',    'A',    'O',    'I',
		'N',    'S',    'H',    'R',    'D',
		'L',    'C',    'U',    'M',    'W',
		'F',    'G',    'Y',    'P',    'B',
		'V',    'K',    'J',    'X',    'Q', 
		'Z'};
		
    double StandardFrequency[NUM_OF_LETTERS] = {
        12.702, 9.056,  8.167,  7.507,  6.996,
        6.749,  6.327,  6.094,  5.987,  4.253,
        4.025,  2.782,  2.758,  2.406,  2.360,
        2.228,  2.015,  1.974,  1.929,  1.492,
        0.978,  0.772,  0.153,  0.150,  0.095,
        0.074};
        
    int j;
    for (j = 0; j < NUM_OF_LETTERS; j++)
        StandardFrequency[j] /= 100;
          
    struct Dict Frequency;

    Frequency = GetFrequency(IFname);
    int *MinGapIndex;
    RankDict(&Frequency, NUM_OF_LETTERS);
    MinGapIndex = GetMinGap(Frequency.Value, NUM_OF_LETTERS);
    
    
    struct Dict F[16];
    
    F[0] = Frequency;
    
    Dswap(&Frequency, MinGapIndex[0], MinGapIndex[0]+1);
    F[1] = Frequency;
    
    Frequency = F[0];
    Dswap(&Frequency, MinGapIndex[1], MinGapIndex[1]+1);
    F[2] = Frequency;
    
    Frequency = F[0];
    Dswap(&Frequency, MinGapIndex[1], MinGapIndex[1]+1);
    Dswap(&Frequency, MinGapIndex[0], MinGapIndex[0]+1);
    F[3] = Frequency;
    
    Frequency = F[0];
    Dswap(&Frequency, MinGapIndex[2], MinGapIndex[2]+1);
    F[4] = Frequency;
    
    Frequency = F[0];
    Dswap(&Frequency, MinGapIndex[2], MinGapIndex[2]+1);
    Dswap(&Frequency, MinGapIndex[0], MinGapIndex[0]+1);
    F[5] = Frequency;
    
    Frequency = F[0];
    Dswap(&Frequency, MinGapIndex[2], MinGapIndex[2]+1);
    Dswap(&Frequency, MinGapIndex[1], MinGapIndex[1]+1);
    F[6] = Frequency;
    
    Frequency = F[0];
    Dswap(&Frequency, MinGapIndex[2], MinGapIndex[2]+1);
    Dswap(&Frequency, MinGapIndex[1], MinGapIndex[1]+1);
    Dswap(&Frequency, MinGapIndex[0], MinGapIndex[0]+1);
    F[7] = Frequency;
    
    Frequency = F[0];
    Dswap(&Frequency, MinGapIndex[3], MinGapIndex[3]+1);
    F[8] = Frequency;
    
    Frequency = F[0];
    Dswap(&Frequency, MinGapIndex[3], MinGapIndex[3]+1);
    Dswap(&Frequency, MinGapIndex[0], MinGapIndex[0]+1);
    F[9] = Frequency;
    
    Frequency = F[0];
    Dswap(&Frequency, MinGapIndex[3], MinGapIndex[3]+1);
    Dswap(&Frequency, MinGapIndex[1], MinGapIndex[1]+1);
    F[10] = Frequency;
    
    Frequency = F[0];
    Dswap(&Frequency, MinGapIndex[3], MinGapIndex[3]+1);
    Dswap(&Frequency, MinGapIndex[2], MinGapIndex[2]+1);
    F[11] = Frequency;
    
    Frequency = F[0];
    Dswap(&Frequency, MinGapIndex[3], MinGapIndex[3]+1);
    Dswap(&Frequency, MinGapIndex[1], MinGapIndex[1]+1);
    Dswap(&Frequency, MinGapIndex[0], MinGapIndex[0]+1);
    F[12] = Frequency;
    
    Frequency = F[0];
    Dswap(&Frequency, MinGapIndex[3], MinGapIndex[3]+1);
    Dswap(&Frequency, MinGapIndex[2], MinGapIndex[2]+1);
    Dswap(&Frequency, MinGapIndex[0], MinGapIndex[0]+1);
    F[13] = Frequency;
    
    Frequency = F[0];
    Dswap(&Frequency, MinGapIndex[3], MinGapIndex[3]+1);
    Dswap(&Frequency, MinGapIndex[2], MinGapIndex[2]+1);
    Dswap(&Frequency, MinGapIndex[1], MinGapIndex[1]+1);
    F[14] = Frequency;
    
    Frequency = F[0];
    Dswap(&Frequency, MinGapIndex[3], MinGapIndex[3]+1);
    Dswap(&Frequency, MinGapIndex[2], MinGapIndex[2]+1);
    Dswap(&Frequency, MinGapIndex[1], MinGapIndex[1]+1);
    Dswap(&Frequency, MinGapIndex[0], MinGapIndex[0]+1);
    F[15] = Frequency;
    
    /*从16个可能密码里筛选*/
    double MinEntrophy[10] = {5,5,5,5,5,5,5,5,5,5};
    int MinEnIndex[10] = {0,1,2,3,4,5,6,7,8,9};
    int i, emaxi;
    double entrophy;
    
    /*取相对熵最小的10个*/
    for (i = 0; i < 16; i++)
    {
        entrophy = GetEntrophy(F[i].Value, StandardFrequency);
        emaxi = ffindMax(MinEntrophy, 10);
        if (entrophy < MinEntrophy[emaxi])
        {
            MinEntrophy[emaxi] = entrophy;
            MinEnIndex[emaxi] = i;
        }
    }
    
    /*密码写入文件*/
    char name[10] = "k";
    char suffix[10];
    for (i = 0 ; i < 10; i++)
    {
        
        suffix[0] = '0' + i;
        suffix[1] = '\0';
        strcat(suffix, ".txt");
        strcat(name, suffix);
        
        writeFrTo(F[MinEnIndex[i]], name);
        
        name[1] = '\0';
        
        
    }
    
    
}
