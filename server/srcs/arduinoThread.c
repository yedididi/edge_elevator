#include "../incs/main.h"

void    *arduinoThread(void *arg)
{
    t_data *arduinoData = (t_data *)arg;
    char buf[BUFSIZE];

    //must add rfid here
    while (1)
    {
        int ret = read(arduinoData->clientfd, buf, BUFSIZE);
        if (strncmp(buf, "BUTTON_PRESSED", ret) == 0)
        {
            *(arduinoData->state) = BUTTON_PRESSED;
        }
    }
    return (NULL);
}