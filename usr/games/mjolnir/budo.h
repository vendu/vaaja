#ifndef MJOLNIR_BUDO_H
#define MJOLNIR_BUDO_H

/* close combat ranks based on karate belts */
#define MJOLNIR_WHITE_RANK      0   // EXP 0-15
#define MJOLNIR_ORANGE_RANK     1   // EXP 16-31
#define MJOLNIR_BLUE_RANK       2   // EXP 32-63
#define MJOLNIR_YELLOW_RANK     3   // EXP 64-127
#define MJOLNIR_GREEN_RANK      4   // EXP 128-191
#define MJOLNIR_BROWN_RANK      5   // EXP 192-319
#define MJOLNIR_BLACK_RANK      6   // EXP 320-575
#define MJOLNIR_RANKS           7

struct budo {
    long                        exp;        // experience points
    long                        level;      // based on experience points
    long                        rank;       // current belt
    long                        ref;        // reflex probability for saves
    long                        hitprob;    // hit success probability
    struct d20dice              hitdice;    // rank-based dice-set for atk
};

#endif /* MJOLNIR_BUDO_H */

