#include "../incs/main.h"

void    *stmThread(void *arg)
{
    t_data *stmData = (t_data *)arg;
    char buf[BUFSIZE];

    while (1)
    {
        if (*(stmData->state) == GET_RFID)
        {
            int ret = read(stmData->clientfd, buf, BUFSIZE);
            buf[ret] = 0;
            strcat(*(stmData->rfidStr), " ");
            strcat(*(stmData->rfidStr), buf);
        }
        if (*(stmData->state) == ELEVATOR_START)
        {
            write(stmData->clientfd, "ELEVATOR_START", 14);
            int ret = read(stmData->clientfd, buf, BUFSIZE);
            if (strncmp(buf, "ARRIVED_DOWNSTAIRS", ret) == 0)
            {
                *(stmData->state) = ARRIVED_DOWNSTAIRS;
                sleep(1);
            }
        }
    }
    return (NULL);
}