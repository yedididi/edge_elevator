#include "../incs/main.h"

// int raspberryfd;

// void voice_command_handling(t_data *raspberryData, const char *command) {
//     printf("voice command handling: %s\n", command);
//     char response[BUFSIZE];

//     if (strncmp(command, "ELEVATOR_START", ) != NULL) 
//     {
//         printf("1층 이동 음성 감지\n");
//         *(raspberryData->state) = 0;
//         // 엘리베이터 제어 명령 전달


//         // 음성 출력 전달
//         strcpy(response, "TTS:floor1");
//         printf("sending to client: %s\n", response);
//         write(raspberryData->clientfd, response, strlen(response));
//     }
// }

void *raspberryThread(void *arg)
{
    t_data *raspberryData = (t_data *)arg;
    char buf[BUFSIZE];
    int ret;
    // int raspberryfd;
    
    // raspberryfd = raspberryData->clientfd;
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

            // voice_command_handling(raspberryData, buf);
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