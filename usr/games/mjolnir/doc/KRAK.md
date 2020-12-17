# Krak; Crack the System Game

- different character classes have different base skills and talent (efficiency
  to develop the skill) for Krak
- a turn starts by a throw of HITDIE (D6) to determine how many cards the
  player is allowed to play during the turn
- an attack' strength is determined by the following formula;

  HIT = XP / 4 + POWER + LUCK + SKILL;

  where XP is the character's experience level in krak, PWR is hit power (D6 by
  default), LUCK is luck factor (D4 by default), and finally SKILL is a per-
  character-type SKILL for krak;
  - HIT         - attack strength in hitpoints
  - POWER is a per-character die (according to XP)
  - LUCK is determined as a function of KARMA in a predefined way (given die for
    the karma value)
  - SKILL is the per-character krak-skill value (evolves depending on TALENT)
    - order of karate belts; white, orange, blue, yellow, green, brown, black,
      1st dan black, 2nd dan black, 3rd dan black
      - WHITE           0
      - ORANGE          1
      - BLUE            2-3
      - YELLOW          4-8
      - GREEN           8-15
      - BROWN           16-31
      - BLACK           32-39
      - BLACK 1ST       40-47
      - BLACK 2ND       48-55
      - BLACK 3RD       56-63

## Cards

- SWAP          - swap positions of the two cards
- THIEF         - steal a card from the opponent and place it on the board
- MELT          - weaken or destroy ICE
- ICE           - defend against attack
- HEAL          - raise a characters mental and/or physical health
- PROBE         - identify [hidden] card on the board
- SWARM         - attack several opponent cards
- RAY           - attack a row or column of cards at once
- EMP           - weaken or destroy opponent deck or mainframe (deactivating
                  connected cards)

### Future Additions

- WORM          - invisibly controls a single instruction slot
- TROJAN        - controls all instruction slots the card has traversed
- VIRUS
- ANTIVIRUS
- ENCRYPT       - ROT13, DES, SHA
- DECRYPT       - ROT13, DES, SHAs

## Gameplay

At start, each player places the top 6 cards from their [shuffled] deck and
places them on the board's homebase slots in left to right order.

The users start rolling a D6-die and moving their characters in left-to-right
order. In case two cards hit each other, they're turned visible and the actions
of the cards are taken. NOTE: in a tabletop/human-only game, the order to move
the characters is arbitrary. The computer game has been simplified slightly not
to require too much AI-like behavior. :) The damage hitpoints are
subtracted from the current number of hitpoints.

