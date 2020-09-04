#ifndef __MJOLNIR_CMD_H__
#define __MJOLNIR_CMD_H__

/* commands */
#define ESC                         '\033'
#define CTRL(x)                     ((x) & 0x80)
/* rogue commands */
#define MJOLNIR_CMD_BOSS            ESC
#define MJOLNIR_CMD_MENU            'm'
#define MJOLNIR_CMD_IDENTIFY        '/'
#define MJOLNIR_CMD_MOVE_LEFT       'h'
#define MJOLNIR_CMD_MOVE_DOWN       'j'
#define MJOLNIR_CMD_MOVE_UP         'k'
#define MJOLNIR_CMD_MOVE_RIGHT      'l'
#define MJOLNIR_CMD_MOVE_UPLEFT     'y'
#define MJOLNIR_CMD_MOVE_UPRIGHT    'u'
#define MJOLNIR_CMD_MOVE_DOWNLEFT   'b'
#define MJOLNIR_CMD_MOVE_DOWNRIGHT  'n'
#if (MJOLNIR_CURSES)
/* hold shift down to run, control to pan */
#define MJOLNIR_CMD_LEFT            (KEY_LEFT)
#define MJOLNIR_CMD_DOWN            (KEY_DOWN)
#define MJOLNIR_CMD_UP              (KEY_UP)
#define MJOLNIR_CMD_RIGHT           (KEY_RIGHT)
/* keypad keys */
#define MJOLNIR_CMD_UPLEFT          (KEY_A1)
#define MJOLNIR_CMD_UPRIGHT         (KEY_A3)
#define MJOLNIR_CMD_DOWNLEFT        (KEY_C1)
#define MJOLNIR_CMD_DOWNRIGHT       (KEY_C3)
#endif
#define MJOLNIR_CMD_RUN_LEFT        'H'
#define MJOLNIR_CMD_RUN_DOWN        'J'
#define MJOLNIR_CMD_RUN_UP          'K'
#define MJOLNIR_CMD_RUN_RIGHT       'L'
#define MJOLNIR_CMD_RUN_UPLEFT      'Y'
#define MJOLNIR_CMD_RUN_UPRIGHT     'U'
#define MJOLNIR_CMD_RUN_DOWNLEFT    'B'
#define MJOLNIR_CMD_RUN_DOWNRIGHT   'N'
#define MJOLNIR_CMD_PAN_LEFT        CTRL('h')
#define MJOLNIR_CMD_PAN_DOWN        CTRL('j')
#define MJOLNIR_CMD_PAN_UP          CTRL('k')
#define MJOLNIR_CMD_PAN_RIGHT       CTRL('l')
#define MJOLNIR_CMD_PAN_UPLEFT      CTRL('y')
#define MJOLNIR_CMD_PAN_UPRIGHT     CTRL('u')
#define MJOLNIR_CMD_PAN_DOWNLEFT    CTRL('b')
#define MJOLNIR_CMD_PAN_DOWNRIGHT   CTRL('n')
/* actions */
#define MJOLNIR_CMD_FIGHT           'f'         // fight a character till death
#define MJOLNIR_CMD_HIT             'h'         // hit an object or character
#define MJOLNIR_CMD_THROW           't'         // throw an object
#define MJOLNIR_CMD_MOVE            'm'         // move an object
#define MJOLNIR_CMD_ZAP             'z'         // zap a wand
#define MJOLNIR_CMD_IDENTIFY_TRAP   '^'         // identify trap type
#define MJOLNIR_CMD_SEARCH          's'         // search adjacent cell
#define MJOLNIR_CMD_LEVEL_DOWN      '>'         // go level down
#define MJOLNIR_CMD_LEVEL_UP        '<'         // go level up
#define MJOLNIR_CMD_REST            '.'         // rest to heal
#define MJOLNIR_CMD_PICK_UP         ','         // pick an object
#define MJOLNIR_CMD_INVENTORY       'i'         // show next inventory page
#define MJOLNIR_CMD_INVENTORY_ONE   'I'         // show inventory item
#define MJOLNIR_CMD_QUAFF           'q'         // drink potion
#define MJOLNIR_CMD_READ            'r'         // read a scroll
#define MJOLNIR_CMD_EAT             'e'         // eat something
#define MJOLNIR_CMD_WIELD           'w'         // wield weapon
#define MJOLNIR_CMD_WEAR            'W'         // wear armor
#define MJOLNIR_CMD_TAKE_ARMOR_OFF  'T'         // unwear armor
#define MJOLNIR_CMD_PUT_RING_ON     'P'         // wear a ring
#define MJOLNIR_CMD_REMOVE_RING     'R'         // unwear a ring
#define MJOLNIR_CMD_DROP            'd'         // drop an object
#define MJOLNIR_CMD_CALL            'c'         // call an object a given name
/* messages and statistics */
#define MJOLNIR_CMD_REPEAT          'a'         // repeat last command
#define MJOLNIR_CMD_PRINT_WEAPONS   ')'         // print wielded weapons
#define MJOLNIR_CMD_PRINT_ARMOR     ']'         // print current armor
#define MJOLNIR_CMD_PRINT_RINGS     '='         // print current rings
#define MJOLNIR_CMD_PRINT_STATS     '@'         // print current statistics
#define MJOLNIR_CMD_RECALL          'D'         // recall discoveries
/* screen and game control */
#define MJOLNIR_CMD_REDRAW          CTRL('R')   // redraw screen
#define MJOLNIR_CMD_REPEAT_MESSAGE  CTRL('P')   // print last message
#define MJOLNIR_CMD_CANCEL          ESC         // cancel action
#define MJOLNIR_CMD_SHELL           '!'         // escape to a shell
#define MJOLNIR_CMD_SAVE            'S'         // save game
#define MJOLNIR_CMD_QUIT            'Q'         // quit
/* mjolnir extensions */
#define MJOLNIR_CMD_OPTIONS         'O'         // game settings
#define MJOLNIR_CMD_GLANCE          ';'         // explain map symbol (far look)
#define MJOLNIR_CMD_LOOK            ':'         // show nearby symbols
#define MJOLNIR_CMD_KNOW            '\'         // show discovered objects
#define MJOLNIR_CMD_OPEN            'o'         // sacrifice to gods
#define MJOLNIR_CMD_CLOSE           'c'         // close a door
#define MJOLNIR_CMD_FIRE            'f'         // fire a weapon
#define MJOLNIR_CMD_ENGRAVE         'E'
#define MJOLNIR_CMD_USE             'a'         // apply/use an object
#define MJOLNIR_CMD_TURN_ON         '0'         // turn something off
#define MJOLNIR_CMD_TURN_OFF        '1'         // turn something on
#define MJOLNIR_PICK_LOCK           'l'         // pick a lock (may use a tool)
#define MJOLNIR_CMD_OFFER           '0'         // sacrifice to gods
#define MJOLNIR_CMD_PAY             'p'         // pay shopping bill
#define MJOLNIR_CMD_PRINT_TOOLS     '('         // print tools in use
#define MJOLNIR_CMD_PRINT_SPELLS    '+'         // list and reorder known spells
#define MJOLNIR_CMD_PRINT_KEY       '&'         // explain what a key does
#define MJOLNIR_CMD_PRINT_COMMANDS  '?'         // show list of these commands
#define MJOLNIR_CMD_OVERVIEW        CTRL('o')   // display level overview
#define MJOLNIR_CMD_PRAY            CTRL('p')   // pray to the gods
#define MJOLNIR_CMD_RIDE            CTRL('r')
#define MJOLNIR_CMD_LOOT            CTRL('L')
#define MJOLNIR_CMD_SIT             CTRL('s')
#define MJOLNIR_CMD_TURN_UNDEAD     CTRL('t')
#define MJOLNIR_UNTRAP              CTRL('u')
#define MJOLNIR_CMD_SUSPEND         CTRL('z')
#define MJOLNIR_CMD_DROP_MULTI      CTRL('d')
#define MJOLNIR_CMD_PICK_MULTI      CTRL('p')
#define MJOLNIR_CMD_COUNT           '$'         // count your gold and money

#endif /* __MJOLNIR_CMD_H__ */

