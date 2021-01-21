#ifndef __MJOLNIR_CHR_H__
#define __MJOLNIR_CHR_H__

void                                    chrinit(struct objchr *chr,
                                                long cat,
                                                const char *name);

//extern struct chrcatname                chrcatnames[MJOLNIR_MAX_CATEGORY + 1];

/* characters */
#define MJOLNIR_CHR_PLAYER              '@'
#define MJOLNIR_CHR_ANT                 'a'
#define MJOLNIR_CHR_SOLDIER_ANT         'A'
#define MJOLNIR_CHR_BEE                 'b'
#define MJOLNIR_CHR_BEE_QUEEN           'B'
#define MJOLNIR_CHR_CYBORG              'c'
#define MJOLNIR_CHR_DOG                 'd'
#define MJOLNIR_CHR_ALIEN               'e'
#define MJOLNIR_CHR_GHOUL               'g'
#define MJOLNIR_CHR_HUMAN               'H'
#define MJOLNIR_CHR_ORACLE              'O'
#define MJOLNIR_CHR_RAT                 'r'
#define MJOLNIR_CHR_SHOPKEEPER          'S'
#define MJOLNIR_CHR_UNICORN             'u'
#define MJOLNIR_CHR_VAMPIRE             'v'
#define MJOLNIR_CHR_WEREWOLF            'w'
#define MJOLNIR_CHR_WIZARD              'W'
#define MJOLNIR_CHR_ZOMBIE              'Z'

/* special characters */
#define MJOLNIR_CHR_FENRIS              'F'
#define MJOLNIR_CHR_KARA                'K'
#define MJOLNIR_CHR_THOR                'T'
#define MJOLNIR_CHR_UKKO                'U'
#define MJOLNIR_CHR_DRACULA             'V'
#define MJOLNIR_CHR_LADY_X              'X'

/* character flags */
#define MJOLNIR_CHR_BLIND               (1 << 0)    // character blind
#define MJOLNIR_CHR_LEVITATING          (1 << 1)    // character levitating
#define MJOLNIR_CHR_INVISIBLE           (1 << 2)    // character invisible
#define MJOLNIR_CHR_STARVING            (1 << 3)    // character starving
#define MJOLNIR_CHR_DROWNING            (1 << 4)    // character drowning
#define MJOLNIR_CHR_SINKING             (1 << 5)    // character sinking (quicksand)
#define MJOLNIR_CHR_POISONED            (1 << 5)    // ghoul-bite or other poisoning
#define MJOLNIR_CHR_DRUNK               (1 << 6)    // character drunk
#define MJOLNIR_CHR_HIGH                (1 << 7)    // character is high

/* speed values */
#define MJOLNIR_CHR_SLOW                (-1)        // slow speed
#define MJOLNIR_CHR_FROZEN              0           // character can't move
#define MJOLNIR_CHR_NORMAL              1           // normal speed
#define MJOLNIR_CHR_FAST                2           // character is moving faster

#endif /* __MJOLNIR_CHR_H__ */

