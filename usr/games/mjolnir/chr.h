#ifndef __MJOLNIR_CHR_H__
#define __MJOLNIR_CHR_H__

struct objchr;

void                            chrinit(struct objchr *chr,
                                        const char *name,
                                        long type,
                                        long cat);

/* character categories */
#define CHR_PROGRAMMER_CATEGORY 1
#define CHR_CRACKER_CATEGORY    2
#define CHR_CYBORG_CATEGORY     3
#define CHR_THIEF_CATEGORY      4
#define CHR_ENGINEER_CATEGORY   5
#define CHR_MAX_CATEGORY        5

/* characters */
#define CHR_PLAYER              '@'
#define CHR_ANT                 'a'
#define CHR_SOLDIER_ANT         'A'
#define CHR_BEE                 'b'
#define CHR_BEE_QUEEN           'q'
#define CHR_DOG                 'd'
#define CHR_ALIEN               'e'
#define CHR_GHOUL               'g'
#define CHR_HUMAN               'H'
#define CHR_ORACLE              'O'
#define CHR_RAT                 'r'
#define CHR_SHOPKEEPER          '£'
#define CHR_UNICORN             'u'
#define CHR_VAMPIRE             'v'
#define CHR_WEREWOLF            'w'
#define CHR_WIZARD              'W'
#define CHR_ZOMBIE              'Z'

/* special characters */
#define CHR_FENRIS              'F'
#define CHR_KARA                'K'
#define CHR_THOR                'T'
#define CHR_UKKO                'U'
#define CHR_DRACULA             'V'
#define CHR_LADY_X              'X'

/* character flags */
#define CHR_BLIND               (1 << 0)    // character blind
#define CHR_LEVITATING          (1 << 1)    // character levitating
#define CHR_INVISIBLE           (1 << 2)    // character invisible
#define CHR_STARVING            (1 << 3)    // character starving
#define CHR_DROWNING            (1 << 4)    // character drowning
#define CHR_SINKING             (1 << 5)    // character sinking (quicksand)
#define CHR_POISONED            (1 << 5)    // ghoul-bite or other poisoning
#define CHR_DRUNK               (1 << 6)    // character drunk
#define CHR_HIGH                (1 << 7)    // character is high

/* speed values */
#define CHR_SLOW                (-1)        // slow speed
#define CHR_FROZEN              0           // character can't move
#define CHR_NORMAL              1           // normal speed
#define CHR_FAST                2           // character is moving faster

#endif /* __MJOLNIR_CHR_H__ */

