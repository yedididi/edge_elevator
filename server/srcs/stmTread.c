#include "../incs/main.h"

void    *stmThread(void *arg)
{
    t_data *stmData = (t_data *)arg;
    char buf[BUFSIZE];

    printf("stm thread starting\n");
    while (1)
    {
        if (*(stmData->state) == ELEVATOR_START)
        {
            printf("wrote to stm(%d), ELEVATOR_START\n", stmData->clientfd);
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