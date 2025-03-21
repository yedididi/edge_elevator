#include "../incs/main.h"

//yeji branch, first goal code

void mainThread(int *state, bool *wheelchair, bool *people, bool *rfidData)
{
    (void)wheelchair;
    (void)people;

    printf("main thread starting\n");
    while (1)
    {
        switch (*state)
        {
            case (ARRIVED_DOWNSTAIRS):
            {
                sleep(5);
                *state = GET_RFID;
                break;
            }

            case (GET_RFID):
            {
                while (1)
                {
                    if (*state == BUTTON_PRESSED)
                    {
                        if (checkInAndOut(wheelchair, people, rfidData))
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
                break;
            }
        }
    }
}

bool checkInAndOut(bool *wheelChair, bool *people, bool *rfidData)
{
    (void)people;
    //returns 0 -> elevator start
    if (*wheelChair == 0 || (*wheelChair == 1 && *rfidData == 0) )
        return (false);
    else
        return (true);
}