# Dungeon Objects

The objects are designed to be based on different mythologies and cyberpunk-
like plot.

## ' ' - floor

Floors are empty space in the dungeon, but may have traps and items hidden on
them.

## # - wall

Walls exist to separate rooms and corridors; they have hidden traps, cervings,
or corridors

## + - door

## . - sand

Sand is usually safe, but has a small probability to be quick-sand which makes
you sink down to the next level

## > - stairway down

Stairs one level down.

## < - stairway up

Stairs one level up.

# Miscellaneous Objects

## % - coffin

Vampires don't like daylight, so they sleep in closed coffins. These are located
at graveyards, often inside tombs.

## 0 - jack
- an outlet used for connecting to the network; connection speed is dictated by
  the deck's NET-module.

## 1 - deck upgrades
- network card with 1-16 plugs for 512K-4M transfer rates
- flash memory with the operating system
- RAM-memory, up to 16x4 megabytes (32-bit bus)
- processor module with 1-16 cores

## 6 - elecronic devices
- automatic and mechanical timers, readiation-meter

## - - ATM- or credit card

ATM/credit-cards you have gotten or stolen from may be used to transfer digital
currency to anynymous accounts with only numerical IDs which you control. They
can also be used to pay for items in shops.

## C - ATM-machine

ATM-machines can be used for cash withdrawals and deposits, and transfers from
one account to another.

## i - identification card (to be used on terminals)

## 9 - Book

## B - bee hive
- you may acquire honey from these; one queen per hive.

## c - carving
- may be hidden under sand

## f - food

## ~ - water

## { - fountain

## o - oxygen tank and mask

## $ - money

## = - bullet

## ! - potion

## * - plant

## : - punchcard

Punchcards contain Core War warriors for competing on mainframe masterhood as
well as other software tools.

## t - tape

Mainframes store Core War warriors and other software + data on tapes.

## & - statue

## ^ - trap
- arrow
- bomb
- pit
- hole
- teleport
- quicksand

## 9 - book

## Spells
- spell of identify
  - identifies player possessions
- spell of teleport
  - teleports player to a random location (on the current level)
- spell of light
  - activates cross of light; it starts to glow and heats up
- spell of firebolt
  - causes burns
- spell of frostbolt
  - causes freezing, even necrosis if bad enough
- spell of shockbolt
  - causes paralyses
- spell of show map - shows the map of the current level
- spell of stop time
- spell of bless

## o - ring
- ring of strength      - gives player 2 x STR
- ring of mind          - gives player 2 * INTEL
- ring of magic         - gives player 2 x MAGIC
- ring of levitation
- ring of invisibility
- ring of teleport
- ring of darkvision

## \ - whip
- may be used to slay non-demonic, usually smaller creatures

## ] - armor

## [ - mirror

## 4 - chest

## g - submachine gun
- may be special weapon Jatimatic

## h - honey

## k - katana

## j - crowbar ('jimmy bar')

## 3 - detonator (used constructing bombs) automatic or mechanical, the latter
       being able to take an EMP-hit

## 4 - c4 plastic explosive

## l - lockpick

## m - mace

## 8 - mainframe

## $ - money

## p - paper [possibly a note]

## ? - pipe

## 5 - shuriken (made of steel, silver, gold)

## 7 - pistol

## R - leftovers; remnants

## S - sword

## s - shopkeeper

## T - terminal

## x - cross

## X - altar

This is where you sacrifice to a dedicated god. You may get special items in
return.

