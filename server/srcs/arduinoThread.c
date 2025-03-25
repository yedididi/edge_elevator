#include "../incs/main.h"

void    *arduinoThread(void *arg)
{
    t_data *arduinoData = (t_data *)arg;
    char buf[BUFSIZE];
    char rfids[BUFSIZE];
    char **split_ids;

    memset(rfids, 0, BUFSIZE);
    printf("arduino thread starting\n");
    while (1)
    {
        switch (*(arduinoData->state))
        {
            case (GET_RFID):
            {
                memset(buf, 0, BUFSIZE);
                int ret = read(arduinoData->clientfd, buf, BUFSIZE);
                printf("arduino:%s, %d read\n", buf, ret);
                if (strncmp(buf, "BUTTON_PRESSED", ret) == 0)
                {
                    printf("changing state to BUTTON_PRESSED\n");
                    split_ids = ft_split(rfids, '*');
                    arduinoData->rfidData = getDatabaseInfo(split_ids);
                    *(arduinoData->state) = BUTTON_PRESSED;
                }
                else
                {
                    buf[12] = '\0';
                    strcat(rfids, buf);
                    strcat(rfids, "*");
                    printf("added rfids:%s\n", rfids);
                }
                break;
            }

            case (MOTOR_ON_FOR_YIELD):
            {
                int readRet;
                char sendBuf[BUFSIZE];
                char readBuf[BUFSIZE];

                sprintf(sendBuf, "MOTOR_ON_FOR_YIELD");
                write(arduinoData->clientfd, sendBuf, strlen(sendBuf));

                while (1)
                {
                    readRet = read(arduinoData->clientfd, readBuf, BUFSIZE);
                    
                    if (strncmp(readBuf, "YIELD_MOTOR_DONE", readRet) == 0)
                    {
                        *(arduinoData->state) = YIELD_MOTOR_DONE;
                        split_ids = eraseRFID(split_ids, arduinoData->rfidData->notDisabled_RFID);
                        break;
                    }
                }
                break;
            }

            case (ELEVATOR_START):
            {
                // freeRFIDS(split_ids);
                memset(rfids, 0, BUFSIZE);
            }
        }


    }
    return (NULL);
}

void freeRFIDS(char **split_ids)
{
    for (int i = 0; split_ids[i]; i++)
    {
        free(split_ids[i]);
    }
    free(split_ids);
}

char **eraseRFID(char **split_ids, char *eraseID)
{
    char newString[BUFSIZE] = {0, };

    for (int i = 0; split_ids[i]; i++)
    {
        if (strncmp(split_ids[i], eraseID, strlen(eraseID)) == 0)
        {
            free(split_ids[i]);
            continue;
        }

        strcat(newString, split_ids[i]);
        strcat(newString, "*");
        free(split_ids[i]);
    }
    free(split_ids);
    
    return (ft_split(newString, '*'));
}