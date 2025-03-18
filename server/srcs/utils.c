#include "../incs/main.h"


int getWd(char *string)
{
    int wd = 0;
    int i = 0;

    while (1)
    {
        if (string[i])
        {
            if (string[i] == ' ')
                wd++;
            i++;
        }
        else
            break;
    }
    return (wd);
}

int getWdLen(char *string)
{
    int i = 0;

    while (string[i] != ' ')
    {
        i++;
    }
    return (i);
}

char **split(char *string)
{
    int index = 0;
    char **ret;
    int wordNum;

    wordNum = getWd(string);
    ret = (char **)malloc(sizeof(char *) * (wordNum + 1));
    if (ret == NULL)
        printf("NULL error\n");
    
    for (int i = 0; i < wordNum; i++)
    {
        int wdLen = getWdLen(string[index]);

        ret[i] = (char *)malloc(sizeof(char) * (wdLen + 1));
        if (ret[i] == NULL)
            printf("NULL error\n");
        
        for (int j = 0; j < wdLen; j++)
            ret[i][j] = string[index];
        ret[i][wdLen] = '\0';
        index += wdLen;
    }
    ret[wordNum] = NULL;
    return (ret);
}