#include "../incs/main.h"

//yeji branch, first goal code

void mainThread(int *state, bool *wheelchair, bool *people, int *disabled, int *notDisabled)
{
    printf("main thread starting\n");
    while (1)
    {
        switch (*state)
        {
            case (ARRIVED_DOWNSTAIRS):
            {
                speakerOn(false, 0, "TTS:elevator_end");
                sleep(1);
                *state = GET_RFID;
                break;
            }

            case (GET_RFID):
            {
                while (1)
                {
                    if (*state == BUTTON_PRESSED)
                    {
                        if (checkInAndOut(wheelchair, people, disabled, notDisabled))
                        {
                            speakerOn(false, 0, "TTS:wheelchair");
                            *state = GET_RFID;
                            break;
                        }
                        else
                        {
                            *state = ELEVATOR_START;
                            speakerOn(false, 0, "TTS:elevator_start");
                            break;
                        }
                    }
                }
                break;
            }
        }
    }
}

bool checkInAndOut(bool *wheelChair, bool *people, int *disabled, int *notDisabled)
{
    (void)people;

    if (*wheelChair && (*notDisabled) > 0)
        return (true);
    else
        return (false);
}