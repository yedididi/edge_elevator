#include "../incs/main.h"

void voice_command_handling(t_data *raspberryData, const char *command) {
    printf("voice command handling: %s\n", command);
    
    if (strstr(command, "엘리베이터") != NULL) 
    {
        printf("엘리베이터 음성 감지\n");
        // 코드 추가
    }
    else if (strstr(command, "1층") != NULL || strstr(command, "일층") != NULL) 
    {
        printf("1층 이동 음성 감지\n");
        *(raspberryData->state) = 0;
        // 코드 추가
    }
    else if (strstr(command, "2층") != NULL || strstr(command, "이층") != NULL) 
    {
        printf("2층 이동 음성 감지\n");
        // 코드 추가
    }
    else if (strstr(command, "문 열어") != NULL) 
    {
        printf("문 열림 음성 감지\n");
        // 코드 추가
    }
    else if (strstr(command, "문 닫아") != NULL ) 
    {
        printf("문 닫힘 음성성 감지\n");
        // 코드 추가
    }
}

void *raspberryThread(void *arg)
{
    t_data *raspberryData = (t_data *)arg;
    char buf[BUFSIZE];
    int ret;

    printf("RaspberryPi Thread started, clientfd: %d\n", raspberryData->clientfd);
    while (1)
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
        voice_command_handling(raspberryData, buf);
    }
    printf("RaspberryPi Thread terminated.\n");
    return (NULL);
}