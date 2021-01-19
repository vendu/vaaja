# KRAKSYS Board Game

KRAKSYS is a quick-to-play board game for 2-4 players. A game should last
approximately 15-60 minutes.

## Getting Started

In the beginning of the game, each player chooses player element of water, air,
fire, or land. There may be no two players with the same element.

A roll of D6 determines the skill factor for each player as follows
- the value 1 indicates we shall roll D4 to for luck
- the value 2 indicates we shall roll D4 to for luck
- the value 3 indicates we shall roll D4 to for luck
- the value 4 indicates we shall roll D6 to for luck
- the value 5 indicates we shall roll D6 to for luck
- the value 6 indicates we shall roll D6 to for luck

A roll of D4 determines the magic skill (the number of spells you may use in
combinations).

The slots protected by the action cards are mainframes.

each player gets the following items (first listed for 2-player sessions)

- 8 mainframe cards
- 8 pieces of your player/native element
- up to 16 spell cards; D4 x frost, D4 x tornado, D4 x tsunami, D4 x quake
- 2 teleport cards; teleport the attacking piece to a random location on the
  board (roll X- and Y-coordinates with D8)
- 2 thief cards; steal a [blindly-picked] card from an opponent's hand
- 2 swap cards; exchange the swap card with a [blindly-picked] one from an
  opponent's deck
- 2 probe cards; identify opponent card on the board (turn it face-side up)
- 2 trojan cards; try to infect/hijack the chosen attacker mainframe
- 2 worm cards; try to hijack as many attacker mainframes as you desire as
  long as the worm is alive or you control them all; this is where you can
  use your heal cards
- 2 emp cards; try to destroy a chosen attacker mainframe
- D4 x heal cards

## Board Layout

The game board looks like this

- 1P    - player 1 protection
- 1A    - player 1 actions
- 2P    - player 2 protection
- 2A    - player 2 actions
- 3P    - player 3 protection
- 3A    - player 3 actions
- 4P    - player 4 protection
- 4A    - player 4 actions

|Y\X | M3 | A3 | P3 | 1  | 2  | 3  | 4  | 5  | 6  | 7  | P4 | A4 | M4 |
|:---|:---|:---|:---|:---|:---|:---|:---|:---|:---|:---|:---|:---|:----
| M1 | M3 |    |1M1 |1M2 |1M3 |1M4 |1M5 |1M6 |1M7 |1M8 |    |    |    |
| A1 |    |    |1A1 |1A2 |1A3 |1A4 |1A5 |1A6 |1A7 |1A8 |    |    |    |
| P1 |    |    |1P1 |1P2 |1P3 |1P4 |1P5 |1P6 |1P7 |1P8 |    |    |    |
| 1  |3M1 |3A1 |3P1 |    |    |    |    |    |    |    |4P1 |4A1 |4M1 |
| 2  |3M2 |3A2 |3P2 |    |    |    |    |    |    |    |4P2 |4A2 |4M2 |
| 3  |3M3 |3A3 |3P3 |    |    |    |    |    |    |    |4P3 |4A3 |4M3 |
| 4  |3M4 |3A4 |3P4 |    |    |    |    |    |    |    |4P4 |4A4 |4M4 |
| 5  |3M5 |3A5 |3P5 |    |    |    |    |    |    |    |4P5 |4A5 |4M5 |
| 6  |3M6 |3A6 |3P6 |    |    |    |    |    |    |    |4P6 |4A6 |4M6 |
| 7  |3M7 |3A7 |3P7 |    |    |    |    |    |    |    |4P7 |4A7 |4M7 |
| 8  |3M8 |3A8 |3P8 |    |    |    |    |    |    |    |4P8 |4A8 |4M8 |
| P2 |    |    |2P1 |2P2 |2P3 |2P4 |2P5 |2P6 |2P7 |2P8 |    |    |    |
| A2 |    |    |2A1 |2A2 |2A3 |2A4 |2A5 |2A6 |2A7 |2A8 |4P8 |2A9 |4M1 |

After the initial steps for the player, they populate the S1-S10 cells with
spell cards face down.

P1-P10 are populated with element pieces.

## Game Goal

Your mission in the game is to crack and capture as many mainframes as possible.
You have captured a mainframe when you have at least 50 % of its live mainframes
jacked. You may destroy mainframes with EMP-cards. Captured mainframes may not
be taken back from you.

## Gameplay

A turn beings by a player moving a piece forward, left, or right; you are not
allowed to move pieces back towards your home location (your S and P columns).
Diagonal moves are allowed.

If you move the piece on top of an opponent card, a combat is triggered between
your element piece and the opponent card; more details on rolling dice and other
game mechanics below.

You may opt to combine 1 or more spell cards with the attack; the maximum number
depends on you magic skill level (throwing the respective dice).

You now roll the die, then the dice for each spell respectively and add the
figures to each other.

Finally, you roll the luck die and add it to the current sum of figures to
determine the total hit strength of your attack.

If the attack leads to a die the attacking player wins and the opponent card
just beaten is discarded from the board. The player may but does not have to
replace the card with their own action card (see Action Cards below).

## Actions

The game actions are attack, defense, and spell combinations.

## Dice

KRAKSYS Gameplay is based on D20 set of dice with dices of 4, 6, 8, 10, 12, and
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

## Cards

Every card represents one element of water, air, fire, or land.

### Combat System

When a player moves a piece to the protection zone of another player, combat
results. The opponent may choose a protection spell from the adjacent spell
cell (right below or after the protection cell just attacked).

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

