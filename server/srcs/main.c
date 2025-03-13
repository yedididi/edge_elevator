#include "../incs/main.h"

char threadName[5][20] = {"jetsonOutside", "jetsonInside", "raspberry", "arduino", "stm"};
void *(*threadFunc[5])(void *) = {jetsonOneThread, jetsonTwoThread, raspberryThread, arduinoThread, stmThread};
t_data *datas[5];

int main(void)
{
    int serverfd, max;
    int next_id = 0;
    fd_set fds, wfds, rfds;
    socklen_t addr_len;
    struct sockaddr_in addr;
    char bufRead[BUFSIZE];
    t_client clients[1024];
    int state, wheelChair;

    serverfd = startSocket(&addr, &addr_len);
    init();

    max = serverfd;
    while (1) 
    {
        wfds = rfds = fds;

        if (select(max + 1, &wfds, &rfds, NULL, NULL) < 0)
            continue;

        for (int i = 0; i <= max; i++)
        {
            if (FD_ISSET(i, &wfds) && i == serverfd)
            {
                int clientSock = accept(serverfd, (struct sockaddr *)&addr, &addr_len);
                if (clientSock < 0)
                    continue;
                max = (clientSock > max) ? clientSock : max;
                clients[i].clientfd = clientSock;
                FD_SET(clientSock, &fds);
                break;
            }
            if (FD_ISSET(i, &wfds) && i != serverfd)
            {
                int res = read(i, bufRead, BUFSIZE);
                if (res <= 0)
                {
                    FD_CLR(i, &fds);
                    close(i);
                    break;
                }
                else
                {
                    for (int j = 0; j < 5; j++)
                    {
                        if (strncmp(threadName[j], bufRead, res) == 0)
                        {
                            datas[j]->state = &state;
                            datas[j]->clientfd = clients[i].clientfd;
                            datas[j]->wheelChair = &wheelChair;
                            pthread_create(&(clients[i].pid), NULL, threadFunc[j], (void *)datas[j]);
                        }
                    }
                }
            }
        }
    }
    
}


