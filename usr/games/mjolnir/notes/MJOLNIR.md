# mjolnir video-game

Copyright (C) Tuomo Petteri Venäläinen 2020. All rights reserved.

This document contains ideas for a Rogue-like adventure game.

## background

I grew up playing mostly point-and-click adventures from the likes of Sierra
On-Line and Lucasarts. Games such as Gabriel Knight series by my favorite game
creator Jane Jensen, another Sierra series of funny Space Quest games, most
Leisure Suit Larry games, and so forth.

I was thrilled when I first ran games such as Hack or Nethack. I realized games
are not all about fancy graphics but that the atmosphere and plot need some
depth to make games really appealing to me. As a great example, I want to
mention Gabriel Knight - Sins of the Fathers from the late Sierra On-Line. I
rank it as one of the very best computer games ever made.

Many Rogue-like genre games are based on characters from Dungeons & Dragons
games or perhaps Tolkien stories or the Cthulhu mythos, which is one reason I
decided to try something different so you're going to meet characters from
different mythologies such as Norse ones together with demons, some animals,
and a few deities.

## the plot

Fenris the wolf has stolen Mjolnir from Thor. Your main mission is to return his
hammer back to Thor and optionally bring Fenris back as well.

## magic and spells

Mjolnir spells are typically written on scrolls. These spells include
- spell of see monsters; shows all monsters on the level
- spell of see hidden objects; shows all hidden objects on the level
- spell of see map; shows level map

## game interface

The first versions of Mjolnir shall be based on characters; e.g., the '@'-
character represents your character, a 'b' would be a bee and so forth. The
characters you know to be friendly are printed in green; yellow means neutral
character, and finally red means characters known to be hostile. Objects such
as stairs are rendered in light green, unrecognized items in gold, and known-to-
be-hazardous objects such as traps are shown in magenta.

The dungeons are generated procedurally utilising pseudo-random numbers. I found
a suitable level generator at

[Dungen cave generator](https://github.com/glouw/dungen

## game intro

Legend has it that many moons ago, Fenris, the eldest child of Loki and
Angrboda, broke free from his chain, Gleipnir, and stole Mjolnir from Thor.
Fenris is believed to have hidden Mjolnir, Thor's hammer, in the Dungeon of
Cyberia.

It is said that Thor shall ascend the one to return Mjolnir to a deity for
saving the world from the forces of evil creating chaos with lightnings and
thunder. Clairvoyants say evil forces are already on the hunt for Mjolnir.

Armed with nothing more than a few food rations, your pet dog, a +1 blessed
armor, a +3 neutral sword, and a piece of Gleipnir, you enter the Underworld.

Good luck, adventurer, and beware of Fenris!

## notes
- unknown characters and objects are shown in white
- neutral characters and objects are shown in yellow
- safe/friendly objects and characters are shown in green
- unsafe/hostile objects are shown in red

## commands

### notes
- modeled after Rogue
- movement can also be done using a PC-style numpad

### rogue command

keybard command | description
|----------------|------------
| ESC           | boss key
| m             | open command menu
| /             | identify object
| h             | left
| j             | down
| k             | up
| l             | right
| y             | up & left
| u             | up & right
| b             | down & left
| n             | down & right
| <SHIFT><dir>  | run
| <CTRL><dir>   | run till adjacent
| f<dir>        | fight till death or near death
| t<dir>        | throw
| m<dir>        | move on top but don't pick upz<
| z<dir>        | zap a wand
| ^<dir>        | identify trap type
| s             | search for trap/secret door
| >             | go down
| <             | go up
| .             | rest for a turn
| ,             | pick up
| i             | inventory
| I             | inventory single item
| q             | quaff potion
| r             | read scroll
| s             | shout
| e             | eat food
| w             | wield a weapon
| W             | wear armor
| T             | take armor off
| P             | put on ring
| R             | remove ring
| d             | drop object
| c             | call object
| a             | repeat last command
| )             | print current weapon
| ]             | print current armor
| =             | print current rings
| @             | print current stats
| D             | recall what's been discovered
| o             | examine/set options
| ^R            | redraw screen
| ^P            | repeat last message
| ^[            | cancel command
| S             | save game
Q               | quit

mjolnir actions | description
|---------------|------------
| u             | use an object with another one
| 0             | turn off
| 1             | turn on

## gameplay
- console commands (invoke console with M-x)
  - bite(obj);
  - bargain(chr, obj, price);
  - buy(chr, obj, src);
  - eat(obj);
  - drink(obj);
  - pick(obj);
  - drop(obj);
  - use(obj, dest);
  - walk(dest);
  - talk(chr);
  - listen(chr);
  - look(loc);
  - open(obj, key);
  - close(obj);
  - unlock(obj, key|pick);
  - read(obj);
  - shout(str);
  - search(loc);
  - touch(obj);
  - unlock(obj, key);
  - lock(obj);
- scrolls
  - teleport, levitation, identify, ice bolt, lightning
- weapons
  - submachine gun (special item Jatimatic)
  - swords (special items Excalibur, Stormbringer, and Kara's Sword)
  - cross of light
  - bullet
  - pistol
  - whip
  - knife
  - laser
  - mace
- potions
  - green dragon, sweet leaf
- plant
  - special item Herb of Love
    - healing power
    - gives better focus and concentration
    - boosts combat and other physical skills
- quests/subgames
  - core war
    - debug and develop warriors with graphical tools
    - simulate battles
      - simulator console mode for keyboard commands
        - peek  - inspect memory contents
        - poke  - change memory contents
        - brk   - set execution or memory breakpoint
        - step  - execute a single instruction
        - stepi - execute n instructions
  - mastermind(chr);
  - battleship(chr);

## tiles
-----
- tiles should have 3 different versions for modes
  - active
  - hilited
  - clicked
- navigation (compass background)
  - buttons; highlight the currently movable directions
    - north
    - east
    - south
    - west
    - northeast
    - southeast
    - southwest
    - northwest

## TODO

### ams
- dragons
- leprechauns
- trolls
- wizards
> <ams> Classic cyberpunk characters were marginalized, alienated loners who
lived on the edge of society in generally dystopic futures where daily life was
impacted by rapid technological change, an ubiquitous datasphere of computerized
information, and invasive modification of the human body. -- lawrence person

