# KRAKSYS Board Game

KRAKSYS is a quick-to-play board game for 2-4 players. A game should last
approximately 15-60 minutes.

# Character Attributes

- CON       - concentration
- REF       - reflex
- EXP       - experience points
- LVL       - experience level
- STR       - [close-combat] attack base strength
- HP        - [close combat] hit points

## Getting Started

In the beginning of the game, each player chooses player element of water, air,
fire, or land. There may be no two players with the same element.

A roll of D& determines the magic skill (the number of spells you may use in
combinations).

The slots protected by the action cards are mainframes.

each player gets the following items (first listed for 2-player sessions)

- 8 runners (of your native element)
- up to 16 spell cards; D4 x frost, D4 x tornado, D4 x tsunami, D4 x quake
- the following action cards
  - 2 teleport cards; teleport the attacking piece to a random location on the
    board (roll X- and Y-coordinates with D8)
    - 2 thief cards; steal a [blindly-picked] card from an opponent's hand
    - 2 swap cards; exchange the swap card with a [blindly-picked] one from an
      opponent's deck
      - 2 probe cards; identify opponent card on the board
      (turn it face-side up)
      - 2 trojan cards; try to infect/hijack a chosen mainframe of the attacker
      - 2 worm cards; try to hijack as many attacker mainframes as you desire
       as long as the worm is alive, you control them all, or you decide to
       stop the invasion
       - 2 emp cards; try to destroy a chosen attacker mainframe
       - D8 x heal cards; these may be used to start a turn

The players locate their 8 runners (miniatures?) to their R-columns and chosen
action cards in their 8 action columns on the board.

## Board Layout

The game board looks like this

- 1P    - player 1 runner
- 1A    - player 1 actions
- 2P    - player 2 runner
- 2A    - player 2 actions
- 3P    - player 3 runner
- 3A    - player 3 actions
- 4P    - player 4 runner
- 4A    - player 4 actions

|    |    |    |    | A  | B  | C  | D  | E  | F  | G  | H   |    |    |    |
|:---|:---|:---|:---|:---|:---|:---|:---|:---|:---|:---|:----|:---|:---|:---|
|    |    |    |    |1M1 |1M2 |1M3 |1M4 |1M5 |1M6 |1M7 |1M8  |    |    |    |
|    |    |    |    |1A1 |1A2 |1A3 |1A4 |1A5 |1A6 |1A7 |1A8  |    |    |    |
|    |    |    |    |1R1 |1R2 |1R3 |1R4 |1R5 |1R6 |1R7 |1R8  |    |    |    |
| 1  |3M1 |3A1 |3R1 |    |    |    |    |    |    |    |     |4R1 |4A1 |4M1 |
| 2  |3M2 |3A2 |3R2 |    |    |    |    |    |    |    |     |4R2 |4A2 |4M2 |
| 3  |3M3 |3A3 |3R3 |    |    |    |    |    |    |    |     |4R3 |4A3 |4M3 |
| 4  |3M4 |3A4 |3R4 |    |    |    |    |    |    |    |     |4R4 |4A4 |4M4 |
| 5  |3M5 |3A5 |3R5 |    |    |    |    |    |    |    |     |4R5 |4A5 |4M5 |
| 6  |3M6 |3A6 |3R6 |    |    |    |    |    |    |    |     |4R6 |4A6 |4M6 |
| 7  |3M7 |3A7 |3R7 |    |    |    |    |    |    |    |     |4R7 |4A7 |4M7 |
| 8  |3M8 |3A8 |3R8 |    |    |    |    |    |    |    |     |4R8 |4A8 |4M8 |
|    |    |    |    | 2R1 |2R2 |2R3 |2R4 |2R5 |2R6 |2R7 |2R8 |    |    |    |
|    |    |    |    | 2A1 |2A2 |2A3 |2A4 |2A5 |2A6 |2A7 |2A8 |    |    |    |
|    |    |    |    | 2M1 |2M2 |2M3 |2M4 |2M5 |2M6 |2M7 |2M8 |    |    |    |

After the initial steps for the player, they populate the S1-S10 cells with
spell cards face down.

