# Zen Kernel Objects

Zen design will be based on objects and implemented mostly in the C and Assembly
programming languges.

This document is currently a quickly-brainstormed list of such objects.

# Directory
- a directory is a container for directories and other objects

# Character Device
- 'raw' I/O interface to a byte-at-a-time device such as a teletype

# Block Device
- block I/O interface to a block-at-a-time device such as a disk

# Regular File
- text, binary, or any format data

# FIFO
- pipe; first-in-first-out buffer

# Socket
- network connection endpoint

## Special Device Files

## /dev/null
- discards all input
- produces no output (may be used as an empty file)

## /dev/zero
- discards all input
- produces continuous stream of zero bytes

## /dev/random

## /dev/urandom

