#ifndef KTIMER_C
#define KTIMER_C

#include "ktimer.h"
#include <stdlib.h>

void ktimer_init() {
  KTimer *toReturn = (KTimer*) malloc(sizeof(KTimer));
}

// Various clock actions
void ktimer_start();
void ktimer_stop();
void ktimer_pause();
void ktimer_unpause();

// Gets the timer's time
Uint64 ktimer_get_ticks();
  
// Checks the status of the timer
bool ktimer_is_started();
bool ktimer_is_paused();

#endif /* ifndef KTIMER_C */
