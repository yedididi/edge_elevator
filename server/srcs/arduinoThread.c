#include "../incs/main.h"

void    *arduinoThread(void *arg)
{
    t_data *arduinoData = (t_data *)arg;
    char buf[BUFSIZE];
    char rfids[BUFSIZE];

    memset(rfids, 0, BUFSIZE);
    printf("arduino thread starting\n");
    while (1)
    {
        if (*(arduinoData->state) == ARRIVED_DOWNSTAIRS)
        {
            int ret = read(arduinoData->clientfd, buf, BUFSIZE);
            printf("arduino:%s, %d read\n", buf, ret);
            if (strncmp(buf, "BUTTON_PRESSED", ret) == 0)
            {
                printf("changing state to BUTTON_PRESSED\n");
                printf("this is database info:%d\n", getDatabaseInfo(ft_split(rfids, '*')));
                *(arduinoData->state) = BUTTON_PRESSED;

            }
            else
            {
                buf[12] = '\0';
                strcat(rfids, buf);
                strcat(rfids, "*");
                printf("added rfids:%s\n", rfids);
            }
        }
    }
    return (NULL);
}