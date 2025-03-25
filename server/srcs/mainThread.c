#include "../incs/main.h"

//yeji branch, first goal code

void mainThread(int *state, bool *wheelchair, bool *people, t_database *rfidData)
{
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
                            speakerOn("yield");
                            *state = MOTOR_ON_FOR_YIELD;
                            
                            while (1)
                            {
                                if (*state == YIELD_MOTOR_DONE)
                                {
                                    *state = GET_RFID;
                                    break;
                                }
                            }
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

bool checkInAndOut(bool *wheelChair, bool *people, t_database *rfidData)
{
    (void)people;
    //returns false -> elevator start
    //returns true  -> elevator stay

    if (*wheelChair && rfidData->notDisabled > 0)
        return (true);
    else
        return (false);
}