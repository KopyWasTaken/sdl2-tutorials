#ifndef KTIMER_H
#define KTIMER_H

#include <stdbool.h>
#include <SDL3/SDL_stdinc.h>

typedef struct KTimer_s {
  Uint64 mStartTicks;
  Uint64 mPausedTicks;
  bool mPaused;
  bool mStarted;
} KTimer;

void ktimer_init();

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

#endif // !KTIMER_H
