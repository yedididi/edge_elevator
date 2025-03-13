#include "../incs/main.h"

void    *jetsonOneThread(void *arg)
{
    t_data *jetsonData = (t_data *)arg;
    char buf[BUFSIZE];

    while (1)
    {
        int ret = read(jetsonData->clientfd, buf, BUFSIZE);
        if (ret <= 0)
            printf("read error\n");

        if (strncmp(buf, "1", ret) == 0)
            printf("wheelchair detacted\n");
        else
            printf("wheelChair not detacted\n");
            
        sleep(5);
    }
    return (NULL);
}