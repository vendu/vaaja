#ifndef MJOLNIR_BUDO_H
#define MJOLNIR_BUDO_H

/* close combat ranks based on karate belts */
#define MJOLNIR_WHITE_RANK      0
#define MJOLNIR_YELLOW_RANK     1
#define MJOLNIR_ORANGE_RANK     2
#define MJOLNIR_GREEN_RANK      3
#define MJOLNIR_BLUE_RANK       4
#define MJOLNIR_PURPLE_RANK     5
#define MJOLNIR_RED_RANK        5
#define MJOLNIR_BROWN_RANK      7
#define MJOLNIR_BLACK_RANK      8
#define MJOLNIR_RANKS           9

struct budo {
    long                        exp;        // experience points
    long                        level;      // based on experience points
    long                        rank;       // current belt
    long                        ref;        // reflex probability for saves
    long                        hitprob;    // hit success probability
    struct d20dice              hitdice;    // rank-based dice-set for atk
};

#endif /* MJOLNIR_BUDO_H */

