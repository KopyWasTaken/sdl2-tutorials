#ifndef DOT_H
#define DOT_H

#include <SDL3/SDL_main.h>
#include "ktexture.h"

const int DOT_HEIGHT = 10;
const int DOT_WIDTH = 10;

typedef struct Dot_s {
  int posX, posY, velX, velY;
} Dot;

void dot_handled_event(Dot *dot, SDL_Event e);
Dot *dot_create();
void dot_move(Dot *dot);
void dot_render(Dot *dot, KTexture *dotTexture, SDL_Renderer *renderer);

#endif // !DOT_H
