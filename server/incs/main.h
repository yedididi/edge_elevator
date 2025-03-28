#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/syscall.h>
#include <pthread.h>
#include <stdbool.h>
#include <sys/select.h>

#define SERVER_PORT 5000
#define LISTEN_BACKLOG 10
#define BUFSIZE 1024

enum e_state 
{
    ARRIVED_DOWNSTAIRS,
    GET_RFID,
    BUTTON_PRESSED,
    ELEVATOR_START,
    SPEAKER_YIELD,
    SPEAKER_TAG_REQUIRED,
    MOTOR_ON_FOR_YIELD,
    YIELD_MOTOR_DONE,
};

enum e_thread_order
{
    JETSON_OUTSIDE,
    JETSON_INSIDE,
    RASPBERRY,
    ARDUINO,
    STM,
};

typedef struct s_client
{
    pthread_t pid;
    int clientfd;
} t_client;

typedef struct s_database
{
    int disabled;
    int notDisabled;
    char notDisabled_RFID[BUFSIZE];
} t_database;

typedef struct s_data
{
    pthread_t pid;
    int *state;
    int clientfd;
    bool *wheelChair;
    bool *people;
    int *disabled;
    int *notDisabled;
    t_database *rfidData;
} t_data;

//socket
int startSocket(struct sockaddr_in *addr_server, socklen_t *addr_len);

//init
t_data **init();
void endThings(t_data **datas);

//stmThread
void    *stmThread(void *arg);

//arduinoThread
void    *arduinoThread(void *arg);
void freeRFIDS(char **split_ids);
char **eraseRFID(char **split_ids, char *eraseID);

//jetsonOneThread
void    *jetsonOneThread(void *arg);

//jetsonTwoThread
void    *jetsonTwoThread(void *arg);

//raspberryThread
void    *raspberryThread(void *arg);
void    speakerOn(bool set, int clientFD, char *str);

//mainThread
void mainThread(int *state, bool *wheelchair, bool *people, int *disabled, int *notDisabled);
bool checkInAndOut(bool *wheelChair, bool *people, int *disabled, int *notDisabled);

//utils
char	**ft_split(char const *s, char c);

//database
t_database *getDatabaseInfo(char** id);

#endif