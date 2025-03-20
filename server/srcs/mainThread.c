#include "../incs/main.h"

//yeji branch, first goal code

void mainThread(int *state, bool *wheelchair, bool *people)
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
                while (1)
                {
                    if (*state == BUTTON_PRESSED)
                        *state = ELEVATOR_START;
                }
            }
        }
    }
}

bool checkInAndOut(int *wheelChair)
{
    if (*wheelChair == 1)
        return (false);
    else
        return (true);
}