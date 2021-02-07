# Mjolnir Introduction

Mjolnir is a game in the Rogue-like genre that is based on characters and items,
and deities from different mythologies as well as perhaps more traditional
animals, horror creatures such as vampires, werewolves, zombies and ghouls, as
well as special weapons and items that are related to common religious beliefs.

The game levels are generated procedurally, somewhat randomly on the fly. This
makes every single game unique and different from all other games.

## A Bit of Cyberpunk

I thought it might be a good idea to mix some cyberpunk vibes into the game;
the dungeons, therefore, have punchcards and tapes with stored programs as well
as terminals and mainframes for running the software. The software shall range
from Core War Redcode development environment to different cracks to tap in on
and forge network traffic. Terminals typically come with punchcard slots to read
and write programs on punchcards.

# Game Time

The game time is designed to be advanced by 10 seconds every turn. However, when
not doing anything, the game advances in real time. It's a good idea to get
shelter for before you go idle.

# Game Start

At the beginning of game, you will have the following possessions
f - 4 food rations
~ - 8 potions of water
G - a bit of Gleipnir
x - the blessed silver cross of Ezekiel
| - the rod of Buddha
S - Silverblade from your worshipped deity
D - deck-computer
  - 2 flashcard slots
1 - 1 jack; 1x512K transfer rate
1 - 1 core; 32 MIPS execution speed
1 - RAM; 1x4M, 16 slots
1 - 4 punchcards
    - COM   - DPL-compiler
    - DEB   - deck debugger
    - DPF   - deck profiler
    - TAP   - network packet monitoring and filtering
      - PKT *tap(src, mask, port, ndx); - create a new packet-queue
        - returns head-pointer or NULL on failure (sets errno)
          - errno: ENOMEM
      - uint16 pktchksum(pkt);          - calculate packet [IPv4] checksum (CRC)
      - uint32 pktgetsrcadr(pkt);       - get packet source address
      - uint32 pktgetdestadr(pkt)       - get packet destination address
      - uint16 pktgetsrcport(pkt);      - get packet source port
      - uint16 pktgetdestport(pkt);     - get packet destination port
      - uint32 pktpushfilt(prog, qid);
        - return: qid, -1 on failure (sets errno)
          - errno: ENOMEM
      - void   pktlog(level);           - log textual information on packet
      - void   pktcpy(pkt, ram);        - copy biary-packet data into RAM
      - void   pktrecv(pkt);            - push to input-chain
      - void   pktsend(pkt);            - push to output-chain
      - void   pktreq(pkt, queue);      - reque packet to specified queue
1 - 1 flashcard with the DOS (Deck Operating System)
1 - 1 flashcard with the following data
  - 9 spellbook
    - 4xfireball + 4xfirebolt OR 4xfrostball + 4xfrostbolt
      OR 4xshockball + 4xshcokbolt depending on your characters element of fire,
      ice, or lightning, respectively
  - 9 notes (level maps with special location, your notes)
  - 2 anonymous bank account credentials
- - ATM-debit card loaded with $5,000

# Levels and Quests

Different levels [and quests] in the game feature themed environment and
characters to interact with.

# Level -1 - Heaven

Once you have returned Mjolnir to Thor or Ukko, you will be ascended to demigod-
hood by the corresponding god. In short, you win the game.

# Level 0 - Ground

The Big War of 2051 has wiped huge amount of the population; humans and other
creatures, animals, and plants included. Radiation levels are intolerable in
most areas.

The ground level has building such as shops and offices, as well as graveyards.
Business is out and the undead roam the world.

## Transylvania

Your quest in Transylvania is to slay the master vampire Vlad Dracula alias
Vlad III Tepes. He's often present at the Bran castle; welcome to the vampire
country. :)

Pay attention to graveyards, especially at dusk and dawn.

# Levels 1-8 - New Atlantis

New Atlamtis consists of procedurally, pseudo-randomly  generated rooms and
walls, to guarantee a unique experience every game.

The population of New Atlantis is said to be about 5,000.

There's up to a few ATM-machines per-level together with half a dozen banks.

Digital currency is normal. Pretty much every inhabitant has an anonymous
digital account + password combination or two.

Your quests in the dungeons include a digital a ATM/credit-card hoax and a
bank robbery.

## Altars

Altars can be used to offer sacrifices (plants etc., only flesh if youre evil
or chaotic). Sometimes your deity gives you a special gift such as a sword

## Monastery

The monks you run into are highly-educated ones with some being experts on demon
and ghost hunting. Pay attention to the monastery library.

# Level 9 - Oracle

# Levels 10-17 - New Atlantis Suburbs

The oracle gives useful hints at different prices - the more you pay, the better
advice. In case you manage to hypnotize the mentally strong oracle, he will slip
out a hint for free.

# Level 18 - Original Atlantis

Your mission in Atlantis is to rescue the captured princess Kara from the
continent once sunk into the sea. You also have to return her golden sword back
to Kara.

# Level 19 - Oracle

# Levels 19-20 - Death Labyrinth

The Death Labyrinth is a place ruled by the daughter of Asmodeus aka Lucifer,
Asmodina. Common shapes of Asmodina include that of a young woman as well as
the snake Apep. Beating Asmodina will require use of white magic weaponry.

Somehow, Sinclair's silver cross [of Ezekiel] has gone missing, and he's been
given a hint that it might be in Asmodina's labyrinth. Your mission is to try
to rescue it if so.

# Levels 21-22 - Hell

Hell is where Lucifer the fallen angel aka Satan rules the game. Your mission is
to either eliminate Asmodis or escape from Hell.

# Levels 23-38 - Old Atlantis

Old Atlantis is where humans and other creatures lived after the fall of the
Original Atlantis in 1024 until Asmodis and Asmodina took over the inside of
the planet below Original Atlantis above them. This exile in 2057AD
wiped out 3/4 of the Atlantean from about 10,000 in their battle against demons.

# Level 39 - Oracle

# Level 40 - Asgard
- the home and fortress of the Aesir

# Level 41 - Vanaheim
- the home and fortress of the Vanir

# Level 42 - Valhalla

Rumor has it Fenris has taken Mjolnir the hammer which he
stole from Thor to Valhalla. It's also a place where half of those who died in
combats have gathered to help Odin with the events of Ragnarök.

