#pragma once
#include <pthread.h>

#include "common.h"
#include "cart.h"
#include "bus.h"
#include "cpu.h"

//stepping - true for stepping mode (DEBUG ONLY)
void emu_begin(bool stepping);


/* Increment Clock by 1 */
uint64_t tick();

/* Return internal clock tick counter */
uint64_t get_clock_ticks();