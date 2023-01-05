#!/bin/bash
clang -Wall bus.c io.c cart.c cpu.c emu.c common.c stack.c instructions.c window.c main.c -o emulator