R1-R10 are populated with runners.

## Game Goal

Your mission in the game is to crack and capture as many mainframes as
possible from the other players.
jacked. You may destroy mainframes with EMP-cards. Captured mainframes may not
be taken back from you.

## Gameplay

A turn beings by a player moving a piece forward, left, or right; you are not
allowed to move pieces back towards your home location (your A and R columns).
Diagonal moves are allowed. Optionally, you may use a heal card before moving
your runner.

## Runner Combat; Close Combat or Ice

If you move the runner on top of an opponent one, a combat is triggered between
your element piece and the opponent card; you may choose either a close-combat
or an ice-attack; more details on rolling dice and other game mechanics below.

You may opt to combine 1 or more spell cards with the attack; the maximum
number depends on you magic skill level (throwing the respective dice).

## Close Combat

First, you roll opponent reflex die D10. In case the opponent has the reflex
of 30 %, the reflex-save is successful if the roll is smaller than or equal
to 3.

Second, you roll the success die D10. If the roll is smaller than or equal to
your success rate, the hit is a success and you continue as follows.

You now roll the attack (ATK) dice, then the hit and success dice for each of
the chosen spells respectively and add the figures to each other.

The total damage, if successful, is

    HIT = STR + ATK + SPELLS

where STR is your base strength, ATK is the hit, and SPELLS is the sum of your
spell dice.

In case the points result in a tie, the attacker has advantage and wins. The
losing runner is removed from the board.

## Ice Combat

The rules for ICE are to be documented later. For now, feel free to take a
peek at ice.c and ice.h.

## Element Combat

## Dice

KRAKSYS Gameplay is based on D20 set of dice with dice of 4, 6, 8, 10, 12, and
20 points.

Here, I list some default actions for the different dice.

| DIE   | use                                   | NAME
|:------|:--------------------------------------|:-------
|  D4   | healing base die                      | HEAL
|  D6   | defanse base die                      | ATTACK
|  D8   | attack base die                       | ATTACK
|  D10  | spell base die                        | SPELL
|  D10  | check action; 1 point is 10 %         | ACTION
|  D20  | check success; 1 point is 5 %         | SUCCESS

## Attack Cards

Every attack card represents one element of water, air, fire, or land.

### Protection/Attack Cards; Elements

#### Water

- attack die D6
- defense die against water D6
- defense die against air D4
- defense die against fire D8
- defense die against land D10

#### Air

- attack die D6
- defense die against water D8
- defense die against air D6
- defense die against fire D4
- defense die against land D10

#### Fire

- attack die D6
- defense die against water D10
- defense die against air D8
- defense die against fire D6
- defense die against land D4

#### Land

- attack die D6
- defense die against water D8
- defense die against air D4
- defense die against fire D10
- defense die against land D6

### Spell Cards

#### Frost

- spell die D10
- hit die against water D10
- hit die against air D6
- hit die against fire D10
- hit die against land D6

#### Tornado

- spell die D10
- hit die against water D6
- hit die against air D10
- hit die against fire D10
- hit die against land D6

#### Tsunami

- spell die D10
- hit die against water D6
- hit die against air D6
- hit die against fire D10
- hit die against land D10

#### Earthquake

- spell die D10
- hit die against water D10
- hit die against air D6
- hit die against fire D6
- hit die against land D10

### Action Cards

#### Thief

When a player moves on a thief card, the thief owner may pick a random card
from the player's deck.

#### Swap

When a player moves on a swap card, the player picks the card and lets its owner
pick one from their deck at random.

#### Probe

Probe cards can be at any time; the probe takes the whole turn and lets you
turn an opponent card face up to recognize it.

#### Trojan

When a trojan card is met, it's placed into the owner's stack of used cards and
the trojan's owner picks up the attacking card.

#### Worm

Attack a given opponent's mainframes until the worm dies or you own all of them.

#### EMP

An electro-magnetic pulse, EMP, card may be played against any protection card;
if the attack succeeds and the cell protected is connected ('jacked') to a
mainframe, the plug is attacked. If the plug gets killed, the player may add a
plug to their own mainframe in the cell.

