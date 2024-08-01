#ifndef STATE_H
#define STATE_H

#include <SDL3/SDL.h>

typedef struct State_s {
  SDL_Window *window;
  SDL_Renderer *renderer;
} State;

#endif // !STATE_H
