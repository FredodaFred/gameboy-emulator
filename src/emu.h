#pragma once

#include "common.h"
#include "cart.h"
#include "bus.h"
#include "cpu.h"

void emu_begin();


/* Increment Clock by 1 */
uint64_t tick();

/* Return internal clock tick counter */
uint64_t get_clock_ticks();