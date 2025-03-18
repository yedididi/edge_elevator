#include "../incs/main.h"

void mainThread(int *state, int *wheelchair, int *people, char **rfidStr)
{
    while (1)
    {
        switch (*state)
        {
            case (ARRIVED_DOWNSTAIRS):
            {
                sleep(5);
                *state = GET_RFID;
            }

            case (GET_RFID):
            {
                while (1)
                {
                    if (*state == BUTTON_PRESSED)
                    {
                        if (checkInAndOut(wheelchair, people, rfidStr) == 0)
                        {
                            *state = SPEAKER_YIELD;
                            break;
                        }
                        else
                        {
                            *state = ELEVATOR_START;
                            break;
                        }
                    }
                }
            }
        }
    }
}

bool checkInAndOut(int *wheelChair, int *people, char **rfidStr)
{
    char **rfid_double;
    (void)people;

    rfid_double = split(rfidStr);
    database(rfid_double);
    if (*wheelChair == 1)
        return (false);
    else
        return (true);
}