#include "dot.h"
#include <stdlib.h>
#include "ktexture.h"
#include "main.h"

const int DOT_STEP_VELOCITY = 10;

void dot_handled_event(Dot *dot, SDL_Event e) {
  // if a key was pressed
  if (e.type == SDL_EVENT_KEY_DOWN && e.key.repeat == 0) {
    // adjust the velocity
    switch (e.key.key) {
    case SDLK_UP:
      dot->velY -= DOT_STEP_VELOCITY;
      break;
    case SDLK_DOWN:
      dot->velY += DOT_STEP_VELOCITY;
      break;
    case SDLK_LEFT:
      dot->velX -= DOT_STEP_VELOCITY;
      break;
    case SDLK_RIGHT:
      dot->velX += DOT_STEP_VELOCITY;
      break;
    }
  }
  // if a key was released
  else if (e.type == SDL_EVENT_KEY_UP && e.key.repeat == 0) {
    // adjust the velocity
    switch (e.key.key) {
    case SDLK_UP:
      dot->velY += DOT_STEP_VELOCITY;
      break;
    case SDLK_DOWN:
      dot->velY -= DOT_STEP_VELOCITY;
      break;
    case SDLK_LEFT:
      dot->velX += DOT_STEP_VELOCITY;
      break;
    case SDLK_RIGHT:
      dot->velX -= DOT_STEP_VELOCITY;
      break;
    }
  }
}

Dot *dot_create() {
  Dot *toReturn = (Dot*) malloc(sizeof(Dot));
  toReturn->posX = 0;
  toReturn->posY = 0;
  toReturn->velX = 0;
  toReturn->velY = 0;
  return toReturn;
}

void dot_move(Dot *dot) {
  // move the dot left or right
  dot->posX += dot->velX;

  //  if the dot went too far to the left or right
  if ((dot->posX < 0) || (dot->posX + DOT_WIDTH > SCREEN_WIDTH)) {
    // move it back
    dot->posX -= dot->velX;
  }

  // move the dot left or right
  dot->posY += dot->velY;

  //  if the dot went too far to the left or right
  if ((dot->posY < 0) || (dot->posY + DOT_HEIGHT > SCREEN_HEIGHT)) {
    // move it back
    dot->posY -= dot->velY;
  }
}

void dot_render(Dot *dot, KTexture *dotTexture, SDL_Renderer *renderer) {
  ktexture_render(dotTexture, renderer, dot->posX, dot->posY);
}
