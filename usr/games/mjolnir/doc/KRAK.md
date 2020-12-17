# Krak; Crack the System Game

## Gameplay

At start, each player places the top 6 cards from their [shuffled] deck and
places them on the board's homebase slots in left to right order.

The users start rolling a D6-die and moving their characters in left-to-right
order. In case a character enters a slot occupied by a card, the card is
revealed and its action taken.

NOTE: in a tabletop/human-only game, the order to move the characters is
arbitrary. The computer game has been simplified slightly not to require to
much AI-like behavior. :) The damage hitpoints are subtracted from the current
number of hitpoints.

## Character Movement

- different character classes have different base skills and talent (efficiency
  to develop the skill) for Krak
- a turn starts by a throw of HITDIE (D6) to determine how many slots the player
  moves forward in the row (or until it meets an opponent character)
  - when an opponent character is met, the forward-moving player reveals the
    topmost card of their upside-down deck;
- in case of a HIT, attack strength is determined by the following formula;

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
      - #1 WHITE        0
      - #2 ORANGE       1
      - #3 BLUE         2-3
      - #4 YELLOW       4-8
      - #5 GREEN        8-15
      - #6 nBROWN       16-31
      - #7 BLACK        32-39
      - #8 BLACK 1ST    40-47
      - #9 BLACK 2ND    48-55
      - #10 BLACK 3RD   56-63

## Cards

- HIT:          - BATTLE/MELT
- SWAP          - swap positions of the two cards
- THIEF         - steal the top card from opponent and place it under your deck
- HEAL          - raise a character's mental and/or physical health
- PROBE         - identify topmost card on the opponent deck
- SWARM         - attack all adjacent (next to each other) opponent characters
- RAY           - attack a row or column of characters at once
- EMP           - weaken or destroy opponent deck or mainframe (deactivate
                  connected cards)
- TELEPORT      - teleport player character to a random location
- WORM          - invisibly controls a single instruction slot
- TROJAN        - controls all instruction slots the card has traversed
- VIRUS         - whenever a character enters, infect all adjacent ones
- ANTIVIRUS     - remove virus from a character
- ENCRYPT       - ROT13, DES, SHA
- DECRYPT       - ROT13, DES, SHA

