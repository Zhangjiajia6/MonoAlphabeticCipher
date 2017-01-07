#include <stdio.h>
#include "./../publicFuncLib/publib.c"

int main(int argc, char **argv)
{
    if (argc < 3)
        printf("Usage: compare [TRPwd] [Pwd]\n");
        
    int i,num = 0;
    FILE *p1, *p2;
    p1 = OpenFileAndCheck(argv[1],"r");
    p2 = OpenFileAndCheck(argv[2],"r");
    char ch1, ch2;
    
    for (i = 0; i < 26; i++)
    {
        ch1 = getc(p1);
        ch2 = getc(p2);
        printf("%c --> %c\n", ch1, ch2);
        
        if (ch1 != ch2)
            num++;
    }
    
    CloseFileAndCheck(p1);
    CloseFileAndCheck(p2);
    
    printf("\nerror num is %d\n", num);
    printf("\nerror rate is %lf\n", ((double)num)/26);
}
