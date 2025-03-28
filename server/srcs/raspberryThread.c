#include "../incs/main.h"

void *raspberryThread(void *arg)
{
    t_data *raspberryData = (t_data *)arg;
    char buf[BUFSIZE];
    int ret;
    printf("RaspberryPi Thread started, clientfd: %d\n", raspberryData->clientfd);
  
    while (1)
    {
        if (*(raspberryData->state) == GET_RFID)
        {    
            memset(buf, 0, BUFSIZE); 
            ret = read(raspberryData->clientfd, buf, BUFSIZE - 1);
            if (ret <= 0)
            {
                printf("RaspberryPi Connection Error\n");
                break;
            }
            buf[ret] = '\0'; 
            printf("RaspberryPi Data: %s\n", buf);

            if (strncmp(buf, "BUTTON_PRESSED", ret) == 0)
            {
                *(raspberryData->state) = BUTTON_PRESSED;
            }
        }
    }
    printf("RaspberryPi Thread terminated.\n");
    return (NULL);
}

void speakerOn(bool set, int clientFD, char *str)
{
    static int raspFD;

    if (set == true)
    {
        raspFD = clientFD;
        return ;
    }
    write(raspFD, str, strlen(str));
    printf("wrote to rasp, %s\n", str);
}