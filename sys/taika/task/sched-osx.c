// approximate_5by8.c
#include <stdio.h>
#include <math.h>
struct shift {     int shift1;     int shift2; };
#define SCHED_DECAY_TICKS 32
static struct shift sched_decay_shifts[SCHED_DECAY_TICKS] = {
    {1, 1}, {1, 3}, {1, -3}, {2, -7},
    {3, 5}, {3, -5}, {4, -8}, {5, 7},
    {5, -7}, {6, -10}, {7, 10}, {7, -9},
    {8, -11}, {9, 12}, {9, -11}, {10, -13},
    {11,14}, {11,-13}, {12,-15}, {13,17},
    {13,-15}, {14,-17}, {15,19}, {16,18},
    {16,-19}, {17,22}, {18,20}, {18,-20},
    {19,26}, {20,22}, {20,-22}, {21,-27}
};

int main(void)
{
    int    i, v, v0 = 10000000;
    double x5_8,  y5_8;
    double const5_8 = (double)5/(double)8;
    struct shift *shiftp;

    for (i = 0; i < SCHED_DECAY_TICKS; i++) {
        shiftp = &sched_decay_shifts[i];
        v = v0;
        if (shiftp->shift2 > 0)
            v = (v >> shiftp->shift1) + (v >> shiftp->shift2);
        else
            v = (v >> shiftp->shift1) - (v >> -(shiftp->shift2));
        x5_8 = pow(const5_8,  (double)i);
        y5_8 = (double)v/(double)v0;
        printf("%10.10f\t%10.10f\t%10.2f\n",
               x5_8, y5_8,
               ((x5_8 - y5_8)/x5_8) * 100.0);
    }

    return 0;
}