| symbol | object         | probability         | description
|:-------|:---------------|:--------------------|:-----------
|  ' '   | floor          | N/A                 | floor/ground
|   #    | wall           | N/A                 | cave wall (may be hidden door)
|   +    | door           | N/A                 | door
|   .    | sand           | 1/4 in Original Atlantis| may be quick-sand (5 %)
|   >    | stairway down  | 1 per level         | stairs one level down
|   <    | stairway up    | 1 per level         | stairs one level up
|   %    | coffin         | 1/4 in graveyards   | a grave coffin (tomb, crypt)
|   0    | jack           | 1/64 in underworld  | connection with NET-module
|   1    | deck upgrades  | NET, MEM, PROC      | upgrades for your deck
|   -    | ATM-card       | 1/128               | bank account access; need PIN-code
|   C    | ATM-machine    | 1/32                | cash withdrawals and deposits
|   f    | food           | 1/16                | food ration
|   ~    | water          | 1/16                | water, potion
|   {    | fountain       | 1/32                | fountain
|   o    | oxygen tank    |
|   $    | gold           | 1/64                | use to get merchandise
|   =    | bullet         | 1/32                | bullet (silver ones work on daemons)
|   !    | potion         | 1/32                | potion
|   *    | plant/mushroom | 1/16                | plant on land, mushroom underground
|   :    | punchcard      | 1/64                | corewar warriors to be run from terminals
|   t    | tape           | mainframes          | software for in-dungeon mainframes
|   &    | statue         | 1/32                | e.g. Buddha statue, elephant statue, ...
|   ^    | trap           | varies              | trap; arrow, bomb, pit, hole, teleport
|   9    | scroll         | 1/32                | spell or other scroll
|   o    | ring           | 1/64                | ring
|   \    | whip           | 1/64                | whip
|   ]    | armor          | 1/32                | suit or armor
|   [    | mirror         | 1/16                | mirror on a wall
|   c    | chest          | 1/32                | chest; may find treasury in them
|   D    | deck           |                     | punchcard and tape readers
|   g    | sub-machine gun| buy from soldiers   | close-range combat
|   h    | honey          | in bee nests        |
|   k    | knife          | 1/32                | knife, dagger, gurkha, ...
|   l    | lockpick       | 1/64                | thieves are naturally best at using these
|   m    | mace           | 1/32                | mace or hammer
|   8    | mainframe      | 1/16                | computer systems for running punchcards
|   $    | money          | 1/256               | money
|   p    | paper          | N/A                 | paper pieces such as fortunes
|   ?    | pipe           | 1/64                | pipe
|   7    | pistol         | 1/64                | pistol
|   R    | remnants       | N/A                 | corpses and belongings
|   S    | sword          | 1/128               | sword
|        | shopkeeper
|   T    | terminal       | 1/16                | terminal connected to mainframe
|   x    | cross          | 1/64                | cross; may be cross of light
|   X    | altar          | 1/32                | altar; sacrifice dead creatures on it

## Special Objects

## Q - Crystal ball

The crystal ball shows things happening right now and/or in the future. The
probability for you to see visions depends on your psionic skill.

## | - Rod of Buddha

The holder of Buddha rod may stop time for a short period to flee from or do
preparations for fights by yelling TOPAR - it is not allowed to kill or attack
anyone during those turns.

## Y - demon whip

Legend has it this weapon against some demons was sown from the backskin of
a demon.

## J - Jatimatic

Finnish-engineered submachine gun, loaded with 9 mm silver bullets it makes a
very nice firearm against vampires and other demons. Another story goes that
quicksilver (mercury)-filled bullets might be the way to go for werewolves.

## K - golden sword
- the golden sword of the princess of Atlantis, Kara, is a strong weapon against
  demons and may be used to travel through dimensions

## ) - boomerang
- may be special Silver boomerang (against demons)

## M - Mjolnir

The hammer you need to return to Thor after Fenris stole it.

## G - Gleipnir

Ribbon that can hold Fenris leashed.

## x - Cross of Light

Special cross, the player's default white-magic weapon.

Distinguished from other crosses by having the names of 4 archangels - Michael,
Raphael, Gabriel, Uriel - carved on it. Very strong white-magic weapon.

| symbol | special object | probability    | description
|:-------|:---------------|:---------------|:-----------
|   Q    | Crystal ball   | 1/256          | may show things happening elsewhere or in the future
|   |   | Rod of Buddha   | 1 per game     | may be used to stop time
|   y   | Demon Whip      | 1 per game     | weapon against different forms of undead
|   J   | Jatimatic       | 1/256          | sub-machine gun for close range combat, may be loaded with 9-mm rounds, perhaps with silver bullets... ;)
|   (   | Golden Sword    | 1 per game     | princess Kara's golden sword
|   M   | Mjolnir         | 1 per game     | the hammer of Thor
|   G   | Gleipnir        | 1 at start     | ribbon that can hold Fenris
|   x   | Cross of Light  | 1 per game     | the blessed cross of Ezekiel with the names of 4 archangels - Michael, Raphael, Gabriel, Uriel

