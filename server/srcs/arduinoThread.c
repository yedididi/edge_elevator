#include "../incs/main.h"

void    *arduinoThread(void *arg)
{
    t_data *arduinoData = (t_data *)arg;
    char buf[BUFSIZE];

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
                *(arduinoData->state) = BUTTON_PRESSED;
            }
        }
    }
    return (NULL);
}