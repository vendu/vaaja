# Deck Soft-Processor Project

## Preface

In this directory tree I'm attempting to design a small and simple processor
for playing around with FPGA gear; I'd hope this would be helpful for the
purposes of education and fun.

The project is early, but it's based on a few different pseudo-designs I've come
up with in past. It was actually inspired by the game project Mjolnir; I thought
a cyberpunk-style Roguelike game could use small computers called decks, one
thought lead to another and I realized why not mess around a bit with something
like this. I still have an FPGA-kit waiting to be put in real use.

One of my visions is to implement a retro-style game console with a custom
32-bit microprocessor, VGA-graphics (my current FPGA-kit does 4,096-colors),
connectors for keyboards and mice, and a couple of digital joystick ports as
well as, of course, audio output.

For a bit more information about these half-related projects, feel free to
browse this repository at

       [vaaja repository](https://github.com/vendu/vaaja/)

Thanks for your interest. :)

Cheers,

/* vendu */

## Goals

The goal of the Deck project is to provide programmers/users of FPGA-kits with
a [relatively] simple, small, and understandable microprocessor implementation.
The initial FPGA-board shall likely be a Gadget Factory Papilio Pro - I have one
of those laying around. For the soft-processor, the implementation is going to
be done in Verilog; for the virtual machine and other system-software, I've
decided on using strictly C (mostly C99). That's been my language of choice for
a couple of decades and I feel it gives us the power and control necessary for
implementing this kind of low-level software.

## Rationale

FPGA boards, quite-usable ones, can be acquired for 100 euros or even less. They
give us pretty much the ultimate tool for exploring not just software, but also
hardware development. I'm thrilled about all of the possibilities we have at our
hands today. Last but not at all least, I find this kind of hacking and
tinkering around a lot of fun. Let's be honest, computer hardware such as micro-
processors are the ultimate toys for creating more toys with - and sometimes
even getting useful things done. :P

Let's engineer the future!

