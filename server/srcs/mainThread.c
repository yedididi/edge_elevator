#include "../incs/main.h"

void mainThread(int *state, int *wheelchair, int *people, char **rfidStr)
{
    while (1)
    {
        switch (*state)
        {
            case (ARRIVED_DOWNSTAIRS):
            {
                sleep(5);
                *state = GET_RFID;
            }

            case (GET_RFID):
            {
                while (1)
                {
                    if (*state == BUTTON_PRESSED)
                    {
                        if (checkInAndOut(wheelchair, people, rfidStr) == 0)
                        {
                            *state = SPEAKER_YIELD;
                            break;
                        }
                        else
                        {
                            *state = ELEVATOR_START;
                            break;
                        }
                    }
                }
            }
        }
    }
}

bool checkInAndOut(int *wheelChair, int *people, char **rfidStr)
{
    char **rfid_double;
    (void)people;

    rfid_double = split(rfidStr);
    database(rfid_double);
    if (*wheelChair == 1)
        return (false);
    else
        return (true);
}

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