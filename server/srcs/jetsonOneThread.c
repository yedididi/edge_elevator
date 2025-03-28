#include "../incs/main.h"

//outside

void    *jetsonOneThread(void *arg)
{
    t_data *jetsonData = (t_data *)arg;
    char buf[BUFSIZE];

    printf("jetson outside thread starting, clientfd is %d\n", jetsonData->clientfd);
    while (1)
    {
        int ret = read(jetsonData->clientfd, buf, BUFSIZE);

        if (strncmp(buf, "1", ret) == 0)
            *(jetsonData->wheelChair) = 1;
        else
            *(jetsonData->wheelChair) = 0;
    }
    return (NULL);
}