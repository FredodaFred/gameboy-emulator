#!/bin/bash
clang -Wall bus.c cart.c cpu.c emu.c common.c instructions.c main.c -o emulator